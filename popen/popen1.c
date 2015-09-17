/*
    Auther      : Heinz Samuelsson
    Date        : 2015-09-17
    File        : popen1.c
    Reference   : -
    Description : Call a function from another file using popen().

    > gcc -std=gnu99 popen1.c -o popen1
    > gcc -std=gnu99 remote_function1.c -o remote_function1
    > gcc -std=gnu99 remote_function2.c -o remote_function2

    How to run:
      > popen1 1
      > popen1 2
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

  char  hzCommand[30];
  char  buffer[100];
  FILE* fd;

  char* cSelectCommand = argv[1];
  int selectCommand = atoi(cSelectCommand);

  switch (selectCommand)
  {
    case 1:
      sprintf(hzCommand, "./remote_function1");
      break;
    case 2:
      sprintf(hzCommand, "./remote_function2");
      break;
    default:
      printf("Input argument not valid!\n");
  }

  fd = popen(hzCommand, "r");
  while (fscanf(fd, "%s", &buffer) == 1)
  {
    printf("%s ", buffer);
  }

  printf("\n");
  pclose(fd);

  return 0;
}


