#ifndef __BSP_WT588_HANDLER_H__
#define __BSP_WT588_HANDLER_H__
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include <bsp_wt588_driver.h>


#define SPK_EVENT_QUEUE_MAX 10
#define SPK_EVENT_ARRAY_MAX 10

typedef enum 
{
    PRO_0 = 0,  
    PRO_1,      
    PRO_2,      /**< Priority level 2: Highest priority */
} spk_priotity_t;

typedef enum 
{
    LOW = 0,
    MIDDLE,
    HIGH,
}spk_volume_t;

typedef enum 
{
    ACTION_SEQUENCE = 0,
    ACTION_INTERRUPT,
}spk_action_t;

/* speaker event */
typedef struct {
    spk_priotity_t spk_priotity;  
    spk_volume_t   spk_volume;
    spk_action_t   spk_action;
    uint8_t        spk_sequence;
} spk_event_t;


typedef struct
{
    spk_gpio_interface_t *spk_gpio_interface;
    spk_busy_interface_t *spk_busy_interface;
    sys_interface_t      *sys_interface;
}spk_handler_arg_t;



typedef struct {
    bool inited;
    QueueHandle_t handler_queue;
    QueueHandle_t executor_queue;
    QueueHandle_t busy_detection_queue;
    QueueHandle_t voice_finish_queue;
        
    TaskHandle_t handler_thread_handle;
    TaskHandle_t executor_thread_handle;
    TaskHandle_t busy_thread_handle;
        
    SemaphoreHandle_t busy_mutex_handle;

    wt588_driver_t *wt588_driver_inst;

} bsp_wt588_handler_t;


int8_t bsp_wt588_handler_inst(spk_handler_arg_t *arg);
int8_t spk_event_to_handler_send(spk_event_t event);

#endif /* __BSP_WT588_HANDLER_H__ */
