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
  ERR,  // 2
} Directions;

#define ENUM2STRING(_str) _str == POS ? "Postive dir" : \
                          _str == NEG ? "Negative dir" : \
                          _str == IDLE ? "Idle" : \
                          _str == ERR ? "Error" : "UNKNOWN"

static int QEM[16] = {0, -1, 1, 2, 1, 0, 2, -1, -1, 2, 0, 1, 2, 1, -1, 0};
static int old_quad = 0;
static int new_quad = 0;
static int out_quad = 0;
static int hall_sensor_1 = 0;
static int hall_sensor_2 = 0;
static int cnt = 0;
static int var = 0;

void f1(int i)
{
	//int a[8] = {0, 1, 1, 0, 0, 1, 1, 0};
	//int b[8] = {0, 0, 1, 1, 0, 0, 1, 1};
	int a[8] = {0, 1, 1, 0, 0, 0, 0, 0};
	int b[8] = {0, 0, 1, 0, 0, 0, 0, 0};

	hall_sensor_1 = a[i];
	hall_sensor_2 = b[i];

	old_quad = new_quad;
	new_quad = hall_sensor_1*2 + hall_sensor_2;
	//printf("[%d] - hall_sensor_1=%d, hall_sensor_2=%d, new_quad=%d\n", i, hall_sensor_1, hall_sensor_2, new_quad);
	out_quad = QEM[old_quad*4 + new_quad];
	//printf("out_quad=%d\n", out_quad);

	switch(out_quad) 
	{
		//case 0:    printf("%s\n", ENUM2STRING(IDLE)); break;
		case 0: break;
		case 1:    printf("%s\n", ENUM2STRING(POS)); cnt++; break;
		case -1:   printf("%s\n", ENUM2STRING(NEG)); cnt--; break;
		case 2:   printf("%s\n", ENUM2STRING(ERR)); var = cnt; break;
		default: printf("---\n");
	}
	 
	var = cnt;

	printf("cnt=%d, var=%d\n", cnt, var);
}


int main(int argc, char *argv[])
{

	// negative direction
	//int a[8] = {0, 0, 1, 1, 0, 0, 1, 1};
	//int b[8] = {0, 1, 1, 0, 0, 1, 1, 0};

	// no direction
	//int a[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	//int b[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	// error
	//int a[8] = {1, 1, 1, 1, 0, 0, 0, 0};
	//int b[8] = {0, 0, 0, 1, 0, 0, 0, 0};

	for (int i=0; i<8; i++) {
		f1(i);
	}

        return 0;
}
