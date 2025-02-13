/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "main.h"
#include "stdio.h"

#include "flash_patch.h"
#include "fpb_lib.h"
#include "rtt_log.h"

#define REG_INDEX        (0x04UL)

extern void origin_function(void);
void patched_function(void);

uint32_t remap_table_addr = 0x20010000;
uint32_t instr_addr = (uint32_t)origin_function;
uint32_t target_addr = (uint32_t)patched_function;

int fpb_setup()
{
	int error_code = 0;
	
	fpb_control_enable();
	fpb_remap_reg_config(remap_table_addr);
	error_code = fpb_redirect_function_call(remap_table_addr, instr_addr, target_addr, REG_INDEX, false);
	
	return error_code;
}

// This is the patched function.
__attribute__((used)) void patched_function(void) {
	while (1) {
		HAL_Delay(5000);
		printf("Patched function is running!!!!!!\r\n");
		LOGI("Patched function is running!!!!!!");
	}
}
