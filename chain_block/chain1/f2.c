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

static struct Output_interface output;

struct Output_interface f2(struct Input_interface *input)
{
	printf("%s() -\n", __func__);

	output.a = 20;
	output.b = 200;
	printff(input, &output);

	return output;
}
