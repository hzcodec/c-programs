/*
    Auther       : Heinz Samuelsson
    Date         : 2017-11-30
    File         : statemachine2.c
    Reference    : -
    Description  : Statemachine with a table.
    		   N.B!  Not finished.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define STATE_NAME_2_STRING(_str) _str == STATE_START ? "STATE_START" : \
//                                  _str == STATE_INITIALIZED ? "STATE_INITIALIZED" : \
//                                  _str == STATE_ERROR ? "STATE_ERROR" : \
//                                  _str == STATE_IDLE ? "STATE_IDLE" : \
//                                  _str == STATE_BLOCK ? "STATE_BLOCK" : \
//                                  _str == STATE_GO ? "STATE_GO" : "UNKNOWN"
//
//#define EVENT_NAME_2_STRING(_str) _str == EV_NONE ? "EV_NONE" : \
//                                  _str == EV_INIT ? "EV_INIT" : \
//                                  _str == EV_ERROR ? "EV_ERROR" : \
//                                  _str == EV_IDLE ? "EV_IDLE" : "UNKNOWN"
//

//int initialize(void)
//{
//    /* Initialize FSM here */
//    printf("  %s() - \n", __func__);
//
//    return (STATE_INITIALIZED);
//}
//
//int error_handler(void)
//{
//    printf("  %s() - \n", __func__);
//
//    return (STATE_START);
//}
//
//int idle_handler(void)
//{
//    printf("  %s() - \n", __func__);
//
//    return (STATE_START);
//}


/* FSM actions and transitions */
//struct FSM{
//   states_t state;
//   events_t event;
//   int (*fn)(void);
//};
#define STATE_NAME_2_STRING(_str) _str == STATE_START ? "STATE_START" : \
                                  _str == STATE_INITIALIZED ? "STATE_INITIALIZED" : \
                                  _str == STATE_GO ? "STATE_GO" : "UNKNOWN"

typedef enum {
    EV_INIT,
    EV_ERROR,
    EV_IDLE,
} events_t;

typedef enum {
    STATE_START,
    STATE_INITIALIZED,
    STATE_ERROR,
    STATE_IDLE,
    STATE_GO,
} states_t;

typedef struct {
    states_t currState;
} stateMachine_t;

typedef struct {
    states_t currState;
    events_t event;
    states_t nextState;
} stateTransMatrixRow_t;

// FSM table
stateTransMatrixRow_t fsm_events[] = {
		                  //  current state     event       next state
                                  { STATE_START,        EV_INIT,    STATE_INITIALIZED  },
                                  { STATE_START,        EV_ERROR,   STATE_ERROR        },
                                  { STATE_INITIALIZED,  EV_IDLE,    STATE_IDLE         },
                                  { STATE_GO,           EV_ERROR,   STATE_IDLE         },
};


void StateMachine_Init(stateMachine_t * stateMachine) {
    printf("%s() - Initializing state machine\n", __func__);
    stateMachine->currState = STATE_START;
}

const char * StateMachine_GetStateName(states_t state) {
    printf("%s() - ", __func__);

    //return stateFunctionA[state].name;
}

int fsm_get_event(void)
{
    return EV_ERROR;
}

int main(int argc, char *argv[])
{
    stateMachine_t stateMachine;
    StateMachine_Init(&stateMachine);

    printf("%s() - State is now: %s\n", __func__, STATE_NAME_2_STRING(stateMachine.currState));

    events_t event = fsm_get_event();

    //int size_of_FSM_struct =  (sizeof(fsm) / sizeof(fsm[0]));

    //states_t current_state = STATE_START;
    //printf("current state=%s\n", STATE_NAME_2_STRING(current_state));

    //event = get_event();

    //printf("%s() - event=%s\n", __func__, EVENT_NAME_2_STRING(event));

    //// find state and event
    //for (int check_state=0; check_state<size_of_FSM_struct; check_state++)
    //{
    //    if (fsm[check_state].state == current_state)
    //    {
    //        printf("%s() - current state OK\n", __func__);

    //        // if event exists in table and corresponds to current state then call function
    //        if (fsm[check_state].event == event)
    //        {
    //            printf("%s() - state=%s, event=%s\n", __func__, STATE_NAME_2_STRING(current_state), EVENT_NAME_2_STRING(event));
    //            fsm[check_state].fn();
    //    	break;
    //        }
    //    }
    //    else
    //    {
    //        printf("No matching state and event\n");
    //    }
    //}

    return 0;
}

