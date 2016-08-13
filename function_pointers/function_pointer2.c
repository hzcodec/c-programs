/*
    Auther       : Heinz Samuelsson
    Date         : 2016-08-05
    File         : function_pointer2.c
    Reference    : -
    Description  : Test of function pointers using typedef.

                   Declaring a function pointer:
		     return_type (*pointer_name)(parameter types)

		   Setting an ordinary function pointer to point to a function:
		     pointer_name = function_name   // simple
		     pointer_name = &function_name  // verbose

		   Calling a function using a pointer:
		     pointer_name(arguments);
		     (*poiter_name)(arguments);

*/ 
 
#include <stdio.h>
#include <stdlib.h>
 
typedef struct {
  int a;
  int b;
}Object;


typedef void (*callbackFunc)(int a);
typedef int (*callbackFunc2)(int a, int b);
typedef void (*callbackFunc3)(Object obj);


void myFunc1(int a)
{
  printf("a: %d\n", a);
}

void myFunc2(int b)
{
  printf("b: %d\n", b);
}

void myFunc3(int c)
{
  printf("c: %d\n", c);
}

int myFunc4(int d, int e)
{
  printf("d: %d\n", d);
  printf("e: %d\n", e);
  return 200;
}

void myFunc5(Object obj)
{
  printf("obj.a= %d\n", obj.a);
  printf("obj.b= %d\n", obj.b);
}


int main()
{

  void (*cb)(int a);
  
  cb = &myFunc1;
  cb(99);

  callbackFunc cbFunc = myFunc2;
  cbFunc(44);

  cbFunc = myFunc3;
  cbFunc(55);


  // functions with typedefs
  callbackFunc2 cbFunc2 = myFunc4;
  int rv = cbFunc2(555, 666);
  printf("rv: %d\n", rv);

  // functions with a struct as inparameter
  Object m = {88, 77};
  callbackFunc3 cbFunc5 = myFunc5;
  cbFunc5(m);

  return 0;
}
