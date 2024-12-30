#ifndef _DELAY_H
#define _DELAY_H
//#include "sys.h"	  
#include <stdint.h>
void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
#endif

