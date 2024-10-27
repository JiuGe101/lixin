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
QueueHandle_t buff_print_queue;
uint8_t *buff1, *buff2;
cir_buff_t *buff3,*buff4;
void a_task(void *param)
{
	uint32_t buff_addr;
	uint8_t data[DATA_SIZE];
	while(1)
	{
		//xTaskNotifyWait(0, 0xffffffff, &buff_addr, portMAX_DELAY);
		xQueueReceive(buff_print_queue, data, portMAX_DELAY);
		for(uint8_t i = 0; i<DATA_SIZE;i++)
		{
			LOGI("a: %x", data[i]);
		}
	}
}

cir_buff_t *get_buff_addr()
{
	static uint8_t flag;
	if(flag)
	{
		flag = !flag;
		return buff3;
	}
	else
	{
		flag = !flag;
		return buff4;
	}
}
void b_task(void *param)
{
	buff3 = create_buffer();
	buff4 = create_buffer();
	if(NULL == buff3||NULL == buff4)
		LOGE("buff1 or buff2 is NULL");
	buff_addr_queue = xQueueCreate(1, sizeof(cir_buff_t*));
	if(NULL == buff_addr_queue)
		LOGE("buff_addr_queue is NULL");
	buff_print_queue = xQueueCreate(2, DATA_SIZE*sizeof(uint8_t));
	if(NULL == buff_print_queue)
		LOGE("buff_print_queue is NULL");
	cir_buff_t *cur_buff = NULL;
	extern uint8_t uart1_data;
	HAL_UART_Receive_IT(&huart1, &uart1_data, 1);
	uint8_t data_buff[BUFF_SIZE];
	while(1)
	{
		cur_buff = get_buff_addr();
		xQueueSend(buff_addr_queue, &cur_buff, portMAX_DELAY);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		LOGI("recv notify");
		for(uint8_t i=0;i<BUFF_SIZE;i++)
			read_buffer(cur_buff, &data_buff[i]);
		if(data_buff[BUFF_SIZE-2] == data_buff[1] + data_buff[2])
		{
			xQueueSend(buff_print_queue, data_buff, portMAX_DELAY);
		}
		else
			LOGW("invalid data");
//		xTaskNotify(a_task_handle, (uint32_t)cur_buff, eSetValueWithOverwrite);
		
	}
}
