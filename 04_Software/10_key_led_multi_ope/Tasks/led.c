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
 * @author ʮԪ
 *
 * @brief change the status of led according to key.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2024-10-08
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

extern xQueueHandle signal_queue;

static void led_overturn()
{
    HAL_GPIO_WritePin(LED_GPIO, LED_GPIO_PIN, !HAL_GPIO_ReadPin(LED_GPIO, LED_GPIO_PIN));
}
static void led_flash(uint32_t num)
{
    uint8_t current_led_state = HAL_GPIO_ReadPin(LED_GPIO, LED_GPIO_PIN);
    for(uint32_t i = 0; i < num; i++)
    {
        current_led_state = !current_led_state;
        HAL_GPIO_WritePin(LED_GPIO, LED_GPIO_PIN, current_led_state);
        vTaskDelay(500);
    }
}
void led_task(void *param)
{
    int8_t queue_value = 0;
    while(1)
    {
        if(pdTRUE == xQueueReceive(signal_queue, &queue_value, portMAX_DELAY))
        {
            switch(queue_value)
            {
            case 1:
                led_overturn();
                break;
            case 2:
                led_flash(6);
                break;
            default:
                break;
            }
        }
    }
}

