#include <stdio.h>
#include <stdlib.h>

// (condition) ? (do something) : (do something else)
// if _value less than 0 then *2 else *10
#define ABS(_value) ((_value) < 0) ? (_value*10) : (_value*2)

int main()
{
    printf("result: %d\n", ABS(1));
    printf("result: %d\n", ABS(-1));

    return 0;
}

