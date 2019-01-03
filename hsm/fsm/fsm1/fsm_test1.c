/*
    Auther       : Heinz Samuelsson
    Date         : 2019-01-03
    File         : fsm_test1.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"

typedef enum {
	FSM_Init,
	FSM_START,
	FSM_Successful,
	FSM_Fail
}FSM_State_t;
 
 
FSM(FSM_FSM, FSM_State_t, FSM_Init)

STATE(FSM_Init)
	printf("FSM_Init\n");
	NEXT_STATE(FSM_Successful);
END_STATE;

STATE(FSM_Successful)
	printf("FSM_Successful\n");
	NEXT_STATE(FSM_Fail);
END_STATE;

STATE(FSM_Fail)
	printf("FSM_Fail\n");
	NEXT_STATE(FSM_Init);
END_STATE;

END_FSM(FSM_FSM);


int main(int argc, char *argv[])
{
	int looping = 0;

	while(looping < 5) {
		FSM_State_t fsm_status = FSM_FSM();
		looping++;
	}
 
        return 0;
}

