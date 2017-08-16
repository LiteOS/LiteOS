/**
  ******************************************************************************
  * @file    EXMC/EXMC_SRAM/EXMC_SRAM.h 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Headfile of EXMC_SRAM.c file.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EXMC_SRAM_H
#define __EXMC_SRAM_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Exported functions ------------------------------------------------------- */
void EXMC_SRAM_Init(void);
void EXMC_SRAM_WriteBuffer_16(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
void EXMC_SRAM_ReadBuffer_16(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);

void EXMC_SRAM_WriteBuffer_8(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void EXMC_SRAM_ReadBuffer_8(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);

void EXMC_SRAM_WriteBuffer_32(uint32_t* pBuffer, uint32_t WriteAddr, uint32_t NumWordToWrite);
void EXMC_SRAM_ReadBuffer_32(uint32_t* pBuffer, uint32_t ReadAddr, uint32_t NumWordToRead);

#endif /* __EXMC_SRAM_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
