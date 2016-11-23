/*
    Auther       : Heinz Samuelsson
    Date         : 2016-11-23
    File         : enum1.c
    Reference    : -
    Description  : Bitwise OR operator.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
 
#define READY	0x0001
#define STEADY	0x0002
#define GO	0x0004

 
enum designFlags {
                  BOLD      = 1,
                  ITALICS   = 2,
                  UNDERLINE = 4,
};


int main()
{
    int myDesign = BOLD | UNDERLINE; 
    int myJumper;

    //    00000001
    //  | 00000100
    //  ___________
    //    00000101

    printf("Vector is: %02x\n", myDesign);

    if (myDesign & ITALICS)
    {
        printf("Italics is selected\n");
    }

    if  (myDesign & BOLD)
    {
        printf("Bold is selected\n");
    }

    myJumper = READY | STEADY;
    printf("Jumper is: %02x\n", myJumper);

    return 0;
}

/*
    Vector is: 05
    Bold is selected
*/
