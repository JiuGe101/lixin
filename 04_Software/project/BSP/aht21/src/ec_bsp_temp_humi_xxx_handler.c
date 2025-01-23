/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file ec_bsp_aht21_driver.h
 * 
 * @par dependencies 
 * - ec_bsp_temp_humi_xxx_handler.h
 * - stdio.h
 * - stdint.h
 * 
 * @author ShiYuan
 * 
 * @brief Provide the handler APIs for temperature and humidity sensors and
 *        corresponding operations.
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
/********************************** Includes *********************************/
#include "ec_bsp_temp_humi_xxx_handler.h"
#include "elog.h"
#include "unity.h"
/********************************** Includes *********************************/
/********************************** Defines **********************************/
#define TEMP_HUMI_NOT_INITIATED false                       /* Not initiated */
#define TEMP_HUMI_INITIATED     true                        /* Initiated     */
#define OS_QUEUE_CREATE         handler_instance->os_interface->os_queue_create
#define MY_MAX_DELAY            0xFFFFFFFFUL
/********************************** Defines **********************************/
/********************************* Variables *********************************/
static bsp_temp_humi_xxx_handler_t *gp_temp_humi_instance = NULL;
typedef struct temp_humi_handler_private_data
{
    bool is_initiated;                  /* A flag that is initialized or not */
}temp_humi_handler_private_data_t;

/********************************* Variables *********************************/
/********************************* Functions *********************************/
/**
 * @brief Mount the handler instance globally for use in other functions.
 *
 * @param[in] instance Pointer to the handler instance to mount.
 */
void __mount_handler(bsp_temp_humi_xxx_handler_t * const instance)
{
    gp_temp_humi_instance = instance;
}
/**
 * @brief Initialize the temperature and humidity handler.
 *
 * @param[in] handler_instance Pointer to the handler instance.
 *
 * @retval TEMP_HUMI_OK            Initialization successful.
 * @retval TEMP_HUMI_ERRORRESOURCE Resource allocation or queue creation failed.
 */
static temp_humi_status_t bsp_temp_xxx_handler_init(
                                 bsp_temp_humi_xxx_handler_t *handler_instance)
{
    log_d("bsp_temp_xxx_handler_init start");
    int8_t ret = 0;
    if(NULL == handler_instance)
    {
        log_e("handler_instance is NULL.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    /* Create queue */
    if(NULL == OS_QUEUE_CREATE)
    {
        log_e("OS_QUEUE_CREATE is NULL.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    ret = OS_QUEUE_CREATE(10, sizeof(temp_humi_xxx_event_t), &(handler_instance->event_queue_handle));
    if(ret)
    {
        log_e("OS_QUEUE_CREATE failed.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    ret = aht21_inst(handler_instance->p_aht21_instance,
                     handler_instance->iic_driver_interface, 
#ifdef OS_SUPPORTING
                     handler_instance->yield_interface,
#endif /* OS_SUPPORTING */
                     handler_instance->timebase_interface);
    if(ret)
    {
        log_e("aht21_inst failed.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    log_d("bsp_temp_xxx_handler_init end");
    return TEMP_HUMI_OK;
}
/**
 * @brief De-initialize the temperature and humidity handler.
 *
 * @return TEMP_HUMI_OK De-initialization successful.
 */
static temp_humi_status_t bsp_temp_xxx_handler_deinit(void)
{
	if(NULL == gp_temp_humi_instance)
    {
        log_e("gp_temp_humi_instance is NULL.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    gp_temp_humi_instance->p_private_data->is_initiated = TEMP_HUMI_NOT_INITIATED;
    return TEMP_HUMI_OK;
}

/**
 * @brief Instantiate the temperature and humidity handler.
 *
 * @param[in] handler_instance Pointer to the handler instance.
 * @param[in] input_arg Pointer to the input arguments containing required
 *                      interfaces.
 *
 * @retval TEMP_HUMI_OK            Initialization successful.
 * @retval TEMP_HUMI_ERRORRESOURCE Null pointer or resource error.
 */
temp_humi_status_t bsp_temp_humi_xxx_handler_inst(
    bsp_temp_humi_xxx_handler_t       *handler_instance,
    temp_humi_handler_all_input_arg_t * const input_arg)
{
    log_d("bsp_temp_humi_xxx_handler_inst start");
    if(NULL == handler_instance)
    {
        log_e("handler_instance is NULL.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    if(NULL == input_arg)
    {
        log_e("input_arg is NULL.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    if(NULL == input_arg->timebase_interface)
    {
        log_e("timebase_interface is NULL.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    if(NULL == input_arg->iic_driver_interface)
    {
        log_e("iic_driver_interface is NULL.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    if(NULL == input_arg->os_interface)
    {
        log_e("os_interface is NULL.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    handler_instance->timebase_interface = input_arg->timebase_interface;
    handler_instance->iic_driver_interface = input_arg->iic_driver_interface;
    handler_instance->os_interface = input_arg->os_interface;
    handler_instance->yield_interface = input_arg->yield_interface;
	
	temp_humi_status_t ret = TEMP_HUMI_OK;
    ret = bsp_temp_xxx_handler_init(handler_instance);
    if(TEMP_HUMI_OK != ret)
    {
        log_e("bsp_temp_xxx_handler_init failed.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    handler_instance->p_private_data->is_initiated = TEMP_HUMI_INITIATED;
    log_d("bsp_temp_humi_xxx_handler_inst end");
    return TEMP_HUMI_OK;
}
/**
 * @brief Fetch temperature and humidity values.
 *
 * @param[in]  handler_instance Pointer to the handler instance.
 * @param[in]  event            Pointer to event data specifying the type of 
 *                              data to fetch and its lifetime.
 * @param[out] temp             Pointer to store the fetched temperature value.
 * @param[out] humi             Pointer to store the fetched humidity value.
 *
 * @retval TEMP_HUMI_OK   Data fetched successfully.
 * @retval TEMP_HUMI_ERROR Error in fetching data.
 */
temp_humi_status_t get_temp_humi(
    bsp_temp_humi_xxx_handler_t *handler_instance,
    temp_humi_xxx_event_t * const event,
    float * const temperature,
    float * const humidity)
{
    log_d("get_temp_humi start");
	if(NULL == handler_instance)
	{
		return TEMP_HUMI_ERRORRESOURCE;
	}
	if(NULL == event)
	{
		return TEMP_HUMI_ERRORRESOURCE;
	}
	if(NULL == temperature || NULL == humidity)
	{
		return TEMP_HUMI_ERRORRESOURCE;
	}
    const uint32_t tim = handler_instance->timebase_interface->pf_get_tick_count();
    switch (event->type)
    {
        case TEMP_HUMI_EVENT_TEMP:
            if((tim - handler_instance->last_temp_tick) > event->lifetime)
            {
                handler_instance->p_aht21_instance->pf_read_temp_humi(
                    handler_instance->p_aht21_instance,
                    temperature, humidity);
                handler_instance->last_temp_tick = tim;
            }
            break;
        case TEMP_HUMI_EVENT_HUMI:
            if((tim - handler_instance->last_temp_tick) > event->lifetime)
            {
                handler_instance->p_aht21_instance->pf_read_humi(
                    handler_instance->p_aht21_instance,
                    humidity);
                handler_instance->last_temp_tick = tim;
            }
            break;
        case TEMP_HUMI_EVENT_BOTH:
            if((tim - handler_instance->last_temp_tick) > event->lifetime)
            {
                handler_instance->p_aht21_instance->pf_read_temp_humi(
                    handler_instance->p_aht21_instance,
                    temperature, humidity);
                handler_instance->last_temp_tick = tim;
            }
            if((tim - handler_instance->last_temp_tick) > event->lifetime)
            {
                handler_instance->p_aht21_instance->pf_read_humi(
                    handler_instance->p_aht21_instance,
                    humidity);
                handler_instance->last_temp_tick = tim;
            }
            break;
    default:
        *temperature = 0;
        *humidity = 0;
        return TEMP_HUMI_ERROR;
    }
    log_d("get_temp_humi end");
    return TEMP_HUMI_OK;
}

/**
 * @brief Handler thread for processing temperature and humidity events.
 *
 * @param[in] argument Pointer to input arguments (input_arg).
 */
void temp_humi_handler_thread(void * const argument)
{
    log_d("temp_humi_handler_thread start");
    float temp = 0;
    float humi = 0;
    temp_humi_handler_all_input_arg_t *input_arg = NULL;
    temp_humi_xxx_event_t event;
    bsp_aht21_driver_t bsp_aht21_driver;
    bsp_temp_humi_xxx_handler_t temp_humi_xxx_handler_instance = {0};
    temp_humi_status_t ret = TEMP_HUMI_OK;
    if(NULL == argument)
    {
        log_e("argument is NULL.");
        return;
    }
    input_arg = (temp_humi_handler_all_input_arg_t *)argument;
    temp_humi_xxx_handler_instance.p_aht21_instance = &bsp_aht21_driver;
    ret = bsp_temp_humi_xxx_handler_inst(
        &temp_humi_xxx_handler_instance,
        input_arg);
    if(TEMP_HUMI_OK == ret) // 如果不是OK怎么办？
    {
        __mount_handler(&temp_humi_xxx_handler_instance);
    }
    while(1)
    {
        ret = temp_humi_xxx_handler_instance.os_interface->os_queue_get(
            temp_humi_xxx_handler_instance.event_queue_handle,
            &event,
            MY_MAX_DELAY);
        ret = get_temp_humi(&temp_humi_xxx_handler_instance, &event, &temp, &humi);
        if(TEMP_HUMI_OK == ret)
        {

        }
        else
        {
            log_e("get_temp_humi failed.");
            continue;
        }
        event.pf_callback(&temp, &humi);
    }
}

/**
 * @brief API to read temperature and humidity.
 *
 * @param[in] event Event containing temperature and humidity request details.
 *
 * @retval TEMP_HUMI_OK            Event successfully queued.
 * @retval TEMP_HUMI_ERRORRESOURCE Resource allocation or initialization error.
 */
temp_humi_status_t bsp_temp_humi_xxx_read(temp_humi_xxx_event_t *event)
{
	TEST_ASSERT_MESSAGE(event, "The event is NULL!");
    log_d("temp_humi_xxx_read start");
    temp_humi_status_t ret = TEMP_HUMI_OK;
	if(NULL == event)
	{
		log_e("event is NULL");
		return TEMP_HUMI_ERRORRESOURCE;
	}
    if(TEMP_HUMI_NOT_INITIATED == gp_temp_humi_instance->p_private_data->is_initiated)
    {
        log_e("gp_temp_humi_instance is not initiated.");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    ret = gp_temp_humi_instance->os_interface->os_queue_put(
        gp_temp_humi_instance->event_queue_handle,
        event,
        MY_MAX_DELAY);
    if(TEMP_HUMI_OK != ret)
    {
        log_e("os_queue_put event failed");
        return TEMP_HUMI_ERRORRESOURCE;
    }
    return TEMP_HUMI_OK;
}
/********************************* Functions *********************************/
