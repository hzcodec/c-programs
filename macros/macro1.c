/*
    Auther      : Heinz Samuelsson
    Date        : 2015-11-14
    File        : macro1.c
    Reference   : -
    Description : Return an error code via a macro call. Use blockRc as a return code from a function
                  that is called.
                  gcc macro1.c -o macro1
*/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int MY_STATUS;

#define MY_ERROR                 (-1)
#define MY_OK                    (0x00) /* Operation succeeded */
#define MY_FAIL                  (0x01) /* Operation failed    */
#define MY_BAD_VALUE             (0x02) /* Illegal value        */
#define MY_OUT_OF_RANGE          (0x03) /* Value is out of range*/
#define MY_BAD_PARAM             (0x04) /* Illegal parameter in function called  */

#define BLOCK_ERROR_CODE_TO_STRING(_Rc)   _Rc == (MY_STATUS)MY_ERROR          ?  "ERROR"                                :\
                                          _Rc == (MY_STATUS)MY_OK             ?  "Operation succeeded"                  :\
                                          _Rc == (MY_STATUS)MY_FAIL           ?  "Operation failed"                     :\
                                          _Rc == (MY_STATUS)MY_BAD_VALUE      ?  "Illegal value"                        :\
                                          _Rc == (MY_STATUS)MY_OUT_OF_RANGE   ?  "Value is out of range"                :\
                                          _Rc == (MY_STATUS)MY_BAD_PARAM      ?  "Illegal parameter in function called"     : "Unknown error string"
 
#define PRODTEST_SUCCESS 0
#define PRODTEST_FAILURE 1

#define BLOCK_FAILURE(_retcode) ((_retcode) != (MY_OK))

#define DIAG_RETURN_ON_ERROR(_function, _retcode) \
  if (BLOCK_FAILURE(_retcode)) { \
    printf("Line: %d in function %s() failed with error code: %d => %s\n", __LINE__, _function, _retcode, BLOCK_ERROR_CODE_TO_STRING(_retcode)); \
    return PRODTEST_FAILURE; \
  }


int main()
{
  MY_STATUS blockRc = MY_FAIL;

  DIAG_RETURN_ON_ERROR("main", blockRc);

  return 0;
}

