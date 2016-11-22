/*
    Auther       : Heinz Samuelsson
    Date         : 2016-11-22
    File         : main.c
    Reference    : -
    Description  : Using function pointers declared in an array.
                   gcc action.c main.c

*/ 
 
 
#include <stdio.h>
#include <stdlib.h>
#include "action.h"
 

// define functions in an array
static const GeneralFunction doActionsArray[] =
{
    doAction0,
    doAction1,
    doAction2,
};


void populate_struct(Coordinate* c, int x, int y, myFunctionPtr ff)
{
    c->x = x;
    c->y = y;
    c->operation = ff;
    c->exec = execute_operation;
}


int main()
{
    Coordinate coord;
    populate_struct(&coord, 200, 400, f1);

    for (int action=Action0; action<NoAction; action++)
    {
	// call action functions with its in parameter
        doActionsArray[action](&coord);
    }

    return 0;
}
