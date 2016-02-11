/*
    Auther      : Heinz Samuelsson
    Date        : 2016-02-06
    File        : print_enums.c
    Reference   : -
    Description : Print out enums.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
  ENUM_VAR1,  // 0
  ENUM_VAR2,  // 1
  ENUM_VAR3,  // 2
} MY_ENUMS;


#define ENUM2STRING(_str) _str == ENUM_VAR1 ? "Enum 1 selected" : \
                          _str == ENUM_VAR2 ? "Enum 2 selected" : \
                          _str == ENUM_VAR3 ? "Enum 3 selected" : "UNKNOWN"

typedef struct
{
  MY_ENUMS packet1;
  int      packet2;
} MY_STRUCT;
 

int main()
{
  MY_STRUCT myStruct;

  myStruct.packet1 = ENUM_VAR3;
  printf("%s: %s\n", "Packet 1", ENUM2STRING(myStruct.packet1));

  myStruct.packet1 = 3;
  printf("%s: %s\n", "Packet 1", ENUM2STRING(myStruct.packet1));

  return 0;
}
