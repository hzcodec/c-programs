/*
    Auther       : Heinz Samuelsson
    Date         : 2018-02-11
    File         : container_of1.c
    Reference    : -
    Description  : How to get the parent structure by its member.

                   Test of container_of(ptr, struct str, member); 
                   First parameter refers to the name of the pointer.
		   The second parameter refers to the structure member.
*/ 
 
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
 
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})


struct numbers
{
        int one;
	int two;
	int three;
} n = {1, 2, 3};


int main()
{
        int *ptr = &n.three;
	struct numbers *n_ptr;

	n_ptr = container_of(ptr, struct numbers, three);
	printf("ptr=%p, n_ptr=%p\n", ptr, n_ptr);

	printf("one=%d, two=%d, three=%d\n", n_ptr->one, n_ptr->two, n_ptr->three);
	printf("ptr.one=%d, ptr.two=%d\n", n_ptr->one, n_ptr->two);

        return 0;
}

