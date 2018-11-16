#include <stdio.h>
#include <stdlib.h>
#include "file2.h" /* need this since f2_block_struct is in the header file */


/* this struct is an internal one */
struct f2_struct
{
	int a;
	int b;
};


void file2__get_pointer(void)
{
	struct f2_struct fs;
	fs.a = 100;
	fs.b = 200;

	struct f2_block_struct fb;
	fb.x = 300;
	fb.y = 400;

	printf("%s() - a=%d, b=%d, x=%d, y=%d\n", __func__, fs.a, fs.b, fb.x, fb.y);
}

