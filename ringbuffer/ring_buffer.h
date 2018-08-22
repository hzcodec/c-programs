#ifndef _RING_BUFFER_H_
#define _RING_BUFFERE_H_

typedef struct
{
     int* buffer;
     int* buffer_end;
     int* data_start;
     int* data_end;
     int  count;
     int  size;
 } RingBuffer;


void rb_init(RingBuffer* rb, int size);
void rb_free(RingBuffer* rb);
bool rb_push(RingBuffer* rb, int data);
int  rb_pop(RingBuffer* rb);
bool rb_full(RingBuffer* rb);

#endif
