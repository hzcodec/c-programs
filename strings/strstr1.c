/*
  Auther      : Heinz Samuelsson
  Date        : Sun Nov 29 21:12:15 CET 2015
  File        : strstr1.c
  Reference   : -
  Description : -
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void command2(const char* args)
{
  char var1[9] = "";
  char var2[9] = "";

  int arguments = sscanf(args, "%s %s", &var1, &var2);
  printf("%s() - Input arguments: n=%d, var1=%s, var2=%s\n", __func__, arguments, var1, var2);

  // find first occurence of the substring in args
  if (strstr(args, "-x") != NULL)
  {
    printf("Found x\n");
  }
  else if(strstr(args, "-y") != NULL)
  {
    printf("Found y\n");
  }
  else
  {
    printf("Not Found\n");
  }
}


int main(int marg, char* argv[])
{
  char cmd_A[] = "-x hello";
  command2(&cmd_A[0]);

  char cmd_B[] = "-y hello";
  command2(&cmd_B[0]);

  return 0;
}

