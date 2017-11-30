/*
    Auther       : Heinz Samuelsson
    Date         : 2017-11-30
    File         : statemachine2.c
    Reference    : -
    Description  : Statemachine with a table.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool buttonPressed = false;

#define STATE_NAME_2_STRING(_str) _str == STATE_START ? "STATE_START" : \
                                  _str == STATE_INITIALIZED ? "STATE_INITIALIZED" : \
                                  _str == STATE_ERROR ? "STATE_ERROR" : \
                                  _str == STATE_IDLE ? "STATE_IDLE" : \
                                  _str == STATE_GO ? "STATE_GO" : "UNKNOWN"

#define EVENT_NAME_2_STRING(_str) _str == EV_INIT ? "EV_INIT" : \
                                  _str == EV_ERROR ? "EV_ERROR" : \
                                  _str == EV_IDLE ? "EV_IDLE" : "UNKNOWN"


void led_init() {printf("%s() - \n", __func__);}
void led_initialized() {printf("%s() - \n", __func__);}
void led_error() {printf("%s() - \n", __func__);}
void led_idle() {printf("%s() - \n", __func__);}
void led_go() {printf("%s() - \n", __func__);}


typedef struct {
    const char* name;
    void (*func)(void);
} stateFunction_t;


static stateFunction_t stateFunctionA[] = {
    {"STATE_START",         &led_init},
    {"STATE_INITIALIZED",   &led_initialized},
    {"STATE_ERROR",         &led_error},
    {"STATE_IDLE",          &led_idle},
    {"STATE_GO",            &led_go},
};

// define events
typedef enum {
    EV_INIT,
    EV_ERROR,
    EV_IDLE,
} events_t;

// define states
typedef enum {
    STATE_START,
    STATE_INITIALIZED,
    STATE_ERROR,
    STATE_IDLE,
    STATE_GO,
} states_t;

typedef struct {
    states_t currentState;
} stateMachine_t;

typedef struct {
    states_t currentState;
    events_t event;
    states_t nextState;
} FSM_t;

// FSM table
FSM_t fsm_events[] = {
		      //  current state     event       next state
                      { STATE_START,        EV_IDLE,    STATE_INITIALIZED  },
                      { STATE_START,        EV_ERROR,   STATE_ERROR        },
                      { STATE_INITIALIZED,  EV_IDLE,    STATE_IDLE         },
                      { STATE_GO,           EV_ERROR,   STATE_IDLE         },
};


void StateMachine_Init(stateMachine_t * stateMachine) {
    printf("%s() - Initializing state machine\n", __func__);
    stateMachine->currentState = STATE_START;
}


const char * StateMachine_GetStateName(states_t state) {
    printf("%s() - ", __func__);
    return stateFunctionA[state].name;
}


int StateMachine_getEvent(void)
{
    if (buttonPressed)
    {
        printf("%s() - buttonPressed=%d\n", __func__, buttonPressed);
	buttonPressed = false;
        return EV_IDLE;
    }
}


int main(int argc, char *argv[])
{
    stateMachine_t stateMachine;
    StateMachine_Init(&stateMachine);

    printf("%s() - State is now: %s\n", __func__, STATE_NAME_2_STRING(stateMachine.currentState));
    int size_of_FSM_struct =  (sizeof(fsm_events) / sizeof(fsm_events[0]));

    buttonPressed = true;

    events_t event = StateMachine_getEvent();
    printf("event=%s\n", EVENT_NAME_2_STRING(event));

    // find state and event
    for (int check_state=0; check_state<size_of_FSM_struct; check_state++)
    {
        if (fsm_events[check_state].currentState == stateMachine.currentState)
        {
            if (fsm_events[check_state].event == event)
	    {
		// transition to next state
	        stateMachine.currentState = fsm_events[check_state].nextState;

		// call associated function for the current transition
	        (stateFunctionA[stateMachine.currentState].func)();
		break;
	    }
	}

        else
        {
            printf("No hit\n");
        }
    }

    return 0;
}

