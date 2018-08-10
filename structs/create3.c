/*
    Auther       : Heinz Samuelsson
    Date         : fre 10 aug 2018 15:23:39 CEST
    File         : create3.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>

struct B {
	int b1;
	int b2;
};
static struct B *res;


struct A {
	int a1;
	struct B *pb;
}; 
static struct A *pa;


struct A *a_create(void)
{
	struct A *fa = calloc(1, sizeof(*fa));
	struct B *fb = calloc(1, sizeof(*fb));

	if (fa == NULL || fb == NULL) {
		printf("Error!\n");
		return NULL;
	}

	printf("%s() - fa=%p, size=%lu\n", __func__, fa, sizeof(*fa));
	printf("%s() - fb=%p, size=%lu\n", __func__, fb, sizeof(*fb));

	fa->pb = fb;
	printf("%s() - fa->pb=%p\n", __func__, fa->pb);
	printf("%s() - fa->pb->b1=%d\n\n", __func__, fa->pb->b1);

	return fa;
}

void a_do(struct A *a)
{
	printf("%s() -\n", __func__);
	a->a1 = 99;
	a->pb->b1 = 12;
	a->pb->b2 = 44;
}


int main(int argc, char *argv[])
{
        pa = a_create();
	a_do(pa);

	printf("%s() - pa->a1=%d\n", __func__, pa->a1);
	printf("%s() - pa->pb->b1=%d\n", __func__, pa->pb->b1);
	printf("%s() - pa->pb->b2=%d\n", __func__, pa->pb->b2);

        return 0;
}

/*
    Result:

      a_create() - fa=0x1fc8010, size=16
      a_create() - fb=0x1fc8030, size=8
      a_create() - fa->pb->b1=0
      
      a_do() -
      main() - pa->a1=99
      main() - pa->pb->b1=12
      main() - pa->pb->b2=44

*/
