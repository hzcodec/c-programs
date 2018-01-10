//============================================================================
//
// $URL: svn+ssh://ristna.unjo.local/var/svn/220_06_005_COMMON/trunk/src/queue.c $
// $Revision: 10323 $
// $Date: 2015-12-04 17:17:38 +0100 (fre, 04 dec 2015) $
// $Author: andreas.fritiofson $
//
// Description :
//
// Copyright   : UNJO AB
//
//============================================================================

/**
 * @addtogroup queue
 * @{
 */

/**
 * @file
 * Queue implementation.
 * @author
 * Andreas Fritiofson <andreas@unjo.com>
 */

#include <string.h>
#include <stdlib.h>
#include "compiler.h"
#include "queue.h"

/** Internal implementation of the queue */
struct queue {
	/**
	 * Size in items of the queue
	 */
	unsigned int queue_size;
	/**
	 * Size in bytes of each item
	 */
	unsigned int item_size;
	/**
	 * Item offset into data where the next item can be placed
	 */
	volatile unsigned int put_index;
	/**
	 * Item offset into data where the next item can be read, if the queue
	 * is not empty
	 */
	volatile unsigned int get_index;
	/**
	 * Storage for the items in the queue
	 */
	char data[1];
};

/**
 * @brief Create a queue object.
 * @param queue_length Maximum number of items the queue can hold.
 * @param item_size Size of each item.
 * @return Handle to the queue, or NULL if memory allocation fails.
 *
 * Allocates memory for a queue and initializes it.
 */
queue_t *queue_create(unsigned int queue_length, unsigned int item_size) {
	queue_t *q;
	unsigned int queue_size = queue_length + 1;

	q = malloc(sizeof(struct queue) + queue_size * item_size);

	if (q) {
		q->queue_size = queue_size;
		q->item_size = item_size;
		q->put_index = 0;
		q->get_index = 0;
	}

	return q;
}

/**
 * @brief Destroy a queue object, freeing its resources.
 * After this call, the queue handle is invalid and must not be referenced. Any
 * pointers retrieved by the queue_get_buffer() and queue_put_buffer() calls
 * are also invalidated. All items the queue may have held are lost.
 * @param queue The queue handle.
 */
void queue_destroy(queue_t *queue) {
	free(queue);
}

/**
 * @brief Inserts an item in the queue.
 * @param queue The queue handle.
 * @param[in] item Pointer to the item to be inserted.
 * @return True if insertion succeeded, false if the queue was full.
 *
 * @note The item pointer must point to an object of at least the same size as
 *   the item_size passed to queue_create().
 */
bool queue_put(queue_t *queue, const void *item) {
	unsigned int put_index;
	unsigned int next_put_index;

	// get index of this put
	put_index = queue->put_index;

	// get index after successful put
	next_put_index = put_index + 1;
	if (next_put_index >= queue->queue_size) {
		next_put_index = 0;
	}

	// check if the queue is full (this put would make the queue look empty)
	if (next_put_index == queue->get_index) {
		return false; // queue is full
	}

	// copy the item into the queue and update the index
	memcpy(queue->data + put_index * queue->item_size, item, queue->item_size);

	REORDER_BARRIER();

	queue->put_index = next_put_index;

	return true; // success
}

/**
 * @brief Gets an item from the queue.
 * @param queue The queue handle.
 * @param[out] item Pointer to where the item will be copied.
 * @return True if an item was fetched, false if the queue was empty.
 *
 * @note The item pointer must point to an object of at least the same size as
 *   the item_size passed to queue_create().
 */
bool queue_get(queue_t *queue, void *item) {
	unsigned int get_index;
	unsigned int item_size;

	// get index of item to fetch, if there is one
	get_index = queue->get_index;

	// check if the queue is empty
	if (get_index == queue->put_index) {
		return false; // queue is empty
	}

	item_size = queue->item_size;

	// get the item and update the index
	memcpy(item, queue->data + get_index * item_size, item_size);

	// increment index with wraparound
	get_index++;
	if (get_index >= queue->queue_size) {
		get_index = 0;
	}

	REORDER_BARRIER();

	queue->get_index = get_index;

	return true; // success
}

/**
 * @brief Get the current length of the queue.
 * @param queue The queue handle.
 * @return The number of items currently in the queue.
 */
unsigned int queue_length(const queue_t *queue) {
	unsigned int length;
	unsigned int get_index;
	unsigned int put_index;

	get_index = queue->get_index;
	put_index = queue->put_index;

	if (get_index > put_index) {
		length = queue->queue_size + put_index - get_index;
	} else {
		length = put_index - get_index;
	}

	return length;
}

/**
 * @brief Get the available space in the queue.
 * @param queue The queue handle.
 * @return The number of items the queue can currently take.
 */
unsigned int queue_avail(const queue_t *queue) {
	unsigned int length;
	unsigned int get_index;
	unsigned int put_index;

	get_index = queue->get_index;
	put_index = queue->put_index;

	if (get_index > put_index) {
		length = get_index - put_index - 1;
	} else {
		length = queue->queue_size - put_index + get_index - 1;
	}

	return length;
}

/**
 * Query the queue about the size of its items.
 * @param queue The queue handle.
 * @return The item_size passed to queue_create().
 */
unsigned int queue_item_size(const queue_t *queue) {
	return queue->item_size;
}

/**
 * @brief Gets a pointer to a contiguous buffer of items from the queue.
 * This is used for DMA or bulk copy purposes. This function does not alter the
 * queue; in order to actually consume the items, queue_consume_buffer() must
 * be called after copying or transferring items.
 * @param queue The queue handle.
 * @param[out] item_buffer Will receive a pointer to the item buffer.
 * @return The number of items the buffer pointed to by *item_buffer contains.
 */
unsigned int queue_get_buffer(queue_t *queue, const void **item_buffer) {
	unsigned int non_wrapping_length;
	unsigned int get_index;
	unsigned int put_index;

	get_index = queue->get_index;
	put_index = queue->put_index;

	if (get_index > put_index) {
		non_wrapping_length = queue->queue_size - get_index;
	} else {
		non_wrapping_length = put_index - get_index;
	}

	if (item_buffer) {
		*item_buffer = queue->data + get_index * queue->item_size;
	}

	return non_wrapping_length;
}

/**
 * @brief Consume items from the buffer returned by queue_get_buffer().
 * This can be called multiple times between calls to queue_get_buffer(), as
 * long as the sum of the items parameters never exceed the return value of the
 * most recent queue_get_buffer() call.
 * After this call, the part of the buffer holding the consumed items must not
 * be accessed.
 * @param queue The queue handle.
 * @param items The number of items consumed.
 */
void queue_consume_buffer(queue_t *queue, unsigned int items) {
	unsigned int get_index;

	get_index = queue->get_index;
	get_index += items;
	if (get_index >= queue->queue_size) {
		get_index = 0;
	}

	REORDER_BARRIER();

	queue->get_index = get_index;
}

/**
 * @brief Gets a pointer to a contiguous buffer of empty space from the queue.
 * This is used for DMA or bulk copy purposes. This function does not alter the
 * queue; in order to actually produce the items, queue_produce_buffer() must
 * be called after copying items into the buffer.
 * @param queue The queue handle.
 * @param[out] item_buffer Will receive a pointer to the empty item buffer.
 * @return The number of items the buffer pointed to by *item_buffer can hold.
 */
unsigned int queue_put_buffer(queue_t *queue, void **item_buffer) {
	unsigned int non_wrapping_length;
	unsigned int get_index;
	unsigned int put_index;

	get_index = queue->get_index;
	put_index = queue->put_index;

	if (get_index > put_index) {
		non_wrapping_length = get_index - put_index - 1;
	} else if (get_index > 0) {
		non_wrapping_length = queue->queue_size - put_index;
	} else {
		non_wrapping_length = queue->queue_size - put_index - 1;
	}

	if (item_buffer) {
		*item_buffer = queue->data + put_index * queue->item_size;
	}

	return non_wrapping_length;
}

/**
 * @brief Commit items copied into the buffer returned by queue_put_buffer().
 * This can be called multiple times between calls to queue_put_buffer(), as
 * long as the sum of the items parameters never exceed the return value of the
 * most recent queue_put_buffer() call.
 * After this call, the part of the buffer holding the produced items must not
 * be accessed.
 * @param queue The queue handle.
 * @param items The number of items produced.
 */
void queue_produce_buffer(queue_t *queue, unsigned int items) {
	unsigned int put_index;

	put_index = queue->put_index;
	put_index += items;
	if (put_index >= queue->queue_size) {
		put_index = 0;
	}

	REORDER_BARRIER();

	queue->put_index = put_index;
}

/** @} */
