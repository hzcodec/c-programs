/*
    Auther       : Heinz Samuelsson
    Date         : 2016-12-16
    File         : typecast1.c
    Reference    : -
    Description  : How to use void parameter as input to a function.
                   Data is type casted.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
 
void f2(void *data, int sizeOfArray)
{
    // to type cast input parameter to uint8_t
    printf("%s() - data (dec): %d\n", __func__, *(uint8_t*)data);

    // type cast input parameter to char
    printf("%s() - data (chr): %c\n", __func__, *(char*)data);

    // print out all data
    for (int i=0; i<sizeOfArray; i++)
    {
        printf("%s() - data (dec): %d, char: %c\n", __func__, ((uint8_t*)data)[i], ((char*)data)[i]);
    }
}


void f1 (void* data, int sizeOfArray)
{
    // to type cast input parameter to uint8_t
    printf("%s() - data (dec): %d\n", __func__, *(uint8_t*)data);

    f2(data, sizeOfArray);
}
 

int main()
{
    uint8_t myData = 88;
    f1(&myData, 1);

    printf("--------------------\n");

    uint8_t myData2[3] = {87, 86, 85};
    f1(&myData2, 3);
    
    return 0;
}
