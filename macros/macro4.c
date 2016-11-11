/*
    Auther      : Heinz Samuelsson
    Date        : 2015-11-14
    File        : macro4.c
    Reference   : -
    Description : Return on error code via a macro call. Use blockRc as a return code from a function
                  that is called.
                  gcc macro4.c -o macro4
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned int MY_STATUS;

#define MY_ERROR                 (-1)
#define MY_OK                    (0x00) /* Operation succeeded */
#define MY_FAIL                  (0x01) /* Operation failed    */
#define MY_BAD_VALUE             (0x02) /* Illegal value        */
#define MY_OUT_OF_RANGE          (0x03) /* Value is out of range*/
#define MY_CREATE_ERROR          (0x0A) /* Fail while creating an item           */

#define BLOCK_ERROR_CODE_TO_STRING(_Rc)   _Rc == (MY_STATUS)MY_ERROR         ?  "ERROR"                       :\
                                          _Rc == (MY_STATUS)MY_OK            ?  "Operation succeeded"         :\
                                          _Rc == (MY_STATUS)MY_BAD_VALUE     ?  "Illegal value"               :\
                                          _Rc == (MY_STATUS)MY_OUT_OF_RANGE  ?  "Value is out of range"       :\
                                          _Rc == (MY_STATUS)MY_CREATE_ERROR  ?  "Fail while createing an item"  : "Unknown error string"

#define DIAG_PRODTEST_SUCCESS 0
#define DIAG_PRODTEST_FAILURE 1

#define MY_FAILURE(_retcode) ((_retcode) != (MY_OK))

#define DIAG_RETURN_ON_ERROR(_function, _retcode) \
  if (MY_FAILURE(_retcode)) { \
    printf("Line: %d in function: %s(), failed with error code: %d => %s\n", __LINE__, _function, _retcode, BLOCK_ERROR_CODE_TO_STRING(_retcode)); \
    return DIAG_PRODTEST_FAILURE; \
  }

 

int f1()
{
  MY_STATUS myRc = MY_BAD_VALUE;
  DIAG_RETURN_ON_ERROR("f1", myRc);
}


int main()
{
  int rv = f1();
  printf("rv=%d\n", rv);
}
