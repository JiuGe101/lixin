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
#include "ec_bsp_aht21_driver.h"
#include "iic_hal.h"
#include "rtt_log.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define AHT_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE()
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
iic_bus_t iic_bus_instance = 
{
	.IIC_SDA_PORT = GPIOB,
	.IIC_SCL_PORT = GPIOB,
	.IIC_SDA_PIN  = GPIO_PIN_13,
	.IIC_SCL_PIN  = GPIO_PIN_14,
};
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
void my_taskYIELD();
void test_task(void *param);
aht21_status_t iic_init_myown(void * bus)
{
        AHT_CLK_ENABLE;
        IICInit(&iic_bus_instance);
        //Could check the register of IIC if required
        return AHT21_OK;
}

aht21_status_t iic_start_myown(void * bus)
{
        IICStart(&iic_bus_instance);
        return AHT21_OK;
}

aht21_status_t iic_stop_myown(void * bus)
{
        IICStop(&iic_bus_instance);
        return AHT21_OK;
}

aht21_status_t pfiic_wait_ack_myown(void * bus)
{
        unsigned char ret = SUCCESS; // should be ErrorStatus but IICWaitAck(X)
        ret = IICWaitAck(&iic_bus_instance);
        if(SUCCESS == ret)
        {
            return AHT21_OK;
        } else {
            return AHT21_ERRORTIMEOUT;
        }
}


aht21_status_t iic_send_ack_myown(void * bus)
{
        IICSendAck(&iic_bus_instance);
        return AHT21_OK;
}

aht21_status_t iic_send_no_ack_myown(void * bus)
{
        IICSendNotAck(&iic_bus_instance);
        return AHT21_OK;
}

aht21_status_t iic_send_byte_myown(void * bus, uint8_t data)
{
        IICSendByte(&iic_bus_instance, data);
        return AHT21_OK;
}

aht21_status_t iic_receive_byte_myown(void * bus, uint8_t * const data)
{
        *data = IICReceiveByte(&iic_bus_instance);
    return AHT21_OK;
}
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
	if(pdPASS != xTaskCreate(test_task, "test_task", 2048, NULL, 5, NULL))
	{
		LOGE("The test_task create failed");
	}
	else
	{
		LOGI("The test_task OK");
	}
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

    for(;;)
    {
		osDelay(500);
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void my_taskYIELD()
{
	taskYIELD();
}
void test_task(void *param)
{
	iic_driver_interface_t iic_driver_interface = 
	{
		.pf_iic_init         = iic_init_myown,
		.pf_iic_start        = iic_start_myown,
		.pf_iic_stop         = iic_stop_myown,
		.pf_iic_wait_ack     = pfiic_wait_ack_myown,
		.pf_iic_send_no_ack  = iic_send_no_ack_myown,
		.pf_iic_send_byte    = iic_send_byte_myown,
		.pf_iic_recv_byte    = iic_receive_byte_myown,
		.pf_iic_send_ack     = iic_send_ack_myown,
		.pf_critical_enter   = (aht21_status_t (*)(void))vPortEnterCritical,
		.pf_critical_exit    = (aht21_status_t (*)(void))vPortExitCritical
	};
	timebase_interface_t timebase_interface = 
	{
		.pf_get_tick_count = HAL_GetTick
	};
	yield_interface_t yield_interface = 
	{
		.pf_rtos_yield = my_taskYIELD
	};
	bsp_aht21_driver_t aht21_driver;
	LOGI("1");
	aht21_status_t ret = aht21_inst(&aht21_driver, &iic_driver_interface, &timebase_interface, &yield_interface);
	//aht21_init();
	float humidity = 0;
	float temperature = 0;
	LOGI("2");
    for(;;)
    {
        aht21_driver.pf_read_temp_humi(&aht21_driver, &temperature, &humidity);
////		printf("the temperature = [%f],\r\n"
////					 "the humidity   = [%f]\r\n",temperature, humidity);
		LOGI("temp: %d, humi: %d", (int)temperature, (int)humidity);
		osDelay(1000);
    }
}
/* USER CODE END Application */

