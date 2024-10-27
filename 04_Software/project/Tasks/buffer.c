#include "buffer.h"
#include "FreeRTOS.h"
#include "task.h"



cir_buff_t *create_buffer()
{
	cir_buff_t *cb = (cir_buff_t*)pvPortMalloc(sizeof(cir_buff_t));
	if(NULL == cb)
		return NULL;
	cb->buffer = (uint8_t*)pvPortMalloc(BUFFER_SIZE*sizeof(uint8_t));
	if(NULL == cb->buffer)
		return NULL;
	cb->head = 0;
	cb->tail = 0;
	cb->size = 0;
	return cb;
}

uint8_t write_buffer(cir_buff_t *cb, uint8_t data)
{
	if(BUFFER_SIZE == cb->size)
		return 1;
	cb->buffer[cb->head] = data;
	cb->head = (cb->head + 1)%BUFFER_SIZE;
	cb->size++;
	return 0;
}
uint8_t read_buffer(cir_buff_t *cb, uint8_t *data)
{
	if(!cb->size)
		return 1;
	*data = cb->buffer[cb->tail];
	cb->tail = (cb->tail + 1)%BUFFER_SIZE;
	cb->size--;
	return 0;
}
