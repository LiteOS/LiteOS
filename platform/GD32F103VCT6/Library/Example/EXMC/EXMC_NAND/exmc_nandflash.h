/**
  ******************************************************************************
  * @file    EXMC/EXMC_NAND/exmc_nandflash.h
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The header file of the read and write nand flash.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EXMC_NANDFLASH_H
#define __EXMC_NANDFLASH_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Exported define -----------------------------------------------------------*/
/* NAND Area definition */
/* A16 = CLE  high */
#define EXMC_CMD_AREA              (uint32_t)(1<<16)

/* A17 = ALE high */
#define EXMC_ADDR_AREA             (uint32_t)(1<<17)
#define EXMC_DATA_AREA             ((uint32_t)0x00000000)

/* NAND memory command */
#define NAND_CMD_READ1_1ST         ((uint8_t)0x00)
#define NAND_CMD_READ1_2ND         ((uint8_t)0x30)
#define NAND_CMD_WRITE_1ST         ((uint8_t)0x80)
#define NAND_CMD_WRITE_2ND         ((uint8_t)0x10)
#define NAND_CMD_ERASE_1ST         ((uint8_t)0x60)
#define NAND_CMD_ERASE_2ND         ((uint8_t)0xD0)
#define NAND_CMD_READID            ((uint8_t)0x90)
#define NAND_CMD_STATUS            ((uint8_t)0x70)
#define NAND_CMD_LOCK_STATUS       ((uint8_t)0x7A)
#define NAND_CMD_RESET             ((uint8_t)0xFF)

/* NAND memory status */
#define NAND_BUSY                  ((uint8_t)0x00)
#define NAND_ERROR                 ((uint8_t)0x01)
#define NAND_READY                 ((uint8_t)0x40)
#define NAND_TIMEOUT_ERROR         ((uint8_t)0x80)

/* NAND memory parameters */
/* Nand zone count */
#define NAND_ZONE_COUNT            ((uint16_t)0x0001)

/* 1024 Block per zone */
#define NAND_ZONE_SIZE             ((uint16_t)0x0400)

/* 64 pages per block */
#define NAND_BLOCK_SIZE            ((uint16_t)0x0040)

/* 2 * 1024 bytes per page */
#define NAND_PAGE_SIZE             ((uint16_t)0x0800)

/* Last 64 bytes as spare area */
#define NAND_SPARE_AREA_SIZE       ((uint16_t)0x0040)

/* Total page size = page size + spare are size */
#define NAND_PAGE_TOTAL_SIZE       (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE)

/* Max read and write address */
#define NAND_MAX_ADDRESS           (((NAND_ZONE_COUNT*NAND_ZONE_SIZE)*NAND_BLOCK_SIZE)*NAND_PAGE_SIZE)

/* block count */
#define NAND_BLOCK_COUNT            1024

/* NAND memory address computation */
#define ADDR_1ST_CYCLE(ADDR)       (uint8_t)((ADDR)& 0xFF)
#define ADDR_2ND_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF00) >> 8)
#define ADDR_3RD_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF0000) >> 16)
#define ADDR_4TH_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF000000) >> 24)

/* define return value of functions */
#define NAND_OK                    0
#define NAND_FAIL                  1

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    uint8_t Maker_ID;
    uint8_t Device_ID;
    uint8_t Third_ID;
    uint8_t Fourth_ID;
}NAND_IDTypeDef;

typedef struct 
{
    uint16_t Zone;
    uint16_t Block;
    uint16_t Page;
    uint16_t PageInOffset;
} NAND_ADDRESS;

/* Exported functions --------------------------------------------------------*/
void    NAND_ReadID(NAND_IDTypeDef* NAND_ID);
void    NAND_Init(void);
uint8_t NAND_Format(void);
uint8_t NAND_Write(uint32_t MemAddr, uint8_t *pWriteBuf, uint16_t ByteCount); 
uint8_t NAND_Read(uint32_t MemAddr, uint8_t *pReadBuf, uint16_t ByteCount);
uint8_t NAND_Reset(void);

#endif /* __EXMC_NANDFLASH_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
