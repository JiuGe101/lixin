/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file key.c
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
 * @version V1.0 2024-10-13
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

extern QueueHandle_t led_queue;
key_t key;
QueueHandle_t key_queue = NULL;

void key_task(void *param)
{
	key_queue = xQueueCreate(1, 4);
	key_t *p_key = NULL;
	uint32_t press_start_time = 0;
	uint8_t led_state = 0;
	while(1)
	{
		if(NULL != key_queue && pdTRUE == xQueueReceive(key_queue, &p_key, portMAX_DELAY))
		{
			if(NULL != p_key)
			{
				if(!(*p_key).press_state)
				{
					press_start_time = (*p_key).press_time;
				}
				else
				{
					uint32_t duration = (*p_key).press_time - press_start_time;
					if(duration > 500)
					{
						led_state = 2;
						xQueueSend(led_queue, &led_state, portMAX_DELAY);
					}
					else if(duration < 10)
					{
						
					}
					else
					{
						led_state = 1;
						xQueueSend(led_queue, &led_state, portMAX_DELAY);
					}
				}
			}
		}
	}
}






