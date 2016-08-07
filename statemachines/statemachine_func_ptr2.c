/*
    Auther       : Heinz Samuelsson
    Date         : 2016-08-07
    File         : statemachine_func_ptr.c
    Reference    : -
    Description  : Same as statemachine_func_ptr.c but with input parameter to each state.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


// function prototypes
void Sm_StateA(int a);
void Sm_StateB(int a);
void Sm_StateC(int a);
void Sm_StateD(int a);
 
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
  void (*func)(int a);
}StateMachineType;
 

StateMachineType StateMachine[] =
{
  {STATE_A, Sm_StateA},
  {STATE_B, Sm_StateB},
  {STATE_C, Sm_StateC},
  {STATE_D, Sm_StateD}
};

// function for state A
void Sm_StateA(int a)
{
  printf("State A, a=%d\n", a);
  SmState = STATE_B;
}

// function for state B
void Sm_StateB(int b)
{
  printf("State B, b=%d\n", b);
  SmState = STATE_C;
}

// function for state C
void Sm_StateC(int c)
{
  printf("State C, c=%d\n", c);
  SmState = STATE_D;
}

// function for state D
void Sm_StateD(int d)
{
  printf("State D, d=%d\n", d);
  SmState = STATE_A;
}

// This function is running the state machine
void Sm_Run(int var)
{

  // just make sure the state is valid
  if(SmState < NUM_STATES)
  {
    (*StateMachine[SmState].func)(var);

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

  // run the state machine with input parameter
  for (int i=0; i<5; i++)
  {
    Sm_Run((i+1)*100);
  }

  return 0;
}

