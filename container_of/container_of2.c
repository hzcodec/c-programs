/*
    Auther       : Heinz Samuelsson
    Date         : 2018-02-11
    File         : container_of2.c
    Reference    : https://www.sockfd.net/c/get-parent-structure-by-its-member
    Description  : How to get the parent structure by its member.
*/ 
 
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
 
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})


typedef struct my_struct
{
        int data;
	char name[100];
	double f;
} my_struct_t; 


int main()
{
        // declare a struct t with basic values
	my_struct_t t = {1, "Hello World", 0.1};

	// Get the parent struct which contains out pointer (ptr).
	// We give the type as 2nd parameter (my_struct_t) and the
	// name of the member ('data').
	// You can use either of the members.
	//my_struct_t *t2 = (my_struct_t *)container_of(&t.data, my_struct_t, data);
	//my_struct_t *t2 = (my_struct_t *)container_of(&t.f, my_struct_t, f);
	my_struct_t *t2 = (my_struct_t *)container_of(&t.name, my_struct_t, name);

	// now t2 is equal to the parent of the member 'data'
	printf("data=%d, name=%s, f=%.2f\n", t2->data, t2->name, t2->f);

        return 0;
}

