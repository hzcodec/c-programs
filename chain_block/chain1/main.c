/*
    Auther       : Heinz Samuelsson
    Date         : 2019-05-20
    File         : main.c
    Reference    : -
    Description  : -
			gcc -c f1.c
			gcc -c f2.c
			gcc -c f3.c
			gcc -c main.c
			gcc f1.o f2.o f3.o main.o -o main

			+----------+       +----------+       +----------+
			|          |       |          |       |          |
	          ----->|    f1    |------>|    f2    |------>|    f3    |
			|          |       |          |       |          |
			+----------+       +----------+       +----------+
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "f1.h"
#include "f2.h"
#include "f3.h"

void printff(struct Input_interface *input, struct Output_interface *output)
{
        printf("%s() - ina=%d, inb=%d, outa=%d, outb=%d\n", __func__, input->a, input->b, output->a, output->b);
	printf("------------------------------------------------\n");
}
 
// setup the chain and get the size
struct Output_interface (*pchain[])(struct Input_interface *input) = {f1, f2, f3};
static int chain_size = sizeof(pchain) / sizeof(pchain[0]);


int main(int argc, char *argv[])
{
	struct Input_interface input;
	struct Output_interface output;

	// initial input values
	input.a = 0;
	input.b = 0;

	// loop through the chain
	for (int i=0; i<chain_size; i++) {
		output = pchain[i](&input);
		input.a = output.a;
		input.b = output.b;
	}

        return 0;
}
