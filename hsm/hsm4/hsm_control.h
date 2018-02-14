#ifndef HSM_CONTROL_H_
#define HSM_CONTROL_H_

#include <stdbool.h>
#include "fixpoint.h"
#include "statemachine.h"

// Convert event enum:s to string. It's a Supercalifragilisticexpialidocious macro :-)
#define ENUM2STRING(_str) _str == EV_INIT ? "EV_INIT" : \
                          _str == EV_ENTRY ? "EV_ENTRY" : \
                          _str == EV_EXIT ? "EV_EXIT" : \
                          _str == EV_DO ? "EV_DO" : "UNKNOWN"

#define FSM_BLOCK "      +-----------------------------------------------------------+\n \
     | main                                                      | \n \
     |                                                           | \n \
     |    +-----------+       +-------------------------------+  | \n \
     |    | disabled  |       | enabled                       |  | \n \
     |    |           |       |                               |  | \n \
     |    |           |       |  +----------+  +----------+   |  | \n \
     |    |           |       |  | running  |  | idle     |   |  | \n \
     |    |           |       |  |          |  |          |   |  | \n \
     |    |           |       |  +----------+  +----------+   |  | \n \
     |    +-----------+       +-------------------------------+  | \n \
     +-----------------------------------------------------------+"


// defines
#define ID1   1
#define ID2   2

// define colors
#define NORM "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHT  "\x1B[37m"
#define BOLD "\x1B[1m"

#define BYEL "\x1B[1m \x1B[33m" // bold + yellow color
#define BBLUE "\x1B[1m \x1B[34m" // bold + blue color
#define BCYAN "\x1B[1m \x1B[36m" // bold + cyan color
#define BRED "\x1B[1m \x1B[31m" // bold + red color

#define EVENTCOL  BYEL  // color of events
#define MYCOL2    BCOL2 // 

#define DELIMITER "-----------------------------------"

struct statemachine *hsm_control_init(void);

#endif /* HSM_CONTROL_H_ */
