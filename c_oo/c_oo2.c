/*
    Auther       : Heinz Samuelsson
    Date         : 2018-03-28
    File         : c_oo2.c
    Reference    : https://www.codementor.io/michaelsafyan/object-oriented-programming-in-c-du1081gw2
    Description  : oo example.
*/ 
 
#include <stdio.h>
#include <stdlib.h>
 
 // Source file
struct Point {
   int x;
   int y;
};

// Constructor (without allocation)
void Point__init(struct Point* self, int x, int y) {
        printf("%s() - \n", __func__);
        self->x = x;
        self->y = y;
 }

// Allocation + initialization (equivalent to "new Point(x, y)")
struct Point* Point__create(int x, int y) {
        printf("%s() - \n", __func__);
        struct Point* result = (struct Point*) malloc(sizeof(struct Point));
        Point__init(result, x, y);
        return result;
}

// Destructor (without deallocation)
void Point__reset(struct Point* self) {
}

// Destructor + deallocation (equivalent to "delete point")
void Point__destroy(struct Point* point) {
  if (point) {
     Point__reset(point);
     free(point);
  }
}

// Equivalent to "Point::x()" in C++ version
int Point__x(struct Point* self) {
        printf("%s() - \n", __func__);
        return self->x;
}

// Equivalent to "Point::y()" in C++ version
int Point__y(struct Point* self) {
        printf("%s() - \n", __func__);
        return self->y;
}


int main(int argc, char *argv[])
{
	struct Point *pt;

	pt = Point__create(88, 99);

	printf("%s() - pt->x=%d, pt->y=%d\n", __func__, pt->x, pt->y);

	printf("%s() - Point__x=%d\n", __func__, Point__x(pt));
	printf("%s() - Point__y=%d\n", __func__, Point__y(pt));

	Point__destroy(pt);
	printf("%s() - pt->x=%d, pt->y=%d\n", __func__, pt->x, pt->y);

        return 0;
}

/*
    Result:
      Point__create() - 
      Point__init() - 
      main() - pt->x=88, pt->y=99
      Point__x() - 
      main() - Point__x=88
      Point__y() - 
      main() - Point__y=99
      main() - pt->x=0, pt->y=0
*/
