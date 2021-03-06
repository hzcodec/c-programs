#include <stdio.h>
#include <stdlib.h>
#include "action.h"
 
void doAction0(Coordinate* coord)
{
    int var = 10;
    printf("%s() - x: %d, y: %d\n", __func__, coord->x, coord->y);
    printf("Return value: %d\n", coord->exec(11));
}


void doAction1(Coordinate* coord)
{
    int var = 77;
    printf("%s() - x: %d, y: %d\n", __func__, coord->x, coord->y);
    printf("Return value: %d\n", coord->exec(22));
    coord->operation(&var);
}


void doAction2(Coordinate* coord)
{
    int var = 10;
    printf("%s() - x: %d, y: %d\n", __func__, coord->x, coord->y);
    printf("Return value: %d\n", coord->exec(33));
}

void doAction3(Coordinate* coord)
{
    int var = 10;
    printf("%s() - x: %d, y: %d\n", __func__, coord->x, coord->y);
    printf("Return value: %d\n", coord->exec(100));
    coord->operation(&var);
}

int execute_operation(int a)
{
    printf("%s() ...", __func__);
    return a*100;
}

void f1(int* var)
{
    printf("%s() - var:%d\n", __func__, *var);
}

void f2(int* var)
{
    printf("%s() - var:%d\n", __func__, *var);
}


