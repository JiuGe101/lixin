/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SEGGER_RTT.h"
#include "elog.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
void MPU_Config(void)
{
    HAL_MPU_Disable();
    // 关闭（禁用）MPU，这一步是在配置 MPU 前确保 MPU 是关闭状态。这样可以安全地进行配置。

    // 配置MPU保护区域（将指定地址设为只读）
    // 下面是 MPU 的配置步骤，我们将定义一个特定的内存区域，并设置它的访问权限。
    
    MPU_Region_InitTypeDef MPU_InitStruct;
    // 创建一个 `MPU_Region_InitTypeDef` 结构体实例，用于存储 MPU 区域的配置参数。

    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    // 启用该 MPU 区域。此选项告诉 MPU 这个区域是有效的，并且将被保护。

    MPU_InitStruct.BaseAddress = (uint32_t)WATCH_ADDRESS;
    // 设置 MPU 保护区域的基地址（起始地址）。`WATCH_ADDRESS` 是你要监视和保护的内存地址。

    MPU_InitStruct.Size = MPU_REGION_SIZE_32B;
    // 设置内存区域的大小。此处设置为 32 字节大小，也就是该区域的总内存范围是 32 字节。

    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    // 设置访问权限。此处为 `MPU_REGION_NO_ACCESS`，表示该区域内没有访问权限，任何访问都会触发异常。

    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    // 设置是否允许缓冲。此处为 `MPU_ACCESS_NOT_BUFFERABLE`，表示该区域内的访问不允许被缓冲。

    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    // 设置是否允许缓存。此处为 `MPU_ACCESS_CACHEABLE`，表示该区域的内存访问是可缓存的。（STM32F411CEU6只有在Flash上有Cache缓存机制，如果你的Watch点是在Flash上，那么这个选项又有，如果你的Watch点在RAM上，这个选项并不会增加访问性能）

    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    // 设置是否允许多个处理器共享该区域的访问。此处为 `MPU_ACCESS_NOT_SHAREABLE`，表示该区域不允许共享。（STM32F411CEU6 是基于单核的 Cortex-M4 的单核架构，因此配置中的 IsShareable 参数在单核处理器上并没有实际的作用。）

    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    // 设置 MPU 区域编号。此处为 `MPU_REGION_NUMBER0`，表示这是 MPU 的第一个区域。

    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    // 设置内存类型扩展字段，`TEX` 用于定义内存类型。此处设置为 `MPU_TEX_LEVEL0`，通常表示普通内存类型。

    MPU_InitStruct.SubRegionDisable = 0x00;
    // 设置子区域禁用位。`0x00` 表示不禁用任何子区域，整个区域都是有效的。

    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    // 设置是否允许从该区域执行代码。此处为 `MPU_INSTRUCTION_ACCESS_DISABLE`，表示不允许执行任何指令，防止恶意代码执行。

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    // 使用 `HAL_MPU_ConfigRegion` 函数配置 MPU 区域。将上述设置应用到第一个 MPU 区域。

    // 启用MPU
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
    // 重新启用 MPU，并设置默认的权限模式为 `MPU_PRIVILEGED_DEFAULT`。这意味着只有特权模式才能访问没有配置的区域。
}

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */
	*WATCH_ADDRESS = 0x1234;
	MPU_Config();
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    /* USER CODE BEGIN 2 */
    SEGGER_RTT_Init();
    /* close printf buffer */
    //setbuf(stdout, NULL);
    /* initialize EasyLogger */
    elog_init();
    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    /* start EasyLogger */
    elog_start();
    /* USER CODE END 2 */

    /* Init scheduler */
    osKernelInitialize();

    /* Call init function for freertos objects (in cmsis_os2.c) */
    MX_FREERTOS_Init();

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 100;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM11 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM11) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
