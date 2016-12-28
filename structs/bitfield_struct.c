/*
    Auther       : Heinz Samuelsson
    Date         : 2016-11-24
    File         : bitfield_struct.c
    Reference    : https://www.tutorialspoint.com/cprogramming/c_bit_fields.htm
    Description  : This structure requires 8 bytes of memory space but in actual, we are going 
                   to store either 0 or 1 in each of the variables. 
		   The C programming language offers a better way to utilize the memory 
		   space in such situations.

                   If you are using such variables inside a structure then you can define the width 
		   of a variable which tells the C compiler that you are going to use only those 
		   number of bytes.

                   The above structure requires 4 bytes of memory space for status variable, 
		   but only 2 bits will be used to store the values.

                   If you will use up to 32 variables each one with a width of 1 bit, then also the 
		   status structure will use 4 bytes. However as soon as you have 33 variables, 
		   it will allocate the next slot of the memory and it will start using 8 bytes. 
		   
                   The declaration of a bit-field has the following form inside a structure −
                   struct
		   {
                      type [member_name] : width ;
                   };
                    
*/ 
 
#include <stdio.h>
#include <string.h>

/* define simple structure */
struct {
   unsigned int widthValidated;
   unsigned int heightValidated;
} status1;

/* define a structure with bit fields */
struct {
   unsigned int widthValidated  : 1;
   unsigned int heightValidated : 1;
} status2;

int main( ) {

   printf( "Memory size occupied by status1 : %d\n", (int)sizeof(status1));
   printf( "Memory size occupied by status2 : %d\n", (int)sizeof(status2));

   status2.widthValidated = 1;
   printf("status2.widthValidated: %d\n", status2.widthValidated);

   if(status2.widthValidated == 1)
   {
       printf("ON\n");
   }
   else
   {
       printf("OFF\n");
   }

   // This will not fit into 1 bit, will give a compiler warning.
   status2.widthValidated = 2;
   printf("status2.widthValidated: %d\n", status2.widthValidated);

   return 0;
}

/*
    struct2.c: In function ‘main’:
    struct2.c:63:29: warning: large integer implicitly truncated to unsigned type [-Woverflow]
        status2.widthValidated = 2;
                                 ^
    heinz.samuelsson@ALMAGRUNDET:~/c-pgm/structs$ ./a.out 
    Memory size occupied by status1 : 8
    Memory size occupied by status2 : 4
    status2.widthValidated: 1
    ON
    status2.widthValidated: 0
*/
