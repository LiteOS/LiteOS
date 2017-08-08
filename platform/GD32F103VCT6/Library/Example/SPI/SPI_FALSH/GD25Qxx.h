/**    
  ******************************************************************************    
  * @file    SPI/SPI_FALSH/GD25Qxx.h
  * @author  MCU SD    
  * @version V1.0    
  * @date    26-Dec-2014    
  * @brief   Headfile of SPI flash driver
  ******************************************************************************    
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GD25Qxx_H
#define GD25Qxx_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Private define ------------------------------------------------------------*/
#define  SPI_FLASH_PageSize    0x100
#define  SPI_FLASH_CS_LOW()    GPIO_ResetBits(GPIOE, GPIO_PIN_3)
#define  SPI_FLASH_CS_HIGH()   GPIO_SetBits(GPIOE, GPIO_PIN_3)

/* Exported functions ------------------------------------------------------- */
void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif /* GD25Qxx_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/ 