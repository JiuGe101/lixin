#ifndef __KEY_H__
#define __KEY_H__
#include <stdint.h>

#define KEY_GPIO GPIOC
#define KEY_GPIO_PIN GPIO_PIN_14

int8_t key_read();

#endif
