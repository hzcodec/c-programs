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
	EV_PWR_SHUTDOWN,			//1
	EV_CAN_START,				//2
	EV_CAN_UNPOWERED,			//3
	EV_CAN_POWERED,				//4
	EV_CAN_FAIL,				//5
	EV_CAN_STOP,				//6
	EV_CTRL_ENABLED,			//7
	EV_CTRL_RUN,				//8
	EV_CTRL_STOP,				//9
	EV_CTRL_DISABLED,			//10
	EV_CTRL_SHUTDOWN,			//11
	EV_DEBUG_ENABLE,			//12
	EV_DEBUG_DISABLE,			//13
	EV_BTN_DOWN,				//14
	EV_BTN_UP,                              //15
	EV_BTN_SHORT,				//16
	EV_BTN_HOLD,				//17
	EV_BTN_LONG,				//18
	EV_CAN_OPERATIONAL,			//19
	EV_CAN_STATUS_UPDATED,	                //20
	EV_EM_STOP,			        //21
	EV_BT_CONNECT,				//22
	EV_BT_DISCONNECT,			//23
	EV_CAN_SLAVE_BOOT_UP,		        //24
	EV_ENTER_BOOT,				//25
	EV_CAN_SDO_DONE,                        //26
	EV_STOP_REMOTE,                         //27
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
	CTRL_SOURCE_NONE,
	CTRL_SOURCE_CMD,
	CTRL_SOURCE_BT,
	CTRL_SOURCE_LOCAL,
};

/* Externally defined */
int hw_init(void);

#endif /* HSM_MAIN_H_ */
