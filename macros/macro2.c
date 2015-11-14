/*
    Auther      : Heinz Samuelsson
    Date        : 2015-11-14
    File        : macro2.c
    Reference   : -
    Description : Print on error code via a macro call. Use myRc as a return code from a function
                  that is called.
                  gcc macro2.c -o macro2
*/

#include <stdio.h>
#include <stdlib.h>

//#define MY_ERROR_CODE_TO_STRING(_Rc) _Rc == (MY_RC)MY_RC__OK                  ?  "Success"                                    :\
//                                     _Rc == (MY_RC)MY_RC__NON_CRITICAL_ERROR  ?  "Failed, the HW state was not changed"       :\
//                                     _Rc == (MY_RC)MY_RC__CRITICAL_ERROR      ?  "Failed, the HW state has been changed"      :\
//                                     _Rc == (MY_RC)MY_RC__ILLEGAL_PARAMETER   ?  "Failed, error returned when calling driver" : "Unknown error string"
#define MY_ERROR_CODE_TO_STRING(_Rc) _Rc == MY_RC__OK                  ?  "Success"                                    :\
                                     _Rc == MY_RC__NON_CRITICAL_ERROR  ?  "Failed, the HW state was not changed"       :\
                                     _Rc == MY_RC__CRITICAL_ERROR      ?  "Failed, the HW state has been changed"      :\
                                     _Rc == MY_RC__ILLEGAL_PARAMETER   ?  "Failed, error returned when calling driver" : "Unknown error string"

#define MY_FAILURE(_retcode) ((_retcode) != MY_RC__OK)
#define MY_SUCCESS(_retcode) ((_retcode) == MY_RC__OK)

#define MY_DIAG_PRINT_ON_ERROR(_function, _retcode) \
  if (MY_FAILURE(_retcode)) { \
    printf("Line: %d: %s() call failed with error code %d, %s\n", __LINE__, _function, _retcode, MY_ERROR_CODE_TO_STRING(_retcode)); \
  } \
  else \
    printf("Line: %d: %s() call was OK, %s\n", __LINE__, _function, MY_ERROR_CODE_TO_STRING(_retcode));

typedef enum
{
  MY_RC__OK                  = 0,  /* Success! */
  MY_RC__NON_CRITICAL_ERROR  = 1,  /* Failed, the HW state was not changed */
  MY_RC__CRITICAL_ERROR      = 2,  /* Failed, the HW state has been changed */
  MY_RC__ILLEGAL_PARAMETER   = 7,  /* Failed, parameter not supported */
} MY_RC;


int main()
{
  MY_RC myRc  = MY_RC__CRITICAL_ERROR;
  MY_DIAG_PRINT_ON_ERROR("main", myRc);

  myRc  = MY_RC__ILLEGAL_PARAMETER;
  MY_DIAG_PRINT_ON_ERROR("main", myRc);

  myRc  = MY_RC__OK;
  MY_DIAG_PRINT_ON_ERROR("main", myRc);

  return 0;
}
