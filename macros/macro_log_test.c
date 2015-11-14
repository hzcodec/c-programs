#include <stdio.h>
#include <stdlib.h>

// (condition) ? (do something) : (do something else)
// if _value less than 0 then *2 else *10
#define LOG printf("Line: %d in file '%s' (function: %s)\n", \
                    __LINE__, __FILE__, __func__) 


void dummy()
{
  LOG;
}

int main()
{
   
  LOG;
  dummy();

  return 0;
}

