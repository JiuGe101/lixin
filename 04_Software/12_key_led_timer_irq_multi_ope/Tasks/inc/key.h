/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file key.h
 *
 * @par dependencies
 * - stdint.h
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
#ifndef __KEY_H__
#define __KEY_H__
//******************************** Includes *********************************//
#include <stdint.h>
//******************************** Includes *********************************//
//******************************** Defines **********************************//
#define KEY_GPIO GPIOA
#define KEY_GPIO_PIN GPIO_PIN_0

#define LONG_PRESS_THRESHOLD 500
typedef struct {
    uint32_t press_time;
	uint8_t press_state;
} key_t;
//******************************** Defines **********************************//
//******************************** Declaring ********************************//
extern key_t key;
void key_task(void *param);
//******************************** Declaring ********************************//
#endif
