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
#include "ec_bsp_temp_humi_xxx_handler.h"
#include "iic_hal.h"
#include "rtt_log.h"
#include "elog.h"
#include "queue.h"
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
// 创建队列
temp_humi_status_t os_queue_create_myown(uint32_t item_num, uint32_t item_size, void **queue_handle)
{
    *queue_handle = xQueueCreate(item_num, item_size);
    log_d("queue_handle addr=%p", *queue_handle);
    if(NULL == *queue_handle)
    {
        return TEMP_HUMI_ERRORPARAMETER;
    } else {
        return TEMP_HUMI_OK;
    }
}
// 写队列
temp_humi_status_t os_queue_put_myown(void *queue_handle, void *item, uint32_t timeout)
{
    if (pdTRUE == xQueueSend(queue_handle, item, timeout))
    {
        return TEMP_HUMI_OK;
    }
    else
    {
        return TEMP_HUMI_RESERVED;
    }
}
// 读队列
temp_humi_status_t os_queue_get_myown(void *queue_handle, void *msg, uint32_t timeout)
{
    if (pdTRUE == xQueueReceive(queue_handle, msg, timeout))
    {
        return TEMP_HUMI_OK;
    }
    else
    {
        return TEMP_HUMI_RESERVED;
    }
}
void temp_humi_callback(float *temperature, float *humidity)
{
    log_d("callback:temperature = %f, humidity = %f", *temperature, *humidity);
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
    // 构造iic总线实例
    static iic_driver_interface_t iic_driver_interface = {
        .pf_iic_init         = iic_init_myown,
        .pf_iic_start        = iic_start_myown,
        .pf_iic_stop         = iic_stop_myown,
        .pf_iic_wait_ack     = pfiic_wait_ack_myown,
        .pf_iic_send_no_ack  = iic_send_no_ack_myown,
        .pf_iic_send_byte    = iic_send_byte_myown,
        .pf_iic_recv_byte = iic_receive_byte_myown,
        .pf_iic_send_ack     = iic_send_ack_myown,
        .pf_critical_enter   = (aht21_status_t (*)(void))vPortEnterCritical,
        .pf_critical_exit    = (aht21_status_t (*)(void))vPortExitCritical
    };
    static yield_interface_t yield_interface =
    {
        .pf_rtos_yield       = vTaskDelay
    };
    static timebase_interface_t timebase_interface =
    {
        .pf_get_tick_count   = HAL_GetTick
    };
// RTOS提供的接口
    static temp_humi_handler_os_interface_t os_interface =
    {
        .os_delay_ms         = vTaskDelay,
        .os_queue_create     = os_queue_create_myown,
        .os_queue_put        = os_queue_put_myown,
        .os_queue_get        = os_queue_get_myown
    };
    static temp_humi_handler_all_input_arg_t input_arg =
    {
        .iic_driver_interface = &iic_driver_interface,
        .timebase_interface   = &timebase_interface,
        .os_interface         = &os_interface,
        .yield_interface      = &yield_interface
    };
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
	if(pdPASS != xTaskCreate(temp_humi_handler_thread, "temp_humi_handler_thread", 1024, (void*)&input_arg, 5, NULL))
	{
		log_e("Failed to create temp_humi_handler_thread!");
	}
	else
	{
		log_d("Successed to create temp_humi_handler_thread");
	}
    if(pdPASS != xTaskCreate(test_task, "test_task", 1024, NULL, 5, NULL))
    {
        log_e("The test_task create failed");
    }
    else
    {
        log_d("The test_task OK");
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
    temp_humi_xxx_event_t event = {
        .lifetime = 2000,
        .type = TEMP_HUMI_EVENT_BOTH,
        .pf_callback = temp_humi_callback
    };
    for(;;)
    {
        log_d("Task start");
		vTaskDelay(1000);
        bsp_temp_humi_xxx_read(&event);
    }
}
/* USER CODE END Application */

