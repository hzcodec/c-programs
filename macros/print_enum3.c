/*
    Auther       : Heinz Samuelsson
    Date         : 2016-11-11
    File         : print_enum3.c
    Reference    : http://userpage.fu-berlin.de/~ram/pub/pub_jf47ht81Ht/c_preprocessor_applications_en
    Description  : Printing enum values as strings
*/ 
 
#include <stdio.h>
#include <stdlib.h>
 
#define NAMES C(RED)C(GREEN)C(BLUE)
#define C(x) x,

enum color {NAMES TOP};
#undef C

#define C(x) #x,

const char* const color_name[] = {NAMES};
 
int main(void)
{
    printf( "The color is %s.\n", color_name[RED]);  
    printf( "The color is %s.\n", color_name[BLUE]);  
    printf( "There are %d colors.\n", TOP ); 

    return 0;
}
