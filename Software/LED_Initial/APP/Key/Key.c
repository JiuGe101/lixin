#include "Key.h"
#include "gpio.h"

int8_t key_read()
{
	if(HAL_GPIO_ReadPin(KEY_GPIO, KEY_GPIO_PIN) == 0)
	{
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(KEY_GPIO, KEY_GPIO_PIN) == 0)
			return 0;
		else 
			return 1;
	}
	else
	{
		return 1;
	}
}