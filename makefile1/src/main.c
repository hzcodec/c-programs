/*
    Auther       : Heinz Samuelsson
    Date         : 2018-04-25
    File         : main.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file1.h" 
#include "file2.h" 


int main(int argc, char *argv[])
{
	printf("=====================================================\n");

	int rv = file1__init_block();
	printf("%s() - rv=%d, NUMBER99=%d\n", __func__, rv, NUMBER99);

	file2__get_pointer();

	struct f2_block_struct my_f2_struct_in_main;
	my_f2_struct_in_main.x = 800;
	my_f2_struct_in_main.y = 900;
	printf("%s() - x=%d, y=%d\n", __func__, my_f2_struct_in_main.x, my_f2_struct_in_main.y);

        return 0;
}
