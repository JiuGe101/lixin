/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file circular_buffer.h
 *
 * @par dependencies
 *
 * - stdint.h
 *
 * @author liu
 *
 * @brief Provide the circular buffer APIs.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2024-12-06
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/
#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__
//******************************** Includes *********************************//
#include <stdint.h>
#include "elog.h"

#define DEBUG_PRINT(format, ...) log_i(format, ##__VA_ARGS__)

typedef struct circular_buffer
{
    uint8_t *buffer; // 缓冲区
    uint8_t rflag;   // 读位置
    uint8_t wflag;   // 写位置
    uint8_t *(*pf_get_wbuffer_addr)(struct circular_buffer *); // 获取写缓冲区地址
    uint8_t *(*pf_get_rbuffer_addr)(struct circular_buffer *); // 获取读缓冲区地址
    void (*pf_data_writed)(struct circular_buffer *);          // 写数据
    void (*pf_data_readed)(struct circular_buffer *);          // 读数据
    uint8_t size;    // 缓冲区大小
} circular_buffer_t;

void buffer_init(circular_buffer_t *buffer, uint8_t size);

#endif /* __CIRCULAR_BUFFER_H__ */
