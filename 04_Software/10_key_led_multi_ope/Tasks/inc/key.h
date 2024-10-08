/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file key.h
 * 
 * @par dependencies 
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
#ifndef __KEY_H__
#define __KEY_H__
//******************************** Defines **********************************//
#define KEY_GPIO GPIOA
#define KEY_GPIO_PIN GPIO_PIN_0
//******************************** Defines **********************************//
//******************************** Declaring ********************************//
void key_task(void *param);
//******************************** Declaring ********************************//
#endif
