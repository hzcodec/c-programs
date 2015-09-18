/*
    Auther      : Heinz Samuelsson
    Date        : 2015-09-17
    File        : memcopy_struct1.c
    Reference   : -
    Description : Test with memory copy and string copy.
                  strcpy() stops when it encounters a NULL, memcpy() does not.
                  printf() stops at NULL.

                  strcpy() terminates when the source string's null terminator is found.
                  memcpy() requires a size parameter be passed.
                  
                  strcpy() copies character from source to destination one by one until
                  it find NULL or '\0' character in the source.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
  char identifier[30];
  int  extendedIdentifier;
  int  connector;
} PROPERTY;


int main(int argc, char* argv[])
{

  PROPERTY property =
    {
      .extendedIdentifier       = 10,
      .connector                = 99
    };

    memcpy(property.identifier, "COMPANY AB     ", sizeof(property.identifier));

    printf("Identifier:      %s\n", property.identifier);
    printf("Ext. Identifier: %d\n", property.extendedIdentifier);
    printf("Connector:       %d\n", property.connector);

    property.connector = 77;
    printf("Connector:       %d\n", property.connector);

    strcpy(property.identifier, "HELLO");
    printf("Identifier:      %s\n", property.identifier);

    // clear struct
    memset(&property, 0, sizeof(property));
    printf("-----------------------------------------\n");
    printf("Identifier:      %s\n", property.identifier);
    printf("Ext. Identifier: %d\n", property.extendedIdentifier);
    printf("Connector:       %d\n", property.connector);

  return 0;
}


