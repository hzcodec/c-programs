/*
   gcc -Wall -DDEBUG_ON=1 -c -DLEVEL=2 logfile.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "logfile.h"

// select debug ON/OFF
#if DEBUG_ON == 1
  int debugFlagOn = 1;
#else
  int debugFlagOn = 0;
#endif

// select level
#if LEVEL == 1
  int debugLevel = 1;
#elif LEVEL == 2
  int debugLevel = 2;
#else
  int debugLevel = 0;
#endif

void internalLogPrintf(char* format, ...)
{
  va_list arglist;
  va_start(arglist, format);
  vprintf(format, arglist);
  va_end(arglist);
}

