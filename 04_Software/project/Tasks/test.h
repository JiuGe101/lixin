#ifndef __TEST_H__
#define __TEST_H__
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#define DATA_SIZE 5
#define DATA_HEADER 0xFB
#define DATA_TAIL 0xAA

typedef enum{
	DATA_WAIT_HEADER,
	DATA_WAIT_TAIL,
}uart1_recv_state_t;
extern TaskHandle_t b_task_handle;
extern TaskHandle_t a_task_handle;
extern QueueHandle_t buff_addr_queue;
void b_task(void *param);
void a_task(void *param);
#endif
