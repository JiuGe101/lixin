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
#include "key.h"
#include <stdint.h>
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

key_t key = {
    .press_flag = 0,
    .press_start_time = 0,
	.press_state = 0,
};






