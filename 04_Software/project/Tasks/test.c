#include "test.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "log.h"
#include "usart.h"
#include "buffer.h"
#define BUFF_SIZE 256
TaskHandle_t b_task_handle;
TaskHandle_t a_task_handle;
QueueHandle_t buff_addr_queue;
QueueHandle_t buff_print_queue;
uint8_t *buff[2];
void a_task(void *param)
{
    uint32_t buff_addr;
    uint8_t data[DATA_SIZE];
    while(1)
    {
        xQueueReceive(buff_print_queue, data, portMAX_DELAY);
		LOGI("a recv: ");
        for(uint8_t i = 0; i < DATA_SIZE; i++)
        {
            LOG("%x ", data[i]);
        }
    }
}

uint8_t *get_buff_addr()
{
    static uint8_t flag;
    if(flag)
    {
        flag = !flag;
        return buff[0];
    }
    else
    {
        flag = !flag;
        return buff[1];
    }
}
void b_task(void *param)
{
    buff[0] = (uint8_t*)pvPortMalloc(BUFF_SIZE * sizeof(uint8_t));
    buff[1] = (uint8_t*)pvPortMalloc(BUFF_SIZE * sizeof(uint8_t));
    if(NULL == buff[0] || NULL == buff[1])
        LOGE("buff1 or buff2 is NULL");
    buff_addr_queue = xQueueCreate(1, sizeof(uint8_t*));
    if(NULL == buff_addr_queue)
        LOGE("buff_addr_queue is NULL");
    buff_print_queue = xQueueCreate(2, DATA_SIZE * sizeof(uint8_t));
    if(NULL == buff_print_queue)
        LOGE("buff_print_queue is NULL");
    uart1_recv_state_t cur_recv_state = DATA_WAIT_HEADER;
    uint8_t *cur_buff = NULL;
    uint8_t  *last_buff = NULL;
    cur_buff = get_buff_addr();
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, cur_buff, BUFF_SIZE);
    uint32_t recv_size;
    uint16_t count = 0;
    char data[DATA_SIZE];
    while(1)
    {

        xTaskNotifyWait(0, 0xffffffff, &recv_size, portMAX_DELAY);
        LOGI("recv notify");
        last_buff = cur_buff;
        cur_buff = get_buff_addr();
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, cur_buff, BUFF_SIZE);
        for(uint16_t i = 0; i < recv_size; i++)
        {
            switch(cur_recv_state)
            {
            case DATA_WAIT_HEADER:
                if(DATA_HEADER == last_buff[i])
                {
                    LOGI("recv header");
                    count++;
                    data[count - 1] = last_buff[i];
                    cur_recv_state = DATA_WAIT_TAIL;
                }
                break;
            case DATA_WAIT_TAIL:
                count++;
                data[count - 1] = last_buff[i];
                if(DATA_TAIL == last_buff[i])
                {
                    LOGI("recv tail");
                    count = 0;
                    cur_recv_state = DATA_WAIT_HEADER;
					if(data[DATA_SIZE-2] == data[1]+data[2])//checknum
						xQueueSend(buff_print_queue, data, portMAX_DELAY);
					else
						LOGI("invalid");
                }
				else if(DATA_SIZE == count)
				{
					LOGI("no tail");
					count = 0;
					cur_recv_state = DATA_WAIT_HEADER;
				}
                break;
            }
            //LOGI("%x ", last_buff[i]);
        }
    }
}
