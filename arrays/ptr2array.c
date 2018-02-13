/*
    Auther       : Heinz Samuelsson
    Date         : 2018-02-09
    File         : ptr2array.c
    Reference    : -
    Description  : Use pointers to 2 dimensional array.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
void print2d(int **array, int rows, int cols)
{
        int cnt = 100;

	for (int i=0; i<rows; i++)
	{
	        for (int j=0; j<cols; j++)
		{
		        array[i][j] = cnt++;
		        printf("%10d ", array[i][j]);
		}

	puts("");

	}
}
 

int main(int argc, char *argv[])
{
 
	int **dynamic_two_D;
	int num_rows = 3;

	dynamic_two_D = malloc(num_rows*sizeof(int));
	printf("dynamic_two_D=%p\n", dynamic_two_D);
	printf("----------------------\n");

	int num_cols = 4;

	for (int i=0; i<num_rows; i++)
	{
	        dynamic_two_D[i] = malloc(num_cols*sizeof(int));
	        printf("  [%d]=%p\n", i, dynamic_two_D[i]);
	}
	printf("----------------------\n");

	printf("*dynamic_two_D=%p\n", *dynamic_two_D);
	printf("*dynamic_two_D=%p\n", *(dynamic_two_D+1));
	printf("*dynamic_two_D=%p\n", *(dynamic_two_D+2));

	print2d(dynamic_two_D, num_rows, num_cols);

	printf("----------------------\n");
	printf("deref 1 = %d\n", **(dynamic_two_D+0));
	printf("deref 2 = %d\n", **(dynamic_two_D+1));
	printf("deref 3 = %d\n", **(dynamic_two_D+2));

	printf("----------------------\n");
	printf("deref a = %d\n", *(*(dynamic_two_D+0)+0));
	printf("deref b = %d\n", *(*(dynamic_two_D+0)+1));
	printf("deref c = %d\n", *(*(dynamic_two_D+0)+2));
	printf("deref c = %d\n", *(*(dynamic_two_D+0)+3));

	printf("----------------------\n");
	printf("deref a = %d\n", *(*(dynamic_two_D+1)+0));
	printf("deref b = %d\n", *(*(dynamic_two_D+1)+1));
	printf("deref c = %d\n", *(*(dynamic_two_D+1)+2));
	printf("deref c = %d\n", *(*(dynamic_two_D+1)+3));

	printf("----------------------\n");
	printf("deref a = %d\n", *(*(dynamic_two_D+2)+0));
	printf("deref b = %d\n", *(*(dynamic_two_D+2)+1));
	printf("deref c = %d\n", *(*(dynamic_two_D+2)+2));
	printf("deref c = %d\n", *(*(dynamic_two_D+2)+3));

        for (int i=0; i<num_rows; i++)
	{
	        free(dynamic_two_D[i]);
	}

        free(dynamic_two_D);

        return 0;
}

/* Result

       dynamic_two_D=0xe22010
       ----------------------
         [0]=0xe22440
         [1]=0xe22460
         [2]=0xe22480
       ----------------------
       *dynamic_two_D=0xe22440
       *dynamic_two_D=0xe22460
       *dynamic_two_D=0xe22480
              100        101        102        103 
              104        105        106        107 
              108        109        110        111 
       ----------------------
       deref 1 = 100
       deref 2 = 104
       deref 3 = 108
       ----------------------
       deref a = 100
       deref b = 101
       deref c = 102
       deref c = 103
       ----------------------
       deref a = 104
       deref b = 105
       deref c = 106
       deref c = 107
       ----------------------
       deref a = 108
       deref b = 109
       deref c = 110
       deref c = 111

*/
