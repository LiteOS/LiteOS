/**
  ******************************************************************************
  * @file    EXMC/EXMC_SRAM/EXMC_SRAM.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   EXMC write or read SRAM functions.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "EXMC_SRAM.h"

/* Private define ------------------------------------------------------------*/
#define Bank1_SRAM1_ADDR    ((u32)0x60000000)

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure EXMC and GPIOs to access the SRAM.
  * @param  None
  * @retval None
  */
void EXMC_SRAM_Init(void)
{
    EXMC_NORSRAMInitPara  EXMC_NORSRAMInitStructure;
    EXMC_NORSRAMTimingInitPara  p;
    GPIO_InitPara GPIO_InitStructure; 

    /* EXMC clock enable */
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_EXMC, ENABLE);

    /*EXMC enable */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOB | RCC_APB2PERIPH_GPIOD | 
                               RCC_APB2PERIPH_GPIOE | RCC_APB2PERIPH_AF, ENABLE);

    /* Config GPIO AD[0-15] */
    /* Set PD14(AD0),PD15(AD1),PD0(AD2),PD1(AD3),PD8(AD13),PD9(AD14),PD10(AD15) as alternate function push pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                                  GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 

    /* Set PD7(AD4),PD8(AD5),PD9(AD6),PD10(AD7),PD11(AD8),PD12(AD9),PD13(AD10),PD14(AD11),
       PD15(AD12) as alternate function push pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                                  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | 
                                  GPIO_PIN_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    /* Config EXMC_A[16:18],PD11(AD16),PD12(AD17),PD13(AD18) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /*Config EXMC NOE(PD4),NWE(PD5) and NE1(PD7) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7 ;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /*Config EXMC NBL0(PE0) and NBL1(PE1) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_1; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* Config EXMC NADV (PB7) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_7; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Config timing parameters */
    p.EXMC_AsynAddressSetupTime = 0x05;
    p.EXMC_AsynAddressHoldTime = 0x02;
    p.EXMC_AsynDataSetupTime = 0x07;
    p.EXMC_BusLatency = 0;
    p.EXMC_SynCLKDivision = 0;
    p.EXMC_SynDataLatency = 0;
    p.EXMC_AsynAccessMode = EXMC_ACCESS_MODE_A;
    
    /* Config EXMC BUS parameters */
  	EXMC_NORSRAMInitStructure.EXMC_NORSRAMBank = EXMC_BANK1_NORSRAM1;
    /* Config the data bus and address bus are multiplexed */
    EXMC_NORSRAMInitStructure.EXMC_AddressDataMux = EXMC_ADDRESS_DATA_MUX_ENABLE;
    /* The type of external memory is SRAM */
    EXMC_NORSRAMInitStructure.EXMC_MemoryType = EXMC_MEMORY_TYPE_SRAM;
    EXMC_NORSRAMInitStructure.EXMC_DatabusWidth = EXMC_DATABUS_WIDTH_16B;
    EXMC_NORSRAMInitStructure.EXMC_BurstMode = EXMC_BURST_MODE_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_AsynWait = EXMC_ASYN_WAIT_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_NWAITPolarity = EXMC_NWAIT_POLARITY_LOW; 
    EXMC_NORSRAMInitStructure.EXMC_WrapBurstMode = EXMC_WRAP_BURST_MODE_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_NWAITConfig = EXMC_NWAIT_CONFIG_BEFORE;
    EXMC_NORSRAMInitStructure.EXMC_MemoryWrite = EXMC_MEMORY_WRITE_ENABLE;
    EXMC_NORSRAMInitStructure.EXMC_NWAITSignal = EXMC_NWAIT_SIGNAL_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_ExtendedMode = EXMC_EXTENDED_MODE_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_WriteMode = EXMC_ASYN_WRITE; 
    EXMC_NORSRAMInitStructure.EXMC_ReadWriteTimingParaStruct = &p;
    EXMC_NORSRAMInitStructure.EXMC_WriteTimingParaStruct = &p;

    EXMC_NORSRAM_Init(&EXMC_NORSRAMInitStructure);

    /* Enable the EXMC Bank1 NORSRAM1 */
    EXMC_NORSRAM_Enable(EXMC_BANK1_NORSRAM1, ENABLE);

}

/**
  * @brief  Write a Half-word buffer(data is 16 bits ) to the EXMC SRAM memory. 
  * @param  pBuffer : pointer to buffer. 
  * @param  WriteAddr : SRAM memory internal address from which the data will be 
  *         written.
  * @param  NumHalfwordToWrite : number of half-words to write. 
  * @retval None
  */
void EXMC_SRAM_WriteBuffer_16(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
    /* While there is data to write */
    for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--) 
    {
        /* Transfer data to the memory */
        *(uint16_t *) (Bank1_SRAM1_ADDR + WriteAddr) = *pBuffer++;
    
        /* Increment the address */  
        WriteAddr += 2;
    }   
}

/**
  * @brief  Read a block of 16-bit data from the EXMC SRAM memory.
  * @param  pBuffer : pointer to the buffer that receives the data read from the 
  *         SRAM memory.
  * @param  ReadAddr : SRAM memory internal address to read from.
  * @param  NumHalfwordToRead : number of half-words to read.
  * @retval None
  */
void EXMC_SRAM_ReadBuffer_16(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
    /* While there is data to read */
    for(; NumHalfwordToRead != 0; NumHalfwordToRead--) 
    {
        /* Read a half-word from the memory */
        *pBuffer++ = *(uint16_t*) (Bank1_SRAM1_ADDR + ReadAddr);
    
        /* Increment the address */
        ReadAddr += 2;
    }  
}

/**
  * @brief  Write a Word buffer(data is 32 bits ) to the EXMC SRAM memory. 
  * @param  pBuffer : pointer to buffer. 
  * @param  WriteAddr : SRAM memory internal address from which the data will be 
  *         written.
  * @param  NumWordToWrite : number of words to write. 
  * @retval None
  */
void EXMC_SRAM_WriteBuffer_32(uint32_t* pBuffer, uint32_t WriteAddr, uint32_t NumWordToWrite)
{
    /* While there is data to write */
    for(; NumWordToWrite != 0; NumWordToWrite--) 
    {
        /* Transfer data to the memory */
        *(uint32_t *) (Bank1_SRAM1_ADDR + WriteAddr) = *pBuffer++;
    
        /* Increment the address */  
        WriteAddr += 4;
    }   
}

/**
  * @brief  Read a block of 32-bit data from the EXMC SRAM memory.
  * @param  pBuffer : pointer to the buffer that receives the data read from the 
  *         SRAM memory.
  * @param  ReadAddr : SRAM memory internal address to read from.
  * @param  NumWordToRead : number of words to read.
  * @retval None
  */
void EXMC_SRAM_ReadBuffer_32(uint32_t* pBuffer, uint32_t ReadAddr, uint32_t NumWordToRead)
{
    /* While there is data to read */
    for(; NumWordToRead != 0; NumWordToRead--) 
    {
        /* Read a word from the memory */
        *pBuffer++ = *(uint32_t*) (Bank1_SRAM1_ADDR + ReadAddr);
    
        /* Increment the address */
        ReadAddr += 4;
    }  
}

/**
  * @brief  Write a Byte buffer(data is 8 bits ) to the EXMC SRAM memory. 
  * @param  pBuffer : pointer to buffer. 
  * @param  WriteAddr : SRAM memory internal address from which the data will be 
  *         written.
  * @param  NumByteToWrite : number of bytes to write. 
  * @retval None
  * @note   You can choose to write a byte to upper byte or Lower byte of 16-bit SRAM memory.
  *         -Config EXMC_NBL1 pin to achive that writing a byte to upper byte of 16-bit sram and
  *          the written address should be odd. Also, you should add 2 to the address each time.
  *         -Config EXMC_NBL0 pin to achive that writing a byte to lower byte of 16-bit sram
  *          and the written address should be even. You should add 2 to the address each time.
  */
void EXMC_SRAM_WriteBuffer_8(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    /* While there is data to write */
    for(; NumByteToWrite != 0; NumByteToWrite--) 
    {
        /* Transfer data to the memory */
        *(uint8_t *) (Bank1_SRAM1_ADDR + WriteAddr) = *pBuffer++;

        /* Increment the address*/  
        WriteAddr += 1;
    }
}

/**
  * @brief  Read a block of 8-bit data from the EXMC SRAM memory.
  * @param  pBuffer : pointer to the buffer that receives the data read from the 
  *         SRAM memory.
  * @param  ReadAddr : SRAM memory internal address to read from.
  * @param  NumByteToRead : number of bytes to read.
  * @retval None
  * @note   If all bytes are stored in upper byte or Lower byte of 16-bit SRAM memory.you should 
  *         add 2 to the address each time.
  */
void EXMC_SRAM_ReadBuffer_8(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
    /* While there is data to read */
    for(; NumByteToRead != 0; NumByteToRead--)
    {
        /* Read a byte from the memory */
        *pBuffer++ = *(uint8_t*) (Bank1_SRAM1_ADDR + ReadAddr);
    
        /* Increment the address */
        ReadAddr += 1;
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
