/*
    Auther       : Heinz Samuelsson
    Date         : 2019-01-07
    File         : fsm_test2.c
    Reference    : -
    Description  : Rullade upp defines.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
 
//          my_Fsm  FSM_State_t  FSM_Init
#define FSM(_name_, _type_, _init_) \
	static _type_ _name_##_state = (_init_); \
	_type_ _name_(void) { \
	static _type_ _prev_state = (_init_); \
	static _type_ _next_state = (_init_); \
	_type_ _state = _name_##_state; \
	switch (_state) {

#define STATE(_state_) case (_state_): {
#define NEXT_STATE(_state_) _next_state = (_state_)
#define END_STATE } break;
#define END_FSM(_name_) } \
	_prev_state = _state; \
	_state = _next_state; \
	_name_##_state = _state; \
	return _state; \
}
 
typedef enum {
	FSM_Init,
	FSM_Successful,
	FSM_Fail
}FSM_State_t;
 
static FSM_State_t my_FSM_state = FSM_Init;

FSM_State_t my_FSM(void) {
	static FSM_State_t _prev_state = FSM_Init;
	static FSM_State_t _next_state = FSM_Init;
	
	FSM_State_t _state = my_FSM_state;
	
	switch(_state) {
	case FSM_Init: { printf("FSM_Init A\n");
	                 _next_state = FSM_Successful; 
			} break;
	
	case FSM_Successful: { printf("FSM_Init B\n");
	                 _next_state = FSM_Init; 
			} break;
	
	}

	_prev_state = _state;
	_state = _next_state;
	my_FSM_state = _state;
	printf("_prev_state=%d, _next_state=%d, _state=%d\n", _prev_state, _next_state, _state);

	return _state;
}

int main(int argc, char *argv[])
{
	FSM_State_t fsm_status;

	for (int i=0; i<3; i++) {
		fsm_status = my_FSM();
	}

        return 0;
}
