/*
    Auther       : Heinz Samuelsson
    Date         : 2019-01-03
    File         : fsm_test1.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include "fsm.h"

typedef enum {
	BIST_Init,
	BIST_START,
	BIST_Successful,
	BIST_Fail
}BIST_State_t;
 
 
FSM(BIST_FSM, BIST_State_t, BIST_Init)
STATE(BIST_Init)
	printf("Bist_Init\n");
	NEXT_STATE(BIST_Successful);
END_STATE;

STATE(BIST_Successful)
	printf("BIST_Successful\n");
	NEXT_STATE(BIST_Fail);
END_STATE;

STATE(BIST_Fail)
	printf("BIST_Fail\n");
	NEXT_STATE(BIST_Init);
END_STATE;

END_FSM(BIST_FSM);


int main(int argc, char *argv[])
{
	int looping = 0;

	while(looping < 5) {
		BIST_State_t bist_status = BIST_FSM();
		looping++;
	}
 
        return 0;
}

