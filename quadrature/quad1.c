/*
    Auther       : Heinz Samuelsson
    Date         : 2019-01-31
    File         : quad1.c
    Reference    : -
    Description  : https://cdn.sparkfun.com/datasheets/Robotics/How%20to%20use%20a%20quadrature%20encoder.pdf

                   The binary values above convert to 0,1,3,2 or 0,2,3,1 depending on the direction.
		   This pattern repeats continuously.

		   By using the current value from the encoder to index one dimension of the array and the
                   previous value to index the other dimension you can quickly get a -1, 0, or +1 output.

		   As you can see, if the value has not changed then the output is 0.
		   The sequence of 0, 1, 3, 2 gives an output of -1.
                   The sequence of 0, 2, 3, 1 gives an output of +1.

                   2 represents a disallowed state and would most likely occur if the encoder outputs are changing too
                   quickly for your code to keep up. Normally this should not happen.

		   When I get an output of 2 I know that I got an error, perhaps due to electrical noise or my code being too slow.
		   If you replace 2 with 0 then the disallowed state will be ignored.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
typedef enum
{
  POS,  // 0
  NEG,  // 1
  IDLE,  // 2
} Directions;

#define ENUM2STRING(_str) _str == POS ? "Postive dir" : \
                          _str == NEG ? "Negative dir" : \
                          _str == IDLE ? "Idle" : "UNKNOWN"

int main(int argc, char *argv[])
{
 
	int QEM[16] = {0, -1, 1, 2, 1, 0, 2, -1, -1, 2, 0, 1, 2, 1, -1, 0};
	int old = 0;
	int new = 0;
	int out = 0;
	int inputA = 0;
	int inputB = 0;

	// negative direction
	//int a[8] = {0, 0, 1, 1, 0, 0, 1, 1};
	//int b[8] = {0, 1, 1, 0, 0, 1, 1, 0};

	// positive direction
	int a[8] = {0, 1, 1, 0, 0, 1, 1, 0};
	int b[8] = {0, 0, 1, 1, 0, 0, 1, 1};

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
