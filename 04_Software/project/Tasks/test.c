#include "test.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "log.h"
#include "usart.h"
#include "buffer.h"
#define BUFF_SIZE DATA_SIZE
TaskHandle_t b_task_handle;
TaskHandle_t a_task_handle;
QueueHandle_t buff_addr_queue;
uint8_t *buff1, *buff2;
cir_buff_t *buff3,*buff4;
void a_task(void *param)
{
	uint32_t buff_addr;
	uint8_t *buff;
	while(1)
	{
		xTaskNotifyWait(0, 0xffffffff, &buff_addr, portMAX_DELAY);
		buff = (uint8_t*)buff_addr;
		for(uint8_t i = 1; i<DATA_SIZE-1;i++)
		{
			*(buff+i) +=1;
		}
		for(uint8_t i = 0; i<DATA_SIZE;i++)
		{
			LOGI("a: %x", *(buff+i));
		}
	}
}

uint8_t *get_buff_addr()
{
	static uint8_t flag;
	if(flag)
	{
		flag = !flag;
		return buff1;
	}
	else
	{
		flag = !flag;
		return buff2;
	}
}
void b_task(void *param)
{
	buff1 = (uint8_t*)pvPortMalloc(BUFF_SIZE*sizeof(uint8_t));
	buff2 = (uint8_t*)pvPortMalloc(BUFF_SIZE*sizeof(uint8_t));
	if(NULL == buff1||NULL == buff2)
		LOGE("buff1 or buff2 is NULL");
	buff3 = create_buffer();
	buff4 = create_buffer();
	if(NULL == buff3||NULL == buff4)
		LOGE("buff1 or buff2 is NULL");

	buff_addr_queue = xQueueCreate(1, sizeof(uint8_t*));
	if(NULL == buff_addr_queue)
		LOGE("buff_addr_queue is NULL");
	uint8_t *cur_buff = NULL;
	extern uint8_t uart1_data;
	HAL_UART_Receive_IT(&huart1, &uart1_data, 1);
	while(1)
	{
		cur_buff = get_buff_addr();
		xQueueSend(buff_addr_queue, &cur_buff, portMAX_DELAY);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		LOGI("recv notify");
		for(uint8_t i = 1; i < DATA_SIZE-1; i++)
		{
			*cur_buff += 1;
		}
		xTaskNotify(a_task_handle, (uint32_t)cur_buff, eSetValueWithOverwrite);
		
	}
}
