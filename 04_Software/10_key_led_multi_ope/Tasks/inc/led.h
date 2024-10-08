#ifndef __LED_H__
#define __LED_H__

#define LED_GPIO GPIOC
#define LED_GPIO_PIN GPIO_PIN_13

void led_task(void *param);
#endif
