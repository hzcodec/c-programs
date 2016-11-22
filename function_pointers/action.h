#ifndef ACTION_H
#define ACTION_H

enum operation {
                Action0,  // Action 0 started
                Action1,  // Action 1 started
                Action2,  // Action 2 started
                NoAction, // no action
               };


typedef struct
{
    int x;
    int y;
    void (*operation)(int*);
    int  (*exec)(int);
} Coordinate;


typedef void (*GeneralFunction)(Coordinate*);
typedef void (*myFunctionPtr)(int*);


void doAction0(Coordinate* coord);
void doAction1(Coordinate* coord);
void doAction2(Coordinate* coord);

void f1(int* var);
int execute_operation(int a);

#endif
