/*
    Auther       : Heinz Samuelsson
    Date         : 2019-02-12
    File         : fsm_with_case.c
    Reference    : -
    Description  : One of many examples of a simple FSM.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
 
static int state;
static int loop;

int main(int argc, char *argv[])
{
	while (loop < 8) {
		loop++;

 		switch (state) {

		case 0: printf("State 0, state=%d\n", state);
			state++;
			break;

		case 1: printf("State 1, state=%d\n", state);
			state++;
			break;

		case 2: printf("State 2, state=%d\n", state);
			state++;
			break;

		case 3: printf("State 3, state=%d\n", state); // restore and go to IDLE mode
			state = 0;
			break;

		// IDLE
		default: break;
		}
	}

        return 0;
}

