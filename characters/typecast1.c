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
 
void f2(void *data, int size)
{
    // need to type cast input parameter
    printf("%s() - data: %d\n", __func__, *(uint8_t*)data);

    // need to type cast input parameter
    printf("%s() - data: %c\n", __func__, *(char*)data);

    // print out all data
    for (int i=0; i<size; i++)
    {
        printf("%s() - char: %c\n", __func__, ((char*)data)[i]);
    }
}


void f1 (void* data, int size)
{
    // need to type cast input parameter
    printf("%s() - data: %d\n", __func__, *(uint8_t*)data);

    f2(data, size);
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
