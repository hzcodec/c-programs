/*
   gcc -std=gnu99 logfile.o test_file.c -o test_file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "logfile.h"

extern int  debugFlagOn;
extern int  debugLevel;
extern void internalLogPrintf();

void f1()
{
    int var1 = 100;
    int var2 = 110;
    LOG_PRINT(DEBUG1, "Var1=%d, Var2=%d\n", var1, var2);
}

void f2()
{
    int var1 = 200;
    char name[10] = "Jennie";
    LOG_PRINT(DEBUG2, "Var1=%d, Name=%s\n", var1, name);
}

void f3() {
    int var1 = 300;
    LOG_PRINT(WARNING, "Var1=%d\n", var1);
}

void f4() {
    int var1 = 400;
    LOG_PRINT(ERROR, "Var1=%d\n", var1);
}


int main()
{
    f1();
    f2();
    f3();
    f4();

    return 0;
}

