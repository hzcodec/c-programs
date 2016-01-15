/*
    Auther       : Heinz Samuelsson
    Date         : 2016-01-15
    File         : logging.c
    Reference    : -
    Description  : - 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void test(char* text, ...)
{
    va_list arglist;
    va_start(arglist, text);
    vprintf(text, arglist);
    va_end(arglist);
    printf("\n");
}

#define LOG_PRINT2(txt...) \
                   printf("%s() - [%s %d]: ", __func__, __FILE__, __LINE__); \
                   test(txt);

int main()
{

    int  a = 99;
    char name[10] = "Mattias";

    LOG_PRINT2(name);

    return 0;
}

