/*
    Auther       : Heinz Samuelsson
    Date         : 2016-01-18
    File         : logging3.c
    Reference    : -
    Description  : Logging function.
                   Function, filename and row is printed together with variables.

                   gcc -std=gnu99 logging3.c -o logging3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef struct
{
  int  data1;
  char char1;
} CompositeDate;

void internalLogPrintf(char* format, ...)
{
  va_list arglist;
  va_start(arglist, format);
  vprintf(format, arglist);
  va_end(arglist);
}

#define LOG_PRINT(txt...) \
                  printf("%s() - [%s %d] ", __func__, __FILE__, __LINE__);\
                  internalLogPrintf(txt);

void f1()
{
  int  a = 99;
  int  b = 88;
  char name[10] = "Jennie";

  CompositeDate myData = {200, 'Q'};

  LOG_PRINT("Name=%s, DataA=%d, DataB=%d, Data1=%d, Char1=%c\n", \
            name, a, b, myData.data1, myData.char1);
}


int main()
{

  f1();
  return 0;
}

