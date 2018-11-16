#include <stdio.h>
#include <stdlib.h>
#include "file1.h"

/* this define is only used inside file1 */
#define HELLO "Hello"

int file1__init_block(void) {
        printf("%s() - %s\n", __func__, HELLO);
	return NUMBER99;
}
