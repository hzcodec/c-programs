/*
    Auther       : Heinz Samuelsson
    Date         : 2018-01-02
    File         : hsm_control.c
    Reference    : -
    Description  : 

                  +-----------------------------------------------------------+
		  | main                                                      |
		  |                                                           |
		  |    +-----------+       +-------------------------------+  |
		  |    | disabled  |       | enabled                       |  |
		  |    |           |       |                               |  |
		  |    |           |       |  +----------+  +----------+   |  |
		  |    |           |       |  | running  |  | idle     |   |  |
		  |    |           |       |  |          |  |          |   |  |
		  |    |           |       |  +----------+  +----------+   |  |
		  |    +-----------+       +-------------------------------+  |
		  +-----------------------------------------------------------+

		  Every state in a UML statechart can have optional entry actions, which are executed upon entry to a state,
		  as well as optional exit actions, which are executed upon exit from a state.

		  Entry and exit actions are associated with states, not transitions. Regardless of haow a state si entered or exited.
		  All its entry and exit actions will be executed.

		  statemachine_subscribe_do(sm);
		  statemachine_unsubscribe_do(sm);
*/ 
 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "hsm_main.h"
#include "hsm_control.h"
#include "types.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// prototypes for the test cases
void trig_current_fsm(int id);
void log_printf(char* file, const char* func, const struct event* e);


struct main_fsm {
	struct statemachine sm;
	int a;
};


STATE(main_fsm, main_fsm_impl, 0);
  STATE(disabled, disabled_impl, &main_fsm);
  STATE(enabled, enabled_impl, &main_fsm);
    STATE(running, running_impl, &enabled);
    STATE(idle, idle_impl, &enabled);

static struct main_fsm control_instance;


static int flag1 = 1;
static int flag2 = 0;
static int var1;
static int var2;


bool flag_active(void)
{
    return 1;
}

static const struct state *main_fsm_impl(struct statemachine *sm, const struct event *ev)
{
    log_printf(__FILE__, __func__, ev);
    struct main_fsm *m = container_of(sm, struct main_fsm, sm);
    m->a = 99;

    switch (ev->id)
    {
        case EV_ENTRY: {
            printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
	    var1 = 10;
            return statemachine_event_handled();
        }

        case EV_INIT: {
            statemachine_subscribe_do(sm);
            printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
            return &disabled;
        }
    
        case EV_DO: {
            printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
	    printf("%s() - var1=%d, var2=%d\n", __func__, var1, var2);
            break;
        }
    }

    return 0;
}

static const struct state *disabled_impl(struct statemachine *sm, const struct event *ev)
{
    log_printf(__FILE__, __func__, ev);
    struct main_fsm *m = container_of(sm, struct main_fsm, sm);
    m->a = 99;

    switch (ev->id)
    {
        case EV_ENTRY: {
            printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
	    printf("%s() - var1=%d, var2=%d\n", __func__, var1, var2);
            return statemachine_event_handled();
        }

        case EV_DO: {
            if (flag1 == 1)
	    {
                printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
	        return &enabled;
	    }

            printf("%s() -%s%s%s, This only happens if flag=0\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
	    break;
	}

        case EV_EXIT: {
            printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
	    var1 = 30;
	    var2 = 20;
            return statemachine_event_handled();
        }
    }

    return 0;
}

static const struct state *enabled_impl(struct statemachine *sm, const struct event *ev)
{
    log_printf(__FILE__, __func__, ev);

    switch (ev->id) 
    {
        case EV_ENTRY: {
        	hsm_broadcast(sm, EV_CTRL_ENABLED, CTRL_SOURCE_LOCAL);
        	printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);

        	return statemachine_event_handled();
        }

        case EV_INIT: 
        {
            printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
            return &running;
        }

        case EV_DO: {
        	if (flag1 == 0)
		{
		    return &disabled;
		}
		break;
	}

        case EV_EXIT: {
        	hsm_broadcast(sm, EV_CTRL_DISABLED, CTRL_SOURCE_LOCAL);
        	printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);

        	return statemachine_event_handled();
        }
    }
    return 0;
}

static const struct state *running_impl(struct statemachine *sm, const struct event *ev)
{
    struct main_fsm *m = container_of(sm, struct main_fsm, sm);
    m->a = 99;

    log_printf(__FILE__, __func__, ev);
    printf("%s() - var1=%d, var2=%d\n", __func__, var1, var2);

    switch (ev->id)
    {
	case EV_ENTRY: {
            //c->control_source = CTRL_SOURCE_LOCAL;
            return statemachine_event_handled();
	    break;
	}

        case EV_INIT: {
            printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
            break;
        }

        case EV_DO: {
	    if (flag2 == 0)
	    {
                return &idle;
	    }
            break;
        }
    }
    return 0;
}


static const struct state *idle_impl(struct statemachine *sm, const struct event *ev)
{
    log_printf(__FILE__, __func__, ev);

    switch (ev->id) {

        case EV_ENTRY: {
        	hsm_broadcast(sm, EV_CTRL_ENABLED, CTRL_SOURCE_LOCAL);
        	printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);

        	return statemachine_event_handled();
        }

        case EV_INIT: {
            printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
            break;
        }

        case EV_DO: {
            printf("%s() -%s%s%s\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);

            if (flag2 != 0)
            {
	       return &running; 
            }

	    else if (flag2 == 0)
	    {
	       var1 = 99;
	       return &disabled; 
	    }
            break;
        }
    }

    return 0;
}


struct statemachine *hsm_control_init(void)
{
        printf("------------------ %s[%s] - %s() %s -------------------\n", BYEL,  __FILE__, __func__, NORM);
	control_instance.sm.name = "main_fsm";
	statemachine_create(&control_instance.sm, &main_fsm);
	return &control_instance.sm;
}


// =====================================================================================================================================
static struct hsm_list hsms_storage;
static struct hsm_list *hsms;


// =====================================================================================================================================
int main(int argc, char* argv[])
{

    // dummy prints to get rid of warnings
    printf("argc=%c\n", argc);
    printf("argv=%s\n\n", argv[0]);

    printf("%s\n\n\n", FSM_BLOCK);

    // build up HSM
    hsms = hsm_list_init(&hsms_storage);
    hsm_add(hsms, hsm_control_init());
    hsm_start_all(hsms);

    trig_current_fsm(ID1);
    //trig_current_fsm(ID2);

    return 0;
}


void trig_current_fsm(int id)
{
    for (int index=0; index<6; index++)
    {
	printf("%s [%d.%d] %s\n", DELIMITER, id, index, DELIMITER);
        hsm_process_queue(hsms);
    }
    printf("\n\n");
}


void log_printf(char* file, const char* func, const struct event* e)
{
    printf("[%s]: %s%s()%s -%s%s%s -  flag1=%d\n", 
                                 file, 
				 BCYAN, func, NORM,
				 EVENTCOL, ENUM2STRING(e->id), NORM, 
				 flag1
				 );
}

