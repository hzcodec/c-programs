/*
    Auther       : Heinz Samuelsson
    Date         : 2019-05-07
    File         : rotate.c
    Reference    : https://en.wikipedia.org/wiki/Circular_shift
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>  // for uint32_t, to get 32-bit-wide rotates, regardless of the size of int.
#include <limits.h>  // for CHAR_BIT
 
/*
 * Shift operations in C are only defined for shift values which are
 * not negative and smaller than sizeof(value) * CHAR_BIT.
 * The mask, used with bitwise-and (&), prevents undefined behaviour
 * when the shift count is 0 or >= the width of unsigned int.
 */
uint16_t rotl16 (uint16_t value, unsigned int count) {
    const unsigned int mask = CHAR_BIT * sizeof(value) - 1;
    count &= mask;
    return (value << count) | (value >> (-count & mask));
}

uint16_t rotr16 (uint16_t value, unsigned int count) {
    const unsigned int mask = CHAR_BIT * sizeof(value) - 1;
    count &= mask;
    return (value >> count) | (value << (-count & mask));
}

 
int main(int argc, char *argv[])
{
	uint16_t indata = 0x8000;
	uint16_t outdata;

	outdata = rotr16(indata, 1);
	printf("Rotate 1:  outdata=%04x\n", outdata);

	outdata = rotr16(indata, 15);
	printf("Rotate 15: outdata=%04x\n", outdata);

	outdata = rotr16(indata, 16);
	printf("Rotate 16: outdata=%04x\n", outdata);

	outdata = rotr16(indata, 30);
	printf("Rotate 30: outdata=%04x\n", outdata);

        return 0;
}
