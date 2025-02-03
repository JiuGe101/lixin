/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_mpuxxx_driver.h
 * 
 * @par dependencies 
 * - stdbool.h
 * - stdio.h
 * - stdint.h
 * 
 * @author ShiYuan
 * 
 * @brief MPUXXX driver.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2025-2-2
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/
#ifndef __BSP_MPUXXX_DRIVER_H__
#define __BSP_MPUXXX_DRIVER_H__
/********************************* Includes **********************************/
#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
/********************************* Includes **********************************/
/********************************* Defines ***********************************/
#define MPU_DRIVER_DEBUG
#define RTOS_SUPPORTING
/********************************* Defines ***********************************/
/********************************* Declaring *********************************/
/* mpuxxx status */
typedef enum
{
    MPU_OK = 0,
    MPU_ERROR,
    MPU_ERROR,
    MPU_ERROR_IIC,
    MPU_ERROR_PARAMETER,
    MPU_ERROR_QUEUE
}mpu_status_t;
/* i2c driver */
typedef struct
{
    void    *hi2c;
    uint8_t dev_addr;
    int8_t  (*pf_init)            (void);
    int8_t  (*pf_deinit)          (void);
    uint8_t (*pf_i2c_mem_write)   (void *   , 
                                   uint16_t , 
                                   uint16_t ,
                                   uint16_t , 
                                   uint8_t *,
                                   uint16_t , 
                                   uint32_t 
                                  );
    uint8_t (*pf_i2c_mem_read)    (void *   , 
                                   uint16_t , 
                                   uint16_t , 
                                   uint16_t , 
                                   uint8_t *,
                                   uint16_t , 
                                   uint32_t );
    uint8_t (*pf_i2c_mem_read_dma)(void *   , 
                                   uint16_t , 
                                   uint16_t , 
                                   uint16_t , 
                                   uint8_t *,
                                   uint16_t );
}i2c_driver_interface_t;

typedef struct
{
    int8_t (*pf_init             )(void);
    int8_t (*pf_deinit           )(void);
    int8_t (*pf_enable_interrupt )(void);
    int8_t (*pf_disable_interrupt)(void);
    int8_t (*pf_enable_clock     )(void);
    int8_t (*pf_disable_clock    )(void);
}hardware_interrupt_interface_t;

typedef struct
{
    uint32_t (*pf_get_tick_count)(void);
}timebase_interface_t;

typedef struct
{
    int8_t (*pf_get_buffer_addr)(uint32_t);
}buffer_interfaces_t;

typedef struct
{
    int8_t (*pf_os_semaphore_create_mutex )(void ** );
    int8_t (*pf_os_semaphore_create_binary)(void ** );
    int8_t (*pf_os_semaphore_give)         (void *  );
    int8_t (*pf_os_delay)                  (uint32_t);
    void * (*pf_os_queue_create)           (uint32_t       item_num, 
                                            uint32_t      item_size, 
                                            void     *         attr);
    int32_t (*pf_os_queue_send)            (void     * queue_handle, 
                                            void     *       p_data, 
                                            uint8_t        msg_prio, 
                                            uint32_t        timeout);
    int32_t (*pf_os_queue_receive)         (void     * queue_handle, 
                                            void     *       p_data, 
                                            uint8_t  *     msg_prio, 
                                            uint32_t        timeout);
}os_interface_t;

typedef struct
{
    int16_t accel_x_raw;
    int16_t accel_y_raw;
    int16_t accel_z_raw;
    double ax;
    double ay;
    double az;

    int16_t gyro_x_raw;
    int16_t gyro_y_raw;
    int16_t gyro_z_raw;
    double gx;
    double gy;
    double gz;

    float tempature;
    double kalman_angle_x;
    double kalman_angle_y;
}mpu6050_data_t;

typedef struct
{
    i2c_driver_interface_t         *   p_iic_driver_interface;
    hardware_interrupt_interface_t * p_interruption_interface;
    timebase_interface_t           *     p_timebase_interface;

    os_interface_t                 *           p_os_interface;
    buffer_interface_t             *       p_buffer_interface;

    mpu_status_t (*pf_deinit)                     (void *);
    mpu_status_t (*pf_sleep)                      (void *);
    mpu_status_t (*pf_wakeup)                     (void *);
    mpu_status_t (*pf_set_gyro_fsr)      (void *, uint8_t);
    mpu_status_t (*pf_set_accel_fsr)     (void *, uint8_t);
    mpu_status_t (*pf_set_lpf)                    (void *);
    mpu_status_t (*pf_set_sample_rate)            (void *);
    mpu_status_t (*pf_get_temperature)  (void *, uint16_t);
    mpu_status_t (*pf_get_accel)(void *, mpu6050_data_t *);
    mpu_status_t (*pf_get_gyro) (void *, mpu6050_data_t *);
    mpu_status_t (*pf_get_all)  (void *, mpu6050_data_t *);
    mpu_status_t (*pf_get_interrupt_enable_reg)   (void *);

    void * semaphore_mutex_handle;
    void *sempahore_binary_handle;
    void *           queue_handle;
    void (*pf_mda_completed_callbacks)(void);
    void (*pf_int_interrupt_callback) (void);
}bsp_mpuxxx_driver_t;

mpu_status_t bsp_mpuxxx_driver_inst(
    bsp_mpuxxx_driver_t    *      p_mpu_driver,
    i2c_driver_interface_t *   p_i2c_interface,
    os_interface_t         *    p_os_interface,
    timbase_interface_t    *timebase_interface,
    void (*callback_register)(void (*callback)(void *, void *)),
    void (*callback_regidster_dma)   (void (*callback)(void *)),
    void *queue__handle
);
/********************************* Declaring *********************************/
#endif /* __BSP_MPUXXX_DRIVER_H__ */
