/**
  ******************************************************************************
  * @file    EXMC/EXMC_NOR/exmc_norflash.h
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The header file of the read and write nor flash.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EXMC_NORFLASH_H
#define __EXMC_NORFLASH_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Exported define -----------------------------------------------------------*/
/* Read ID Command define */
#define CMD_READID_ADD_1ST                     0x0555
#define CMD_READID_DATA_1ST                    0x00AA
#define CMD_READID_ADD_2ND                     0x02AA
#define CMD_READID_DATA_2ND                    0x0055
#define CMD_READID_ADD_3RD                     0x0555
#define CMD_READID_DATA_3RD                    0x0090

/* Block Erase Command define */
#define CMD_BLOCKERASE_ADD_1ST                 0x0555
#define CMD_BLOCKERASE_DATA_1ST                0x00AA
#define CMD_BLOCKERASE_ADD_2ND                 0x02AA
#define CMD_BLOCKERASE_DATA_2ND                0x0055
#define CMD_BLOCKERASE_ADD_3RD                 0x0555
#define CMD_BLOCKERASE_DATA_3RD                0x0080
#define CMD_BLOCKERASE_ADD_4TH                 0x0555
#define CMD_BLOCKERASE_DATA_4TH                0x00AA
#define CMD_BLOCKERASE_ADD_5TH                 0x02AA
#define CMD_BLOCKERASE_DATA_5TH                0x0055
#define CMD_BLOCKERASE_DATA_6TH                0x0030

/* Chip Erase Command define */
#define CMD_CHIPERASE_ADD_1ST                  0x0555
#define CMD_CHIPERASE_DATA_1ST                 0x00AA
#define CMD_CHIPERASE_ADD_2ND                  0x02AA
#define CMD_CHIPERASE_DATA_2ND                 0x0055
#define CMD_CHIPERASE_ADD_3RD                  0x0555
#define CMD_CHIPERASE_DATA_3RD                 0x0080
#define CMD_CHIPERASE_ADD_4TH                  0x0555
#define CMD_CHIPERASE_DATA_4TH                 0x00AA
#define CMD_CHIPERASE_ADD_5TH                  0x02AA
#define CMD_CHIPERASE_DATA_5TH                 0x0055
#define CMD_CHIPERASE_ADD_6TH                  0x0555
#define CMD_CHIPERASE_DATA_6TH                 0x0010

/* Reset Command define */
#define CMD_RESET_ADD_1ST                      0x0555
#define CMD_RESET_DATA_1ST                     0x00AA
#define CMD_RESET_ADD_2ND                      0x02AA
#define CMD_RESET_DATA_2ND                     0x0055
#define CMD_RESET_DATA_3RD                     0x00F0

/* Read Command define */
#define CMD_READ_ADD_1ST                       0x0555
#define CMD_READ_DATA_1ST                      0x00AA
#define CMD_READ_ADD_2ND                       0x02AA
#define CMD_READ_DATA_2ND                      0x0055
#define CMD_READ_DATA_3RD                      0x00F0

/* Write Command define */
#define CMD_WRITE_ADD_1ST                      0x0555
#define CMD_WRITE_DATA_1ST                     0x00AA
#define CMD_WRITE_ADD_2ND                      0x02AA
#define CMD_WRITE_DATA_2ND                     0x0055
#define CMD_WRITE_ADD_2RD                      0x0555
#define CMD_WRITE_DATA_3RD                     0x00A0

/* Max read and write address */
#define NOR_MAX_ADDRESS                        ((uint32_t)0x01000000)

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    uint16_t Manufacturer_Code;
    uint16_t Device_Code;
    uint16_t Block_Protection_Indicator;
    uint16_t Block_Protection_Status;
}NOR_IDTypeDef;

/* NOR Status */
typedef enum
{
    NOR_SUCCESS = 0,
    NOR_ONGOING,
    NOR_ERROR,
    NOR_TIMEOUT
}NOR_Status;

/* Exported functions --------------------------------------------------------*/
void EXMC_NorFlash_Init(void);
void NOR_ReadID(NOR_IDTypeDef* NOR_ID);
NOR_Status NOR_EraseBlock(uint32_t BlockAddr);
NOR_Status NOR_EraseChip(void);
NOR_Status NOR_WriteHalfWord(uint32_t WriteAddr, uint16_t Data);
NOR_Status NOR_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t HalfwordCount);
uint16_t NOR_ReadHalfWord(uint32_t ReadAddr);
void NOR_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t HalfwordCount);
void NOR_Reset(void);
NOR_Status NOR_GetStatus(uint32_t Timeout);
void NOR_ReturnToReadMode(void);

#endif /* __EXMC_NORFLASH_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
