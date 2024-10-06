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
 * @brief change led state
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
#ifndef __LED_TASK_H__
#define __LED_TASK_H__
//******************************** Includes *********************************//

//******************************** Includes *********************************//
//******************************** Defines **********************************//
#define LED_GPIO GPIOC
#define LED_GPIO_PIN GPIO_PIN_13
//******************************** Defines **********************************//
//******************************** Declaring ********************************//
void led_task(void *param);
//******************************** Declaring ********************************//
#endif
