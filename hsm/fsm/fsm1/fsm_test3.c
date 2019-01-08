/*
    Auther       : Heinz Samuelsson
    Date         : 2019-01-08
    File         : fsm_test3.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include "fsm.h"

typedef enum {
	State_A,
	State_B,
	State_C,
}FSM1_State_t;

typedef enum {
	State_D,
	State_E,
}FSM2_State_t;

static int signal1;
 
 
FSM(my_First_FSM, FSM1_State_t, State_A)

	STATE(State_A)
		printf("State_A\n");
		NEXT_STATE(State_B);
	END_STATE;
	
	STATE(State_B)
		for (int i=0; i<2; i++) {
			printf("State_B [%d]\n", i);
		}
	
		NEXT_STATE(State_C);
	END_STATE;
	
	STATE(State_C)
		printf("State_C, signal1 is set\n");
		signal1 = 1;
		NEXT_STATE(State_A);
	END_STATE;

END_FSM(my_First_FSM);


FSM(my_Second_FSM, FSM2_State_t, State_D)

	STATE(State_D)
		printf("  State_D\n");
		if (signal1 == 1) {
			NEXT_STATE(State_E);
		}
	END_STATE;
	
	STATE(State_E)
		printf("  State_E\n");
		NEXT_STATE(State_D);
	END_STATE;
	
END_FSM(my_Second_FSM);

int main(int argc, char *argv[])
{
	int looping = 0;
	FSM1_State_t fsm_status1;
	FSM2_State_t fsm_status2;

	while(looping < 5) {
		fsm_status1 = my_First_FSM();
		fsm_status2 = my_Second_FSM();
		looping++;
	}
 
        return 0;
}

