/*
    Auther       : Heinz Samuelsson
    Date         : 2017-12-28
    File         : rot1.c
    Reference    : -
    Description  : 

                   routine             -- routine
                  *routine             -- is a pointer
                 (*routine)(      )    -- to a function
                 (*routine)(void *)    -- taking a single parameter of type void *
                *(*routine)(void *)    -- returning a pointer
           void *(*routine)(void *)    -- to void

*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 

void* foo(void* x) {
    printf("%s() - Hello\n", __func__);
}

void* bar(int x) {
    printf("%s() - There, x=%d\n", __func__, x);
}

void* fizz(void* a) {
    printf("%s() - There, a=%p\n", __func__, (int*)a);
    printf("%s() - There, a=%d\n", __func__, *(int*)a);
}

void* fuzz(void* a) {
    printf("%s() - There, a=%p\n", __func__, (int*)a);
    printf("%s() - There, a=%d\n", __func__, *(int*)a);
}


typedef void *(*RoutinePtr)(void *);  // alias to make your life easier
typedef void *(*RoutinePtr2)(int);  // alias to make your life easier
typedef void *(*RoutinePtr3)(void *);  // alias to make your life easier
typedef void *(*RoutinePtr4)(void *);  // alias to make your life easier


void routineInvoker(RoutinePtr routine) {
    (*routine)(NULL); // invokes the routine
}

void routineInvoker2(RoutinePtr2 routine2) {
    (*routine2)(99); // invokes the routine2
}

void routineInvoker3(RoutinePtr3 routine3) {
    int kalle = 88;
    (*routine3)((int*)&kalle); // invokes the routine3
}

void routineInvoker4(RoutinePtr4 routine4, int data) {
    int kalle = data;
    (*routine4)((int*)&kalle); // invokes the routine3
}

 
int main(int argc, char *argv[])
{
    RoutinePtr routine = foo;   // creates a function pointer
    routineInvoker(routine);    // and passes it to our invoker
 
    RoutinePtr2 routine2 = bar;  // creates a function pointer
    routineInvoker2(routine2);   // and passes it to our invoker

    RoutinePtr3 routine3 = fizz;  // creates a function pointer
    routineInvoker3(routine3);    // and passes it to our invoker

    RoutinePtr4 routine4 = fuzz;   // creates a function pointer
    routineInvoker4(routine4, 44); // and passes it to our invoker
     
    return 0;
}

/* Result

     foo() - Hello
     bar() - There, x=99
     fizz() - There, a=0x7fff1ae21da4
     fizz() - There, a=88
     fuzz() - There, a=0x7fff1ae21da4
     fuzz() - There, a=44

*/
