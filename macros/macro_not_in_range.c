/*
    Auther      : Heinz Samuelsson
    Date        : Sat Dec  5 17:34:59 CET 2015
    File        : macro_not_in_range.c
    Reference   : -
    Description : -
*/

#include <stdio.h>
#include <stdlib.h>

// check if less or larger
#define NOT_IN_RANGE(min, requested, max)\
  ((requested) < (min)) || ((requested) > (max)) ? 1:0

int main()
{
  printf("Return value: %d\n", NOT_IN_RANGE(2, 5, 4));

  return 0;
}
