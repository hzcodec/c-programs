/*
 *   gcc -std=gnu99 dummy.c -o dummy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE* fp;
    int   data;
    char  msg[15] = "OK";
    char  buffer[50];

    char* arg1 = argv[1];
    int inputArgument = atoi(arg1);

    if (inputArgument == 5)
    {
        data = 55;
    }

    else if (inputArgument == 6)
    {
        data = 66;
    }

    else
    {
        data = 99;
        strcpy(msg, "INVALID");
    }

    sprintf(buffer, "Message: %s %d", msg, data);
    printf(buffer);

    return 0;
}

