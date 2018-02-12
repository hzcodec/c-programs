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

#include <stdlib.h>
#include "hsm_main.h"
#include "types.h"
#include "timer.h"
#include "hsm_control.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef DELAY_MOTOR
#define DELAY_MOTOR 10
#endif
#ifndef DELAY_BRAKE
#define DELAY_BRAKE 100
#endif
#ifndef DELAY_ERROR
#define DELAY_ERROR 3000
#endif

struct main_sm {
	struct statemachine sm;
	struct timer t_main;
	struct timer t_powered;
	struct timer t_bist;
	struct timer t_drive;
	struct {
		s16b16 start_pos;
		s16b16 max_diff;
	} brake_test;
	int btn_count;
};

STATE(main_sm, main_sm_impl, 0);
STATE(init, init_impl, &main_sm);
STATE(wait_for_power, wait_for_power_impl, &main_sm);
STATE(wait_for_wakeup, wait_for_wakeup_impl, &main_sm);
STATE(wait_for_button, wait_for_button_impl, &main_sm);
STATE(check_supply, check_supply_impl, &main_sm);
STATE(shutdown, shutdown_impl, &main_sm);
STATE(wait_shutdown, wait_shutdown_impl, &main_sm);
STATE(enter_boot, enter_boot_impl, &main_sm);
STATE(wait_save, wait_save_impl, &main_sm);
STATE(powered, powered_impl, &main_sm);
STATE(self_test, self_test_impl, &powered);
STATE(bist_step_1, bist_step_1_impl, &self_test);
STATE(bist_step_2, bist_step_2_impl, &self_test);
STATE(bist_step_2_old, bist_step_2_old_impl, &self_test);
STATE(bist_step_2_5, bist_step_2_5_impl, &self_test);
STATE(bist_step_3, bist_step_3_impl, &self_test);
STATE(bist_step_4, bist_step_4_impl, &self_test);
STATE(bist_step_5, bist_step_5_impl, &self_test);
STATE(bist_step_6, bist_step_6_impl, &self_test);
STATE(bist_step_7, bist_step_7_impl, &self_test);
STATE(error, error_impl, &powered);
STATE(temp_error, temp_error_impl, &powered);
STATE(bt_pair, bt_pair_impl, &powered);
STATE(wait_btn_release, wait_btn_release_impl, &powered);
STATE(active, active_impl, &powered);
STATE(multi_button, multi_button_impl, &active);
STATE(idle, idle_impl, &active);
STATE(drive_enabled, drive_enabled_impl, &active);
STATE(enabling, enabling_impl, &drive_enabled);
STATE(brake_released, brake_released_impl, &drive_enabled);
STATE(releasing, releasing_impl, &brake_released);
STATE(running, running_impl, &brake_released);
STATE(stopping, stopping_impl, &brake_released);
STATE(braking, braking_impl, &drive_enabled);
STATE(debug, debug_impl, &main_sm);

static struct main_sm main_sm_instance;

/* Following 2 variables for EMCY check during powering up */
static uint8_t check_emcy_pin_manually;
static struct timer delay;

s16b16 abs_diff_pos(s16b16 a, s16b16 b)
{
	/* FIXME: Handle overflow without UB */
	s16b16 abs_diff = a > b ? a - b : b - a;
	return abs_diff >= 0 ? abs_diff : INT32_MAX;
}

static const struct state *main_sm_impl(struct statemachine *sm, const struct event *ev)
{
	// HzS -  GPIO_InitTypeDef GPIO_InitStructure;

	switch (ev->id) {
	case EV_INIT: {
		// HzS -  rgb_led_set_brightness(led_get_brightness() ? param.led_duty_hi : param.led_duty_lo);
		return &init;
	}
	case EV_DEBUG_ENABLE: {
		return &debug;
	}

	/* Following 3 events for EMCY check during powering up */
	/* Special trick to use DISABLED as output during start up to be able to read status on EMCY button */
	case EV_ENTRY: {
		/* PP outputs */
		// HzS - GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		// HzS - GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		// HzS - GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		// HzS - GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		// HzS - GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	// DISABLED_A_N
		// HzS - GPIO_Init(GPIOB, &GPIO_InitStructure);

		// HzS - dio_set_output(DOUT_DISABLED_A_N, true);
		// HzS - dio_set_output(DOUT_EMERG_EN, true);

		// HzS -  timer_set(&delay, 50);
		check_emcy_pin_manually = true;

		statemachine_subscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_EXIT: {				// will never occur!?

		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&delay) && check_emcy_pin_manually)			// only check pins manually until "powered" state is entered,
		// HzS -  {																// then the selftest and BRK function will take over
			// HzS - if( !dio_get_input(DIN_EMERG_BREAK_A_N_PB5) && !dio_get_input(DIN_EMERG_BREAK_A_N_PD2) )
			// HzS - {
			// HzS - 	rgb_led_color(RGB_RED);
			// HzS - }
		// HzS -  }
		break;
	}
	}
	return 0;
}

static const struct state *debug_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_DEBUG_DISABLE: {
		return &shutdown;
	}
	}
	return 0;
}

static const struct state *init_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_PWR_WAKEUP: {
		if (ev->data == WAKEUP_REASON_LOCAL || ev->data == WAKEUP_REASON_PWON) {
			return &wait_for_button;
		} else {
			return &shutdown;
		}
	}
	case EV_BTN_SHORT: {
		return &wait_for_wakeup;
	}
	}
	return 0;
}

static const struct state *wait_for_wakeup_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_PWR_WAKEUP: {
		return &shutdown;
	}
	}
	return 0;
}

static const struct state *wait_for_button_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		// HzS -  timer_set(&m->t_main, 2000);
		statemachine_subscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_BTN_DOWN: {
		return statemachine_self_transition();
	}
	case EV_BTN_HOLD: {
		return &wait_for_power;
	}
	case EV_BTN_SHORT: {
		return &shutdown;
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_main)) {
		if (1 == 0){
			return &shutdown;
		}
		break;
	}
	}
	return 0;
}

static const struct state *wait_for_power_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_ENTRY: {

		// HzS -  rgb_led_color(RGB_GREEN_FLASH(100));

		hsm_broadcast(sm, EV_CAN_START, 0);
		return statemachine_event_handled();
	}
	case EV_CAN_POWERED: {
		return &powered;
	}
	case EV_CAN_FAIL: {
		 // HzS - if (param.psu_ok) {
		// HzS - 	return &check_supply;
		// HzS - } else {
		// HzS - 	// HzS -  log_event(LOG_EV_POWER_OFF_BAT);
		// HzS - 	return &error;
		// HzS - }
	}
	}
	return 0;
}

static const struct state *check_supply_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		// HzS -  timer_set(&m->t_main, 100);
		statemachine_subscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_DO: {
		 // HzS - if (timer_expired(&m->t_main)) {
		// HzS - 	if ((s16b16)aio_get_input(AIN_VBAT) * 75 > int_s16b16(30)) {
		// HzS - 		return &powered;
		// HzS - 	} else {
		// HzS - 		// HzS -  log_event(LOG_EV_POWER_OFF_SUPPLY);
		// HzS - 		return &error;
		// HzS - 	}
		// HzS - }
		break;
	}
	}
	return 0;
}

static const struct state *shutdown_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_ENTRY: {
		/* TODO: Clean up */
		hsm_broadcast(sm, EV_PWR_SHUTDOWN, 0);
		return statemachine_event_handled();
	}
	case EV_PWR_WAKEUP: {
		if (ev->data == WAKEUP_REASON_LOCAL || ev->data == WAKEUP_REASON_PWON) {
			return &wait_for_button;
		} else {
			hsm_broadcast(sm, EV_PWR_SHUTDOWN, 0);
			return statemachine_event_handled();
		}
	}
	}
	return 0;
}

static const struct state *bt_pair_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *s = container_of(sm, struct main_sm, sm);

	switch (ev->id) {
	case EV_ENTRY: {
		statemachine_subscribe_do(sm);
		// HzS -  timer_set(&s->t_main, 30000);
		// HzS -  bt_enable_pairing(true);
		// HzS -  rgb_led_color(RGB_BLUE_FLASH(100));
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		// HzS -  bt_enable_pairing(false);
		// HzS -  rgb_led_color(RGB_OFF);
		return statemachine_event_handled();
	}
	case EV_BT_CONNECT: {
		return &idle;
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&s->t_main)) {
		if (1 == 0) {
			return &idle;
		}
		break;
	}
	}
	return 0;
}

static const struct state *wait_shutdown_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_ENTRY: {
		hsm_broadcast(sm, EV_CAN_STOP, 0);
		return statemachine_event_handled();
	}
	case EV_CAN_UNPOWERED: {
		return &wait_save;
	}
	}
	return 0;
}

static const struct state *enter_boot_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_ENTRY: {
		hsm_broadcast(sm, EV_CAN_STOP, 0);
		return statemachine_event_handled();
	}
	case EV_CAN_UNPOWERED: {
		// HzS -  boot_start_bootload(BOOT_ID_AREA_BOOT2);
		return &error;
	}
	}
	return 0;
}

static volatile bool save_done_flag;
static struct timer e2_write_timeout_timer;

static void save_done(int s)
{
	save_done_flag = true;
}

static const struct state *wait_save_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_ENTRY: {
		save_done_flag = false;
		// HzS -  timer_set(&e2_write_timeout_timer, 20000);
		// HzS -  log_save(save_done);
		statemachine_subscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_DO: {
		// HzS -  if (save_done_flag || timer_expired(&e2_write_timeout_timer)) {
		if (1 == 0) {
			return &shutdown;
		}
		break;
	}
	}
	return 0;
}

static const struct state *powered_impl(struct statemachine *sm, const struct event *ev)
{
	// HzS -  GPIO_InitTypeDef GPIO_InitStructure;

	switch (ev->id) {
	case EV_INIT: {
		return &self_test;
	}
	case EV_ENTRY: {

		/* Following for EMCY check during powering up */
		/* Restore DOUT_DISABLED_A_N to input and clear variable for manually checking status of EMCY button status */
		check_emcy_pin_manually = false;
		// HzS -  dio_set_output(DOUT_DISABLED_A_N, false);
		/* Inputs w/ pull-down */
		// HzS - GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		// HzS - GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		// HzS - GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		// HzS - GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		// HzS - GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	// DISABLED_A_N
		// HzS - GPIO_Init(GPIOB, &GPIO_InitStructure);


		// HzS -  dio_set_output(DOUT_POWER_AUX_EN, true);
		// HzS -  log_event(LOG_EV_POWER_ON);
		statemachine_subscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
#if 0 /* TODO: De-init HW again? This is currently a one-shot operation which
       * is OK since we keep the battery alive until we power off completely. */
		dio_set_output(DOUT_POWER_AUX_EN, false);
#endif
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_ENTER_BOOT: {
		return &enter_boot;
	}
	case EV_DO: {
		// HzS -  log_update();
		break;
	}
	}
	return 0;
}

static const struct state *self_test_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_INIT: {
		return &bist_step_1;
	}
	case EV_ENTRY: {
		statemachine_subscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_EM_STOP: {
		if (ev->data == EM_STOP_TEMP) {
			// HzS -  log_event(LOG_EV_EM_TEMP);
			return &temp_error;
		}
		break;
	}
	}
	return 0;
}

static const struct state *bist_step_1_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		// HzS -  timer_set(&m->t_bist, 20);
		/* Prepare test */
		// HzS - dio_set_output(DOUT_EMERG_EN, false);
		// HzS - dio_set_output(DOUT_DRIVE_A_EN, false);
		// HzS - dio_set_output(DOUT_DRIVE_B_EN, false);
		return statemachine_event_handled();
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_bist)) {
			uint32_t major = 0;
			// HzS -  boot_get_int(BOOT_ID_AREA_BOOT, 7, &major);
			// HzS - if (major >= 2
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_A_N_PB5) == 0
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_A_N_PD2) == 0
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_B_N_PA10) == 0
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_B_N_PE15) == 0
			// HzS -  && dio_get_input(DIN_DISABLED_A_N) == 0
			// HzS -  && dio_get_input(DIN_DISABLED_B_N) == 0) {
			// HzS - 	return &bist_step_2; /* Next step or active */
			// HzS - } else if (major == 1
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_A_N_PB5) == 0
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_A_N_PD2) == 0
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_B_N_PE15) == 0) {
			// HzS - 	return &bist_step_2_old; /* Next step or active */
			// HzS - } else {
			// HzS - 	log_event(LOG_EV_SELF_TEST_FAIL_BREAK);
			// HzS - 	return &error;
			// HzS - }
		// HzS -  }
		break;
	}
	}
	return 0;
}

static const struct state *bist_step_2_old_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		// HzS -  timer_set(&m->t_bist, 50);
		/* Prepare test */
		// HzS -  dio_set_output(DOUT_EMERG_EN, true);
		return statemachine_event_handled();
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_bist)) {
			// HzS - if (dio_get_input(DIN_EMERG_BREAK_A_N_PB5) != 0
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_A_N_PD2) != 0
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_B_N_PE15) != 0) {
			// HzS - 	return &bist_step_3; /* Next step or active */
			// HzS - } else {
			// HzS - 	log_event(LOG_EV_SELF_TEST_FAIL_EMSTOP);
			// HzS - 	return &error;
			// HzS - }
		// HzS -  }
		break;
	}
	}
	return 0;
}

static const struct state *bist_step_2_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		// HzS -  timer_set(&m->t_bist, 50);
		/* Prepare test */
		// HzS -  dio_set_output(DOUT_EMERG_EN, true);
		// HzS -  dio_set_output(DOUT_DRIVE_A_EN, true);
		return statemachine_event_handled();
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_bist)) {
			// HzS - if (dio_get_input(DIN_EMERG_BREAK_A_N_PB5) != 0
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_A_N_PD2) != 0
			// HzS -  && dio_get_input(DIN_DISABLED_A_N) != 0) {
			// HzS - 	return param.num_motor_ch > 1 ? & bist_step_2_5 : &bist_step_3; /* Next step or active */
			// HzS - } else {
			// HzS - 	log_event(LOG_EV_SELF_TEST_FAIL_EMSTOP);
			// HzS - 	return &error;
			// HzS - }
		// HzS -  }
		break;
	}
	}
	return 0;
}

static const struct state *bist_step_2_5_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		// HzS -  timer_set(&m->t_bist, 50);
		/* Prepare test */
		// HzS -  dio_set_output(DOUT_EMERG_EN, true);
		// HzS -  dio_set_output(DOUT_DRIVE_B_EN, true);
		return statemachine_event_handled();
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_bist)) {
			// HzS - if (dio_get_input(DIN_EMERG_BREAK_B_N_PE15) != 0
			// HzS -  && dio_get_input(DIN_EMERG_BREAK_B_N_PA10) != 0
			// HzS -  && dio_get_input(DIN_DISABLED_B_N) != 0) {
			// HzS - 	return &bist_step_3; /* Next step or active */
			// HzS - } else {
			// HzS - 	log_event(LOG_EV_SELF_TEST_FAIL_EMSTOP);
			// HzS - 	return &error;
			// HzS - }
		// HzS -  }
		break;
	}
	}
	return 0;
}

static const struct state *bist_step_3_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id)
	{
		case EV_DO:
		{
			/* Check motor setup ok, basically checking current offsets. */
			// HzS -  if (hw_init()) {
				return &bist_step_4; /* Next step or active */
			// HzS - } else {
			// HzS - 	// HzS -  log_event(LOG_EV_SELF_TEST_FAIL_CURR);
			// HzS - 	return &error;
			// HzS - }
			break;
		}
		case EV_EM_STOP:
		{
			if (ev->data == EM_STOP_BREAK)
			{
				// HzS -  log_event(LOG_EV_EM_BREAK);
				return &error;
			}
			break;
		}
	}
	return 0;
}

static const struct state *bist_step_4_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id)
	{
		case EV_ENTRY:
		{
			// HzS -  timer_set(&m->t_bist, 50);
			/* Prepare test */
			// HzS -  motor_sincos_reset_fault();
			return statemachine_event_handled();
		}
		case EV_DO:
		{
			// HzS -  if (timer_expired(&m->t_bist)) {
				 // HzS - if (!motor_sincos_has_fault()) {
				// HzS - 	return &bist_step_5; /* Next step or active */
				// HzS - } else {
				// HzS - 	// HzS -  log_event(LOG_EV_SELF_TEST_FAIL_SINCOS);
				// HzS - 	return &error;
				// HzS - }
			// HzS -  }
			break;
		}
		case EV_EM_STOP:
		{
			if (ev->data == EM_STOP_BREAK)
			{
				// HzS -  log_event(LOG_EV_EM_BREAK);
				return &error;
			}
			break;
		}
	}
	return 0;
}

static const struct state *bist_step_5_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id)
	{
		case EV_DO:
		{
			s16b16 temp;
			// HzS -  temp = temp_get_value(TEMP_MOTOR_A);
			if (temp < int_s16b16(-40) || temp > int_s16b16(100)) {
				// HzS -  log_event(LOG_EV_SELF_TEST_FAIL_MOTOR_TEMP);
				return &temp_error;
			}
			// HzS -  temp = temp_get_value(TEMP_DRIVE_A);
			if (temp < int_s16b16(-40) || temp > int_s16b16(100)) {
				// HzS -  log_event(LOG_EV_SELF_TEST_FAIL_DRIVE_TEMP);
				return &temp_error;
			}
			 // HzS - if (param.num_motor_ch > 1) {
			// HzS - 	// HzS -  temp = temp_get_value(TEMP_DRIVE_B);
			// HzS - 	if (temp < int_s16b16(-40) || temp > int_s16b16(100)) {
			// HzS - 		// HzS -  log_event(LOG_EV_SELF_TEST_FAIL_DRIVE_TEMP);
			// HzS - 		return &temp_error;
			// HzS - 	}
			// HzS - }
			return &bist_step_6;
		}
		case EV_EM_STOP:
		{
			if (ev->data == EM_STOP_BREAK)
			{
				// HzS -  log_event(LOG_EV_EM_BREAK);
				return &error;
			}
			break;
		}
	}
	return 0;
}

static const struct state *bist_step_6_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id)
	{
		case EV_ENTRY:
		{
			// HzS -  timer_set(&m->t_bist, 200);
			/* Prepare test */
			// HzS -  m->brake_test.start_pos = motor_get_position();
			// HzS -  m->brake_test.max_diff = 0;
			// HzS -  motor_manual_mode(MAN_CURRENT);
			// HzS -  motor_set_iq(0, mul(param.brake_test.pos_curr, param.motor.cl_param.max));
			// HzS -  motor_set_iq(1, mul(param.brake_test.pos_curr, param.motor.cl_param.max));
			// HzS -  motor_enable();
			return statemachine_event_handled();
		}
		case EV_EXIT:
		{
			// HzS -  motor_set_iq(0, 0);
			// HzS -  motor_set_iq(1, 0);
			return statemachine_event_handled();
		}
		case EV_DO:
		{
			// HzS -  s16b16 diff = abs_diff_pos(m->brake_test.start_pos, motor_get_position());
			// HzS - if (m->brake_test.max_diff < diff) {
			// HzS - 	m->brake_test.max_diff = diff;
			// HzS - }
			 // HzS - if (diff > float_s16b16(0.05)) {
			// HzS - 	motor_disable();
			// HzS - 	// HzS -  motor_manual_mode(MAN_NONE);
			// HzS - 	// HzS -  log_event(LOG_EV_SELF_TEST_FAIL_BRAKE);
			// HzS - 	return &error;
			// HzS - } else if (timer_expired(&m->t_bist)) {
			// HzS -  	LOG_DEBUG("Max diff: %F\n", m->brake_test.max_diff);
				return &bist_step_7;
			// HzS -  }
			break;
		}
		case EV_EM_STOP:
		{
			if (ev->data == EM_STOP_BREAK)
			{
				// HzS -  log_event(LOG_EV_EM_BREAK);
				return &error;
			}
			break;
		}
	}
	return 0;
}

static const struct state *bist_step_7_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id)
	{
		case EV_ENTRY:
		{
			// HzS -  timer_set(&m->t_bist, 200);
			/* Prepare test */
			m->brake_test.max_diff = 0;
			// HzS -  motor_set_iq(0, mul(param.brake_test.neg_curr, param.motor.cl_param.min));
			// HzS -  motor_set_iq(1, mul(param.brake_test.neg_curr, param.motor.cl_param.min));
			return statemachine_event_handled();
		}
		case EV_EXIT:
		{
			// HzS -  motor_disable();
			// HzS -  motor_manual_mode(MAN_NONE);
			// HzS -  motor_set_iq(0, 0);
			// HzS -  motor_set_iq(1, 0);
			return statemachine_event_handled();
		}
		case EV_DO:
		{
			// HzS - s16b16 diff = abs_diff_pos(m->brake_test.start_pos, motor_get_position());
			// HzS - if (m->brake_test.max_diff < diff) {
			// HzS - 	m->brake_test.max_diff = diff;
			// HzS - }
			// HzS - if (diff > float_s16b16(0.05)) {
			// HzS - 	// HzS -  log_event(LOG_EV_SELF_TEST_FAIL_BRAKE);
			// HzS - 	return &error;
			// HzS - } else if (timer_expired(&m->t_bist)) {
			// HzS - 	LOG_DEBUG("Max diff: %F\n", m->brake_test.max_diff);
				return &active;
			// HzS -  }
			break;
		}
		case EV_EM_STOP:
		{
			if (ev->data == EM_STOP_BREAK)
			{
				// HzS -  log_event(LOG_EV_EM_BREAK);
				return &error;
			}
			break;
		}
	}
	return 0;
}

static const struct state *error_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		// HzS -  rgb_led_color(RGB_RED);
		statemachine_subscribe_do(sm);
		// HzS -  timer_set(&m->t_powered, DELAY_ERROR);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		// HzS -  rgb_led_color(RGB_OFF);
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_powered)) {
		if (1 == 0) {
			return &wait_shutdown;
		}
		break;
	}
	}
	return 0;
}

static const struct state *temp_error_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		// HzS -  rgb_led_color(RGB_ORANGE);
		statemachine_subscribe_do(sm);
		// HzS -  timer_set(&m->t_powered, DELAY_ERROR);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		// HzS -  rgb_led_color(RGB_OFF);
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_powered)) {
		if (1 == 0) {
			return &wait_shutdown;
		}
		break;
	}
	}
	return 0;
}

static const struct state *wait_btn_release_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_BTN_LONG: {
		// HzS -  log_event(LOG_EV_POWER_OFF);
		return &wait_shutdown;
	}
	}
	return 0;
}

static const struct state *active_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_INIT: {
		return &idle;
	}
	case EV_ENTRY: {
		/* panel_enable() */
		// HzS -  dio_set_output(DOUT_DRIVE_A_EN, true);  // HzS - drive what? 
		// HzS -  dio_set_output(DOUT_DRIVE_B_EN, true);
		// HzS -  rgb_led_color(RGB_GREEN);
		control_local_enabled(true);            // HzS - activate throttle and green LED
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		// HzS -  dio_set_output(DOUT_DRIVE_A_EN, false);
		// HzS -  dio_set_output(DOUT_DRIVE_B_EN, false);
		control_local_enabled(false);            // HzS - deactivate throttle and LED off
		// HzS -  rgb_led_color(RGB_OFF);
		return statemachine_event_handled();
	}
	case EV_BTN_HOLD: {
		// HzS -  rgb_led_color(RGB_OFF);
		return &wait_btn_release;
	}
	case EV_EM_STOP: {
		if (ev->data == EM_STOP_BREAK) {
			// HzS -  log_event(LOG_EV_EM_BREAK);
		} else if (ev->data == EM_STOP_TEMP) {
			// HzS -  log_event(LOG_EV_EM_TEMP);
			return &temp_error;
		} else if (ev->data == EM_STOP_SUPPLY_HIGH) {
			// HzS -  log_event(LOG_EV_EM_SUPPLY_HIGH);
		} else if (ev->data == EM_STOP_SUPPLY_LOW) {
			// HzS -  log_event(LOG_EV_EM_SUPPLY_LOW);
		} else if (ev->data == EM_STOP_SINCOS) {
			// HzS -  log_event(LOG_EV_EM_SINCOS);
		}
		return &error;
	}
	case EV_CTRL_ENABLED: {
		if (ev->data == CTRL_SOURCE_BT) {
			// HzS -  rgb_led_color(RGB_BLUE);
			return statemachine_event_handled();
		}
		break;
	}
	case EV_CTRL_DISABLED: {
		if (ev->data == CTRL_SOURCE_BT) {
			// HzS -  rgb_led_color(RGB_GREEN);
			return statemachine_event_handled();
		}
		break;
	}
	}
	return 0;
}

static const struct state *multi_button_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *s = container_of(sm, struct main_sm, sm);

	switch (ev->id) {
	case EV_ENTRY: {
		statemachine_subscribe_do(sm);
		// HzS -  timer_set(&s->t_main, 500);
		s->btn_count = 0;
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_BTN_SHORT: {
		s->btn_count++;
		// HzS -  timer_restart(&s->t_main);
		return statemachine_event_handled();
	}
	case EV_DO: {
		// HzS - if (timer_expired(&s->t_main)) {
		// HzS - 	if (s->btn_count == 5) {
		// HzS - 		return &bt_pair;
		// HzS - 	} else if (s->btn_count == 2) {
		// HzS - 		// HzS -  bool led_level = !led_get_brightness();
		// HzS - 		// HzS -  led_set_brightness(led_level);
		// HzS - 		// HzS -  rgb_led_set_brightness(led_level ? param.led_duty_hi : param.led_duty_lo);
		// HzS - 	}
		// HzS - 	return &idle;
		// HzS - }
		break;
	}
	}
	return 0;
}

static const struct state *idle_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		statemachine_subscribe_do(sm);
		// HzS -  timer_set(&m->t_drive, param.idle_timeout * 1000);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_BTN_DOWN: {
		return &multi_button;
	}
	case EV_CTRL_RUN: {
		return &drive_enabled;
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_drive)) {
			// HzS -  log_event(LOG_EV_POWER_OFF_TIMEOUT);
			return &wait_shutdown;
		// HzS -  }
		break;
	}
	}
	return 0;
}

static const struct state *drive_enabled_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_INIT: {
		return &enabling;
	}
	case EV_ENTRY: {
		// HzS -  trajec_reset();
		// HzS -  motor_set_speed(trajec_get_speed());
		// HzS -  motor_enable();
		// HzS -  log_event(LOG_EV_START);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		// HzS -  motor_disable();
		// HzS -  motor_set_speed(0);
		// HzS -  log_event(LOG_EV_STOP);
		return statemachine_event_handled();
	}
	case EV_CTRL_DISABLED:
	case EV_CTRL_SHUTDOWN: {
		statemachine_defer(sm, ev->id, ev->data);
		return statemachine_event_handled();
	}
	}
	return 0;
}

static const struct state *enabling_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		statemachine_subscribe_do(sm);
		// HzS -  timer_set(&m->t_drive, DELAY_MOTOR);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_CTRL_STOP: {
		return &idle;
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_drive)) {
		if (1 == 0) {
			return &brake_released;
		}
		break;
	}
	}
	return 0;
}

static const struct state *brake_released_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_INIT: {
		return &releasing;
	}
	case EV_ENTRY: {
		// HzS -  brake_release();
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		// HzS -  brake_engage();
		return statemachine_event_handled();
	}
	}
	return 0;
}

static const struct state *releasing_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		statemachine_subscribe_do(sm);
		// HzS -  timer_set(&m->t_drive, DELAY_BRAKE);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_CTRL_STOP : {
		return &braking;
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_drive)) {
		if (1 == 0) {
			return &running;
		}
		break;
	}
	}
	return 0;
}

static const struct state *running_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id)
	{
		case EV_ENTRY: {
			statemachine_subscribe_do(sm);
			return statemachine_event_handled();
		}
		case EV_EXIT: {
			statemachine_unsubscribe_do(sm);
			// HzS -  trajec_set_target_speed(0);
			return statemachine_event_handled();
		}
		case EV_CTRL_STOP: {
			return &stopping;
		}
		case EV_DO:
		{

			/* If battery SOC is <= 8% only allow running down */

			// HzS -  s16b16 targetspeed = mul(motor_get_max_speed(), control_get_value());

			// HzS - if(BatterySOC > param.soc_limit_run_up)
			// HzS - {
			// HzS - 	trajec_set_target_speed(targetspeed);

			// HzS - }
			// HzS - else
			// HzS - {
			// HzS - 	if(targetspeed > 0)
			// HzS - 	{
			// HzS - 		trajec_set_target_speed(targetspeed);
			// HzS - 	}
			// HzS - 	else
			// HzS - 	{
			// HzS - 		trajec_set_target_speed(0);
			// HzS - 	}
			// HzS - }

			// HzS -  motor_set_speed(trajec_get_speed());

			break;
		}
	}
	return 0;
}

static const struct state *stopping_impl(struct statemachine *sm, const struct event *ev)
{
	switch (ev->id) {
	case EV_ENTRY: {
		statemachine_subscribe_do(sm);
		// HzS -  trajec_set_target_speed(0);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_CTRL_RUN : {
		return &running;
	}
	case EV_DO: {
		// HzS -  motor_set_speed(trajec_get_speed());
		// HzS -  if (trajec_get_speed() == 0) {
		if (1 == 0) {
			return &braking;
		}
		break;
	}
	}
	return 0;
}

static const struct state *braking_impl(struct statemachine *sm, const struct event *ev)
{
	struct main_sm *m = container_of(sm, struct main_sm, sm);
	switch (ev->id) {
	case EV_ENTRY: {
		statemachine_subscribe_do(sm);
		// HzS -  timer_set(&m->t_drive, DELAY_BRAKE);
		return statemachine_event_handled();
	}
	case EV_EXIT: {
		statemachine_unsubscribe_do(sm);
		return statemachine_event_handled();
	}
	case EV_CTRL_RUN : {
		return &releasing;
	}
	case EV_DO: {
		// HzS -  if (timer_expired(&m->t_drive)) {
		if (1 == 0) {
			return &idle;
		}
		break;
	}
	}
	return 0;
}

struct statemachine *hsm_main_init(void)
{
	main_sm_instance.sm.name = "main";
	statemachine_create(&main_sm_instance.sm, &main_sm);
	return &main_sm_instance.sm;
}
