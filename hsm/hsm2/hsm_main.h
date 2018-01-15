//============================================================================
//
// $URL: $
// $Revision: $
// $Date: $
// $Author: $
//
// Description :
//
// Copyright   : UNJO AB
//
//============================================================================

#ifndef HSM_MAIN_H_
#define HSM_MAIN_H_

#include "statemachine.h"

struct statemachine *hsm_main_init(void);

enum hsm_main_event {
	EV_PWR_WAKEUP = EV_USER,                //0
	EV_DEBUG_ENABLE,			//12
	EV_DEBUG_DISABLE,			//13
};

/**
 * Parameter to EV_PWR_WAKEUP.
 */
enum wakeup_reason {
	WAKEUP_REASON_UNKNOWN,
	WAKEUP_REASON_PWON,
	WAKEUP_REASON_LOCAL,
	WAKEUP_REASON_REMOTE,
};

enum em_stop_reason {
	EM_STOP_BREAK,
	EM_STOP_SUPPLY_HIGH,
	EM_STOP_SUPPLY_LOW,
	EM_STOP_SINCOS,
	EM_STOP_TEMP,
};

enum ctrl_source {
	CTRL_SOURCE_LOCAL,
};

/* Externally defined */
int hw_init(void);

#endif /* HSM_MAIN_H_ */
