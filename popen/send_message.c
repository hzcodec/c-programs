/*
    Auther      : Heinz Samuelsson
    Date        : 2015-10-02
    File        : send_message.c
    Reference   : -
    Description : sys_cmd is accessing the file.

                  gcc -std=gnu99 send_message.c -o send_message
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 50

int test_function(int par, char* message)
{
    char  msg[15] = "OK";
    strcpy(message, msg);

    // check input parameter
    if (par == 5)
    {
        return 55;
    }

    else if (par == 6)
    {
        return 66;
    }

    // if input data is invalid i.e not '5' or '6'
    else
    {
        strcpy(message, "INVALID");
        return 99;
    }

    return 0;
}


int main(int argc, char *argv[])
{
    FILE* fp;
    int   data;
    char  msg[15];
    char  buffer[BUFFER_SIZE];

    // get first argument and convert to an integer
    char* arg1 = argv[1];
    int inputArgument = atoi(arg1);

    data = test_function(inputArgument, msg);

    // build up message and print it out
    sprintf(buffer, "Message: %s %d", msg, data);
    printf(buffer);

    return 0;
}

