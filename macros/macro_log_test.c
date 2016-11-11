#include <stdio.h>
#include <stdlib.h>

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

