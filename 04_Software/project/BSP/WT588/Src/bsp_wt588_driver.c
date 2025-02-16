#include "bsp_wt588_driver.h"
#include "elog.h"
#include "FreeRTOS.h"
#include "task.h"

// 给wt588发送命令
int8_t bsp_wt588fxx_write_register(wt588_driver_t *wt588_driver, uint8_t data)
{
    log_d("bsp_wt588fxx_write_register");
    log_d("data=%d", data);
    uint8_t j;
    uint8_t b_data;
    uint8_t s_data = data;
    
    wt588_driver->spk_gpio_interface->pf_io_write(0);
    wt588_driver->sys_interface->pf_delay_ms(5);

    b_data = (s_data & 0x01);
        
    // 增加临界区保护
    taskENTER_CRITICAL();
    for(j = 0; j < 8; j++)
    {
//      log_d("j=%d",j);
        if(b_data == 1)        // high level
        {
            wt588_driver->spk_gpio_interface->pf_pfio_write(1);
            wt588_driver->sys_interface->pf_delay_us(600);
            wt588_driver->spk_gpio_interface->pf_pfio_write(0);
            wt588_driver->sys_interface->pf_delay_us(200);
        }
        else
        {
            wt588_driver->spk_gpio_interface->pf_pfio_write(1);
            wt588_driver->sys_interface->pf_delay_us(200);
            wt588_driver->spk_gpio_interface->pf_pfio_write(0);
            wt588_driver->sys_interface->pf_delay_us(600);
        }
        
        s_data = s_data >> 1;
        b_data = s_data & 0X01;
    }
    taskEXIT_CRITICAL();
    wt588_driver->spk_gpio_interface->pf_io_write(1);
        
    return 0;
}

// 开始播放
int8_t start_play(wt588_driver_t *wt588_driver, uint8_t index)
{
    log_d("start_play");
    if(NULL == wt588_driver)
    {
        log_e("wt588_driver is NULL");
        return -1;
    }

    if(index > 244)
    {
        log_e("index is error");
        return -1;
    }

    bsp_wt588fxx_write_register(wt588_driver, index);

    return 0;
}
// 停止播放
int8_t stop_play(wt588_driver_t *wt588_driver)
{
    if(NULL == wt588_driver)
    {
        log_e("wt588_driver is NULL");
        return -1;
    }
    bsp_wt588fxx_write_register(wt588_driver, 0xFE);
    return 0;
}


// 音量调节
int8_t set_volume(wt588_driver_t *wt588_driver, uint8_t vol)
{        
        if (vol < 0xE0 || vol > 0xEF)
        {
            return -1;
        }
        bsp_wt588fxx_write_register(wt588_driver, vol);
        return 0;
}

// 初始化
int8_t wt588_driver_init(wt588_driver_t *wt588_driver)
{
    log_d("wt588_driver_init");
    wt588_driver->spk_gpio_interface->pf_init();
    wt588_driver->spk_busy_interface->pf_init();
}
// 获取busy电平状态
bool get_busy_level(wt588_driver_t *wt588_driver)
{
    uint8_t ret = wt588_driver->spk_busy_interface->pf_is_speaker_busy();
    // log_d("get_busy_level:ret=%d", ret);
    return ret;
}


int8_t wt588_driver_inst(wt588_driver_t      *wt588_driver, 
                        spk_gpio_interface_t *spk_gpio_interface, 
                        spk_busy_interface_t *spk_busy_interface,
                        sys_interface_t      *sys_interface
                        )
{
    log_d("wt588_driver_inst");
    if(NULL == wt588_driver       || 
       NULL == spk_gpio_interface || 
       NULL == spk_busy_interface || 
       NULL == sys_interface)
    {
        return -1;
    }

    wt588_driver->spk_gpio_interface = spk_gpio_interface;
    wt588_driver->spk_busy_interface = spk_busy_interface;
    wt588_driver->sys_interface      = sys_interface;

    wt588_driver->pf_start_play      = start_play;
    wt588_driver->pf_stop_play       = stop_play;
    wt588_driver->pf_set_volume      = set_volume;
    wt588_driver->pf_is_speaker_busy = get_busy_level;
    
    wt588_driver_init(wt588_driver);

    return 0;
}