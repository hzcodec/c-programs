/*
  Auther      : Heinz Samuelsson
  Date        : Sun Nov 29 21:12:15 CET 2015
  File        : print_format1.c
  Reference   : -
  Description : -
*/

#include <stdio.h>
#include <string.h>

static char classHeadLine[50] = "---- Headline ----";

static void ClassName_PrintHeaderTable(const char* header, int numIndex)
{
  // print formated headline
  printf("\n%45s\n\n", header);

  if (numIndex == 0)
  {
    printf("    No rules in %s\n", header);
  }

  else
  {
    printf("    VALUE1    |   VALUE2  |    NAME              | HANDLE\n");
  }
}


static void ClassName_Internal_Log_Test()
{

  const char* ruleFormat = "%8u      |  %5u    | %4s    | %3u\n";
  int handle     = 5;
  int numIndex   = 2;
  int localValue2 = 99;
  char localString[20] = "Jennie Samuelsson";

  ClassName_PrintHeaderTable(classHeadLine, numIndex);

  for (int localValue1=0; localValue1<4; localValue1++)
  {
    printf(ruleFormat, localValue1, (localValue1+localValue2), localString, handle);
  }
}
  

int main(int marg, char* argv[])
{
  ClassName_Internal_Log_Test();
 
  return 0;
}

