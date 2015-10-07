/*
    Auther      : Heinz Samuelsson
    Date        : 2015-10-07
    File        : define1.c
    Reference   : -
    Description : --
                  gcc define1.c -D FLAG=1 -o define
                  gcc define1.c -D FLAG=0 -o define
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(void) {

#if FLAG == 0

#define func1()
    printf("%s(): FLAG = 0\n", __func__);

#else

#define func1()
    printf("%s(): FLAG = 1\n", __func__);
#endif

    return 0;
}

