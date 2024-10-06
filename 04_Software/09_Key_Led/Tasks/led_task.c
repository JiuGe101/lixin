/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file key_task.c
 *
 * @par dependencies
 * - led_task.h
 * - gpio.h
 * - FreeRTOS.h
 * - semphr.h
 *
 * @author ʮԪ
 *
 * @brief detect key state
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2024-10-06
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/
#include "led_task.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "semphr.h"
extern SemaphoreHandle_t flash_sem;
void led_task(void *param)
{
    uint8_t led_state = 0;
    while(1)
    {
        if(NULL != flash_sem)
        {
            if(pdTRUE == xSemaphoreTake(flash_sem, portMAX_DELAY))
            {
                led_state = !led_state;
                HAL_GPIO_WritePin(LED_GPIO, LED_GPIO_PIN, led_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
            }
        }
    }
}

