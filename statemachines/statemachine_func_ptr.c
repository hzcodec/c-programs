/*
    Auther       : Heinz Samuelsson
    Date         : 2016-08-07
    File         : statemachine_func_ptr.c
    Reference    : http://www.edn.com/electronics-blogs/embedded-basics/4406821/Function-pointers---Part-3--State-machines
    Description  : Example of statemachine.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// function prototypes
void Sm_StateA(void);
void Sm_StateB(void);
void Sm_StateC(void);
void Sm_StateD(void);
 
// define states for the FSM
typedef enum
{
  STATE_A,
  STATE_B,
  STATE_C,
  STATE_D,
  NUM_STATES
}StateType;

StateType SmState = STATE_A;

typedef struct
{
  StateType State;
  void (*func)(void);
}StateMachineType;
 

StateMachineType StateMachine[] =
{
  {STATE_A, Sm_StateA},
  {STATE_B, Sm_StateB},
  {STATE_C, Sm_StateC},
  {STATE_D, Sm_StateD}
};

// function for state A
void Sm_StateA(void)
{
  printf("State A\n");
  SmState = STATE_B;
}

// function for state B
void Sm_StateB(void)
{
  printf("State B\n");
  SmState = STATE_C;
}

// function for state C
void Sm_StateC(void)
{
  printf("State C\n");
  SmState = STATE_D;
}

// function for state D
void Sm_StateD(void)
{
  printf("State D\n");
  SmState = STATE_A;
}

// This function is running the state machine
void Sm_Run(void)
{

  // just make sure the state is valid
  if(SmState < NUM_STATES)
  {
    (*StateMachine[SmState].func)();

    // just print out next state for test purpose
    printf("Next state: %d\n", StateMachine[SmState].State);
  }
  else
  {
    // Throw an exception
  }
}


int main()
{

  // run the state machine
  for (int i=0; i<5; i++)
  {
    Sm_Run();
  }

  return 0;
}

