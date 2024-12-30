/********************************** Includes *********************************/
#include "ec_bsp_aht21_driver.h"
#include "ec_bsp_aht21_reg.h"
#include "rtt_log.h"
/********************************** Includes *********************************/

/********************************** Defines **********************************/
#define DEBUG 0

#define AHT21_MEASURE_WAITING_TIME 80    // Neasurement time [ms]
#define AHT21_NOT_INITED           0    // Not init flag
#define AHT21_INITED               1    // Init flag
#define AHT21_ID                0x18    // AHT21 ID

#define CRC8_POLYNOMIAL         0x31    // CRC-8 polynomial
#define CRC8_INITAL             0xFF    // CRC-8 initial value

#define IS_INITED (AHT21_INITED == g_inited)

/********************************** Defines **********************************/

/********************************* Variables *********************************/
static int8_t  g_inited    = AHT21_NOT_INITED;  // Init flag 
static uint8_t g_device_id =                0;  // Device ID
/********************************* Variables *********************************/

/**
 * @brief CRC-8 checksum.
 * @param[in] p_data : Pointer to the input data.
 * @param[in] length : length of input data.
 * 
 * @return uint8_t : The calculated CRC-8 checksum
 */
static uint8_t check_crc8(const uint8_t *p_data, const uint8_t lenght)
{
    uint8_t crc = CRC8_INITAL;
    for (uint8_t i = 0; i < lenght; i++)
    {
        crc ^= p_data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}
/**
 * @brief read ID.
 * @param[in] p_aht21_instance : Pointer to the AHT21 driver instance.
 * 
 * @return aht21_status_t.
 */
static aht21_status_t __read_id(bsp_aht21_driver_t *const p_aht21_instance)
{
    uint8_t data = 0;
#ifndef HARDWARE_IIC
    p_aht21_instance->p_iic_driver_instance->pf_critical_enter();
#endif /* HARDWARE_IIC */
    // Send the IIC start signal.
    p_aht21_instance->p_iic_driver_instance->pf_iic_start(NULL);
    // Send command to read ID.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_byte(NULL, AHT21_REG_READ_ADDR);
    // Wait the ACK of IIC slave device.
    if (AHT21_OK == p_aht21_instance->p_iic_driver_instance->pf_iic_wait_ack(NULL))
    {
        // Receive the data from slave device.
        p_aht21_instance->p_iic_driver_instance->pf_iic_recv_byte(NULL, &data);

    }
    // Send the IIC stop signal.
    p_aht21_instance->p_iic_driver_instance->pf_iic_stop(NULL);
#ifndef HARDWARE_IIC
    p_aht21_instance->p_iic_driver_instance->pf_critical_exit();
#endif /* HARDWARE_IIC */
    // Check the ID.
    if (AHT21_ID == (data & AHT21_ID))
    {
        g_device_id = AHT21_ID;
        return AHT21_OK;
    }
    return AHT21_ERRORREOURCE;
}

/**
 * @brief Function for reading AHT21 ID from outside calling.
 * @param[in] p_aht21_instance : Pointer to the AHT21 driver instance.
 * 
 * @return ID of AHT21 device.
 */
static aht21_status_t aht21_read_id(bsp_aht21_driver_t * const p_aht21_instance)
{
    if (!IS_INITED)
    {
        return AHT21_ERRORREOURCE;
    }
    LOGI("aht21_read_id");
    return g_device_id;
}

/**
 * @brief Function for initializing AHT21 device.
 * @param[in] p_aht21_instance : Pointer to the AHT21 driver instance.
 * 
 * @return AHT21_OK if initialization is successful, otherwise AHT21_ERRORREOURCE.
 */
static aht21_status_t aht21_init(bsp_aht21_driver_t * const p_aht21_instance)
{
    aht21_status_t ret = AHT21_OK;
    LOGI("aht21_init start");
    p_aht21_instance->p_yield_instance->pf_rtos_yield(300);
    if(NULL == p_aht21_instance->p_iic_driver_instance || 
       NULL == p_aht21_instance->p_iic_driver_instance->pf_iic_init)
    {
        LOGE("p_iic_driver_instance is NULL.");
        return AHT21_ERRORREOURCE;
    }
    // Enter critical segment.
#ifndef HARDWARE_IIC
    p_aht21_instance->p_iic_driver_instance->pf_critical_enter();
#endif /* HARDWARE_IIC */
    // Init the IIC interface.
    p_aht21_instance->p_iic_driver_instance->pf_iic_init(NULL);
    // Read the ID internally.
    ret = __read_id(p_aht21_instance);
    if (AHT21_OK != ret)
    {
        return AHT21_ERRORREOURCE;
    }
    // Exit critical segment.
#ifndef HARDWARE_IIC
    p_aht21_instance->p_iic_driver_instance->pf_critical_exit();
#endif /* HARDWARE_IIC */
    // Change the flag of the internal flag.
    g_inited = AHT21_INITED;
    LOGI("aht21's ID = [0x%x]", aht21_read_id(p_aht21_instance));
    return AHT21_OK;
}

/**
 * @brief Function for deinit aht21.
 * 
 * @param[in] p_aht21_instance : Pointer to AHT21 instance.
 * 
 * @return AHT21_OK on success.
 */
static aht21_status_t aht21_deinit(bsp_aht21_driver_t * const p_aht21_instance)
{
    g_inited = AHT21_NOT_INITED;
    return AHT21_OK;
}

/**
 * @brief Function for reading AHT21 status.
 * 
 * @param[in] p_aht21_instance : Pointer to ATH21 instance.
 */
static uint8_t aht21_read_status(bsp_aht21_driver_t * const p_aht21_instance)
{
    if(!IS_INITED)
    {
        return AHT21_ERRORREOURCE;
    }
    uint8_t rx_data = 0;
    // Enter critical segment.
#ifndef HARDWARE_IIC
    p_aht21_instance->p_iic_driver_instance->pf_critical_enter();
#endif /* HARDWARE_IIC */
    // Send the IIC start signal.
    p_aht21_instance->p_iic_driver_instance->pf_iic_start(NULL);
    // Send the address of IIC slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_byte(NULL, AHT21_REG_READ_ADDR);
    // Wait the ACK of IIC slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_wait_ack(NULL);
    // Receive the data from slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_recv_byte(NULL, &rx_data);
    // Send the non-ack signal from master to slave.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_no_ack(NULL);
    // Send the IIC stop signal.
    p_aht21_instance->p_iic_driver_instance->pf_iic_stop(NULL);
    // Exit critical segment.
#ifndef HARDWARE_IIC
    p_aht21_instance->p_iic_driver_instance->pf_critical_exit();
#endif /* HARDWARE_IIC */
    LOGI("rx_data=%#x", rx_data);
    return rx_data;
}

/**
 * @brief Function for reading AHT21 temperature and humidity.
 * 
 * @param[in] p_aht21_instance : Pointer to AHT21 instance.
 * @param[in] temp : Pointer to store temperature value.
 * @param[in] humi : Pointer to store humidity value.
 * 
 * @return aht21_status.
 */
static aht21_status_t aht21_read_temp_humi(bsp_aht21_driver_t * const p_aht21_instance,
                                           float * const temp, 
                                           float * const humi)
{
    if(!IS_INITED)
    {
        return AHT21_ERRORREOURCE;
    }
    uint8_t  cnt        = 5;
    uint8_t  byte_1th   = 0;
    uint8_t  byte_2th   = 0;
    uint8_t  byte_3th   = 0;
    uint8_t  byte_4th   = 0;
    uint8_t  byte_5th   = 0;
    uint8_t  byte_6th   = 0;
    uint32_t retu_data  = 0;
    // Enter critical segment.
#ifndef HARDWARE_IIC
    p_aht21_instance->p_iic_driver_instance->pf_critical_enter();
#endif /* HARDWARE_IIC */
    // Send the IIC start signal.
    p_aht21_instance->p_iic_driver_instance->pf_iic_start(NULL);
    // Send the address of IIC slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_byte(NULL, AHT21_REG_WRITE_ADDR);
    // Wait the ACK of IIC slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_wait_ack(NULL);
    // Send the command to ack aht21 prepare data.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_byte(NULL, AHT21_REG_MEASURE_CMD);
    // Wait the ACK of IIC slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_wait_ack(NULL);
    // Send the command to configure aht21 parameter[1]
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_byte(NULL, AHT21_REG_MEASURE_CMD_ARGS1);
    // Wait the ACK of IIC slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_wait_ack(NULL);
    // Send the command to configure aht21 parameter[2]
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_byte(NULL, AHT21_REG_MEASURE_CMD_ARGS2);
    // Wait the ACK of IIC slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_wait_ack(NULL);
    // Send the IIC stop signal.
    p_aht21_instance->p_iic_driver_instance->pf_iic_stop(NULL);
    // Exit critical segment.
#ifndef HARDWARE_IIC
    p_aht21_instance->p_iic_driver_instance->pf_critical_exit();
#endif /* HARDWARE_IIC */
    // Do the delay with AHT21_MEASURE_WAITING_TIME
    int32_t start_time = p_aht21_instance->p_timebase_instance->pf_get_tick_count();
    while(p_aht21_instance->p_timebase_instance->pf_get_tick_count() - start_time <AHT21_MEASURE_WAITING_TIME)
    {
#ifdef OS_SUPPORTING
        p_aht21_instance->p_yield_instance->pf_rtos_yield(AHT21_MEASURE_WAITING_TIME);
#endif /* OS_SUPPORTING */
    }
    // Do the delay with AHT21_MEASURE_WAITING_TIME
    while((0x80 == (aht21_read_status(p_aht21_instance)&0x80))&&cnt)
    {
        p_aht21_instance->p_yield_instance->pf_rtos_yield(5);
        cnt--;
        if(0 == cnt)
        {
            return AHT21_ERRORTIMEOUT;
        }
    }

    LOGI("read temp");
    // Enter critical segment.
#ifndef HARDWARE_IIC
    p_aht21_instance->p_iic_driver_instance->pf_critical_enter();
#endif /* HARDWARE_IIC */
    // Send the IIC start signal.
    p_aht21_instance->p_iic_driver_instance->pf_iic_start(NULL);
    // Send the address of IIC slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_byte(NULL, AHT21_REG_READ_ADDR);
    // Wait the ACK of IIC slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_wait_ack(NULL);
    // Receive the data from slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_recv_byte(NULL, &byte_1th);
    // Send the ack signal from master to slave.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_ack(NULL);
    // Receive the data from slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_recv_byte(NULL, &byte_2th);
    // Send the ack signal from master to slave.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_ack(NULL);
    // Receive the data from slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_recv_byte(NULL, &byte_3th);
    // Send the ack signal from master to slave.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_ack(NULL);
    // Receive the data from slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_recv_byte(NULL, &byte_4th);
    // Send the ack signal from master to slave.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_ack(NULL);
    // Receive the data from slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_recv_byte(NULL, &byte_5th);
    // Send the ack signal from master to slave.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_ack(NULL);
    // Receive the data from slave device.
    p_aht21_instance->p_iic_driver_instance->pf_iic_recv_byte(NULL, &byte_6th);
    // Send the non-ack signal from master to slave.
    p_aht21_instance->p_iic_driver_instance->pf_iic_send_no_ack(NULL);

    //Send the stop signal.
    p_aht21_instance->p_iic_driver_instance->pf_iic_stop(NULL);
    // Exit critical segment.
#ifndef HARDWARE_IIC
    p_aht21_instance->p_iic_driver_instance->pf_critical_exit();
#endif /* HARDWARE_IIC */    
    retu_data = (retu_data | byte_2th) << 8;
    retu_data = (retu_data | byte_3th) << 8;
    retu_data = (retu_data | byte_4th);
    retu_data = retu_data >> 4;
    *humi = (retu_data * 1000 >> 20);
    *humi /= 10;
    
    retu_data = 0;
    retu_data = (retu_data | (byte_4th & 0x0f)) << 8;
    retu_data = (retu_data | byte_5th) << 8;
    retu_data = (retu_data | byte_6th);
    retu_data = retu_data & 0xfffff;
    *temp = ((retu_data * 2000 >> 20) - 500);
    *temp /= 10;
    return AHT21_OK;
}

/**
 * @brief Function for make AHT21 wake-up.
 * 
 * @param p_aht21_instance : Pointer to AHT21 instance.
 * 
 * @return aht21_status.
 */
static aht21_status_t aht21_wakeup(bsp_aht21_driver_t * const p_aht21_instance)
{
    if(!IS_INITED)
    {
        return AHT21_ERRORREOURCE;
    }
    return AHT21_OK;
}
/**
 * @brief Function for make AHT21 sleep.
 * 
 * @param p_aht21_instance : Pointer to AHT21 instance.
 * 
 * @return aht21_status.
 */
static aht21_status_t aht21_sleep(bsp_aht21_driver_t * const p_aht21_instance)
{
    if(!IS_INITED)
    {
        return AHT21_ERRORREOURCE;
    }
    return AHT21_OK;
}

aht21_status_t aht21_inst(
                          bsp_aht21_driver_t * const p_aht21_instance,
                          iic_driver_interface_t * const p_iic_driver_instance,
                          timebase_interface_t * const p_timebase_interface,
#ifdef OS_SUPPORTING
                          yield_interface_t * const p_yield_interface
#endif /* end of OS_SUPPORTING */
                         )
{
    if(IS_INITED)
    {
        return AHT21_ERRORREOURCE;
    }
    uint8_t ret = 0;
    if(NULL == p_aht21_instance || NULL == p_iic_driver_instance)
    {
        return AHT21_ERRORREOURCE;
    }
    p_aht21_instance->p_iic_driver_instance = p_iic_driver_instance;
    p_aht21_instance->p_timebase_instance = p_timebase_interface;
#ifdef OS_SUPPORTING
    p_aht21_instance->p_yield_instance = p_yield_interface;
#endif /* end of OS_SUPPORTING */
    p_aht21_instance->pf_init = (aht21_status_t (*)(void * const))aht21_init;
    p_aht21_instance->pf_deinit = (aht21_status_t (*)(void * const))aht21_deinit;
    p_aht21_instance->pf_read_id = (aht21_status_t (*)(void * const))aht21_read_id;
    p_aht21_instance->pf_read_temp_humi = (aht21_status_t (*)(void * const, float * const, float * const))aht21_read_temp_humi;
    p_aht21_instance->pf_sleep = (aht21_status_t (*)(void * const))aht21_sleep;
    p_aht21_instance->pf_wakeup = (aht21_status_t (*)(void * const))aht21_wakeup;
    ret = aht21_init(p_aht21_instance);
    if(AHT21_OK != ret)
    {
		LOGW("AHT21 init failed");
        return AHT21_ERRORREOURCE;
    }
    return AHT21_OK;
}