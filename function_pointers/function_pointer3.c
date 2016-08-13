/*
    Auther       : Heinz Samuelsson
    Date         : 2016-08-05
    File         : function_pointer3.c
    Reference    : -
    Description  : Example of function pointer. Using an in parameter.
*/ 
 
#include <stdio.h>
#include <stdlib.h>


typedef void (*GenericMessageFunction)(int a);


void sayHello(int a)
{
    printf("Hello -> a=%d\n", a);  
}

void sayGoodbye(int a)
{
    printf("Goodbye -> a=%d\n", a);  
}


void checkPin (int pin, GenericMessageFunction response)
{
  printf("pin= %d\n", pin);
  response(pin);
}


int main()
{
  checkPin(8, sayHello);    
  checkPin(9, sayGoodbye);

  return 0;
}
