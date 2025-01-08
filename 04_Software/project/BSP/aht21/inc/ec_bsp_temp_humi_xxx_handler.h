/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file ec_bsp_aht21_driver.h
 * 
 * @par dependencies 
 * - ec_bsp_aht21_driver.h
 * - stdio.h
 * - stdint.h
 * 
 * @author Shiyuan
 * 
 * @brief Handler layer.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2025-1-07
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/

#ifndef __EC_BSP_TEMP_HUMI_XXX_HANDLER_H__
#define __EC_BSP_TEMP_HUMI_XXX_HANDLER_H__
/********************************** Includes *********************************/
#include "ec_bsp_aht21_driver.h"

#include <stdbool.h>
#include <stdint.h>
/********************************** Includes *********************************/

/********************************** Defines **********************************/
#define AHT21_HANDLER_DEBUG                              /* Is in debug mode */
/********************************** Defines **********************************/

/********************************** Typedefs *********************************/
/* Emulation of return case */
typedef enum
{
    TEMP_HUMI_OK               = 0,      /* Operation completed successfully */
    TEMP_HUMI_ERROR,                     /* Operation failed                 */
    TEMP_HUMI_ERROR_TIMEOUT,             /* Operation timed out              */
    TEMP_HUMI_ERROR_RESOURCE,            /* Resource unavailable             */
    TEMP_HUMI_ERROR_PARAMETER,           /* Parameter error                  */
    TEMP_HUMI_ERROR_NOMEMORY,            /* No memory available              */
    TEMP_HUMI_ERROR_ISR,                 /*  Not allowed in ISR context      */
    TEMP_HUMI_RESWEVED      = 0x7FFFFFFF /* Reserved for future use          */
}temp_humi_status_t;

typedef enum
{
    TEMP_HUMI_EVENT_TEMP = 0,
    TEMP_HUMI_EVENT_HUMI,
    TEMP_HUMI_EVENT_BOTH
}temp_humi_xxx_event_data_type_t;

/* Class of event */
typedef struct
{
    float                   *temperature;   /* Temperature                   */
    float                      *humidity;   /* Humidity                      */
    uint32_t                    lifttime;   /* Lifetime of data              */
    uint32_t                   timestamp;   /* Timestamp of event            */
    temp_humi_xxx_event_data_type_t type;   /* Type of event                 */
}temp_humi_xxx_event_t;
/********************************** Typedefs *********************************/

/********************************* Interfaces ********************************/
/*  Interface from OS layer */
typedef struct
{
    void (*os_delay_ms)(uint32_t const ms);       /* Delay in ms             */
    temp_humi_status_t (*os_queue_create)(        /* Queue Creat interface   */
                                          uint32_t const item_num,
                                          uint32_t const item_size,
                                          void **  const queue_handle); 
    temp_humi_status_t (*os_queue_put)(           /* Queue send interface    */
                                         void * const queue_handle,
                                         void * const item,
                                         uint32_t timeout);
    temp_humi_status_t (*os_queue_get)(           /* Queue receive interface */
                                         void *queue_handle,
                                         void *msg,
                                         uint32_t timeout);
}temp_humi_handle_os_interface_t;

typedef struct
{
    iic_driver_interface_t *  iic_driver_interface;  /* IIC driver instance  */
    timebase_interface_t   *    timebase_interface;  /* Timebase             */
    temp_humi_handler_os_interface_t *os_interface;  /* OS interface         */
    yield_interface_t      *       yield_interface;  /* CPU yield interface  */
}temp_humi_handler_all_input_arg_t;

/********************************* Interfaces ********************************/

/********************************** Classes **********************************/
typedef struct temp_humi_handler_private_data temp_humi_handler_private_data_t;

/* Handler Class BAP Handler*/
typedef struct bsp_temp_humi_xxx_handler
{
    /* Interface passed to driver layer */
    timebase_interface_t             *  timebase_interface;
    iic_driver_interface_t           *iic_driver_interface;
    yield_interface_t                *     yield_interface;
    /* Interface from OD layer */
    temp_humi_handler_os_interface_t *        os_interface;
    /* Instance of driver instance */
    bsp_aht21_driver_t               *    p_aht21_instance;
    /* Handler of event queue */
    void                             *  event_queue_handle;
    /* Private data */
    temp_humi_handler_private_data_t *      p_private_data;
    /* Timestamp of last temprature reading */
    uint32_t                                last_temp_tick;
    /* Timestamp of last humidity reading */
    uint32_t                                last_humi_tick;
}bsp_temp_humi_xxx_handler_t;
/********************************** Classes **********************************/

/*********************************** APIs ************************************/
/**
 * @brief A thread function that handles the temperature and humidity events.
 * 
 * This function continuously checks the temperature and humidity updates and 
 * sends them to the event queue. It also updates the last temperature and 
 * humidity.
 */
void temp_humi_handler_thread(void *argument);
/**
 * @brief Initializes the temperature and humidity handler instance.
 * 
 * This function takes the temperature and humidity handler. It sets up the 
 * necessary interface, creates aqueue for handling events, and initializes
 * the last temperature and humidity timestamps.
 * 
 * @param handler_instance A pointer to the temperature and humidity handler instance.
 * @param input_arg A pointer to the input arguments containing the required interfaces.
 * 
 * @return An 8-bit unsigned integer representint the status of the initialization.
 *         - 0: Initialization successful.
 *         - Non-zero: Initialization failed.
 */
temp_humi_status_t bsp_temp_humi_xxx_handler_inst(
    bsp_temp_humi_xxx_handler_t       *handler_instance,
    temp_humo_handler_all_input_arg_t *       input_arg);

/**
 * @brief Reads the temperature and humidity from the AHT21 sensor.
 * 
 * This function sends an event to the AHT21 to read the temperature and
 * humidity. When the temperature and humidity data is ready, the callback function or provided pointers for temp.
 * 
 * @param[in] event A pointer to the event made by the caller
 * 
 * @return An 8-bit unsigned interger representing the status of the event passed.
 *         - 0: Initialization successful.
 *         - Non-zero: Initialization failed.   
 */
temp_humi_status_t bsp_temp_humi_xxx_read(temp_humi_xxx_event_t *event);
/*********************************** APIs ************************************/

/********************************** Declaring ********************************/

/********************************** Declaring ********************************/
#endif /* __EC_BSP_TEMP_HUMI_XXX_HANDLER_H__ */