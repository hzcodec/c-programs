/*
    Auther       : Heinz Samuelsson
    Date         : 2019-05-20
    File         : f1.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

struct Output_interface f1(struct Input_interface *input)
{
	printf("%s() -\n", __func__);
	struct Output_interface output;
	output.a = 10;
	output.b = 100;
	printff(input, &output);

	return output;
}

