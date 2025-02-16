#ifndef __BSP_WT588_DRIVER_H__
#define __BSP_WT588_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>
typedef struct
{
    void (*pf_init)(void);
    void (*pf_io_write)(uint8_t level);
}spk_gpio_interface_t;

typedef struct
{
    void (*pf_init)(void);
    void (*pf_is_speaker_busy)(void);
}spk_busy_interface_t;

typedef struct
{
    void (*pf_delay_us)(uint32_t us);
    void (*pf_delay_ms)(uint32_t ms);
}sys_interface_t;

typedef struct
{
    spk_gpio_interface_t *p_spk_gpio_interface;
    spk_busy_interface_t *p_spk_busy_interface;
    sys_interface_t      *p_sys_interface;
    int8_t (*pf_start_play)     (void *wt588_driver, uint8_t index);
    int8_t (*pf_stop_play)      (void *wt588_driver);
    int8_t (*pf_set_volume)     (void *wt588_driver, uint8_t volume);
    bool   (*pf_is_speaker_busy)(void *wt588_driver);
}wt588_driver_t;

int8_t wt588_driver_inst(
                         wt588_driver_t       *p_wt588_driver,
                         spk_gpio_interface_t *p_spk_gpiio_interface,
                         spk_busy_interface_t *p_spk_busy_interface,
                         sys_interface_t      *p_sys_interface
                        );

#endif /* __BSP_WT588_DRIVER_H__ */
