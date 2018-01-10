#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef SM_INTERNAL_EVENT_QUEUE
#define SM_INTERNAL_EVENT_QUEUE 1
#endif
#ifndef SM_CONTAINER_API
#define SM_CONTAINER_API 1
#elif SM_CONTAINER_API && !SM_INTERNAL_EVENT_QUEUE
#error SM_CONTAINER_API requires SM_INTERNAL_EVENT_QUEUE
#endif
#ifndef SM_NUM_TIMERS
#define SM_NUM_TIMERS 8
#endif

#include <stdint.h>
#if SM_INTERNAL_EVENT_QUEUE
#include "queue.h"
#endif

struct statemachine {
	const char *name;
	const struct state *state;
	const struct state *initial;
#if SM_INTERNAL_EVENT_QUEUE
	queue_t *events;
#endif
	struct statemachine *next; /**< For use by the framework */
	struct hsm_list *list; /* For use by the framework */
	int do_subscribed; /* For use by the framework */
	int level;
	struct {
		int remaining;
		const struct state *state;
	} timers[SM_NUM_TIMERS];
};

#if SM_CONTAINER_API
struct hsm_list {
	struct statemachine *first;
	queue_t *q;
};
#endif

struct event {
	int id;
	intptr_t data;
};

struct state {
	const char *name;
	const struct state *(*func)(struct statemachine *, const struct event *);
	const struct state *parent;
};

/** Event ID codes. Negative codes are defined by the framework. */
enum event_id {
	EV_INIT = -1,
	EV_ENTRY = -2,
	EV_EXIT = -3,
	EV_DO = -4,
	EV_TIMEOUT = -5,
	EV_USER = 0, /**< Base number for common event codes. */
	EV_PRIVATE = 10000, /**< Base number for per-statemachine events. */
};

#define STATE(_name, _func, _parent) \
static const struct state *_func(struct statemachine *, const struct event *); \
static const struct state _name = { #_name, _func, _parent };

#define STATE_DECL(_name) \
static const struct state _name;

#define STATE_DEF(_name, _parent) \
static const struct state *_name##_impl(struct statemachine *, const struct event *); \
static const struct state _name = { #_name, _name##_impl, _parent }; \
static const struct state *_name##_impl(struct statemachine *sm, const struct event *ev)

#define AFTER(n) \
if (ev->id == EV_ENTRY) statemachine_subscribe_timeout(sm, n); \
if (ev->id == EV_EXIT) statemachine_unsubscribe_timeout(sm); \
if (ev->id == EV_TIMEOUT)

#define STATEMACHINE(_name, _initial) \
struct statemachine _name = { #_name, _initial };

const struct state *statemachine_event_handled(void);
const struct state *statemachine_self_transition(void);
int statemachine_create(struct statemachine *sm, const struct state *initial);
int statemachine_init(struct statemachine *sm, const struct state *initial);
void statemachine_send_sync(struct statemachine *sm, int id, intptr_t data);
#if SM_INTERNAL_EVENT_QUEUE
void statemachine_send(struct statemachine *sm, struct statemachine *dst,
                       int id, intptr_t data);
void statemachine_defer(struct statemachine *sm, int id, intptr_t data);
#endif
void statemachine_subscribe_do(struct statemachine *sm);
void statemachine_unsubscribe_do(struct statemachine *sm);
void statemachine_subscribe_timeout(struct statemachine *sm, int ms);
void statemachine_unsubscribe_timeout(struct statemachine *sm);

struct hsm_list *hsm_list_init(struct hsm_list *l);
void hsm_add(struct hsm_list *list, struct statemachine *sm);
void hsm_del(struct hsm_list *list, struct statemachine *sm);
void hsm_start_all(const struct hsm_list *list);
bool hsm_process_queue(const struct hsm_list *list);
void hsm_broadcast_group(struct hsm_list *list, int id, intptr_t data);
void hsm_broadcast(struct statemachine *sm, int id, intptr_t data);
void hsm_increment_time(struct hsm_list *list, int increment);
int hsm_get_next_timeout(struct hsm_list *list);

#endif /* STATEMACHINE_H_ */
