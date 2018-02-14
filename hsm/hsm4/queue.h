//============================================================================
//
// $URL: svn+ssh://ristna.unjo.local/var/svn/220_06_005_COMMON/trunk/include/queue.h $
// $Revision: 9330 $
// $Date: 2015-01-12 12:10:02 +0100 (mån, 12 jan 2015) $
// $Author: andreas.fritiofson $
//
// Description :
//
// Copyright   : UNJO AB
//
//============================================================================

/**
 * @addtogroup sys
 * @{
 */

/**
 * @defgroup queue Queue library
 *
 * A lock-free and wait-free, single-producer, single-consumer queue for fixed
 * (but arbitrarily) sized items.
 *
 * This queue implementation is intended to synchronize data flow between two
 * different preemptive contexts, such as between an interrupt handler and
 * application code.
 * @{
 */

/**
 * @file
 * Queue library header file.
 * @author
 * Andreas Fritiofson <andreas@unjo.com>
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>

/**
 * Opaque type for the queue object.
 */
typedef struct queue queue_t;

queue_t *queue_create(unsigned int queue_length, unsigned int item_size);
void queue_destroy(queue_t *queue);

bool queue_put(queue_t *queue, const void *item);
bool queue_get(queue_t *queue, void *item);

unsigned int queue_length(const queue_t *queue);
unsigned int queue_avail(const queue_t *queue);
unsigned int queue_item_size(const queue_t *queue);

unsigned int queue_get_buffer(queue_t *queue, const void **item_buffer);
void queue_consume_buffer(queue_t *queue, unsigned int items);

unsigned int queue_put_buffer(queue_t *queue, void **item_buffer);
void queue_produce_buffer(queue_t *queue, unsigned int items);

#endif /* QUEUE_H_ */

/** @} */
/** @} */
