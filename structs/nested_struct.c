/*
  Auther      : Heinz Samuelsson
  Date        : Fri Dec 18 23:42:52 CET 2015
  File        : nested_struct.c
  Reference   : -
  Description : Test of nested structs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char  name[20];
  int   ssn;
  float salary;

  struct date
  {
    int year;
    int month;
    int day;
  } date;
} Employee;


int main(int argc, char* argv[])
{

  Employee employee = {"Albert Einstein",
                       393,
                       1000.56,
                       {2015,6,10}}; 

  printf("Employee Name   : %s\n", employee.name);
  printf("Employee SSN    : %d\n", employee.ssn);
  printf("Employee Salary : %4.2f\n", employee.salary);
  printf("Employee Date   : %d-%02d-%d\n", employee.date.year, employee.date.month, employee.date.day);

  return 0;
}

