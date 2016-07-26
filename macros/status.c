/*
    Auther      : Heinz Samuelsson
    Date        : tis 26 jul 2016 16:24:44 CEST
    File        : status.c
    Reference   : -
    Description : Print out status information from an 8 bit vector.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// check which bit is set
#define CHECK_BIT(val, pos) (((val) >> (pos)) & 1)

// define status bits
#define CONFIGURATION_STATUS 0x01
#define TRANSMISSION_STATUS  0x02
#define RECEPTION_STATUS     0x04
#define ACTIVE_LAYER         0x08
#define OVERCURRENT_FLAG     0x10
#define ESTIMATED_TEMPERATUR 0x20

#define ENUM2STRING(_str) _str == CONFIG_STATUS ? "Configuration Status" : \
                          _str == TRANS_STATUS ? "Transmission Status" : \
                          _str == REC_STATUS ? "Reception Status" : \
                          _str == ACT_LAYER ? "Active Layer" : \
                          _str == OCURRENT_FLAG ? "Overcurrent Flag" : \
                          _str == EST_TEMPERATURE ? "Estimated Temperature" : "UNKNOWN"

typedef enum
{
  CONFIG_STATUS,
  TRANS_STATUS,
  REC_STATUS,
  ACT_LAYER,
  OCURRENT_FLAG,
  EST_TEMPERATURE,
} STATUS;


int main()
{
  
  uint8_t test;

  test = ESTIMATED_TEMPERATUR | OVERCURRENT_FLAG; 

  // find which bit that is set
  for (int i=0; i<8; i++)
  {
    if (CHECK_BIT(test, i))
    {
      printf("%s %d - %s\n", "Status:", i, ENUM2STRING(i));
    }
  }

  return 0;
}

