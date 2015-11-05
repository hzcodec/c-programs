/*

    Auther      : Heinz Samuelsson
    Date        : 2015-11-05
    File        : checkbit.c
    Reference   : -
    Description : Different ways to check if a bit is set
                  or not.

                  gcc -std=c99 checkbit.c -o checkbit
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define CHECK_BIT(val, pos) (((val) >> (pos)) & 1)

typedef struct
{
  int portBitValue;
} PORT_BITMAP_VALUE;


int main()
{
  PORT_BITMAP_VALUE res;
  int               bitSet;
  uint32_t          bitMap = 0xa;

  // find bit value
  for (int i=0; i<8; i++)
  {
    res.portBitValue = ((1 << i) & bitMap);
    printf("[%d] - portBitValue: %d\n", i, res.portBitValue);
  }

  printf("------------------------\n");

  // find if bit is set
  for (int i=0; i<8; i++)
  {
    bitSet = (((bitMap) >> i) & 1);
    printf("bit[%d] - bitSet: %d\n", i, bitSet);
  }

  printf("------------------------\n");

  // find which bit that is set
  for (int i=0; i<8; i++)
  {
    if (CHECK_BIT(bitMap, i))
    {
      printf("bit[%d]\n", i);
    }
  }

  return 0;
}


