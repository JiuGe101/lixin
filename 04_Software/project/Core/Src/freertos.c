/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "elog.h"
#include "rtt_log.h"
#include "stdio.h"
#include "fpb.h"
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
/* USER CODE BEGIN Variables */
extern volatile uint32_t debug_var;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void set_breakpoint(uint32_t address);
void test_function();
__attribute__((noinline)) void origin_function(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
//	printf("Default Task\r\n");
	//log_i("Default Task");
//	printf("Default Task1\r\n");
//	int a = 10;
//	int b = 5;
//	int c = a / b;
//	debug_var = 1;
//	printf("Default Task2\r\n");
	// __BKPT(1);
	// *WATCH_ADDRESS = 0x5678;
//	set_breakpoint((uint32_t)test_function);
//	test_function();
	origin_function();
    for(;;)
    {
		// __BKPT(1);
		//log_i("Hello");
        osDelay(100);
    }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void set_breakpoint(uint32_t address)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;  // enable tracing
  CoreDebug->DEMCR |= CoreDebug_DEMCR_MON_EN_Msk;  // enable debug interrupt
  
  uint8_t res = (FPB->CTRL & FPB_CTRL_NUM_CODE1_Msk) >> FPB_CTRL_NUM_CODE1_Pos;
  assert_param(res);  // the num of comparators
  
  FPB->CTRL = FPB_CTRL_KEY_Msk | FPB_CTRL_ENABLE_Msk;
  FPB->COMP[0] = (address & 0xFFFFFFFE) | (0x01 << FPB_COMP_REPLACE_Pos)| FPB_COMP_ENABLE_Msk;
}
void test_function()
{
  for(uint8_t i = 0; i < 5; i++){
    vTaskDelay(1000);
    printf("Running to test_func !!!\r\n");
	LOGI("Running to test_func !!!");
  }
}
__attribute__((noinline)) void origin_function(void) {
  while (1){
    HAL_Delay(5000);
    printf("Original function is running!!!\r\n");  
	LOGI("Original function is running!!!");
  }
}
/* USER CODE END Application */

