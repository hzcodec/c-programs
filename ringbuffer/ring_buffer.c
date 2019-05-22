/*
    Auther       : Heinz Samuelsson
    Date         : 2018-08-22
    File         : ring_buffer.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ring_buffer.h" 

void rb_init(RingBuffer* rb, int size)
{
	rb->buffer = malloc(sizeof(int) * size);
	rb->buffer_end = rb->buffer + size;
	rb->size = size;
	rb->data_start = rb->buffer;
	rb->data_end = rb->buffer;
	rb->count = 0;
	printf("%s() - buffer=%p, buffer_end=%p, size=%d\n", __func__, rb->buffer, rb->buffer_end, rb->size);
}

void rb_free(RingBuffer* rb)
{
    free(rb->buffer);
}

bool rb_push(RingBuffer* rb, int data)
{
	if (rb == NULL || rb->buffer == NULL)
	    return false;
	
	*rb->data_end = data;
	rb->data_end++;
	printf("%s() - data=%d, data_end=%p\n", __func__, *rb->data_end, rb->data_end);

	if (rb->data_end == rb->buffer_end)
	    rb->data_end = rb->buffer;
	
	if (rb_full(rb)) {
	    if ((rb->data_start + 1) == rb->buffer_end)
	        rb->data_start = rb->buffer;
	    else
	        rb->data_start++;
	} else {
	    rb->count++;
	}
	
	return true;
}

int rb_pop(RingBuffer* rb)
{
	printf("%s() -\n", __func__);

	if (rb == NULL || rb->buffer == NULL)
	    return false;
	
	int data = *rb->data_start;
	rb->data_start++;

	if (rb->data_start == rb->buffer_end) {
	    rb->data_start = rb->buffer;
	}

	rb->count--;
	
	return data;
}

bool rb_full(RingBuffer* rb)
{
    return rb->count == rb->size;
}

 
int main(int argc, char *argv[])
{
 	RingBuffer rb;

	rb_init(&rb, 10);

	rb_push(&rb, 200);
	rb_push(&rb, 100);

	int rv = rb_pop(&rb);
	printf("rv=%d\n", rv);
	rv = rb_pop(&rb);
	printf("rv=%d\n", rv);

	printf("Status=%d\n", rb_full(&rb));

        return 0;
}

