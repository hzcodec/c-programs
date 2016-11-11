/*
    Auther       : Heinz Samuelsson
    Date         : 2016-11-11
    File         : print_enums2.c
    Reference    : http://stackoverflow.com/questions/3168306/print-text-instead-of-value-from-c-enum
    Description  : Print out text instead of value from enum.
*/ 
 
#include <stdio.h>
#include <stdlib.h>

enum Days{Monday=1, Tuesday=2, Wednesday=3, Thursday=4, Friday=5, Saturday=6, Sunday=7, NoDay=8};

const char* dayNames[] = {
                          [Monday]    = "Monday",
                          [Tuesday]   = "Tuesday", 
                          [Wednesday] = "Wednesday", 
                          [Thursday]  = "Thursday", 
                          [Friday]    = "Friday", 
                          [Saturday]  = "Saturday", 
                          [Sunday]    = "Sunday", 
                          [NoDay]     = "NoDay", 
                         };
 
int main()
{
    for (int day=Monday; day<NoDay; day++)
    {
        printf("day %d: %s\n", day, dayNames[day]);
    }

    return 0;
}
