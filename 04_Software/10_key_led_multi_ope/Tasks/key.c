/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file key.c
 * 
 * @par dependencies 
 * - key.h
 * - stdint.h
 * - gpio.h
 * - FreeRTOS.h
 * - task.h
 * - queue.h
 * 
 * @author ʮԪ
 * 
 * @brief detect click or hold.
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

#include "key.h"
#include <stdint.h>
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern QueueHandle_t signal_queue;
typedef enum {
    KEY_IDLE,
    KEY_DOWN_STATE,
    KEY_CLICK_STATE,
    KEY_HOLD_STATE,
} key_state_t;

key_state_t current_key_state = KEY_IDLE;
void key_task(void *param)
{
    int8_t queue_value = 0;
    while(1)
    {
        switch(current_key_state)
        {
        case KEY_IDLE:
            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_GPIO, KEY_GPIO_PIN))
            {
                vTaskDelay(20);
                if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_GPIO, KEY_GPIO_PIN))
                {
                    current_key_state = KEY_DOWN_STATE;
                }
            }
            break;
        case KEY_DOWN_STATE:
            vTaskDelay(500);
            if(GPIO_PIN_SET == HAL_GPIO_ReadPin(KEY_GPIO, KEY_GPIO_PIN))
            {
                queue_value = 1;
                current_key_state = KEY_CLICK_STATE;
            }
            else
            {
                queue_value = 2;
                current_key_state = KEY_HOLD_STATE;
            }
            break;
        case KEY_CLICK_STATE:
            if(NULL != signal_queue)
                xQueueSend(signal_queue, &queue_value, portMAX_DELAY);
            queue_value = 0;
            current_key_state = KEY_IDLE;
            break;
        case KEY_HOLD_STATE:
            if(NULL != signal_queue)
                xQueueSend(signal_queue, &queue_value, portMAX_DELAY);
            queue_value = 0;
            current_key_state = KEY_IDLE;
            break;
        default:
            break;
        }
    }
}



