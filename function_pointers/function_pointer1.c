/*
    Auther       : Heinz Samuelsson
    Date         : 2016-08-05
    File         : function_pointer1.c
    Reference    : http://www.gammon.com.au/callbacks
    Description  : Example of function pointer.
*/ 
 
#include <stdio.h>
#include <stdlib.h>


typedef void (*GenericMessageFunction)();


void sayHello ()
  {
    printf("Hello\n");  
  }

void sayGoodbye ()
  {
    printf("Goodbye\n");  
  }


void checkPin (const int pin, GenericMessageFunction response)
{
  printf("pin= %d\n", pin);
  response();
}


int main()
{
  checkPin(8, sayHello);    
  checkPin(9, sayGoodbye);

  return 0;
}
