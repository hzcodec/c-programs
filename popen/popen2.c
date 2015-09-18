/*
    Auther      : Heinz Samuelsson
    Date        : 2015-09-19
    File        : popen2.c
    Reference   : -
    Description : Call a function from another file using popen().

    > gcc -std=gnu99 popen2.c -o popen2
    > gcc -std=gnu99 dummy2.c -o dummy2

    How to run:
      > popen2
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    char msg1[30] = {0};
    char msg2[30] = {0};
    int  data;

    fp = popen("./dummy2 6", "r");

    if (fp == NULL)
    {
        printf("Error! [%s]\n", strerror(errno));
        return -1;
    }

    printf("***********************************************\n");

    int rv = fscanf(fp, "%s%s%d", msg1, msg2, &data);
    printf("Number of items:%d\n",rv);

    printf("-> %s\n", msg1);
    printf("-> %s\n", msg2);
    printf("-> %d\n", data);

    printf("***********************************************\n");

    pclose(fp);

    return 0;
}

