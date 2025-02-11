/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rtt_log.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim11;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1)
    {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */
    printf("HardFault_Handler");
    /* USER CODE END HardFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */
    uint32_t cfsr = SCB->CFSR;
    LOGE("CFSR: %#x", cfsr);
    if(cfsr & (1 << 7))
    {
        uint32_t fault_address = SCB->MMFAR;
        LOGE("Fault Address: %#x", fault_address);
    }
    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */
//    if(DWT->FUNCTION0 & DWT_FUNCTION_MATCHED_Msk) {
//        uint32_t cmp_addr = DWT->COMP0;
//        while(1) {
//            for(volatile uint32_t i = 0; i < 3000; i++) {
//                for(volatile uint32_t j = 0; j < 3000; j++) {
//                }
//            }
//            printf("Address [0x%x] watchpoint triger!\r\n", cmp_addr);
//        }
//    }
    uint32_t dfsr = (*((volatile uint32_t *)0xE000ED30));

    // 打印 DFSR 内容，用于调试
    printf("DFSR = 0x%08X\n", dfsr);
	LOGI("DFSR = 0x%08X", dfsr);
    // 判断不同的调试异常源
    if (dfsr & (1 << 0)) {
        // DEBUGEVT：表示调试事件触发（例如硬件断点触发）
        printf("Debug Event Triggered (DEBUGEVT)\n");
		LOGI("Debug Event Triggered (DEBUGEVT)");
    }
    else if (dfsr & (1 << 1)) {
        // VCATCH：表示虚拟中断捕获触发（如硬件断点）
        printf("Virtual Catch Triggered (VCATCH)\n");
		LOGI("Virtual Catch Triggered (VCATCH)");
    }
    else if (dfsr & (1 << 2)) {
        // DWTTRAP：数据监视器触发
        printf("Data Watchpoint Triggered (DWTTRAP)\n");
		LOGI("Data Watchpoint Triggered (DWTTRAP)");
    }
    else if (dfsr & (1 << 3)) {
        // BKPT：表示软件断点或硬件断点触发
        printf("Breakpoint Triggered (BKPT)\n");
		LOGI("Breakpoint Triggered (BKPT)");
    }
    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM1 trigger and commutation interrupts and TIM11 global interrupt.
  */
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
    /* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 0 */

    /* USER CODE END TIM1_TRG_COM_TIM11_IRQn 0 */
    HAL_TIM_IRQHandler(&htim11);
    /* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 1 */

    /* USER CODE END TIM1_TRG_COM_TIM11_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
