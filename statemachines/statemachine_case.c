/*
    Auther       : Heinz Samuelsson
    Date         : 2016-08-07
    File         : statemachine_case.c
    Reference    : http://www.microchip.com/forums/m520736.aspx
    Description  : Statemachine with case statements.
*/ 
 
 
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

 
// define states for the FSM
typedef enum
 {
   IDLE = 0,
   STATE_A,
   STATE_B
 } StateType;
 
StateType state;

// internal flags
bool flag1;
bool flag2;

// function prototypes
void checkBitFlags(void);
void checkState(void);


int main()
{

  state = IDLE;

  checkBitFlags();

  // run state machine
  checkState();
  checkState();
  checkState();

  return 0;
}


void checkState(void)
{
  switch(state)
  {
    case IDLE:
      if(flag1 == true)
      {
        state = STATE_A;
	printf("State IDLE\n");
      }
      break;

    case STATE_A:
      if(flag2 == true)
      {
        state = STATE_B;
	printf("State STATE_A - flag2 true\n");
      }
      else
      {
        state = STATE_B;
	printf("State STATE_A - flag2 false\n");
      }
      break;

    case STATE_B:
      if(flag1 == true)
      {
        state = IDLE;
	printf("State STATE_B\n");
      }
      break;

    default:
      break;
  }
}


void checkBitFlags()
{
  printf("%s() : \n", __func__);
  flag1 = true;
}
