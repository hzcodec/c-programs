/*
    Auther       : Heinz Samuelsson
    Date         : 2018-01-02
    File         : hsm_control.c
    Reference    : -
    Description  : 

                  +---------------------------------------+
                  | main                                  |
                  |                                       |
                  |    +-----------+     +-----------+    |
                  |    | disabled  |     | enabled   |    |
                  |    |           |     |           |    |
                  |    |           |     |           |    |
                  |    |           |     |           |    |
                  |    |           |     |           |    |
                  |    |           |     |           |    |
                  |    +-----------+     +-----------+    |
                  +---------------------------------------+

                  EV_INIT : must go to a sub state
                  EV_INIT : cannot go to itself
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

struct main_fsm {
	struct statemachine sm;
	int a;
} myFSM;


// define all states for the FSM
STATE(main_fsm, main_fsm_impl, 0);
  STATE(disabled, disabled_impl, &main_fsm);
  STATE(enabled, enabled_impl, &main_fsm);

static struct main_fsm control_instance;


static int flag1 = 1;
static int flag2 = 0;


static const struct state *main_fsm_impl(struct statemachine *sm, const struct event *ev)
{
    struct main_fsm *m = container_of(sm, struct main_fsm, sm);
    m->a = 99;

    switch (ev->id)
    {
        case EV_ENTRY: {
            statemachine_subscribe_do(sm);  // if this one is removed then the FSM stops at disabled
            printf("%s(1) -%s%s%s\n", __func__, BMAG, ENUM2STRING(ev->id), NORM);
            return statemachine_event_handled();
        }

        case EV_INIT: {
            //statemachine_subscribe_do(sm);  // if this one is removed then the FSM stops
            printf("%s(2) -%s%s%s\n", __func__, BMAG, ENUM2STRING(ev->id), NORM);
            return &disabled;
        }
    
        case EV_DO: {
            printf("%s(3) -%s%s%s\n", __func__, BMAG, ENUM2STRING(ev->id), NORM);
	    flag2++;
            break;
        }
    }

    return 0;
}

static const struct state *disabled_impl(struct statemachine *sm, const struct event *ev)
{
    struct main_fsm *m = container_of(sm, struct main_fsm, sm);
    m->a = 77;

    switch (ev->id)
    {
        case EV_ENTRY: {
            printf("%s(1) -%s%s%s\n", __func__, EVENTCOL2, ENUM2STRING(ev->id), NORM);
            return statemachine_event_handled();
        }

        case EV_DO: {
            if (flag1 == 1)
	    {
                printf("%s(2) -%s%s%s\n", __func__, EVENTCOL2, ENUM2STRING(ev->id), NORM);
	        return &enabled;
	    }

            printf("%s() -%s%s%s, This only happens if flag1=0\n", __func__, EVENTCOL, ENUM2STRING(ev->id), NORM);
            return statemachine_event_handled();
	}

        case EV_EXIT: {
            printf("%s(3) -%s%s%s\n", __func__, EVENTCOL2, ENUM2STRING(ev->id), NORM);
            return statemachine_event_handled();
        }
    }

    return 0;
}

static const struct state *enabled_impl(struct statemachine *sm, const struct event *ev)
{
    switch (ev->id) 
    {
        case EV_ENTRY: {
            statemachine_subscribe_do(sm);
            printf("%s(1) -%s%s%s\n", __func__, EVENTCOL3, ENUM2STRING(ev->id), NORM);

            return statemachine_event_handled();
        }

        case EV_INIT: 
        {
            printf("%s(2) -%s%s%s\n", __func__, EVENTCOL3, ENUM2STRING(ev->id), NORM);
            break;
        }

        case EV_DO: {
            if (flag1 == 1)
	    {
                return &disabled;
	    }
	    break;
	}

        case EV_EXIT: {
        	printf("%s(3) -%s%s%s\n", __func__, EVENTCOL3, ENUM2STRING(ev->id), NORM);
        	return statemachine_event_handled();
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


// ================================================================================================================
static struct hsm_list hsms_storage;
static struct hsm_list *hsms;

