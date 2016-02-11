/*
  Auther      : Heinz Samuelsson
  Date        : Sun Nov 29 21:12:15 CET 2015
  File        : sscanf1.c
  Reference   : -
  Description : -
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void command1(const char* args)
{
  int  var1    = 0;
  char var2[6] = "";
  char var3[6] = "";

  int arguments = sscanf(args, "%d %s %s", &var1, &var2, &var3);
  printf("%s() - Input argmuments [%d]: var1=%d, var2=%s, var3=%s\n", __func__, arguments, var1, var2, var3);

  // scan input string
  if (arguments != 3)
  {
    printf("Invalid argument(s), number of arg=%d\n", arguments);
    return;
  }

  // check first argument
  if (var1 == 1)
  {
    printf("Argument 1 hit\n");
  }
  else
  {
    printf("Argument 1 no hit\n");
  }
}


int main(int marg, char* argv[])
{
  char cmd_A[20] = "1 jennie def";
  command1(&cmd_A[0]);
  printf("==================================================================\n"); 

  char cmd_B[20] = "2 ghi jkl";
  command1(&cmd_B[0]);
  printf("==================================================================\n"); 

  char cmd_C[20] = "2 mno pqr";
  command1(&cmd_C[0]);
  printf("==================================================================\n"); 

  char cmd_D[20] = "9 xyz";
  command1(&cmd_D[0]);
  printf("==================================================================\n"); 

  return 0;
}

