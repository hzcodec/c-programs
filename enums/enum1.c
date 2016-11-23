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
 
 
enum designFlags {
                  BOLD      = 1,
                  ITALICS   = 2,
                  UNDERLINE = 4,
};


int main()
{
    int myDesign = BOLD | UNDERLINE; 

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

    return 0;
}

/*
    Vector is: 05
    Bold is selected
*/
