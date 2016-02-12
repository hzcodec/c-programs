/*
  Auther      : Heinz Samuelsson
  Date        : Sun Nov 29 21:12:15 CET 2015
  File        : trim_white_space.c
  Reference   : -
  Description : -
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* trimWhiteSpace(const char* str)
{
  const char* end;
  
  // trim leading space
  while(isspace(*str)) str++;

  // all spaces?
  if (*str == 0)
    return str;

  // trim trailing space
  end = str + strlen(str) - 1;
//  while(end > str && isspace(*end)) end--;
//
//  // write new null terminator
//  *(end+1) = 0; 
//
//  return str;
}

// trim all leading spaces
void trim1(char* str)
{
  printf("1 - str=%s\n", str);

  while(isspace(*str))
  {
    str++;
    printf("str=%s\n", str);
  }

  printf("2 - str=%s\n", str);
}


// same as trim1 + checking  all spaces
void trim2(char* str)
{
  while(isspace(*str))
  {
    str++;
    printf("str=%s\n", str);
  }

  // all spaces=
  if(*str == 0)
  {
    printf("Only spaces!\n");
  }
  else
  {
    printf("Not only spaces!\n");
  }
}

int main()
{

  char name1[10] = "   Jennie";
  char name2[10] = "  ";
  char name3[10] = "";

  trim1(name1);
  printf("----------------------\n");

  trim2(name1);
  printf("----------------------\n");

  trim2(name2);
  printf("----------------------\n");

  trim2(name3);
  printf("----------------------\n");

  return 0;
}

