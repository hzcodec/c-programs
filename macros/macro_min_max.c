/*
    Auther      : Heinz Samuelsson
    Date        : 2015-11-14
    File        : macro_min_max.c
    Reference   : -
    Description : -
*/

#include <stdio.h>
#include <stdlib.h>

typedef enum
{
  PORT_MIN = 0,
  PORT_MAX = 1
}PORTS;

#define ConvertPorts(_port_no)\
  (((_port_no) >= PORT_MIN) && ((_port_no) <= PORT_MAX))


int main()
{
  int port = 1;

  if (ConvertPorts(port))
  {
    printf("True\n");
  }
  else
  {
    printf("False\n");
  }

  return 0;
}

