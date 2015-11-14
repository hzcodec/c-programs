/*
    Auther      : Heinz Samuelsson
    Date        : 2015-11-14
    File        : macro3.c
    Reference   : -
    Description : -
                  gcc macro3.c -o macro3
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLOCK_ID_LO             (9)
#define BLOCK_ID_LO_MASK        (0xFA)
#define BLOCK_ID_LO_SHIFT       (2)
#define BLOCK_ID_LO_BITS        (8)
#define BLOCK_ID_HI             (8)
#define BLOCK_ID_HI_MASK        (0x03)
#define BLOCK_ID_HI_SHIFT       (0)


//#define BLOCK_HILO_PATTERN(_array, _udb, _value) \
//  _array[_udb ## _LO] |= (_value << _udb ## _LO_SHIFT) & _udb ## _LO_MASK; \
//  _array[_udb ## _HI] |= ((_value >> _udb ## _LO_BITS) << _udb ## _HI_SHIFT) & _udb ## _HI_MASK;

#define BLOCK_HILO_PATTERN(_array, _udb, _value) \
  printf("%d\n",(_udb ## _LO)); \
  printf("%d\n",(_udb ## _HI)); \
  printf("%02x\n",(_udb ## _LO_MASK)); \
  printf("%02x\n",(_udb ## _LO_MASK) << _udb ## _LO_SHIFT); \

#define BLOCK_MAX_NUMBER_CNS 50
#define BLOCK_ID             (4)

typedef struct
{
  int  myVar[BLOCK_MAX_NUMBER_CNS];
} RULE_FORMAT_INGRESS;

typedef struct
{
    int myVar[BLOCK_MAX_NUMBER_CNS];
} RULE_FORMAT_EGRESS;

typedef union
{
    RULE_FORMAT_INGRESS ruleIngr;
    RULE_FORMAT_EGRESS  ruleEgr;
} RULE_FORMAT_UNT;
 
typedef struct
{
  RULE_FORMAT_UNT myPattern;
  RULE_FORMAT_UNT cpssMask;
} BLOCK_RULE_DATA;


int main()
{
  BLOCK_RULE_DATA pRuleData;

  pRuleData.myPattern.ruleIngr.myVar[0] = 0;
  BLOCK_HILO_PATTERN(pRuleData.myPattern.ruleIngry.myVar, BLOCK_ID, BLOCK_ID);

  return 0;
}
