/*
    Auther       : Heinz Samuelsson
    Date         : 2019-01-31
    File         : quad1.c
    Reference    : -
    Description  : https://cdn.sparkfun.com/datasheets/Robotics/How%20to%20use%20a%20quadrature%20encoder.pdf
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
 
 
typedef enum
{
  POS,  // 0
  NEG,  // 1
  IDLE,  // 2
} Directions;

#define ENUM2STRING(_str) _str == POS ? "Postive dir" : \
                          _str == NEG ? "Negative dir" : \
                          _str == IDLE ? "IDLE" : "UNKNOWN"

int main(int argc, char *argv[])
{
 
	int QEM[16] = {0, -1, 1, 2, 1, 0, 2, -1, -1, 2, 0, 1, 2, 1, -1, 0};
	int old = 0;
	int new = 0;
	int out = 0;
	int inputA = 0;
	int inputB = 0;

	// negative direction
	int a[8] = {0, 0, 1, 1, 0, 0, 1, 1};
	int b[8] = {0, 1, 1, 0, 0, 1, 1, 0};

	// positive direction
	//int a[8] = {0, 1, 1, 0, 0, 1, 1, 0};
	//int b[8] = {0, 0, 1, 1, 0, 0, 1, 1};

	// no direction
	//int a[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	//int b[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	for (int i=0; i<8; i++) {
		inputA = a[i];
		inputB = b[i];

		old = new;
		new = inputA*2 + inputB;
		//printf("[%d] - inputA=%d, inputB=%d, new=%d\n", i, inputA, inputB, new);
		out = QEM[old*4 + new];
		//printf("out=%d\n", out);

		switch(out) 
		{
			case 0:    printf("%s\n", ENUM2STRING(IDLE)); break;
			case 1:    printf("%s\n", ENUM2STRING(POS)); break;
			case -1:   printf("%s\n", ENUM2STRING(NEG)); break;
			default: printf("---\n");
		}
	}

        return 0;
}
