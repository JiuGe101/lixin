/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file led.c
 *
 * @par dependencies
 * - led.h
 * - stdint.h
 * - gpio.h
 * - FreeRTOS.h
 * - task.h
 * - queue.h
 * - tim.h
 *
 * @author ShiYuan
 *
 * @brief change the status of led according to key.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2024-10-13
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/
#include "led.h"
#include <stdint.h>
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "tim.h"

volatile uint32_t run_times;
QueueHandle_t led_queue;

void led_task(void *param)
{
    led_queue = xQueueCreate(1, sizeof(uint8_t));
    uint8_t key_state = 0;
    while(1)
    {
        if(NULL != led_queue && pdTRUE == xQueueReceive(led_queue, &key_state, portMAX_DELAY))
        {
            if(1 == key_state)
            {
                run_times = 2;
                HAL_TIM_Base_Start_IT(&htim2);
            }
            else if(2 == key_state)
            {
                run_times = 20;
                HAL_TIM_Base_Start_IT(&htim2);
            }
            key_state = 0;
        }
    }
}
