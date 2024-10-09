/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file led.h
 *
 * @par dependencies
 *
 * @author ʮԪ
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
#ifndef __LED_H__
#define __LED_H__
//******************************** Defines **********************************//
#define LED_GPIO GPIOC
#define LED_GPIO_PIN GPIO_PIN_13
//******************************** Defines **********************************//
//******************************** Declaring ********************************//
void led_task(void *param);
//******************************** Declaring ********************************//
#endif
