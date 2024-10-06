/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file key_task.c
 * 
 * @par dependencies 
 * - key_task.h
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

#include "key_task.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "semphr.h"
extern SemaphoreHandle_t flash_sem;

void key_task(void *param)
{
	
	while(1)
	{
		if(HAL_GPIO_ReadPin(KEY_GPIO, KEY_GPIO_PIN)==0)
		{
			if(NULL != flash_sem)
			{
				xSemaphoreGive(flash_sem);
			}
			while(HAL_GPIO_ReadPin(KEY_GPIO, KEY_GPIO_PIN)==0);
		}
	}
}
