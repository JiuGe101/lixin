#include "delay.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim2;

void delay_init(void)
{
    // 初始化TIM2
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 100-1;        // 100MHz时钟，分频后为1MHz，即1us记一个数
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFFFFFF;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    HAL_TIM_Base_Start(&htim2);
}

void delay_us(uint32_t nus)
{
    uint32_t startVal = __HAL_TIM_GET_COUNTER(&htim2);
    
    while(__HAL_TIM_GET_COUNTER(&htim2) - startVal < nus)
    {
    }
}

void delay_ms(uint32_t nms)
{
    delay_us(nms * 1000);
}

