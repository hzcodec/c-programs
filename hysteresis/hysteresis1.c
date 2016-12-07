/*
    Auther       : Heinz Samuelsson
    Date         : 2016-12-07
    File         : hyst1.c
    Reference    : -
    Description  : Hysteresis function.
*/ 
 
#include <stdio.h>
#include <stdlib.h>

#define HYSTERESIS_LOWER_LIMIT 5 // hysteresis lower limits
#define HYSTERESIS_UPPER_LIMIT 8 //     -"-    upper limits

// loop var for test purpose
#define HI_VAL      10+1
 
enum flag {
           OFF = 0,
           ON  = 1,
};

int pos = OFF;
 

void hyst(int signal)
{
    if(pos == OFF && signal >= HYSTERESIS_UPPER_LIMIT)
    {
        printf("  ON - ");
	pos = ON;
    }
    else if (pos == ON && signal <= HYSTERESIS_LOWER_LIMIT)
    {
        printf("  OFF - ");
	pos = OFF;
    }

    printf("Input signal: %d, pos: %d\n", signal, pos);
}


int main()
{
    for (int inputSignal=0; inputSignal<HI_VAL; inputSignal++)
    {
        hyst(inputSignal);
    }

    for (int inputSignal=HI_VAL; inputSignal>0; inputSignal--)
    {
        hyst(inputSignal);
    }

    for (int inputSignal=0; inputSignal<HI_VAL; inputSignal++)
    {
        hyst(inputSignal);
    }

    return 0;
}

