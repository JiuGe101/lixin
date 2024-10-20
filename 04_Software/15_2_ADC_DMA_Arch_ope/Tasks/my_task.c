#include "my_task.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "elog.h"
#include "adc.h"
#include "gpio.h"
#include "user_rtt_printf.h"
QueueHandle_t msg_queue = NULL;
TaskHandle_t adc_task_handle = NULL;
TaskHandle_t adc_process_task_handle = NULL;
SemaphoreHandle_t adc_sem = NULL;

void adc_task(void *param)
{
    uint32_t *buff = (uint32_t*)pvPortMalloc(2 * sizeof(uint32_t));
    uint32_t *addr = NULL;
    if(NULL == buff)
    {
        log_a("buff is NULL");
        while(1);
    }
    uint8_t buff_index = 0;
    while(1)
    {
        buff_index = !buff_index;
        HAL_ADC_Start_DMA(&hadc1, &buff[buff_index], 1);

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        addr = &buff[buff_index];

        xSemaphoreTake(adc_sem, portMAX_DELAY);
        xQueueSend(msg_queue, &addr, portMAX_DELAY);
        xSemaphoreGive(adc_sem);
    }
}

void adc_process_task(void *param)
{
    uint32_t *data = NULL;
    float adc_vol;
    int32_t adc_int;
    while(1)
    {
        xQueuePeek(msg_queue, &data, portMAX_DELAY);
        xSemaphoreTake(adc_sem, portMAX_DELAY);
        xQueueReceive(msg_queue, &data, portMAX_DELAY);
        adc_vol = *data * 3.3 / 4096;
        xSemaphoreGive(adc_sem);
        adc_int = adc_vol * 100;
		log_i("adc_vol: %d", adc_int);
		//LOGI("adc_vol: %d", adc_int);
    }
}
void my_task(void *param)
{
    adc_sem = xSemaphoreCreateMutex();
    if(NULL == adc_sem)
    {
        log_a("adc_sem create failed\n");
        while(1);
    }
    msg_queue = xQueueCreate(1, sizeof(uint32_t*));
    if(NULL == msg_queue)
    {
        log_a("msg_queue create failed\n");
        while(1);
    }
    xTaskCreate(adc_task, "adc_task", 128, NULL, 5, &adc_task_handle);
    xTaskCreate(adc_process_task, "adc_process_task", 128, NULL, 6, &adc_process_task_handle);
    vTaskDelete(NULL);
}