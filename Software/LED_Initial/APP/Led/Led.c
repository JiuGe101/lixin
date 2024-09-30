#include "Led.h"
#include "gpio.h"
void led_on()
{
	HAL_GPIO_WritePin(LED_GPIO, LED_GPIO_PIN, 0);
}

void led_off()
{
	HAL_GPIO_WritePin(LED_GPIO, LED_GPIO_PIN, 1);
}