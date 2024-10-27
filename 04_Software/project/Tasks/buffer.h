#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <stdint.h>
typedef struct {
    uint8_t *buffer;   // ���ݴ洢
    uint16_t head;       // д��ָ��
    uint16_t tail;       // ��ȡָ��
    uint16_t size;       // ��ǰ��С
} cir_buff_t;
cir_buff_t *create_buffer();
uint8_t write_buffer(cir_buff_t *cb, uint8_t data);
uint8_t read_buffer(cir_buff_t *cb, uint8_t *data);
#endif

