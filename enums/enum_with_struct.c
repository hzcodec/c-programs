/*
    Auther       : Heinz Samuelsson
    Date         : 2018-08-17
    File         : enum_with_strutct.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ENUM2STRING(_str) _str == CMD_MODE_STATUS_CHECK ? "CMD_MODE_STATUS_CHECK" : \
                          _str == CMD_MP_RESET ? "CMD_MP_RESET" : \
                          _str == MP_WRITE_SREG ? "MP_WRITE_SREG" : \
                          _str == IDLE ? "IDLE" : \
                          _str == OK ? "OK" : \
                          _str == NOK ? "NOK" : \
			  "UNKNOWN"

struct test_information {
	enum {
		CMD_MODE_STATUS_CHECK = 0,
		CMD_MP_RESET,
		MP_WRITE_SREG,
		IDLE,
		NO_STATE,
	} state;

	enum {
		OK = NO_STATE+1,
		NOK,
        } status;
};
static struct test_information ti = {CMD_MODE_STATUS_CHECK, OK};


void fsm(void)
{
	int a = 3;
	int b = 1;

	// fsm is active as long as there is no error
	if (ti.status == OK) {

		switch (ti.state)
		{
 			case CMD_MODE_STATUS_CHECK:
				printf("Test: %s\n", ENUM2STRING(ti.state));
				ti.state = CMD_MP_RESET;
				break;

 			case CMD_MP_RESET:
				printf("Test: %s\n", ENUM2STRING(ti.state));
				if (a == b) {
					//printf("OK\n");
				        ti.state = MP_WRITE_SREG;
				}
				else {
					ti.status = NOK;
					printf("      %s, CMD_MP_RESET test failed\n", ENUM2STRING(ti.status));
				}
				break;

 			case MP_WRITE_SREG:
				printf("Test: %s\n", ENUM2STRING(ti.state));
				ti.state = IDLE;
				break;

 			case IDLE:
				printf("Test: %s\n", ENUM2STRING(ti.state));
				break;

			default:
				printf("Error\n");
		} // switch (ti.state)

	} // if (ti.status == OK)
}
 
int main(int argc, char *argv[])
{
	printf("%s() - ti.status=%s, ti.state=%s\n\n", __func__, ENUM2STRING(ti.status), ENUM2STRING(ti.state));

	for (int i=0; i<7; i++)
	{
		fsm();
	}

        return 0;
}
