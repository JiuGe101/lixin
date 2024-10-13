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
 *
 * @author ShiYuan
 *
 * @brief change the status of led according to key.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2024-10-10
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

QueueHandle_t led_queue;

static void led_overturn()
{
    HAL_GPIO_TogglePin(LED_GPIO, LED_GPIO_PIN);
}
static void led_flash(uint32_t num)
{
    for(uint32_t i = 0; i < num; i++)
    {
        HAL_GPIO_TogglePin(LED_GPIO, LED_GPIO_PIN);
        vTaskDelay(200);
    }
}

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
                led_overturn();
            }
            else if(2 == key_state)
            {
                led_flash(10);
            }
            key_state = 0;
        }
    }
}
