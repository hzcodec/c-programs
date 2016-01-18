/*
    Auther       : Heinz Samuelsson
    Date         : 2016-01-18
    File         : logging4.c
    Reference    : -
    Description  : Logging function.
                   
                   To log debug both DEBUG_ON and LEVEL need to be set.

                   Select debug ON and set debug level.
                   gcc -std=gnu99 -DDDEBUG_ON=1 -DLEVEL=3 -DSUPRESSERR=0 logging4.c -o logging4

                   Turn off debug
                   gcc -std=gnu99 -DDDEBUG_ON=0 logging4.c -o logging4

                   Supress errors
                   gcc -std=gnu99 -DDDEBUG_ON=1 -DLEVEL=3 -DSUPRESSERR=1 logging4.c -o logging4
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// ============================================================================================
// Setup variables at pre processing
//
// select debug ON/OFF
#if DEBUG_ON
  int debugFlagOn = 1;
#else
  int debugFlagOn = 0;
#endif

// select debug level
#if (LEVEL == 1)
  int selectDebugLevel = 1;
#elif (LEVEL == 2)
  int selectDebugLevel = 2;
#elif (LEVEL == 3)
  int selectDebugLevel = 3;
#else
  int selectDebugLevel = 0;
#endif

// supress error message
#if (SUPRESSERR == 1)
  int supressErrors = 1;
#else
  int supressErrors = 0;
#endif
// ============================================================================================

// selected debug level in function call, is compared with debug level at pre processing
typedef enum {
  DEBUG1,
  DEBUG2,
  DEBUG3,
  INFO,
  WARNING,
  ERROR,
} DEBUG_LEVEL;

void internalLogPrintf(char* format, ...)
{
  va_list arglist;
  va_start(arglist, format);
  vprintf(format, arglist);
  va_end(arglist);
}

#define LOG_PRINT(_level, txt...) \
                  if (debugFlagOn == 1 && _level < selectDebugLevel) { \
                     printf("%s() - [%s %d]: ", __func__, __FILE__, __LINE__);\
                     internalLogPrintf(txt);\
                  }\
                  else if (debugFlagOn == 1 && _level == WARNING) {\
                     printf("%s() - [%s %d]: ", __func__, __FILE__, __LINE__);\
                     internalLogPrintf(txt);\
                  }\
                  else if (supressErrors != 1 && _level == ERROR) {\
                     printf("%s() - [%s %d]: ", __func__, __FILE__, __LINE__);\
                     internalLogPrintf(txt);\
                  }\
                  else if (debugFlagOn == 1) {\
                    printf("\n");\
                  }


// ============================================================================================
// Test functions
//
// test function 1 with debug level 1, highest prio
void f1()
{
  int  var1 = 9;
  int  var2 = 8;

  LOG_PRINT(DEBUG1, "Var1=%d, Var2=%d\n", var1, var2);
}


// test function 2 with debug level 2, middle prio
void f2()
{
  int  var1 = 7;
  int  var2 = 6;

  LOG_PRINT(DEBUG2, "Var1=%d, Var2=%d\n", var1, var2);
}


// test function 3 with debug level 3, lowest prio
void f3()
{
  int  var1 = 7;
  int  var2 = 6;

  LOG_PRINT(DEBUG3, "Var1=%d, Var2=%d\n", var1, var2);
}


// test function 4 with debug Warning
void f4()
{
  int  a = 10;
  char message[10] = "Warning";

  LOG_PRINT(WARNING, "Message=%s, Note value is=%d\n", message, a);
}


// test function 5 with debug Error
void f5()
{
  int  a = 100;
  char message[10] = "Error";

  LOG_PRINT(ERROR, "Message=%s, Value to large=%d\n", message, a);
}


// test function 6
void f6()
{
  int  var1 = 200;
  int  var2 = 300;

  LOG_PRINT(DEBUG1, "Var1=%d\n", var1);

  if (var1 == 200)
  {
    printf("  200 is met\n");
    LOG_PRINT(DEBUG2, "Var2=%d\n", var2);
  }
}

int main()
{

  printf("-----------------------------------------------------------\n");
  printf("Debug on/off=%d, Debug level=%d, Supress errors=%d\n", 
          debugFlagOn, selectDebugLevel, supressErrors);

  f1();
  f2();
  f3();
  f4();
  f5();
  f6();

  return 0;
}

