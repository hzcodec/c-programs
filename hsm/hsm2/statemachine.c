#include <assert.h>
#include "statemachine.h"
#include "log.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

#if SM_INTERNAL_EVENT_QUEUE
#ifndef SM_EVENT_QUEUE_LEN
#define SM_EVENT_QUEUE_LEN 8 ///< Length of event queue
#endif
#endif

/** Special entry event instance. */
static const struct event ev_entry = { EV_ENTRY, 0 };
/** Special exit event instance. */
static const struct event ev_exit = { EV_EXIT, 0 };
/** Special init event instance. */
static const struct event ev_init = { EV_INIT, 0 };

/**
 * Special token state returned to signal a handled event for an internal
 * transition.
 */
static const struct state handled = { "event_handled", 0, 0};

/**
 * Special token state returned to signal an external self transition.
 */
static const struct state self = { "external_self", 0, 0};

/**
 * Determine the lowest common ancestor of two states.
 * The only requirement is that the parameters are elements of non-cyclic
 * linked lists terminated by NULL.
 * Identities:
 * lca(a,b) = lca(b,a)
 * lca(a,0) = 0
 * lca(a,a) = a
 * @param a State
 * @param b State
 * @return Lowest common ancestor
 */
static const struct state *lca(const struct state *a, const struct state *b) {
	int a_gen = 0;
	int b_gen = 0;

	/* Find the generation (number of parents) of a and b. The null
	 * (root) state counts as a parent. This could be cached in the
	 * state if it was writable. */
	for (const struct state *p = a; p; p = p->parent) {
		a_gen++;
	}
	for (const struct state *p = b; p; p = p->parent) {
		b_gen++;
	}

	/* If there is a common ancestor, it must be at a generation
	 * less than that of both a and b. So move a or b up in the
	 * tree until their generations are equal. */
	while (a_gen > b_gen) {
		a = a->parent;
		a_gen--;
	}
	while (b_gen > a_gen) {
		b = b->parent;
		b_gen--;
	}

	/* Because the number of parents for both a and b are now equal
	 * we can move them in unison up the tree and test for equality
	 * at each generation. The search terminates when the same node
	 * is found in both branches, in which case we have the lca, or
	 * when both are null, in which case the nodes are in disjoint
	 * root branches and the lca is the null root node. */
	while (a != b) {
		a = a->parent;
		b = b->parent;
	}

	return a; /* or b, whatever, they're equal */
}

/**
 * Exit from every state to its parent, while sending exit events, until
 * arriving at the goal state, which must be an ancestor to the current state.
 * @param sm Statemachine
 * @param goal The goal state
 */
static void statemachine_exit_until(struct statemachine *sm, const struct state *goal)
{
	assert(sm != 0);
	while (sm->state != goal) {
		assert(sm->state != 0); /* Trying to exit to a non-ancestor */
		assert(sm->state->func != 0); /* Malformed state */
		const struct state *ret = sm->state->func(sm, &ev_exit);
		assert(ret == &handled || ret == 0); /* Trying to transition from exit */
		if (ret == &handled) {
			// HzS LOG_DEBUG("%s::%s(exit)\n", sm->name, sm->state->name);
		}
		sm->state = sm->state->parent;
	}
}

/**
 * Follow the path that will lead to the goal state, sending entry events to
 * all entered states in turn. The goal state must be a descendant to the
 * current state.
 * @param sm Statemachine
 * @param goal The goal state
 */
static void statemachine_entry_until(struct statemachine *sm, const struct state *goal)
{
	assert(sm != 0);
	assert(goal != 0); /* Trying to enter a non-state or to a non-descendant */
	if (sm->state == goal) {
		/* Already done, don't send an event. */
		return;
	}
	if (sm->state != goal->parent) {
		/* First make sure to go to the parent, recursively. */
		statemachine_entry_until(sm, goal->parent);
	}
	sm->state = goal;
	assert(sm->state->func != 0); /* Malformed state */
	const struct state *ret = sm->state->func(sm, &ev_entry);
	assert(ret == &handled || ret == 0); /* Trying to transition from entry */
	if (ret == &handled) {
		// HzS LOG_DEBUG("%s::%s(entry)\n", sm->name, sm->state->name);
	}
}

/**
 * Helper function for downwards transition, including initial transition
 * recursion.
 * @param sm Statemachine
 * @param target Target state
 */
static void statemachine_transition_down(struct statemachine *sm, const struct state *target)
{
	statemachine_entry_until(sm, target);
	/* If the target state is composite, it may define an initial transition
	 * in which case we should follow that, recursively.
	 * Note that the initial transition is mandatory for a composite state in
	 * UML2, since a leaf state must always be active. We don't enforce it. */
	const struct state *initial = sm->state->func(sm, &ev_init);
	if (initial != 0) {
		assert(initial != &handled && initial != &self); /* Invalid return value for init event */
		assert(target != initial); /* Initial transition cannot go to itself */
		assert(target == lca(target, initial)); /* Initial transition must go to a substate */
		//LOG_DEBUG("%s::%s(initial) -> %s\n", sm->name, target->name, initial->name);
		statemachine_transition_down(sm, initial);
	}
}

/**
 * Perform a regular, local transition.
 * When transitioning between a superstate and a substate, the superstate will
 * not be exited and re-entered.
 * @param sm Statemachine
 * @param source Source state of the transition
 * @param target The target state of the transition
 */
static void statemachine_transition(struct statemachine *sm, const struct state *source, const struct state *target)
{
	assert(sm != 0);
	assert(target != 0); /* Trying to transition to a non-state */
	const struct state *common = lca(source, target);
	statemachine_exit_until(sm, common);
	statemachine_transition_down(sm, target);
}

/**
 * Perform an external transition.
 * When transitioning between a superstate and a substate (including
 * self-transitions), the superstate will be exited and re-entered. Otherwise,
 * it behaves like a regular transition.
 * @param sm Statemachine
 * @param source The source state of the transition
 * @param target The target state of the transition
 */
static void statemachine_external_transition(struct statemachine *sm, const struct state *source, const struct state *target)
{
	assert(sm != 0);
	assert(target != 0); /* Trying to transition to a non-state */
	const struct state *common = lca(source, target);
	if (common != 0 && (common == source || common == target)) {
		common = common->parent;
	}
	statemachine_exit_until(sm, common);
	statemachine_transition_down(sm, target);
}

/**
 * Executes a specific state of a state machine.
 * Only for internal use during recursion.
 * @param sm Statemachine
 * @param state The state to execute
 * @param event The event that triggered the execution
 */
static void statemachine_exec_inner(struct statemachine *sm, const struct state *state, const struct event *event)
{
	assert(state->func != 0); /* Malformed state */

	const struct state *next = state->func(sm, event);
	if (next == 0) {
		/* Unhandled event, pass on to parent, if any. */
		if (state->parent != 0 && event->id != EV_TIMEOUT) {
			statemachine_exec_inner(sm, state->parent, event);
		}
	} else if (next == &self) {
		/* External self transition. There's no way currently to request an
		 * external transition to sub- or super-state. */
		// HzS LOG_DEBUG("%s::%s(%d,%lu) x-> %s\n", sm->name, state->name, event->id, event->data, next->name);
		statemachine_external_transition(sm, state, state);
	} else if (next != &handled) {
		/* Regular transition. These are local. */
		// HzS LOG_DEBUG("%s::%s(%d,%lu) -> %s\n", sm->name, state->name, event->id, event->data, next->name);
		statemachine_transition(sm, state, next);
	} else if (event->id != EV_DO) {
		/* The state function consumed the event, we're done. */
		// HzS LOG_DEBUG("%s::%s(%d,%lu)\n", sm->name, state->name, event->id, event->data);
	}
}

/**
 * Executes a step of a state machine.
 * @param sm Statemachine
 * @param event The event that triggered the execution
 */
static inline void statemachine_exec(struct statemachine *sm, const struct event *event)
{
	assert(sm != 0);
	assert(sm->state != 0);

	statemachine_exec_inner(sm, sm->state, event);
}

/**
 * Initializes a state machine instance.
 *
 * Sets up internal data structures but does not perform the initial
 * transition. The initial transition may be deferred until all state machines
 * have been added to a hsm container, so all will see events triggered by
 * the initial transition.
 *
 * @param sm Statemachine
 * @param initial The initial transition
 * @return Positive integer on success, zero on allocation failure.
 * @sa hsm_start_all
 */
int statemachine_create(struct statemachine *sm, const struct state *initial)
{
	assert(sm != 0);
	sm->state = 0;
	sm->initial = initial;
#if SM_INTERNAL_EVENT_QUEUE
	sm->events = queue_create(SM_EVENT_QUEUE_LEN, sizeof(struct event));
	if (sm->events == 0)
		return 0;
#endif
	for (size_t i = 0; i < COUNTOF(sm->timers); i++) {
		sm->timers[i].remaining = -1;
	}
	return 1;
}

/**
 * Initializes a state machine instance.
 *
 * Sets up internal data structures and performs the initial transition.
 *
 * @param sm Statemachine
 * @param initial The initial transition
 * @return Positive integer on success, zero on allocation failure.
 */
int statemachine_init(struct statemachine *sm, const struct state *initial)
{
	int ret = statemachine_create(sm, initial);
	if (ret != 1) {
		return ret;
	}
	statemachine_transition(sm, 0, sm->initial);
	return 1;
}

/**
 * Send an event synchronously to a state machine.
 * @param sm Statemachine
 * @param id Event ID
 * @param data Associated event data
 */
void statemachine_send_sync(struct statemachine *sm, int id, intptr_t data)
{
	struct event ev = { id, data };
	statemachine_exec(sm, &ev);
}

#if SM_INTERNAL_EVENT_QUEUE
/**
 * Queue an event from one state machine to another.
 *
 * @note It's usually better to avoid coding dependencies between state machines
 * like this. Use hsm_broadcast() if possible.
 * @sa hsm_broadcast
 *
 * @param sm The calling state machine. This is only used for the debug print,
 *           which is rather ugly.
 * @param dst The target state machine
 * @param id Event ID
 * @param data Associated event data
 */
void statemachine_sendto(struct statemachine *sm, struct statemachine *dst,
                         int id, intptr_t data)
{
	struct event ev = { id, data };
	if (queue_put(dst->events, &ev)) {
		LOG_DEBUG("%s->%s(%d,%lu)\n", sm->name, dst->name, id, data);
	} else {
		// HzS LOG_DEBUG("%s event queue full\n", dst->name);
	}
}

/**
 * Defer an event.
 * @todo This mechanism needs to be tested, it might not work very well.
 * @param sm Statemachine
 * @param id Event ID
 * @param data Associated event data
 */
void statemachine_defer(struct statemachine *sm, int id, intptr_t data)
{
	struct event ev = { id, data };
	if (queue_put(sm->events, &ev)) {
		// HzS LOG_DEBUG("%s defer(%d,%lu)\n", sm->name, id, data);
	} else {
		// HzS LOG_DEBUG("%s event queue full\n", sm->name);
	}
}
#endif

/**
 * Return this in response to handled internal events (entry, exit, do).
 * @return Special state token
 */
const struct state *statemachine_event_handled(void)
{
	return &handled;
}

/**
 * Return this to perform a self transition.
 * @return Special state token
 */
const struct state *statemachine_self_transition(void)
{
	return &self;
}

/**
 * Subscribe a state machine to DO events.
 * This is typically done in the entry action of states that use DO events.
 * @param sm Statemachine
 */
void statemachine_subscribe_do(struct statemachine *sm)
{
	sm->do_subscribed++;
}

/**
 * Unsubscribe a state machine from DO events.
 * This is typically done in the exit action of states that use DO events.
 * Subscriptions are recursive so they need to be unregistered the same number
 * of times as they were subscribed.
 * @param sm Statemachine
 */
void statemachine_unsubscribe_do(struct statemachine *sm)
{
	if (sm->do_subscribed > 0) {
		sm->do_subscribed--;
	}
}

void statemachine_subscribe_timeout(struct statemachine *sm, int ms)
{
	if (sm->level >= SM_NUM_TIMERS) {
		// HzS LOG_DEBUG("Timer list full.");
		return;
	}
	assert(ms > 0);
	assert(sm->timers[sm->level].remaining == -1);
	sm->timers[sm->level].remaining = ms;
	sm->timers[sm->level].state = sm->state;
	sm->level++;
}

void statemachine_unsubscribe_timeout(struct statemachine *sm)
{
	if (sm->level <= 0) {
		// HzS LOG_DEBUG("Timer misuse detected.");
		return;
	}
	sm->level--;
	sm->timers[sm->level].remaining = -1;
}

/**
 * Increment time, sending timeout event as required.
 * @param sm Statemachine
 * @param ms Number of ms elapsed since last call.
 */
void statemachine_increment_time(struct statemachine *sm, int ms)
{
	for (int i = 0; i < sm->level; i++) {
		if (sm->timers[i].remaining >= ms) {
			sm->timers[i].remaining -= ms;
		} else if (sm->timers[i].remaining > 0) {
			sm->timers[i].remaining = 0;
		}
	}

	/* Send the timeout event in a separate pass, because transitions may
	 * arm new timers that must not be decremented now, also the level may
	 * change during the loop. */
	for (int i = 0; i < sm->level; i++) {
		if (sm->timers[i].remaining == 0) {
			/* Must disable timer before sending the event to avoid clobbering
			 * the timeout if a transition changes it. */
			sm->timers[i].remaining = -1;
			const struct event timeout = { EV_TIMEOUT, 0 };
			statemachine_exec_inner(sm, sm->timers[i].state, &timeout);
		}
	}
}

/**
 * Find the time remaining until the next timeout elapses.
 * @param sm Statemachine
 * @return The number of ms until the next timeout, or -1
 *         if no timeouts are pending.
 */
int statemachine_get_next_timeout(struct statemachine *sm)
{
	int remaining = -1;
	for (int i = 0; i < sm->level; i++) {
		int this_remaining = sm->timers[i].remaining;
		if (remaining > this_remaining || remaining < 0) {
			remaining = this_remaining;
		}
	}
	return remaining;
}

#if SM_CONTAINER_API
/**
 * Initialize a state machine container.
 * @param l Storage for a struct hsm_list, or NULL to allocate it.
 * @return A new state machine container, or NULL on allocation failure.
 */
struct hsm_list *hsm_list_init(struct hsm_list *l)
{
	struct hsm_list *l_alloced = NULL;

	if (l == NULL) {
		l_alloced = malloc(sizeof(*l_alloced));
		l = l_alloced;
	}

	if (l == NULL) {
		return NULL;
	}

	memset(l, 0, sizeof(*l));
	l->q = queue_create(SM_EVENT_QUEUE_LEN, sizeof(struct event));

	if (l->q == NULL) {
		free(l_alloced);
		return NULL;
	}

	return l;
}

/**
 * Add a state machine to a container.
 * @param list Container
 * @param sm Statemachine
 */
void hsm_add(struct hsm_list *list, struct statemachine *sm)
{
	struct statemachine **s;

	for (s = &list->first; *s != NULL; s = &(*s)->next) {
	}

	*s = sm;
	sm->list = list;
}

/**
 * Delete a state machine from a container.
 * @param list Container
 * @param sm Statemachine
 */
void hsm_del(struct hsm_list *list, struct statemachine *sm)
{
	struct statemachine *s;

	for (s = list->first; s != NULL; s = s->next) {
		if (s->next == sm) {
			s->next = sm->next;
			sm->next = NULL;
		}
	}
}

/**
 * Perform the initial transitions for all state machines in a container.
 * @param list Container
 */
void hsm_start_all(const struct hsm_list *list)
{
	for (struct statemachine *sm = list->first; sm; sm = sm->next) {
		statemachine_transition(sm, 0, sm->initial);
	}
}

/**
 * Process the event queues of all state machines in a container.
 * @param list Container
 */
bool hsm_process_queue(const struct hsm_list *list)
{
	bool did_exec = false;
	struct event broadcast;
	bool has_broadcast = queue_get(list->q, &broadcast);

	for (struct statemachine *sm = list->first; sm; sm = sm->next) {
		struct event ev;
		if (queue_get(sm->events, &ev)) {
			statemachine_exec(sm, &ev);
			did_exec = true;
		}
		if (has_broadcast) {
			statemachine_exec(sm, &broadcast);
			did_exec = true;
		}
		if (sm->do_subscribed) {
			statemachine_send_sync(sm, EV_DO, 0);
		}
	}
	return did_exec;
}

/**
 * Broadcast an event to all state machines in a container.
 * @param list Container
 * @param id Event ID
 * @param data Associated event data
 */
void hsm_broadcast_group(struct hsm_list *list, int id, intptr_t data)
{
	struct event ev = { id, data };
	if (queue_put(list->q, &ev)) {
		// HzS LOG_DEBUG("->*(%d,%lu)\n", id, data);
	} else {
		// HzS LOG_DEBUG("broadcast queue full\n");
	}
}

/**
 * Broadcast an event to all state machines in the same container.
 * @param sm Statemachine
 * @param id Event ID
 * @param data Associated event data
 */
void hsm_broadcast(struct statemachine *sm, int id, intptr_t data)
{
	struct event ev = { id, data };
	if (sm->list == NULL) {
		if (!queue_put(sm->events, &ev)) {
			// HzS LOG_DEBUG("%s event queue full\n", sm->name);
		}
	} else if (queue_put(sm->list->q, &ev)) {
		// HzS LOG_DEBUG("%s->*(%d,%lu)\n", sm->name, id, data);
	} else {
		// HzS LOG_DEBUG("broadcast queue full\n");
	}
}

/**
 * Increment time, sending timeout event as required.
 * @param list Container
 * @param ms Number of ms elapsed since last call.
 */
void hsm_increment_time(struct hsm_list *list, int increment)
{
	for (struct statemachine *sm = list->first; sm; sm = sm->next) {
		statemachine_increment_time(sm, increment);
	}
}

/**
 * Find the time remaining until the next timeout elapses.
 * @param list Container
 * @return The number of ms until the next timeout, or -1
 *         if no timeouts are pending.
 */
int hsm_get_next_timeout(struct hsm_list *list)
{
	int timeout = -1;
	for (struct statemachine *sm = list->first; sm; sm = sm->next) {
		int this_timeout = statemachine_get_next_timeout(sm);
		if (timeout > this_timeout || timeout < 0) {
			timeout = this_timeout;
		}
	}
	return timeout;
}
#endif
