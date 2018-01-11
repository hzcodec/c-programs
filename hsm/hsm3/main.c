/*
    Auther       : Heinz Samuelsson
    Date         : 2018-01-11
    File         : main.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include "hsm_control.h"
#include "hsm_test.h"
 
void trig_current_fsm(int id)
{
    for (int i=0; i<8; i++)
    {
	printf("%s [%d.%d] %s\n", DELIMITER, id, i, DELIMITER);
        hsm_process_queue(hsms);
    }
    printf("\n\n");
}

 
int main(int argc, char *argv[])
{
    // build up HSM
    hsms = hsm_list_init(&hsms_storage);
    hsm_add(hsms, hsm_control_init());
    hsm_add(hsms, hsm_test_init());
    hsm_start_all(hsms);

    trig_current_fsm(ID1);

    return 0;
}
 
    return 0;
}
