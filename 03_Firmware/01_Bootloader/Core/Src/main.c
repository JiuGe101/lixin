/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"
#include "gpio.h"
#include "uart.h"
#include "rtt_log.h"
#include "common.h"
#include "flash.h"
#include "aes.h"
#include <stdio.h>

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef void (*pFunction)(void);
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;
uint8_t Package_receive_down  = 0;
char *check = (char*)0x8042CA0;
uint32_t BackSource;
uint8_t tab_1024[1024];
unsigned char IV[16] = {0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32};
unsigned char Key[32] = {0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, \
                         0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32, 0x31, 0X32
                        };  //原密文数据缓存

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);
void rcc_init(void);
void jump_to_app(void);
void STMFLASH_Read_Byte(uint32_t addr, uint8_t *buff, uint16_t num);
void Write_Flash_After_AES_Decode(u8 *IV_IN_OUT, u8 *key256bit);
uint8_t Key_Scan(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	RCC_ClockSecuritySystemCmd(ENABLE);
	SystemCoreClockUpdate();
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
    rcc_init();
    gpio_init();
    uart_init();
    SEGGER_RTT_Init();
//    if(FLASH_COMPLETE != FLASH_EraseSector(ApplicationAddress, VoltageRange_3))
//    {
//        LOGE("erase app sector err");
//    }
//    else
//    {
//        LOGE("erase app sector ok");
//    }
//    Flash_Write(ApplicationAddress, 0xA5);
//    uint8_t temp[16];
//    STMFLASH_Read_Byte(ApplicationAddress, temp, 16);
//    LOGE("0x%x", *temp);

    
    int32_t app_data_size = 0;
		SerialPutString("goto bootloader\r\n");
		Main_Menu();
    if(1 == Key_Scan())
    {
		
        app_data_size = Ymodem_Receive(tab_1024);
        //Back flash to App flash
        //if(0 == Back_to_App_Flash(app_data_size))
        if(app_data_size > 0)
        {
            Write_Flash_After_AES_Decode(IV, Key);//encode data
            jump_to_app();
        }
    }
	//if(0xAA == *check)
		jump_to_app();
//	else
//		LOGI("not 0xAA");
    /* Infinite loop */
    while (1)
    {
        //LOGI("hello world");
    }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
    uwTimingDelay = nTime;

    while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
    if (uwTimingDelay != 0x00)
    {
        uwTimingDelay--;
    }
}
uint8_t Key_Scan(void)
{
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
    {
        //Delay(50);
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
        {
            return 1;
        }
    }
    return 0;
}
void Write_Flash_After_AES_Decode(u8 *IV_IN_OUT, u8 *key256bit)
{
    u8 Temp[16];  //原密文数据缓存
    u8 wirteTime = 0; //一个解析包写入次数
    u16 readTime = 0, readDataCount = 0; //读取数据再解密的次数（每次解密16个字节）
    u16 writeTime = 0, flashWriteCount = 0; //写入升级包的次数 （每次写入1024个字节）
    u32 AppSize = 0; //升级包的大小
    u32 FlashReadAddr = BackAppAddress;
    u32 FlashDestination = ApplicationAddress;

    /*
      除了原有数据之外，首个16byte的数据是表达了文件的大小
    */
    STMFLASH_Read_Byte(FlashReadAddr, Temp, 16); //读取头文件（包含自定义内容+升级包总字节）
    Aes_IV_key256bit_Decode(IV_IN_OUT, Temp, key256bit); //解析得到自定义内容+文件大小
    AppSize = (Temp[15] << 24) + (Temp[14] << 16) + (Temp[13] << 8) + Temp[12];
    LOGI("AppSize=%d", AppSize);

    //计算升级包读取次数
    readDataCount = AppSize / 16;
    if(AppSize % 16 != 0)
    {
        readDataCount += 1;
    }

    uint8_t flash_erase_state = Flash_erase(FlashDestination, AppSize);
    if(0 == flash_erase_state)
    {
        FlashReadAddr += 16; //偏移16个字节，偏移后，当前地址为升级包头字节
        FlashDestination = ApplicationAddress;

        for(uint16_t i = 0; i < readDataCount; i++)
        {
            STMFLASH_Read_Byte(FlashReadAddr + i * 16, Temp, 16); //读取头文件（包含自定义内容+升级包总字节）
            Aes_IV_key256bit_Decode(IV_IN_OUT, Temp, key256bit); //解析数据

            BackSource = (uint32_t)Temp;
            for (wirteTime = 0; wirteTime < 4; wirteTime++)
            {
                Flash_Write(FlashDestination, *(uint32_t*)BackSource);
                FlashDestination += 4;
                BackSource += 4;
            }
        }
    }
}
void STMFLASH_Read_Byte(uint32_t addr, uint8_t *buff, uint16_t num)
{
    while(num--)
    {
        *(buff++) = *((uint8_t*)addr++);
    }
}
void rcc_init(void)
{
    /* Enable GPIOA clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    /* Enable UART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}
void jump_to_app(void)
{
    uint32_t JumpAddress;
    pFunction Jump_To_Application;

    /* 检查栈顶地址是否合法 */
    if(((*(__IO uint32_t *)ApplicationAddress) & 0x2FFE0000) == 0x20000000)
    {
        LOGI("jump start");
        /* 屏蔽所有中断，防止在跳转过程中，中断干扰出现异常 */
        __disable_irq();

        /* 用户代码区第二个 字 为程序开始地址(复位地址) */
        JumpAddress = *(__IO uint32_t *) (ApplicationAddress + 4);

        /* Initialize user application's Stack Pointer */
        /* 初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址) */
        __set_MSP(*(__IO uint32_t *) ApplicationAddress);

        /* 类型转换 */
        Jump_To_Application = (pFunction) JumpAddress;

        /* 跳转到 APP */
        Jump_To_Application();
    }
    else
    {
        LOGI("no jump");
    }
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**
  * @}
  */


