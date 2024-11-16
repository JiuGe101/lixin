/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file Flash.c
 * 
 * @par dependencies 
 * - Flash.h
 * 
 * @author Jack | R&D Dept. | EternalChip 立芯嵌入式
 * 
 * @brief Functions related to reading and writing in the chip's flash area.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2024-09-13
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "flash.h"
#include "common.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint16_t STMFLASH_GetFlashSector(u32 addr);
/* Private functions ---------------------------------------------------------*/
/* extern variables ---------------------------------------------------------*/

const uint32_t Flash_Sectorsize[12] = \
{   0x08000000, //Sector1
    0x08004000, //Sector2
    0x08008000, //Sector3
    0x0800C000, //Sector4
    0x08010000, //Sector5
    0x08020000, //Sector6
    0x08040000, //Sector7
    0x08060000, //Sector8
    0x08080000, //Sector9
    0x080A0000, //Sector10
    0x080C0000, //Sector11
    0x080E0000  //Sector12
};

uint8_t Back_to_App_Flash(uint32_t App_size)
{
    uint8_t flash_state = Flash_erase(ApplicationAddress,App_size);
    if (1 == flash_state)
    {
        return 1;
    }
    uint32_t FlashDest = ApplicationAddress; 
    uint32_t Back_Source = (uint32_t)BackAppAddress;

    for (uint32_t j = 0;(j < App_size) && (FlashDest <  ApplicationAddress + App_size);j += 4)
    {
        Flash_Write(FlashDest,*(uint32_t*)Back_Source);
        //写完之后校验一下，是否写入
        if (*(uint32_t*)FlashDest != *(uint32_t*)Back_Source)
        {
            return 1;
        }
        FlashDest += 4;
        Back_Source += 4;
    }

    return 0;
}

/**
 * @brief  This function is used to erase the flash area.
 * @param  None
 * @retval 0 ： Success
 *         1 ： Fail
 */
uint8_t Flash_erase(u32 addr,u32 size)
{
    //将App代码的所有涉及到的扇区全部擦除

    /*判断size在几个扇区内*/
    FLASH_Status ret = 1;
    uint32_t flash_start_sector = 0;
    uint32_t flash_end_sector = 0;
    flash_start_sector = STMFLASH_GetFlashSector(addr);
    flash_end_sector = STMFLASH_GetFlashSector(addr + size);

    for(uint8_t i = 0; i <= 12; i++)
    {
        if((Flash_Sectorsize[i]) >= flash_start_sector && (Flash_Sectorsize[i]) <= flash_end_sector)
        {
            if(EreaseAppSector(i) != FLASH_COMPLETE) return 1;
        }
    }

    return 0;
}

//通过地址获取扇区位置
static uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}

void Flash_Unlock(void) {
    FLASH_Unlock();
    while (FLASH_GetStatus() == FLASH_BUSY);
}

void Flash_Lock(void) {
    FLASH_Lock();
}

//擦除APP区域的数据
// f4是按扇区操作，计划将app放在扇区6  FLASH_Sector_6，备份app放在扇区7
FLASH_Status EreaseAppSector(uint32_t FLASH_Sector)
{
	Flash_Unlock();
	FLASH_Status FLASHStatus = FLASH_EraseSector(FLASH_Sector, VoltageRange_3);
	Flash_Lock();
	return FLASHStatus;
}


void Flash_Write(uint32_t address, uint32_t data) {
    // 解锁Flash
    Flash_Unlock();
    
    // 清除所有标志位
	  // FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    
    // 编程一个字（32位）
    FLASH_Status status = FLASH_ProgramWord(address, data);
    
    // 检查编程是否成功
    if (status == FLASH_COMPLETE) {
        // 数据写入成功
			  // log_d("Flash_Write success"); 
    } else {
        // 数据写入失败
        // 在这里添加错误处理代码
			  //log_e("Flash_Write fail"); 
    }
    
    // 锁定Flash
    Flash_Lock();
}



