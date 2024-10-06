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

#ifndef __KEY_TASK_H__
#define __KEY_TASK_H__

//******************************** Includes *********************************//

//******************************** Includes *********************************//
//******************************** Defines **********************************//
#define KEY_GPIO GPIOA
#define KEY_GPIO_PIN GPIO_PIN_0
//******************************** Defines **********************************//
//******************************** Declaring ********************************//
void key_task(void *param);
//******************************** Declaring ********************************//

#endif
