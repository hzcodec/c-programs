/*
    Auther       : Heinz Samuelsson
    Date         : 2018-03-26
    File         : enum_in_struct.c
    Reference    : -
    Description  : Enum:s in struct.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
 
struct interface{
        enum {
	    START,
	    STOP,
	    IDLE,
	} state;

	int a;
	int b;
};
 
int main(int argc, char *argv[])
{
 
        struct interface if0;

	if0.state = IDLE;
	if0.a = 200;
	printf("state=%d, a=%d\n", if0.state, if0.a);

	if0.state = START;
	if0.a = 2;
	printf("state=%d, a=%d\n", if0.state, if0.a);

        return 0;
}
 /* Result:
      state=2, a=200
      state=0, a=2
 */
