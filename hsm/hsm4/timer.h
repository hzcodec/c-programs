//============================================================================
//
// $URL: svn+ssh://ristna.unjo.local/var/svn/220_06_005_COMMON/trunk/include/timer.h $
// $Revision: 5425 $
// $Date: 2012-04-24 11:06:19 +0200 (tis, 24 apr 2012) $
// $Author: andreas $
//
// Description :
//
// Copyright   : UNJO AB
//
//============================================================================

/** @addtogroup sys
 * @{ */

/**
 * @defgroup timer Timer library
 *
 * The timer library provides functions for setting, reloading and
 * restarting timers, and for checking if a timer has expired. An
 * application must "manually" check if its timers have expired; this
 * is not done automatically.
 *
 * A timer is declared as a \c struct \c timer and all access to the
 * timer is made by a pointer to the declared timer.
 * @{
 */


/**
 * @file
 * Timer library header file.
 * @author
 * Adam Dunkels <adam@sics.se>
 * @author
 * Andreas Fritiofson <andreas@unjo.com>
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef unsigned int timer_time_t;

/**
 * A timer.
 *
 * This structure is used for declaring a timer. The timer must be set
 * with timer_set() before it can be used.
 *
 * @hideinitializer
 */
struct timer {
  timer_time_t start;
  timer_time_t interval;
};

void timer_driver(void);
timer_time_t timer_time(void);
void timer_set(struct timer *t, timer_time_t interval);
void timer_reload(struct timer *t);
void timer_restart(struct timer *t);
int timer_expired(struct timer *t);
timer_time_t timer_remaining(struct timer *t);


#endif /* TIMER_H_ */

/** @} */
/** @} */
