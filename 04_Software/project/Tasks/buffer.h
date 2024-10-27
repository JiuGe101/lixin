#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <stdint.h>
typedef struct {
    uint8_t *buffer;   // 数据存储
    uint16_t head;       // 写入指针
    uint16_t tail;       // 读取指针
    uint16_t size;       // 当前大小
} cir_buff_t;
cir_buff_t *create_buffer();
uint8_t write_buffer(cir_buff_t *cb, uint8_t data);
uint8_t read_buffer(cir_buff_t *cb, uint8_t *data);
#endif

