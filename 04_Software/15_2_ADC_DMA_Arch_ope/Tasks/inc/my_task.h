#ifndef __MY_TASK_H__
#define __MY_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
extern TaskHandle_t adc_task_handle;
extern TaskHandle_t adc_process_task_handle;
extern SemaphoreHandle_t adc_sem;

void my_task(void *param);
#endif
