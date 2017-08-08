/**
  ******************************************************************************
  * @file    EXMC/EXMC_NAND/exmc_nandflash.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The file of the read and write nand flash file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "exmc_nandflash.h"

/* Exported macro ------------------------------------------------------------*/
/* Defines the physical address of nand flash, and it is determined by the hardware */
#define BANK2_NAND_ADDR    ((uint32_t)0x70000000)
#define BANK_NAND_ADDR     BANK2_NAND_ADDR

/* Define operating nand flash macro */
#define NAND_CMD_AREA      *(__IO uint8_t *)(BANK_NAND_ADDR | EXMC_CMD_AREA)
#define NAND_ADDR_AREA     *(__IO uint8_t *)(BANK_NAND_ADDR | EXMC_ADDR_AREA)
#define NAND_DATA_AREA     *(__IO uint8_t *)(BANK_NAND_ADDR | EXMC_DATA_AREA)

/* The macro of calculate nand flash operating address */
#define ROW_ADDRESS        (Address.Page + (Address.Block + (Address.Zone * NAND_ZONE_SIZE)) * NAND_BLOCK_SIZE)

/* Page bit count per block */
#define PAGE_BIT           6

/* Private function prototypes -----------------------------------------------*/
static uint8_t EXMC_NAND_GetStatus(void);
static uint8_t EXMC_NAND_WriteData(uint8_t *pBuffer, NAND_ADDRESS PhysicalAddress, uint16_t ByteCount);
static uint8_t EXMC_NAND_ReadData(uint8_t *pBuffer, NAND_ADDRESS PhyAddress, uint16_t ByteCount);
static uint8_t EXMC_NAND_WritePage(uint8_t *pBuffer, NAND_ADDRESS Address, uint16_t ByteCount);
static uint8_t EXMC_NAND_ReadPage(uint8_t *pBuffer, NAND_ADDRESS  Address, uint16_t ByteCount);
uint8_t EXMC_NAND_ReadSpare(uint8_t *pBuffer,NAND_ADDRESS  Address, uint16_t ByteCount);
uint8_t EXMC_NAND_WriteSpare(uint8_t *pBuffer, NAND_ADDRESS Address, uint16_t ByteCount);
static uint8_t EXMC_NAND_EraseBlock(uint32_t BlockNum);
static uint8_t EXMC_NAND_ReadStatus(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure EXMC and GPIO interface for nand flash.
  * @param  None
  * @retval None
  */
void NAND_Init(void)
{
    GPIO_InitPara GPIO_InitStructure;
    EXMC_NANDInitPara EXMC_NANDInitStructure;
    EXMC_NAND_PCCARDTimingInitPara  p;

    /* Enable EXMC clock*/
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_EXMC, ENABLE);

    /* Enable GPIOD¡¢GPIOE¡¢GPIOF¡¢GPIOG clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOD | RCC_APB2PERIPH_GPIOE |
                               RCC_APB2PERIPH_GPIOF | RCC_APB2PERIPH_GPIOG, ENABLE);

    /* EXMC GPIO configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15 |
                                   GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 

    /* D4-D7 pins configured as an alternate function */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* INT2 pins configured for internal input, used in the busy signal */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IPU;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* EXMC configuration */
    p.EXMC_SetupTime = 0x1;
    p.EXMC_WaitTime = 0x3;
    p.EXMC_HoldTime = 0x2;
    p.EXMC_DatabusHiZTime = 0x1;

    EXMC_NANDInitStructure.EXMC_NANDBank = EXMC_BANK2_NAND;
    EXMC_NANDInitStructure.EXMC_WaitFeature = EXMC_WAIT_FEATURE_ENABLE;
    EXMC_NANDInitStructure.EXMC_DatabusWidth = EXMC_DATABUS_WIDTH_8B;
    EXMC_NANDInitStructure.EXMC_ECCLogic = EXMC_ECC_LOGIC_ENABLE;
    EXMC_NANDInitStructure.EXMC_ECCSize = EXMC_ECC_SIZE_2048BYTES;
    EXMC_NANDInitStructure.EXMC_CTRLatency = 0x00;
    EXMC_NANDInitStructure.EXMC_ATRLatency = 0x00;
    EXMC_NANDInitStructure.EXMC_CommonSpaceTimingParaStruct = &p;
    EXMC_NANDInitStructure.EXMC_AttributeSpaceTimingParaStruct = &p;

    EXMC_NAND_Init(&EXMC_NANDInitStructure);

    /* Enable EXMC NAND Bank2 */
    EXMC_NAND_Enable(EXMC_BANK2_NAND, ENABLE);
}

/**
  * @brief  Read nand flash ID.
  * @param  NAND_ID: Structure of nand flash ID.
  * @retval None
  */
void NAND_ReadID(NAND_IDTypeDef* NAND_ID)
{
    uint32_t data = 0;

    /* Send Command to the command area */
    NAND_CMD_AREA = NAND_CMD_READID;

    /* Send address to the address area */
    NAND_ADDR_AREA = 0x00;

    /* Read ID from NAND flash */
    data = *(__IO uint32_t *)(BANK_NAND_ADDR | EXMC_DATA_AREA);

    NAND_ID->Maker_ID   = ADDR_1ST_CYCLE (data);
    NAND_ID->Device_ID  = ADDR_2ND_CYCLE (data);
    NAND_ID->Third_ID   = ADDR_3RD_CYCLE (data);
    NAND_ID->Fourth_ID  = ADDR_4TH_CYCLE (data);
}

/**
  * @brief  Write a set of data to nand flash for the specified pages addresses.
  * @param  pBuffer: Pointer on the buffer containing data to be written.
  * @param  Address: The address of the data to be written.
  * @param  ByteCount: Byte count to be written(ByteCount + Address.PageInOffset <= NAND_PAGE_TOTAL_SIZE).
  * @retval NAND_OK: Write data successfully.
  *         NAND_FAIL: Write data failure.
  */
static uint8_t EXMC_NAND_WritePage(uint8_t *pBuffer, NAND_ADDRESS Address, uint16_t ByteCount)
{
    uint16_t i;

    /* Send 1st cycle page programming command to the command area */
    NAND_CMD_AREA = NAND_CMD_WRITE_1ST;

    /* Send address to the address area£¬ for HY27UF081G2A
                    Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
       First byte:  A7   A6   A5   A4   A3   A2   A1   A0    (it7 - bit0 of page address)
       Second byte: 0    0    0    0    A11  A10  A9   A8    (bit11 - bit8 of page address, high 4bit must be zero)
       Third byte£º A19  A18  A17  A16  A15  A14  A13  A12
       Fourth byte£ºA27  A26  A25  A24  A23  A22  A21  A20
    */
    NAND_ADDR_AREA = Address.PageInOffset;
    NAND_ADDR_AREA = Address.PageInOffset >> 8;
    NAND_ADDR_AREA = ADDR_1ST_CYCLE(ROW_ADDRESS);
    NAND_ADDR_AREA = ADDR_2ND_CYCLE(ROW_ADDRESS);

    /* Write data to data area */
    for(i = 0; i < ByteCount; i++)
    {
        NAND_DATA_AREA = pBuffer[i];
    }

    /* Send 2nd cycle page programming command to the command area */
    NAND_CMD_AREA = NAND_CMD_WRITE_2ND;

    /* Check operation stauts */
    if (EXMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }
    return NAND_FAIL;
}

/**
  * @brief  Read a set of data from nand flash for the specified pages addresses.
  * @param  pBuffer: Pointer on the buffer filling data to be read.
  * @param  Address: The address of the data to be read.
  * @param  ByteCount: Byte count to be read(ByteCount + Address.PageInOffset <= NAND_PAGE_TOTAL_SIZE).
  * @retval NAND_OK: Read data successfully.
  *         NAND_FAIL: Read data failure.
  */
static uint8_t EXMC_NAND_ReadPage(uint8_t *pBuffer, NAND_ADDRESS Address, uint16_t ByteCount)
{
    uint16_t i;

    /* Send 1st cycle read command to the command area */
    NAND_CMD_AREA = NAND_CMD_READ1_1ST;

     /* Send address to the address area£¬for HY27UF081G2A
                     Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
        First byte:  A7   A6   A5   A4   A3   A2   A1   A0    (it7 - bit0 of page address)
        Second byte: 0    0    0    0    A11  A10  A9   A8    (bit11 - bit8 of page address, high 4bit must be zero)
        Third byte£º A19  A18  A17  A16  A15  A14  A13  A12
        Fourth byte£ºA27  A26  A25  A24  A23  A22  A21  A20
    */
    NAND_ADDR_AREA = Address.PageInOffset;
    NAND_ADDR_AREA = Address.PageInOffset >> 8;
    NAND_ADDR_AREA = ADDR_1ST_CYCLE(ROW_ADDRESS);
    NAND_ADDR_AREA = ADDR_2ND_CYCLE(ROW_ADDRESS);
    
    /* Send 2nd cycle read command to the command area */
    NAND_CMD_AREA = NAND_CMD_READ1_2ND;
        
    /* Read data to pBuffer */
    for(i = 0; i < ByteCount; i++)
    {
        pBuffer[i] = NAND_DATA_AREA;
    }

    return NAND_OK;
}

/**
  * @brief  Write the spare area information for the specified pages addresses.
  * @param  pBuffer: Pointer on the buffer containing data to be written.
  * @param  Address: The address of the data to be written.
  * @param  ByteCount: Byte count to be written(ByteCount + (Address.PageInOffset - NAND_PAGE_SIZE) <= NAND_SPARE_AREA_SIZE).
  * @retval NAND_OK: Write data successfully.
  *         NAND_FAIL: Write data failure.
  */
uint8_t FSMC_NAND_WriteSpare(uint8_t *pBuffer, NAND_ADDRESS Address, uint16_t ByteCount)
{
    /* Address.PageInOffset > NAND_PAGE_SIZE */
    if(Address.PageInOffset <= NAND_PAGE_SIZE)
    {
        return NAND_FAIL;
    }

    /* ByteCount + Address.PageInOffset < NAND_PAGE_TOTAL_SIZE */
    if (ByteCount + Address.PageInOffset >= NAND_PAGE_TOTAL_SIZE)
    {
        return NAND_FAIL;
    }

    /* Write spare area */
    return EXMC_NAND_WritePage(pBuffer, Address, ByteCount);
}

/**
  * @brief  Read the spare area information for the specified pages addresses.
  * @param  pBuffer: Pointer on the buffer containing data to be read.
  * @param  Address: The address of the data to be read.
  * @param  ByteCount: Byte count to be read(ByteCount + (Address.PageInOffset - NAND_PAGE_SIZE) <= NAND_SPARE_AREA_SIZE).
  * @retval NAND_OK: Read data successfully.
  *         NAND_FAIL: Read data failure.
  */
uint8_t EXMC_NAND_ReadSpare(uint8_t *_pBuffer,NAND_ADDRESS Address, uint16_t _usByteCount)
{
    /* Address.PageInOffset > NAND_PAGE_SIZE */
    if(Address.PageInOffset <= NAND_PAGE_SIZE)
    {
        return NAND_FAIL;
    }

    /* ByteCount + Address.PageInOffset < NAND_PAGE_TOTAL_SIZE */
    if (_usByteCount + Address.PageInOffset >= NAND_PAGE_TOTAL_SIZE)
    {
        return NAND_FAIL;
    }

    /* Read spare area */
    return EXMC_NAND_ReadPage(_pBuffer,Address, _usByteCount);
}

/**
  * @brief  Write the main area information for the specified pages addresses.
  *         If the number of data bytes to be written plus the offset is greater than the page size,
  *         the automatic next page.
  * @param  pBuffer: Pointer on the buffer containing data to be written.
  * @param  Address: The address of the data to be written.
  * @param  ByteCount: Byte count to be written.
  * @retval NAND_OK: Write data successfully.
  *         NAND_FAIL: Write data failure.
  */
static uint8_t EXMC_NAND_WriteData(uint8_t *pBuffer, NAND_ADDRESS PhysicalAddress, uint16_t ByteCount)
{
    uint8_t *temp_pBuffer = pBuffer;

    /* Erase block before write data based on the feature of nand flash*/
    EXMC_NAND_EraseBlock(PhysicalAddress.Block);

    /* If the number of data bytes to be written plus the offset is greater than the page size,
       the automatic next page. */
    while(ByteCount + PhysicalAddress.PageInOffset > NAND_PAGE_SIZE)
    {
        if(EXMC_NAND_WritePage(temp_pBuffer, PhysicalAddress, NAND_PAGE_SIZE-PhysicalAddress.PageInOffset) != NAND_OK)
        {
            return NAND_FAIL;
        }
        /* Compute address of the next block */
        ByteCount-=NAND_PAGE_SIZE-PhysicalAddress.PageInOffset;
        temp_pBuffer+=NAND_PAGE_SIZE-PhysicalAddress.PageInOffset;
        PhysicalAddress.Page++;
        PhysicalAddress.PageInOffset=0;
    }

    /* Write the last less than one block of data */
    if(ByteCount > 0)
    {
        if(EXMC_NAND_WritePage(temp_pBuffer, PhysicalAddress,ByteCount) != NAND_OK)
        {
            return NAND_FAIL;
        }
    }

    return NAND_OK;
}

/**
  * @brief  Read the main area information for the specified pages addresses.
  *         If the number of data bytes to be read plus the offset is greater than the page size, 
  *         the automatic next page.
  * @param  pBuffer: Pointer on the buffer containing data to be read.
  * @param  Address: The address of the data to be read.
  * @param  ByteCount: Byte count to be read.
  * @retval NAND_OK: Read data successfully.
  *         NAND_FAIL: Read data failure.
  */
static uint8_t EXMC_NAND_ReadData(uint8_t *pBuffer, NAND_ADDRESS PhyAddress, uint16_t ByteCount)
{
    uint8_t *temp_pBuffer = pBuffer;
    /* If the number of data bytes to be read plus the offset is greater than the page size,
       the automatic next page. */
    while(ByteCount + PhyAddress.PageInOffset > NAND_PAGE_SIZE)
    {
        if(EXMC_NAND_ReadPage(temp_pBuffer, PhyAddress, NAND_PAGE_SIZE - PhyAddress.PageInOffset) != NAND_OK)
        {
            return NAND_FAIL;
        }
        PhyAddress.Page++;
        temp_pBuffer+=NAND_PAGE_SIZE - PhyAddress.PageInOffset;
        ByteCount-=NAND_PAGE_SIZE - PhyAddress.PageInOffset;
        PhyAddress.PageInOffset=0;
    }

    if(ByteCount>0)
    {
        if(EXMC_NAND_ReadPage(temp_pBuffer, PhyAddress, ByteCount) != NAND_OK)
        {
            return NAND_FAIL;
        }
    }

      return NAND_OK;
}

/**
  * @brief  Erase data specified block.
  * @param  BlockNum: Block number to be erased data.
  * @retval NAND memory status.
  */
static uint8_t EXMC_NAND_EraseBlock(uint32_t BlockNum)
{
    /* Send 1st cycle erase command to command area */
    NAND_CMD_AREA = NAND_CMD_ERASE_1ST;
        
    /* Block number into a block number and the page number */
    BlockNum <<= PAGE_BIT;
    NAND_ADDR_AREA = ADDR_1ST_CYCLE(BlockNum);
    NAND_ADDR_AREA = ADDR_2ND_CYCLE(BlockNum);

    /* Send 2nd cycle erase command to command area */
    NAND_CMD_AREA = NAND_CMD_ERASE_2ND;

    return (EXMC_NAND_GetStatus());
}

/**
  * @brief  Reset nand flash.
  * @param  None.
  * @retval NAND_OK: Reset nand flash successfully.
  *         NAND_FAIL: Reset nand flash failure.
  */
uint8_t NAND_Reset(void)
{
    NAND_CMD_AREA = NAND_CMD_RESET;

    /* Check operation stauts */
    if (EXMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }

    return NAND_FAIL;
}

/**
  * @brief  Reads the NAND memory status. 
  * @param  None
  * @retval NAND memory status..
  */
static uint8_t EXMC_NAND_ReadStatus(void)
{
    uint8_t ucData;
    uint8_t ucStatus = NAND_BUSY;

    /* Send read status command to the command area */
    NAND_CMD_AREA = NAND_CMD_STATUS;

    ucData = NAND_DATA_AREA;

    if((ucData & NAND_ERROR) == NAND_ERROR)
    {
        ucStatus = NAND_ERROR;
    } 
    else if((ucData & NAND_READY) == NAND_READY)
    {
        ucStatus = NAND_READY;
    }
    else
    {
        ucStatus = NAND_BUSY; 
    }

    return (ucStatus);
}

/**
  * @brief  Get the NAND operation status.
  * @param  None
  * @retval New status of the NAND operation. 
  */
static uint8_t EXMC_NAND_GetStatus(void)
{
    uint32_t ulTimeout = 0x10000;
    uint8_t ucStatus = NAND_READY;

    ucStatus = EXMC_NAND_ReadStatus();

    /* Waiting for Nand operation over, it will exit after a timeout. */
    while ((ucStatus != NAND_READY) &&( ulTimeout != 0x00))
    {
        ucStatus = EXMC_NAND_ReadStatus();
        ulTimeout--;
    }

    if(ulTimeout == 0x00)
    {
        ucStatus =  NAND_TIMEOUT_ERROR;
    } 

    return (ucStatus);
}

/**
  * @brief  Write the main area information for the specified logic addresses.
  *         If the number of data bytes to be written plus the offset is greater than the block size, 
  *         the automatic next block.
  * @param  MemAddr: The logic address of the data to be written.
  * @param  pWriteBuf: Pointer on the buffer containing data to be written.
  * @param  ByteCount: Byte count to be written.
  * @retval NAND_OK: Write data successfully.
  *         NAND_FAIL: Write data failure.
  */
uint8_t NAND_Write(uint32_t MemAddr, uint8_t *pWriteBuf, uint16_t ByteCount)
{
    uint32_t temp_BlockRemainSize;
    NAND_ADDRESS  PhysicalAddress;
    uint32_t  ulTemp;

    ulTemp = MemAddr % (NAND_BLOCK_SIZE * NAND_PAGE_SIZE);
    
    /* compute physical zone number */
    PhysicalAddress.Zone= MemAddr / (NAND_BLOCK_SIZE * NAND_PAGE_SIZE * NAND_ZONE_SIZE);

    /* compute physical block number */
    PhysicalAddress.Block = MemAddr / (NAND_BLOCK_SIZE * NAND_PAGE_SIZE);

    /* compute physical page number */
    PhysicalAddress.Page = ulTemp / NAND_PAGE_SIZE;

    /* compute physical offset into page  */
    PhysicalAddress.PageInOffset = ulTemp % NAND_PAGE_SIZE;
    temp_BlockRemainSize=(NAND_BLOCK_SIZE * NAND_PAGE_SIZE)-(NAND_PAGE_SIZE * PhysicalAddress.Page + 
                          PhysicalAddress.PageInOffset);

    /* If the number of data bytes to be written plus the offset is greater than the block size, 
       the automatic next block. */
    while(ByteCount>temp_BlockRemainSize)
    {
        if(EXMC_NAND_WriteData(pWriteBuf,PhysicalAddress,temp_BlockRemainSize)== NAND_FAIL)
        {
            return NAND_FAIL;
        }
        PhysicalAddress.Block++;
        pWriteBuf+=temp_BlockRemainSize;
        ByteCount-=temp_BlockRemainSize;
        PhysicalAddress.Page=0;
        PhysicalAddress.PageInOffset=0;
        temp_BlockRemainSize=(NAND_BLOCK_SIZE * NAND_PAGE_SIZE);
    }

    if(ByteCount>0)
    {
        if(EXMC_NAND_WriteData(pWriteBuf, PhysicalAddress, ByteCount)== NAND_FAIL)
        {
            return NAND_FAIL;
        }
    }

    return NAND_OK;
}

/**
  * @brief  Read the main area information for the specified logic addresses.
  *         If the number of data bytes to be read plus the offset is greater than the block size, 
  *         the automatic next block.
  * @param  MemAddr: The logic address of the data to be read.
  * @param  pWriteBuf: Pointer on the buffer containing data to be read.
  * @param  ByteCount: Byte count to be reas.
  * @retval NAND_OK: Read data successfully.
  *         NAND_FAIL: Read data failure.
  */
uint8_t NAND_Read(uint32_t MemAddr, uint8_t *pReadBuf, uint16_t ByteCount)
{
    uint32_t temp_BlockRemainSize;
    NAND_ADDRESS  PhysicalAddress;
    uint32_t  ulTemp;

    ulTemp = MemAddr % (NAND_BLOCK_SIZE * NAND_PAGE_SIZE);

    /* compute physical zone number */
    PhysicalAddress.Zone= MemAddr / (NAND_BLOCK_SIZE * NAND_PAGE_SIZE * NAND_ZONE_SIZE); 

    /* compute physical block number */
    PhysicalAddress.Block = MemAddr / (NAND_BLOCK_SIZE * NAND_PAGE_SIZE);
      
      /* compute physical page number */
    PhysicalAddress.Page = ulTemp / NAND_PAGE_SIZE;

    /* compute physical offset into page  */
    PhysicalAddress.PageInOffset = ulTemp % NAND_PAGE_SIZE;

    temp_BlockRemainSize=(NAND_BLOCK_SIZE * NAND_PAGE_SIZE)-(NAND_PAGE_SIZE * PhysicalAddress.Page +
                          PhysicalAddress.PageInOffset);

    /* If the number of data bytes to be read plus the offset is greater than the block size,
       the automatic next block. */
    while(ByteCount>temp_BlockRemainSize)
    {
        if(EXMC_NAND_ReadData(pReadBuf,PhysicalAddress,temp_BlockRemainSize)== NAND_FAIL)
        {
            return NAND_FAIL;
        }
        PhysicalAddress.Block++;
        pReadBuf+=temp_BlockRemainSize;
        ByteCount-=temp_BlockRemainSize;
        PhysicalAddress.Page=0;
        PhysicalAddress.PageInOffset=0;
        temp_BlockRemainSize=(NAND_BLOCK_SIZE * NAND_PAGE_SIZE);
    }

    if(ByteCount > 0)
    {
        if(EXMC_NAND_ReadData(pReadBuf,PhysicalAddress, ByteCount)== NAND_FAIL)
        {
            return NAND_FAIL;
        }
    }

    return NAND_OK;
}

/**
  * @brief  Format nand flash.
  * @param  None.
  * @retval NAND_OK: Format nand flash successfully.
  *         NAND_FAIL: Format nand flash failure.
  */
uint8_t NAND_Format(void)
{
    uint16_t i;

    for (i = 0; i < NAND_BLOCK_COUNT; i++)
    {
        if(EXMC_NAND_EraseBlock(i) !=NAND_READY)
        {
            return NAND_FAIL;
        }
    }
    
    return NAND_OK;
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
