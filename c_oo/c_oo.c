/*
    Auther      : Heinz Samuelsson
    Date        : Sun Nov 29 01:08:07 CET 2015
    File        : c_oo.c
    Reference   : -
    Description : Object oriented c.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  int x;
  int y;
} DataSet;

typedef struct MAIN_BLOCK
{
  // members
  char  name[20];
  float version;

  // methods
  void (*op)(char*    currentOperation);
  int  (*add)(int* pNum1, int* pNum2);
  void (*sub)(int* pNum1, int* pNum2);
  void (*mul)(DataSet* pNum);

  // internal help functions
  void (*message)();
  void (*printSelf)(struct MAIN_BLOCK* self);
} MAIN_BLOCK;

void operation_op(char* currentOperation)
{
  printf("op: %s\n", currentOperation);
}


int operation_add(int* pNum1, int* pNum2)
{
  printf("Add: %d\n", *pNum1 + *pNum2);
  return 1;
}


void operation_sub(int* pNum1, int* pNum2)
{
  printf("Sub: %d\n", *pNum1 - *pNum2);
}


void operation_mul(DataSet* pNum)
{
  printf("Mul: %d\n", pNum->x * pNum->y);
}

void operation_message()
{
  printf("*** Now a message is printed\n");
}

void operation_printSelf(struct MAIN_BLOCK* self)
{
  printf("---------\n");
  printf("Self - Name:    %s\n", self->name);
  printf("Self - Version: %.2f\n", self->version);
  printf("---------\n");
}

void printValues(MAIN_BLOCK* pObj)
{
  printf("Current Version: %.2f\n", pObj->version);
  printf("Current Name: %\n", pObj->name);
}


void initObject(MAIN_BLOCK* man)
{
  if (man == NULL)
  {
    man = malloc(sizeof(struct MAIN_BLOCK));
  }
 
  strcpy(man->name, "MAIN OPERATIONS");
  man->version    = 1.01;
  man->op         = operation_op;
  man->add        = operation_add;
  man->sub        = operation_sub;
  man->mul        = operation_mul;
  man->message    = operation_message;
  man->printSelf  = operation_printSelf;
}

int main(int argc, char* argv[])
{
  int a     = 10;
  int b     = 20;
  DataSet c = {5, 6};
  int rv    = 0;
  MAIN_BLOCK operation;

  // initialize object
  initObject(&operation);

  printf("Version: %.2f\n", operation.version);
  printf("Name:    %s\n", operation.name);

  operation.op("Adding numbers");
  rv = operation.add(&a, &b);
  printf("--- %d ---\n", rv);

  operation.op("Substracting numbers");
  operation.sub(&a, &b);

  operation.op("Muliplying numbers");
  operation.mul(&c);

  operation.op("Message");
  operation.message();

  operation.op("Print out self data");
  operation.printSelf(&operation);

  return 0;
}
