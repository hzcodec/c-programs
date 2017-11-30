/*
    Auther       : Heinz Samuelsson
    Date         : 2017-11-30
    File         : statemachine1.c
    Reference    : -
    Description  : Statemachine with a table.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define colors
#define CNRM "\x1B[0m"
#define CRED "\x1B[31m"
#define CGRN "\x1B[32m"
#define CYEL "\x1B[33m"
#define CBLU "\x1B[34m"
#define CMAG "\x1B[35m"
#define CCYN "\x1B[36m"
#define CWHT "\x1B[37m"
#define BOLD "\x1B[1m"

/* FSM States */
typedef enum {
               STATE_START, 
	       STATE_INITIALIZED, 
	       STATE_GO,
} states_t;

/* FSM Events */
typedef enum {
              EVENT_NONE,
              EVENT_INIT,
              EVENT_ERROR,
              EVENT_IDLE,
} events_t;


#define STATE_NAME_2_STRING(_str) _str == STATE_START ? "STATE_START" : \
                                  _str == STATE_INITIALIZED ? "STATE_INITIALIZED" : \
                                  _str == STATE_GO ? "STATE_GO" : "UNKNOWN"

#define EVENT_NAME_2_STRING(_str) _str == EVENT_NONE ? "EVENT_NONE" : \
                                  _str == EVENT_INIT ? "EVENT_INIT" : \
                                  _str == EVENT_ERROR ? "EVENT_ERROR" : \
                                  _str == EVENT_IDLE ? "EVENT_IDLE" : "UNKNOWN"


int initialize(void)
{
    /* Initialize FSM here */
    printf("  %s() - \n", __func__);

    return (STATE_INITIALIZED);
}

int error_handler(void)
{
    printf("  %s() - \n", __func__);

    return (STATE_START);
}

int idle_handler(void)
{
    printf("  %s() - \n", __func__);

    return (STATE_START);
}


/* FSM actions and transitions */
struct FSM{
   states_t state;
   events_t event;
   int (*fn)(void);
};


// FSM table
struct FSM fsm[] = {
		   //  state              event           f()
                   { STATE_START,        EVENT_INIT,    initialize      },
                   { STATE_START,        EVENT_ERROR,   error_handler   },
                   { STATE_INITIALIZED,  EVENT_IDLE,    idle_handler    },
                   { STATE_GO,           EVENT_ERROR,   error_handler   },
};


int get_event(void)
{
    return EVENT_ERROR;
}

int main(int argc, char *argv[])
{
    events_t event;

    int size_of_FSM_struct =  (sizeof(fsm) / sizeof(fsm[0]));

    states_t current_state = STATE_START;
    printf("%s() - current state=%s%s%s\n", __func__, CYEL, STATE_NAME_2_STRING(current_state), CNRM);

    event = get_event();

    printf("%s() - event=%s%s%s\n", __func__, CRED, EVENT_NAME_2_STRING(event), CNRM);

    // find state and event
    for (int check_state=0; check_state<size_of_FSM_struct; check_state++)
    {
        if (fsm[check_state].state == current_state)
	{
            printf("%s() - current state OK\n", __func__);

	    // if event exists in table and corresponds to current state then call function
            if (fsm[check_state].event == event)
            {
                printf("%s() - state=%s%s%s, event=%s%s%s\n", __func__, CYEL, STATE_NAME_2_STRING(current_state), CNRM, CRED, EVENT_NAME_2_STRING(event), CNRM);
                fsm[check_state].fn();
		break;
            }
	}
	else
	{
	    printf("No matching state and event\n");
	}
    }

    return 0;
}

