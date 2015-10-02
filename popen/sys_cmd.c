/*
    Auther      : Heinz Samuelsson
    Date        : 2015-10-02
    File        : sys_cmd.c
    Reference   : -
    Description : Linux command 'ls' and 'pwd' are called. Also send_message.
                  An example of using popen() to call other functions.

                  gcc -std=gnu99 sys_cmd.c -o sys_cmd
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 50


int executeSystemCommand(char* pString)
{
    FILE *fp = NULL;
    char pBuffer[BUFFER_SIZE];

    printf("--- Executed command: %s\n", pString);

    fp = popen(pString, "r");
    if (fp == NULL)
    {
        return -1;
    }

    while (fgets(pBuffer, BUFFER_SIZE, fp) != NULL)
    {
      printf("%s", pBuffer);
    }

    pclose(fp);
    return 0;
}


int main(int argc, char *argv[])
{

    char command[20] = "ls";
    int  rv = 0;

    printf("*****************************\n");
    rv = executeSystemCommand(command);
    if (rv < 0)
    {
        printf("Executed command failed!\n");
    }

    printf("*****************************\n");
    rv = executeSystemCommand("pwd");
    if (rv < 0)
    {
        printf("Executed command failed!\n");
    }
    printf("*****************************\n");

    rv = executeSystemCommand("./send_message 5");
    if (rv < 0)
    {
        printf("Executed command failed!\n");
    }
    printf("\n*****************************\n");

    rv = executeSystemCommand("./send_message 9");
    if (rv < 0)
    {
        printf("Executed command failed!\n");
    }
    printf("\n*****************************\n");

    return 0;
}

