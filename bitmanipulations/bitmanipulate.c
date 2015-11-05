/*

    Auther      : Heinz Samuelsson
    Date        : 2015-11-05
    File        : bitmanipulate.c
    Reference   : -
    Description : Different ways to manipulate bits.

                  gcc -std=c99 bitmanipulate.c -o bitmanipulate
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main()
{

  uint8_t number = 0x0a;
  int x = 2;

  // ********************************************************************
  //Setting a bit. Use the bitwise OR operator (|) to set a bit.
  // That will set bit x.
  // ********************************************************************
  number |= 1 << x;
  printf("number = %02x\n", number);
  
  // ********************************************************************
  // Clearing a bit. Use the bitwise AND operator (&) to clear a bit.
  // That will clear bit x. You must invert the bit string with 
  // the bitwise NOT operator (~), then AND it.
  // ********************************************************************
  number = 0x0a;
  x = 3;
  number &= ~(1 << x);
  printf("number = %02x\n", number);
  
  // ********************************************************************
  // Toggling a bit. The XOR operator (^) can be used to toggle a bit.
  // That will toggle bit x.
  // ********************************************************************
  number = 0x0a;
  x = 0;
  number ^= 1 << x;
  printf("number = %02x\n", number);
  
  // ********************************************************************
  // Checking a bit.  You didn't ask for this but I might as well add it.
  // To check a bit, shift the number x to the right, then bitwise AND it.
  // That will put the value of bit x into the variable bit.
  // ********************************************************************
  number = 0x0a;
  x = 1;
  int bit = (number >> x) & 1;
  printf("bit = %d\n", bit);
  
  // ********************************************************************
  // Changing the nth bit to x.
  // Setting the nth bit to either 1 or 0 can be achieved with the following.
  // Bit n will be set if x is 1, and cleared if x is 0.
  // ********************************************************************
  number = 0x0a;
  int n = 2;
  x = 1;
  number ^= (-x ^ number) & (1 << n);
  printf("number = %02x\n", number);

  return 0;
}

/*
    Result:

      number = 0e
      number = 02
      number = 0b
      bit = 1
      number = 0e
*/
