/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file ec_bsp_aht21_driver.h
 * 
 * @par dependencies 
 * - ec_bsp_aht21_reg.h
 * - stdio.h
 * - stdint.h
 * 
 * @author ShiYuan
 * 
 * @brief Provide the HAL APIs of AHT21 and corresponding opetions.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2024-11-12
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/

#ifndef __EC_BSP_AHT21_DRIVER_H__
#define __EC_BSP_AHT21_DRIVER_H__

/********************************** Includes *********************************/
#include "ec_bsp_aht21_reg.h"
#include <stdio.h>
#include <stdint.h>
/********************************** Includes *********************************/

/********************************** Defines **********************************/
#define OS_SUPPORTING

/*#define HARDWARE_IIC*/
/*    Return values from function    */
typedef enum 
{
    AHT21_OK                = 0,    /*  */
    AHT21_ERROR,        
    AHT21_ERRORTIMEOUT,
    AHT21_ERRORRESOURCE,
    AHT21_ERRORPARAMETER,
    AHT21_ERRORNOMEMORY,
    AHT21_ERRORISR,
    AHT21_RESERVED          = 0x7FFFFFFF
}aht21_status_t;
/********************************** Defines **********************************/

/********************************** Declaring ********************************/
/* From Core Layer: IIC Port */
#ifndef HARDWARE_IIC    /* True : Software IIC */
typedef struct 
{
    aht21_status_t (*pf_iic_init)       (void *);   /* IIC init interface    */
    aht21_status_t (*pf_iic_deinit)     (void *);   /* IIC deinit interface  */
    aht21_status_t (*pf_iic_start)      (void *);   /* IIC start interface   */
    aht21_status_t (*pf_iic_stop)       (void *);   /* IIC stop interface    */
    aht21_status_t (*pf_iic_wait_ack)   (void *);   /* IIC wait ack interface*/
    aht21_status_t (*pf_iic_send_ack)   (void *);   /* IIC send ack interface*/
    aht21_status_t (*pf_iic_send_no_ack)(void *);   /* send no-ack interface */
    aht21_status_t (*pf_iic_recv_byte)  (void *,    /* read byte interface   */
                                         uint8_t * const);
    aht21_status_t (*pf_iic_send_byte) (void *,    /* write byte interface  */
                                         const uint8_t);

    aht21_status_t (*pf_critical_enter) (void);   /* enter critical section*/
    aht21_status_t (*pf_critical_exit)  (void);   /* exit critical section */
}iic_driver_interface_t;
#endif /* end of HARDWARE_I2C */

#ifdef HARDWARE_IIC    /* True : Hardware IIC */
typedef struct {
    aht21_status_t (*pf_iic_init)       (void *);   /* IIC init interface    */
    aht21_status_t (*pf_iic_deinit)     (void *);   /* IIC deinit interface  */
    //aht21_status_t (*pf_iic_start)      (void *);   /* IIC start interface   */
    //aht21_status_t (*pf_iic_stop)       (void *);   /* IIC stop interface    */
    //aht21_status_t (*pf_iic_wait_ack)   (void *);   /* IIC wait ack interface*/
    //aht21_status_t (*pf_iic_send_ack)   (void *);   /* IIC send ack interface*/
    //aht21_status_t (*pf_iic_send_no_ack)(void *);   /* send no-ack interface */
    aht21_status_t (*pf_iic_read_byte)  (void *);   /* read byte interface   */
    aht21_status_t (*pf_iic_write_byte) (void *);   /* write byte interface  */

    aht21_status_t (*pf_critical_enter) (void *);   /* enter critical section*/
    aht21_status_t (*pf_critical_exit)  (void *);   /* exit critical section */
}iic_driver_interface_t;
#endif /* end of HARDWARE_IIC */

/* From Core Layer: TimeBase */
typedef struct
{
    uint32_t (*pf_get_tick_count)(void);         /* Get Tick count interface */
}timebase_interface_t;

/* From Core Layer: OS_Delay */
#ifdef OS_SUPPORTING
typedef struct
{
    void (*pf_rtos_yield)(const uint32_t);          /* OS Not-Blocking Delay */
}yield_interface_t;
#endif /* end of OS_SUPPORTING */

/* AHT21_hal_driver instance class */
typedef struct
{
    iic_driver_interface_t *p_iic_driver_instance;   /* IIC Driver Interface */
    timebase_interface_t     *p_timebase_instance;   /* Timebase Interface   */
#ifdef OS_SUPPORTING
    yield_interface_t           *p_yield_instance;   /* Yield Interface      */
#endif /* end of OS_SUPPORTING */
    aht21_status_t (*pf_inst)(void              * const       p_aht21_instance,
                       iic_driver_interface_t   * const  p_iic_driver_instance,
                       timebase_interface_t     * const    p_timebase_instance,
                       yield_interface_t        * const      p_yield_instance);
    aht21_status_t (*pf_init)            (void  * const      p_aht21_instance);
    aht21_status_t (*pf_deinit)          (void  * const      p_aht21_instance);
    aht21_status_t (*pf_read_id)         (void  * const      p_aht21_instance);
    aht21_status_t (*pf_read_temp_humi)  (void  * const      p_aht21_instance,
                                          float * const           temperature,
                                          float * const              humidity);
	aht21_status_t (*pf_read_humi)       (void  * const      p_aht21_instance,
										  float * const              humidity);
    aht21_status_t (*pf_sleep)           (void  * const      p_aht21_instance);
    aht21_status_t (*pf_wakeup)          (void  * const      p_aht21_instance);
}bsp_aht21_driver_t;

/* AHT21_hal_driver instance class inst */
extern aht21_status_t aht21_inst(
                          bsp_aht21_driver_t     * const      p_aht21_instance,
                          iic_driver_interface_t * const p_iic_driver_instance,
#ifdef OS_SUPPORTING
                          yield_interface_t      * const      p_yield_instance,							  
#endif /* end of OS_SUPPORTING */
						  timebase_interface_t   * const  p_timebase_instance);
/********************************** Declaring ********************************/
#endif  // end of __EC_BSP_AHT21_DRIVER_H__
