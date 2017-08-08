/**    
  ******************************************************************************
  * @file    SPI/SPI_FALSH/GD25Qxx.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   SPI flash driver
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "GD25Qxx.h"
#include "gd32f10x.h"
#include <string.h>

/* Private define ------------------------------------------------------------*/
#define WRITE      0x02  /* Write to Memory instruction */
#define WRSR       0x01  /* Write Status Register instruction */
#define WREN       0x06  /* Write enable instruction */

#define READ       0x03  /* Read from Memory instruction */
#define RDSR       0x05  /* Read Status Register instruction  */
#define RDID       0x9F  /* Read identification */
#define SE         0x20  /* Sector Erase instruction */
#define BE         0xC7  /* Bulk Erase instruction */        

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte 0xA5

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Erases the specified FLASH sector
  * @param  SectorAddr: address of the sector to erase
  * @retval None    
  */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
    /* Send write enable instruction */
    SPI_FLASH_WriteEnable();

    /* Sector Erase */
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();
    /* Send Sector Erase instruction */
    SPI_FLASH_SendByte(SE);
    /* Send SectorAddr high nibble address byte */
    SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    SPI_FLASH_SendByte(SectorAddr & 0xFF);
    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();

    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd();
}


/**
  * @brief  Erases the entire FLASH
  * @param  None
  * @retval None    
  */
void SPI_FLASH_BulkErase(void)
{
    /* Send write enable instruction */
    SPI_FLASH_WriteEnable();

    /* Bulk Erase */
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();
    /* Send Bulk Erase instruction  */
    SPI_FLASH_SendByte(BE);
    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();

    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  Writes more than one byte to the FLASH
  * @param  pBuffer   : pointer to the buffer
  * @param  WriteAddr : FLASH's internal address to write to
  * @param  NumByteToWrite : number of bytes to write to the FLASH
  * @retval None    
  */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    /* Enable the write access to the FLASH */
    SPI_FLASH_WriteEnable();

    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();
    /* Send "Write to Memory " instruction */
    SPI_FLASH_SendByte(WRITE);
    /* Send WriteAddr high nibble address byte to write to */
    SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /* Send WriteAddr medium nibble address byte to write to */
    SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /* Send WriteAddr low nibble address byte to write to */
    SPI_FLASH_SendByte(WriteAddr & 0xFF);

    /* while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /* Send the current byte */
        SPI_FLASH_SendByte(*pBuffer);
        /* Point on the next byte to be written */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();

    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  Writes block of data to the FLASH
  * @param  pBuffer   : pointer to the buffer
  * @param  WriteAddr : FLASH's internal address to write to
  * @param  NumByteToWrite : number of bytes to write to the FLASH
  * @retval None    
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % SPI_FLASH_PageSize;
    count = SPI_FLASH_PageSize - Addr;
    NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
     /* WriteAddr is SPI_FLASH_PageSize aligned  */
    if (Addr == 0)
    {   
        /* NumByteToWrite < SPI_FLASH_PageSize */
        if (NumOfPage == 0) 
        {
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            while (NumOfPage--)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0)
        {
            /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
            if (NumOfSingle > count) 
            {
                temp = NumOfSingle - count;
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
            }
            else
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;

            while (NumOfPage--)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            if (NumOfSingle != 0)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

 /**
  * @brief  Reads a block of data from the FLASH
  * @param  pBuffer   : pointer to the buffer that receives the data read from the FLASH
  * @param  ReadAddr  : FLASH's internal address to read from
  * @param  NumByteToRead : number of bytes to read from the FLASH
  * @retval None    
  */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();

    /* Send "Read from Memory " instruction */
    SPI_FLASH_SendByte(READ);

    /* Send ReadAddr high nibble address byte to read from */
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte to read from */
    SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte to read from */
    SPI_FLASH_SendByte(ReadAddr & 0xFF);
    /* while there is data to be read */
    while (NumByteToRead--) 
    {
        /* Read a byte from the FLASH */
        *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  Reads FLASH identification
  * @param  None
  * @retval FLASH identification
  */
uint32_t SPI_FLASH_ReadID(void)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();

    /* Send "RDID " instruction */
    SPI_FLASH_SendByte(0x9F);

    /* Read a byte from the FLASH */
    Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();

    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}

 /**
  * @brief  Initiates a read data byte (READ) sequence from the Flash
  * @param  ReadAddr: FLASH's internal address to read from
  * @retval None
  */
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();

    /* Send "Read from Memory " instruction */
    SPI_FLASH_SendByte(READ);

    /* Send the 24-bit address of the address to read from -----------------------*/
    /* Send ReadAddr high nibble address byte */
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte */
    SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte */
    SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

 /**
  * @brief  Reads a byte from the SPI Flash
  * @param  None 
  * @retval Byte Read from the SPI Flash
  */
uint8_t SPI_FLASH_ReadByte(void)
{
    return (SPI_FLASH_SendByte(Dummy_Byte));
}

 /**
  * @brief  Sends a byte through the SPI interface and return the byte received from the SPI bus
  * @param  byte : byte to send
  * @retval The value of the received byte
  */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_TBE) == RESET);

    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI1, byte);

    /* Wait to receive a byte */
    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_RBNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);
}

 /**
  * @brief  Sends a Half Word through the SPI interface and return the Half Word received from the SPI bus
  * @param  HalfWord : Half Word to send
  * @retval The value of the received Half Word
  */
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_TBE) == RESET);

    /* Send Half Word through the SPI1 peripheral */
    SPI_I2S_SendData(SPI1, HalfWord);

    /* Wait to receive a Half Word */
    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_RBNE) == RESET);

    /* Return the Half Word read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);
}

 /**
  * @brief  Enables the write access to the FLASH
  * @param  None
  * @retval None
  */
void SPI_FLASH_WriteEnable(void)
{
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();

    /* Send "Write Enable" instruction */
    SPI_FLASH_SendByte(WREN);

    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's status  register  
  * @param  None
  * @retval None
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t FLASH_Status = 0;

    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();

    /* Send "Read Status Register" instruction */
    SPI_FLASH_SendByte(RDSR);

    /* Loop as long as the memory is busy with a write cycle */
    do
    {
        /* Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
    }
    while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/ 

