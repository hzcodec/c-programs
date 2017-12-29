/*
    Auther       : Heinz Samuelsson
    Date         : 2017-12-29
    File         : pfunc.c
    Reference    : -
    Description  : Function pointers.
*/ 
 
#include <stdio.h>
#include <math.h>

// Define a type for a function pointer returning a double
// and expecting two doubles as parameters
typedef double (*area_calculator)(double, double);

// Functions to calculate the area of various shapes
double calc_area_circle(double radius, double unused)
{
   return(M_PI * radius * radius);
}

double calc_area_triangle(double height, double base)
{
   return(0.5 * height * base);
}

double calc_area_rectangle(double length, double width)
{
   return(length * width);
}

double calc_area_pentagon(double side, double unused)
{
   return (0.25 * sqrt(5.0 * (5.0 + 2.0) * sqrt(5.0)) * side * side);
}

double calc_area_rhombus(double diag1, double diag2)
{
   return ((diag1 * diag2) / 2.0);
}

// An enum for our shapes
enum shape
{
   CIRCLE=0, TRIANGLE, RECTANGLE, PENTAGON, RHOMBUS
};

// An array of functions to calc area for each shape
area_calculator calculators[5] =
{
   &calc_area_circle,
   &calc_area_triangle, 
   &calc_area_rectangle,
   &calc_area_pentagon,
   &calc_area_rhombus
};


// Calc area of any shape using enum
double calc_area_by_enum(enum shape sp, double a, double b)
{
   return((*calculators[sp])(a,b));
}

// Calc by passing in calculator
double calc_area_by_pf_parameter(area_calculator pf, double a, double b)
{
   return((*pf)(a,b));
}


int main(int argc, int argv[])
{
   double result;
   enum shape sp;

   printf("Calc the area using the enum as a parameter\n");
   result = calc_area_by_enum(CIRCLE, 11.4, 0);
   printf("Area of circle: %f\n", result);

   result = calc_area_by_enum(TRIANGLE, 5.0, 2.0);
   printf("Area of triangle: %f\n", result);

   result = calc_area_by_enum(RECTANGLE, 4.2, 2.5);
   printf("Area of rectangle: %f\n", result);

   result = calc_area_by_enum(PENTAGON, 4.2, 0);
   printf("Area of pentagon: %f\n", result);

   result = calc_area_by_enum(RHOMBUS, 7.1, 5.0);
   printf("Area of rhombus: %f\n", result);

   printf("------------------------------------------------------------------\n");

   printf("Calc the same using a PF parameter\n");
   result = calc_area_by_pf_parameter(calculators[CIRCLE], 11.4, 0);
   printf("Area of circle: %f\n", result);

   result = calc_area_by_pf_parameter(calculators[TRIANGLE], 5.0, 2.0);
   printf("Area of triangle: %f\n", result);

   result = calc_area_by_pf_parameter(calculators[RECTANGLE], 4.2, 2.5);
   printf("Area of rectangle: %f\n", result);

   result = calc_area_by_pf_parameter(calculators[PENTAGON], 4.2, 0);
   printf("Area of pentagon: %f\n", result);

   result = calc_area_by_pf_parameter(calculators[RHOMBUS], 7.1, 5.0);
   printf("Area of rhombus: %f\n", result);

   printf("------------------------------------------------------------------\n");


   // How about if each measurement was exactly 6.0?
   for (sp=CIRCLE; sp<=RHOMBUS; sp++) 
   {
      result = calc_area_by_pf_parameter(calculators[sp], 6.0, 6.0);
      printf("Area shape %d is %f\n", sp, result);
   }
}
