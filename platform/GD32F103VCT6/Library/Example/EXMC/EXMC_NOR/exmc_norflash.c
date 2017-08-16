/**
  ******************************************************************************
  * @file    EXMC/EXMC_NOR/exmc_norflash.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The file of the read and write nor flash file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "exmc_norflash.h"

/* Exported macro ------------------------------------------------------------*/
#define BANK_NOR2_ADDR                  ((uint32_t)0x60000000)
#define BANK_NORFLASH_ADDR              BANK_NOR2_ADDR

/* Delay definition */
#define BLOCKERASE_TIMEOUT              ((uint32_t)0x00A00000)
#define CHIPERASE_TIMEOUT               ((uint32_t)0x30000000)
#define PROGRAM_TIMEOUT                 ((uint32_t)0x00001400)

#define ADDR_SHIFT(A)                   (BANK_NOR2_ADDR + (2 * (A)))
#define NOR_WRITE(Address, Data)        (*(__IO uint16_t *)(Address) = (Data))


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure EXMC and GPIO interface for nor flash.
  * @param  None
  * @retval None
  */
void EXMC_NorFlash_Init(void) 
{
    EXMC_NORSRAMInitPara  EXMC_NORSRAMInitStructure;
    EXMC_NORSRAMTimingInitPara  p;
    GPIO_InitPara GPIO_InitStructure;

    /* Enable EXMC clock*/
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_EXMC, ENABLE);

    /* Enable GPIOB GPIOD¡¢GPIOE clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOB | RCC_APB2PERIPH_GPIOD |
                               RCC_APB2PERIPH_GPIOE , ENABLE);

    /* EXMC GPIO configuration */
    /* Set PD0(D2), PD1(D3), PD8(D13), PD9(D14), PD10(D15), PD14(D0), PD15(D1),
       as alternate function push pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_1| GPIO_PIN_8 | GPIO_PIN_9 |
                                  GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Set PE7(D4), PE8(D5), PE9(D6), PE10(D7), PE11(D8), PE12(D9), PE13(D10),
       PE14(D11), PE15(D12) as alternate function push pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                                  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |
                                  GPIO_PIN_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    /* Configure PD11(A16), PD12(A17), PD13(A18)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Configure PE3(A19), PE4(A20), PE5(A21), PE6(A22) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* Configure PD4(NOE), PD5(NWE), PD7(NE1) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* Configure PE0(NBL0), PE1(NBL1) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* Configure PB7(NADV) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* Configure PD6(NWAIT) for NOR memory Ready/Busy signal */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* EXMC configuration */
    p.EXMC_AsynAddressSetupTime = 0x06;
    /* if AD multiplex, EXMC_AsynAddressHoldTime >= 1 */
    p.EXMC_AsynAddressHoldTime = 0x02;
    p.EXMC_AsynDataSetupTime = 0x07; 
    p.EXMC_BusLatency = 0x00; 
    p.EXMC_SynCLKDivision = 0x00;
    p.EXMC_SynDataLatency = 0x00;
    p.EXMC_AsynAccessMode = EXMC_ACCESS_MODE_B;

    EXMC_NORSRAMInitStructure.EXMC_NORSRAMBank = EXMC_BANK1_NORSRAM1;
    EXMC_NORSRAMInitStructure.EXMC_AddressDataMux = EXMC_ADDRESS_DATA_MUX_ENABLE;
    EXMC_NORSRAMInitStructure.EXMC_MemoryType = EXMC_MEMORY_TYPE_NOR;
    EXMC_NORSRAMInitStructure.EXMC_DatabusWidth = EXMC_DATABUS_WIDTH_16B; 
    EXMC_NORSRAMInitStructure.EXMC_BurstMode = EXMC_BURST_MODE_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_NWAITPolarity = EXMC_NWAIT_POLARITY_LOW;
    EXMC_NORSRAMInitStructure.EXMC_WrapBurstMode = EXMC_WRAP_BURST_MODE_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_NWAITConfig = EXMC_NWAIT_CONFIG_BEFORE;
    EXMC_NORSRAMInitStructure.EXMC_MemoryWrite = EXMC_MEMORY_WRITE_ENABLE;
    EXMC_NORSRAMInitStructure.EXMC_NWAITSignal = EXMC_NWAIT_SIGNAL_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_AsynWait = EXMC_ASYN_WAIT_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_ExtendedMode = EXMC_EXTENDED_MODE_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_WriteMode = EXMC_ASYN_WRITE;
    EXMC_NORSRAMInitStructure.EXMC_ReadWriteTimingParaStruct = &p;
    EXMC_NORSRAMInitStructure.EXMC_WriteTimingParaStruct = &p;

    EXMC_NORSRAM_Init(&EXMC_NORSRAMInitStructure);
    EXMC_NORSRAM_Enable(EXMC_BANK1_NORSRAM1, ENABLE);
}

/**
  * @brief  Read NOR memory's Manufacturer, Device Code, 
  *         Block_Protection_Indicator, Block_Protection_Status.
  * @param  NOR_ID: pointer to a NOR_IDTypeDef structure.
  * @retval None
  */
void NOR_ReadID(NOR_IDTypeDef* NOR_ID)
{
    NOR_WRITE(ADDR_SHIFT(CMD_READID_ADD_1ST), CMD_READID_DATA_1ST);
    NOR_WRITE(ADDR_SHIFT(CMD_READID_ADD_2ND), CMD_READID_DATA_2ND);
    NOR_WRITE(ADDR_SHIFT(CMD_READID_ADD_3RD), CMD_READID_DATA_3RD);

    NOR_ID->Manufacturer_Code = *(__IO uint16_t *) ADDR_SHIFT(0x0000);
    NOR_ID->Device_Code = *(__IO uint16_t *) ADDR_SHIFT(0x0001);
    NOR_ID->Block_Protection_Indicator = *(__IO uint16_t *) ADDR_SHIFT(0x000E);
    NOR_ID->Block_Protection_Status = *(__IO uint16_t *) ADDR_SHIFT(0x000F);
}

/**
  * @brief  Erase the specified nor flash block.
  * @param  BlockAddr: Address of the block to be erased.
  * @retval NOR_SUCCESS: Erase block successfully. 
  *         NOR_ERROR: Erase block failure.
  *         NOR_TIMEOUT: Erase block timeout.
  */
NOR_Status NOR_EraseBlock(uint32_t BlockAddr)
{
    NOR_WRITE(ADDR_SHIFT(CMD_BLOCKERASE_ADD_1ST), CMD_BLOCKERASE_DATA_1ST);
    NOR_WRITE(ADDR_SHIFT(CMD_BLOCKERASE_ADD_2ND), CMD_BLOCKERASE_DATA_2ND);
    NOR_WRITE(ADDR_SHIFT(CMD_BLOCKERASE_ADD_3RD), CMD_BLOCKERASE_DATA_3RD);
    NOR_WRITE(ADDR_SHIFT(CMD_BLOCKERASE_ADD_4TH), CMD_BLOCKERASE_DATA_4TH);
    NOR_WRITE(ADDR_SHIFT(CMD_BLOCKERASE_ADD_5TH), CMD_BLOCKERASE_DATA_5TH);
    NOR_WRITE((BANK_NOR2_ADDR  + BlockAddr), CMD_BLOCKERASE_DATA_6TH);

    return (NOR_GetStatus(BLOCKERASE_TIMEOUT));
}

/**
  * @brief  Erase the entire chip.
  * @param  None
  * @retval NOR_SUCCESS: Erase chip successfully. 
  *         NOR_ERROR: Erase chip failure.
  *         NOR_TIMEOUT: Erase chip timeout.
  */
NOR_Status NOR_EraseChip(void)
{
    NOR_WRITE(ADDR_SHIFT(CMD_CHIPERASE_ADD_1ST), CMD_CHIPERASE_DATA_1ST);
    NOR_WRITE(ADDR_SHIFT(CMD_CHIPERASE_ADD_2ND), CMD_CHIPERASE_DATA_2ND);
    NOR_WRITE(ADDR_SHIFT(CMD_CHIPERASE_ADD_3RD), CMD_CHIPERASE_DATA_3RD);
    NOR_WRITE(ADDR_SHIFT(CMD_CHIPERASE_ADD_4TH), CMD_CHIPERASE_DATA_4TH);
    NOR_WRITE(ADDR_SHIFT(CMD_CHIPERASE_ADD_5TH), CMD_CHIPERASE_DATA_5TH);
    NOR_WRITE(ADDR_SHIFT(CMD_CHIPERASE_ADD_6TH), CMD_CHIPERASE_DATA_6TH);

    return (NOR_GetStatus(CHIPERASE_TIMEOUT));
}

/**
  * @brief  Write a half-word to the specified address of nor flash.
  * @param  WriteAddr: Nor flash internal address to write to.
  * @param  Data: Data to be written.
  * @retval NOR_SUCCESS: Write data successfully.
  *         NOR_ERROR: Write data failure.
  *         NOR_TIMEOUT: Write data timeout.
  */
NOR_Status NOR_WriteHalfWord(uint32_t WriteAddr, uint16_t Data)
{
    NOR_WRITE(ADDR_SHIFT(CMD_WRITE_ADD_1ST), CMD_WRITE_DATA_1ST);
    NOR_WRITE(ADDR_SHIFT(CMD_WRITE_ADD_2ND), CMD_WRITE_DATA_2ND);
    NOR_WRITE(ADDR_SHIFT(CMD_WRITE_ADD_2RD), CMD_WRITE_DATA_3RD);
    NOR_WRITE((BANK_NOR2_ADDR  + WriteAddr), Data);

    return (NOR_GetStatus(PROGRAM_TIMEOUT));
}

/**
  * @brief  Write a half-word buffer to the specified address of nor flash.
  * @param  pBuffer: Pointer to a half-word buffer.
  * @param  WriteAddr: Nor flash internal address from which the data will be written.
  * @param  HalfwordCount: Count of Half words to write.
  * @retval NOR_SUCCESS: Write buffer successfully.
  *         NOR_ERROR: Write buffer failure.
  *         NOR_TIMEOUT: Write buffer timeout.
  */
NOR_Status NOR_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t HalfwordCount)
{
    NOR_Status status = NOR_ONGOING; 
    
    do
    {
        /* Write data to nor flash */
        status = NOR_WriteHalfWord(WriteAddr, *pBuffer++);
        WriteAddr = WriteAddr + 2;
        HalfwordCount--;
    }
    while((status == NOR_SUCCESS) && (HalfwordCount != 0));

    return (status); 
}

/**
  * @brief  Read a half-word from the specified address of nor flash. 
  * @param  ReadAddr: Nor flash internal address to read from.
  * @retval A half-word read from the nor flash.
  */
uint16_t NOR_ReadHalfWord(uint32_t ReadAddr)
{
    NOR_WRITE(ADDR_SHIFT(CMD_READ_ADD_1ST), CMD_READ_DATA_1ST);
    NOR_WRITE(ADDR_SHIFT(CMD_READ_ADD_2ND), CMD_READ_DATA_2ND);
    NOR_WRITE((BANK_NOR2_ADDR  + ReadAddr), CMD_READ_DATA_3RD);

    return (*(__IO uint16_t *)((BANK_NOR2_ADDR  + ReadAddr)));
}

/**
  * @brief  Read a set of data from the specified address of nor flash.
  * @param  pBuffer: Pointer to the received buffer.
  * @param  ReadAddr: Nor flash internal address to read from.
  * @param  HalfwordCount : Conut of half word to read.
  * @retval None
  */
void NOR_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t HalfwordCount)
{
    NOR_WRITE(ADDR_SHIFT(CMD_READ_ADD_1ST), CMD_READ_DATA_1ST); 
    NOR_WRITE(ADDR_SHIFT(CMD_READ_ADD_2ND), CMD_READ_DATA_2ND);  
    NOR_WRITE((BANK_NOR2_ADDR  + ReadAddr), CMD_READ_DATA_3RD);

    for(; HalfwordCount != 0x00; HalfwordCount--)
    {
        /* Read a halfword from the nor flash */
        *pBuffer++ = *(__IO uint16_t *)((BANK_NOR2_ADDR  + ReadAddr));
        ReadAddr = ReadAddr + 2; 
    }  
}

/**
  * @brief  Return the nor flash to read mode and reset the errors in the nor
  *         flash status register.
  * @param  None
  * @retval None
  */
void NOR_Reset(void)
{
    NOR_WRITE(ADDR_SHIFT(CMD_RESET_ADD_1ST), CMD_RESET_DATA_1ST); 
    NOR_WRITE(ADDR_SHIFT(CMD_RESET_ADD_2ND), CMD_RESET_DATA_2ND); 
    NOR_WRITE(BANK_NOR2_ADDR , CMD_RESET_DATA_3RD); 
}

/**
  * @brief  Return the nor flash to read mode.
  * @param  None
  * @retval None
  */
void NOR_ReturnToReadMode(void)
{
    NOR_WRITE(BANK_NOR2_ADDR , CMD_RESET_DATA_3RD); 
}

/**
  * @brief  Return the nor flash operation status.
  * @param  Timeout: Nor flash  progamming timeout.
  * @retval NOR_SUCCESS, NOR_ERROR or NOR_TIMEOUT.
  */
NOR_Status NOR_GetStatus(uint32_t Timeout)
{ 
    uint16_t val1 = 0x00, val2 = 0x00;
    NOR_Status status = NOR_ONGOING; 
    uint32_t timeout = Timeout;

    /* Poll on nor flash ready/busy signal */
    while((GPIO_ReadInputBit(GPIOD, GPIO_PIN_6) != RESET) && (timeout > 0)) 
    {
        timeout--;
    }

    timeout = Timeout;

    while((GPIO_ReadInputBit(GPIOD, GPIO_PIN_6) == RESET) && (timeout > 0))   
    {
        timeout--;
    }

    /* Get the nor flash operation status */
    while((Timeout != 0x00) && (status != NOR_SUCCESS))
    {
        Timeout--;

        /* Read DQ6 and DQ5 */
        val1 = *(__IO uint16_t *)(BANK_NOR2_ADDR );
        val2 = *(__IO uint16_t *)(BANK_NOR2_ADDR );

        if((val1 & 0x0040) == (val2 & 0x0040)) 
        {
            return NOR_SUCCESS;
        }

        if((val1 & 0x0020) != 0x0020)
        {
            status = NOR_ONGOING;
        }

        val1 = *(__IO uint16_t *)(BANK_NOR2_ADDR );
        val2 = *(__IO uint16_t *)(BANK_NOR2_ADDR );

        if((val1 & 0x0040) == (val2 & 0x0040)) 
        {
            return NOR_SUCCESS;
        }
        else if((val1 & 0x0020) == 0x0020)
        {
            return NOR_ERROR;
        }
    }

    if(Timeout == 0x00)
    {
        status = NOR_TIMEOUT;
    } 

    return (status);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
