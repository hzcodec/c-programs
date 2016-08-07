/*
    Auther       : Heinz Samuelsson
    Date         : 2016-08-07
    File         : statemachine_case2.c
    Reference    : -
    Description  : Statemachine with case statements.
*/ 
 
 
#include <stdio.h>
#include <stdlib.h>

#define IDLE   0
#define SEATED 1
#define BELTED 2
#define BUZZER 3

int state = 0;
int seat  = 1;

 
void FSM()
{
  switch(state)
  {
    case IDLE:
      printf("IDLE\n");
      if (seat)
      {
        state = SEATED;
      }
      break;

    case SEATED:
      printf("SEATED\n");
      if (seat)
      {
        state = BUZZER;
      }
      break;

    case BELTED:
      printf("BELTED\n");
      if (seat)
      {
        state = SEATED;
      }
      else if (!seat)
      {
        state = SEATED;
      }
      break;

    case BUZZER:
      printf("BUZZER\n");
      if (seat)
      {
        state = SEATED;
      }
      else if (!seat)
      {
        state = IDLE;
      }
      break;
  }
}
 
int main()
{
 
  FSM();
  FSM();
  FSM();
  FSM();

  return 0;
}
