/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file ymodem.c
 * 
 * @par dependencies 
 * - ymodem.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __YMODEM_H
#define __YMODEM_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (16)

#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define EOT                     (0x04)  /* end of transmission 结束传输 */
#define ACK                     (0x06)  /* acknowledge 确认*/
#define NAK                     (0x15)  /* negative acknowledge 无确认 */
#define CA                      (0x18)  /* two of these in succession aborts transfer 其中两个相继中止传输 */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC 请求CRC */

#define ABORT1                  (0x41)  /* 'A' == 0x41, abort by user */
#define ABORT2                  (0x61)  /* 'a' == 0x61, abort by user */

#define NAK_TIMEOUT             (0x100000)//无确认超时时间
#define MAX_ERRORS              (3)//最大错误数量

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
int32_t Ymodem_Receive (uint8_t *);
uint8_t Ymodem_Transmit (uint8_t *,const  uint8_t* , uint32_t );
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte);
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size);
uint8_t CalChecksum(const uint8_t* data, uint32_t size);
int32_t Ymodem_CheckResponse(uint8_t c);
void Ymodem_PrepareIntialPacket(uint8_t *data, const uint8_t* fileName, uint32_t *length);
void Ymodem_PreparePacket(uint8_t *SourceBuf, uint8_t *data, uint8_t pktNo, uint32_t sizeBlk);
void Ymodem_SendPacket(uint8_t *data, uint16_t length);

#endif

