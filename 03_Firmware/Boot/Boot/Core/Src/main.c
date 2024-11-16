/**
  ******************************************************************************
	WeAct 微行创新 
	>> 标准库实例例程
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

// 全局定义 STM32F411xE 或者 STM32F401xx
// 当前定义 STM32F411xE

// STM32F411 外部晶振25Mhz，考虑到USB使用，内部频率设置为96Mhz
// 需要100mhz,自行修改system_stm32f4xx.c

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* APP flash address */

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;
uint8_t tab_1024[1024];

//下面的IV和Key就是你自定义的向量和密钥，我这里是1212121212
unsigned char IV[16]={0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32};  
unsigned char Key[32]={0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,\
                       0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32,0x31,0X32};  //原密文数据缓存

/* Private function prototypes -----------------------------------------------*/
/* 定义类型 */
typedef void (*pFunction)(void);

void jump_to_app(void)
{
  uint32_t JumpAddress;
  pFunction Jump_To_Application;

  /* 检查栈顶地址是否合法 */
  if(((*(__IO uint32_t *)ApplicationAddress) & 0x2FFE0000) == 0x20000000)
  {
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
}
/**********************Aes 加密 Start********************************/
uint32_t BackSource;
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read_Byte(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)   	
{
	while(NumToRead--)
	{
		*(pBuffer++) = *((vu8*)ReadAddr++) ;
	}
}

void Write_Flash_After_AES_Decode(u8 *IV_IN_OUT, u8 *key256bit)
{
    u8 Temp[16];  //原密文数据缓存
    u8 wirteTime=0;  //一个解析包写入次数
    u16 readTime=0,readDataCount=0;   //读取数据再解密的次数（每次解密16个字节）
    u16 writeTime=0,flashWriteCount=0;  //写入升级包的次数 （每次写入1024个字节）	
    u32 AppSize=0;  //升级包的大小
    u32 FlashReadAddr=BackAppAddress;
    u32 FlashDestination=ApplicationAddress;

    /*
      除了原有数据之外，首个16byte的数据是表达了文件的大小
    */
    STMFLASH_Read_Byte(FlashReadAddr,Temp,16);//读取头文件（包含自定义内容+升级包总字节）
    Aes_IV_key256bit_Decode(IV_IN_OUT,Temp,key256bit);//解析得到自定义内容+文件大小
    AppSize=(Temp[15]<<24)+(Temp[14]<<16)+(Temp[13]<<8)+Temp[12];
    log_d("AppSize=%d",AppSize);

    //计算升级包读取次数
    readDataCount=AppSize/16;
    if(AppSize%16!=0)
    {
      readDataCount+=1;
    }

    uint8_t flash_erase_state = Flash_erase(FlashDestination,AppSize);
    if(0 == flash_erase_state)
    {
      FlashReadAddr+=16;//偏移16个字节，偏移后，当前地址为升级包头字节
      FlashDestination=ApplicationAddress;

      for(uint16_t i=0;i<readDataCount;i++)
      {
        STMFLASH_Read_Byte(FlashReadAddr + i * 16,Temp,16);//读取头文件（包含自定义内容+升级包总字节）
        Aes_IV_key256bit_Decode(IV_IN_OUT,Temp,key256bit);//解析数据
        
        BackSource = (uint32_t)Temp;
        for (wirteTime = 0;wirteTime<4;wirteTime++)
        {
          Flash_Write(FlashDestination, *(uint32_t*)BackSource);
          FlashDestination += 4;
          BackSource += 4;
        }
      }
    }
}

/**********************Aes 加密 End********************************/
/* Private functions ---------------------------------------------------------*/
 /*
  *power by WeAct Studio
  *The board with `WeAct` Logo && `version number` is our board, quality guarantee. 
  *For more information please visit: https://github.com/WeActTC/MiniF4-STM32F4x1
  *更多信息请访问：https://gitee.com/WeActTC/MiniF4-STM32F4x1
  */
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	/* Enable Clock Security System(CSS): this will generate an NMI exception
     when HSE clock fails *****************************************************/
  RCC_ClockSecuritySystemCmd(ENABLE);
	
 /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files before to branch to application main.
       To reconfigure the default setting of SystemInit() function, 
       refer to system_stm32f4xx.c file */

  /* SysTick end of count event each 1ms */
  SystemCoreClockUpdate();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

  /* Init Key */
  Key_IO_Init();

  /* Init LED */
  Led_IO_Init();

  /* Init USART */
  USART1_Init();

  elog_init();
  /* set EasyLogger log format */
  elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
  elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
  elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
  elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
  /* start EasyLogger */
  elog_start();

  log_d("Jump_To_Application start");
#if 0
  /* 跳转到APP */
  jump_to_app();

  /* Add your application code here */
  /* Insert 50 ms delay */
  // Delay(50);
#endif
  int32_t app_data_size = 0;
  if(1 == Key_Scan())
  {
    app_data_size = Ymodem_Receive(tab_1024);
    //Back flash to App flash
    //if(0 == Back_to_App_Flash(app_data_size))
    if(app_data_size > 0)
    {
      Write_Flash_After_AES_Decode(IV,Key);
      jump_to_app();
    }
  }
  //GPIO_Config();
  //TIM_Config();   
  /* Infinite loop */
  while (1)
  {
    log_i("Not jump_to_application");
    Breathing_light();
#if 0
    /* 测试Ymodem */
    if(1 == Key_Scan())
		  Ymodem_Receive(tab_1024);
    /* 测试串口 */
		if('a' == USART_ReceiveChar(USART1))
			USART_SendChar(USART1,'C');
    /*测试Flash*/
    if(EreaseAppSector(FLASH_Sector_6) == FLASH_COMPLETE)
    {
      Flash_Write(0x08040000,0x55555555);
    }
    /*测试Led*/
    Breathing_light();
#endif
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
