/*
    Auther       : Heinz Samuelsson
    Date         : 2016-01-15
    File         : logging2.c
    Reference    : -
    Description  : - 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void test(char* format, ...)
{
    va_list arglist;
    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);
}


int main()
{

    test("%s %d %s\n", "Failed", 100, "times");
    test("%s %s %d %s\n", "Test", "OK", 99, "times");

    return 0;
}

