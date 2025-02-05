#ifndef __MPU6050_H__
#define __MPU6050_H__
#include "bsp_mpuxxx_driver.h"
#include "circular_buffer.h"
#include "delay.h"
#include  "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
extern bsp_mpuxxx_driver_t bsp_mpu6050_driver;
extern iic_driver_interface_t iic_driver_interface;
extern yield_interface_t yield_interface;
extern os_interface_t os_interface;
extern delay_interface_t delay_interface;
extern timebase_interface_t timebase_interface;
extern QueueHandle_t queue_handle;
extern circular_buffer_t circular_buf;

extern void callback_register(void (*callback)(void *, void *));
extern void callback_register_dma(void (*callback)(void *, void *));

#endif
