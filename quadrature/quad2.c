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

// define colors
#define CNRM "\x1B[0m"
#define CRED "\x1B[31m"
#define CGRN "\x1B[32m"
#define CYEL "\x1B[33m"
#define CBLU "\x1B[34m"

#define POSDIR CGRN "Positive direction" CNRM
#define NEGDIR CBLU "Negative direction" CNRM
#define ERRDIR CRED "ERROR" CNRM
#define IDLEDIR CYEL "IDLE" CNRM
 
#define ASIZE 12

typedef enum
{
  POS,  // 0
  NEG,  // 1
  ERR,
  IDLE,  // 2
} Directions;

#define ENUM2STRING(_str) _str == POS ? POSDIR : \
                    _str == NEG ? NEGDIR : \
                    _str == ERR ? ERRDIR : \
                    _str == IDLE ? IDLEDIR : "UNKNOWN"

//                          0   1  2  3  4  5  6   7   8  9  0  1  2  3   4  5
static const int QEM[16] = {0, -1, 1, 2, 1, 0, 2, -1, -1, 2, 0, 1, 2, 1, -1, 0};
static int cnt_quad;

int main(int argc, char *argv[])
{
	uint16_t old_quad = 0;
	uint16_t new_quad = 0;
	int out_quad = 0;

	//uint16_t a[ASIZE] = {0x300, 0x300, 0x300, 0x300, 0x300, 0x300, 0x300, 0x300, 0x300, 0x300, 0x300, 0x300};
	//uint16_t a[ASIZE] = {0x300, 0x200, 0x000, 0x100, 0x300, 0x200, 0x000, 0x100, 0x300, 0x300, 0x300, 0x300};
	uint16_t a[ASIZE] = {0x300, 0x100, 0x000, 0x200, 0x300, 0x100, 0x000, 0x200, 0x300, 0x300, 0x300, 0x300};

	for (int i=0; i<ASIZE; i++) {

		old_quad = new_quad;
		new_quad = (a[i] >> 8) & 0x03;

		printf("[%d] - old_quad=%02x, new_quad=%02x, idx=%0d, ", i, old_quad, new_quad, old_quad*4+new_quad);

		out_quad = QEM[old_quad*4 + new_quad];
		printf("out_quad=%d  --  ", out_quad);

		switch(out_quad) 
		{
			case 0:   printf("%s\n", ENUM2STRING(IDLE)); break;
			case 1:   printf("%s\n", ENUM2STRING(POS)); cnt_quad++; break;
			case -1:  printf("%s\n", ENUM2STRING(NEG)); cnt_quad--; break;
			case 2:   printf("%s\n", ENUM2STRING(ERR)); break;
			default: break;
		}

		printf("cnt_quad=%d\n", cnt_quad);

		printf("-----------------------------------------------------------------------\n");
	}

        return 0;
}
