
#ifdef LOS_FLASH

#include "los_bsp_flash.h"
#include "los_bsp_led.h"
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"


/* Private variables ---------------------------------------------------------*/
/*Variable used for Erase procedure*/
uint32_t FirstSector = 0, NbOfSectors = 0, Address = 0;
uint32_t SectorError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

static FLASH_EraseInitTypeDef EraseInitStruct;
static volatile DSTATUS TM_FATFS_FLASH_SPI_Stat = STA_NOINIT;         /* Physical drive status */
static u8 BackUpData[STM_FLASH_SECTOR_COUNT][STM_FLASH_SECTOR_SIZE] = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Error_Handler(void);
static uint32_t GetSector(uint32_t Address);

/******************************************************************************
* Function Name  : FLASH_ClearFlag
* Description    : Clears the FLASH's pending flags
* Input          : FLASH_FLAG: specifies the FLASH flags to clear.
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
    /* Check the parameters */
    assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;

    /* Clear the flags */
    FLASH->SR = FLASH_FLAG;

    return;
}

/******************************************************************************
* Function Name  : Flash_WritePage
* Description    : This routine is for writing one or several 512 Bytes Page size.
* Input          : pBuffer£ºpointer on the Buffer to read
                   PageNum 
                   NumPageToWrite
* Output         : None
* Return         : STM_FLASH_SUCCESS¡¢STM_FLASH_ERROR
*******************************************************************************/
u32 Flash_WritePage(u8 *pBuffer, u32 PageNum, u32 NumPageToWrite)
{
    HAL_StatusTypeDef status = HAL_OK;
    u32 STMFlashStatus = STM_FLASH_SUCCESS;
    u32 Address;
    u32 index, data, data32;

    /* Unlock the Flash Program Erase controller */
    HAL_FLASH_Unlock();
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

    while( (NumPageToWrite != 0x00) && (STMFlashStatus != STM_FLASH_INVALID_ADDRESS) )
    {
        Address = FLASH_USER_START_ADDR + PageNum*STM_FLASH_SECTOR_SIZE;

        for(index=0; index < STM_FLASH_SECTOR_SIZE; index++)
        {
            data32 = *pBuffer++;

            switch(index%4)
            {
                case 0:
                    data  = ( data32 << 0 ) & 0x000000FF;
                    break;
                case 1:
                    data += ( data32 << 8 ) & 0x0000FF00;
                    break;
                case 2:
                    data += ( data32 << 16) & 0x00FF0000;
                    break;
                case 3:
                    data += ( data32 << 24) & 0xFF000000;
                    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address, data);
                    Address += 4;

                    if(status != HAL_OK)
                    {
                        HAL_FLASH_Lock();
                        return STM_FLASH_ERROR;
                    }

                    break;
                default:
                    break;
            }
        }
        NumPageToWrite--;
        if(NumPageToWrite != 0)
        {
            PageNum++;

            /* Calculate Next page Address */
            if(PageNum >= STM_FLASH_SECTOR_TOTAL)
            STMFlashStatus = STM_FLASH_INVALID_ADDRESS;
        }
    }

    HAL_FLASH_Lock();

    return STMFlashStatus;
}

/******************************************************************************
* Function Name  : Flash_ReadPage
* Description    : This routine is for sequential read from one or several
* Input          : - PageNum£ºform which Page to read
                   - NumPageToRead£ºread Page num 
* Output         : - pBuffer: pointer on the Buffer to fill
* Return         : New status of the NAND operation. This parameter can be:
*******************************************************************************/
u32 Flash_ReadPage(u8 *pBuffer, u32 PageNum, u32 NumPageToRead)
{
    u32 STMFlashStatus = STM_FLASH_SUCCESS;
    u32 Address;
    u32 index, data;
    u8 tmpdata = 0x00;
    
    /* Unlock the Flash Program Erase controller */
    HAL_FLASH_Unlock();
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    
    while( (NumPageToRead != 0x00) && (STMFlashStatus != STM_FLASH_INVALID_ADDRESS) )
    {
        Address = FLASH_USER_START_ADDR + PageNum*STM_FLASH_SECTOR_SIZE;

        for(index=0; index < STM_FLASH_SECTOR_SIZE; index++)
        {
            switch(index%4)
            {
                case 0:
                    data = *(u32*)Address;
                    Address += 4;

                    tmpdata = DATA_1st(data);
                    *pBuffer = tmpdata;
                    pBuffer++;
                    break;
                case 1:
                    tmpdata = DATA_2nd(data);
                    *pBuffer = tmpdata;
                    pBuffer++;
                    break;
                case 2:
                    tmpdata = DATA_3rd(data);
                    *pBuffer = tmpdata;
                    pBuffer++;
                    break;
                case 3:
                    tmpdata = DATA_4th(data);
                    *pBuffer = tmpdata;
                    pBuffer++;
                    break;
                default:
                    break;
            }
        }
        NumPageToRead--;
        if(NumPageToRead != 0)
        {
            PageNum++;

            /* Calculate Next page Address */
            if(PageNum >= STM_FLASH_SECTOR_TOTAL)
            STMFlashStatus = STM_FLASH_INVALID_ADDRESS;
        }
    }
    HAL_FLASH_Lock();
    return STMFlashStatus;
}

/******************************************************************************
* Function Name  : Flash_EraseBlock
* Description    : Erase Block before write
* Input          : - BlockNum£ºform which Page to read
* Output         : None
* Return         : STM_FLASH_ERROR/STM_FLASH_SUCCESS
*******************************************************************************/
u32 Flash_EraseBlock(u32 BlockNum)
{
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Erase the user Flash area
        (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    /* Get the 1st sector to erase */
    FirstSector = GetSector(FLASH_USER_START_ADDR);

    /* Get the number of sector to erase from 1st sector*/
    NbOfSectors = 1;

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector + BlockNum;
    EraseInitStruct.NbSectors = NbOfSectors;
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
        /*
            Error occurred while sector erase. 
            User can add here some code to deal with this error. 
            SectorError will contain the faulty sector and then to know the code error on this sector,
            user can call function 'HAL_FLASH_GetError()'
        */
        /*
            FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
        */
        Error_Handler();
        return STM_FLASH_ERROR;
    }

    /*
        Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
        you have to make sure that these data are rewritten before they are accessed during code
        execution. If this cannot be done safely, it is recommended to flush the caches by setting the
        DCRST and ICRST bits in the FLASH_CR register. */

    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();

    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();

    return STM_FLASH_SUCCESS;
}

/*******************************************************************************
* Function name : Fill_Buffer
* Description   : Fill the buffer
* Input         : - pBuffer: pointer on the Buffer to fill
*                 - BufferSize: size of the buffer to fill
*                 - Offset: first value to fill on the Buffer
* Output param  : None
*******************************************************************************/
void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset)
{
    u16 IndexTmp = 0;

    /* Put in global buffer same values */
    for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++)
    {
        pBuffer[IndexTmp] = IndexTmp + Offset;
    }
}

/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_initialize
* Description    : Initializes the peripherals used by the FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
DSTATUS TM_FATFS_FLASH_disk_initialize(void)
{
    return RES_OK;
}

/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_status
* Description    : Get disk status.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
DSTATUS TM_FATFS_FLASH_disk_status(void)
{
    return RES_OK;
}

/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_ioctl
* Description    : detail disk iocotl cmd
* Input          : cmd
* Output         : buff
* Return         : RES_OK/RES_ERROR
*******************************************************************************/
DRESULT TM_FATFS_FLASH_disk_ioctl(BYTE cmd, char *buff)
{
    switch (cmd) 
    {
        case GET_SECTOR_SIZE:                    // Get R/W sector size (WORD)
            *(WORD * )buff = STM_FLASH_SECTOR_SIZE; // flash read/write unit 4096
            break;
        case GET_SECTOR_COUNT:
            *(DWORD * )buff = STM_FLASH_SECTOR_TOTAL;   // sector num
            break;
        case GET_BLOCK_SIZE:                         // Get erase block size in unit of sector (DWORD)
            *(DWORD * )buff = STM_FLASH_SECTOR_COUNT;       // Erase unit
            break;
        case CTRL_SYNC:
            break;
        default:
            return RES_ERROR;
    }

    return RES_OK;
}

/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_read
* Description    : Read sector(s)
* Input          : DWORD sector
                   UINT count
* Output         : BYTE *buff
* Return         : RES_OK/RES_ERROR
*******************************************************************************/
DRESULT TM_FATFS_FLASH_disk_read(BYTE *buff, DWORD sector, UINT count)
{
    u32 result;

    if (!count)return RES_PARERR;

    result = Flash_ReadPage(buff, sector, count);

    if(result == STM_FLASH_SUCCESS)
        return RES_OK;
    else
        return RES_ERROR;
}

/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_write
* Description    : Write sector(s)
* Input          : DWORD sector
                   UINT count
                   BYTE *buff
* Output         : None
* Return         : RES_OK/RES_ERROR
*******************************************************************************/
DRESULT TM_FATFS_FLASH_disk_write(BYTE *buff, DWORD sector, UINT count)
{
    u32 WriteBlockNum,ulRet = STM_FLASH_SUCCESS;
    u16 IndexTmp, OffsetPage;
    
    if(!count)return RES_PARERR;

    //data in witch Block
    WriteBlockNum = sector/STM_FLASH_SECTOR_COUNT;
    //data Offset Page of current Block
    OffsetPage = sector%STM_FLASH_SECTOR_COUNT;
    //Copy data to backup
    ulRet = Flash_ReadPage((u8*)(&BackUpData[0][0]), WriteBlockNum*STM_FLASH_SECTOR_COUNT, STM_FLASH_SECTOR_COUNT);
    if(STM_FLASH_SUCCESS != ulRet)
    {
        return RES_ERROR;
    }
    //Erase block data
    ulRet = Flash_EraseBlock(WriteBlockNum);
    if(STM_FLASH_SUCCESS != ulRet)
    {
        return RES_ERROR;
    }
    // write data
    for (IndexTmp = 0; IndexTmp < STM_FLASH_SECTOR_COUNT; IndexTmp++ )
    {
        if((IndexTmp>=OffsetPage)&&(IndexTmp < (OffsetPage+count)))
        {
            ulRet = Flash_WritePage((u8 *)buff, WriteBlockNum*STM_FLASH_SECTOR_COUNT+IndexTmp, 1);
            if(STM_FLASH_SUCCESS != ulRet)
            {
                return RES_ERROR;
            }
            buff = (u8 *)buff + STM_FLASH_SECTOR_SIZE;
        }
        else
        {
            //write backup data to flash
            ulRet = Flash_WritePage((u8 *)(&BackUpData[IndexTmp][0]), WriteBlockNum*STM_FLASH_SECTOR_COUNT+IndexTmp, 1);
            if(STM_FLASH_SUCCESS != ulRet)
            {
                return RES_ERROR;
            }
        }
    }
    return RES_OK;
}


/**
  * @brief  Gets sector Size
  * @param  None
  * @retval The size of a given sector
  */
uint32_t GetSectorSize(uint32_t Sector)
{
    uint32_t sectorsize = 0x00;
    if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) ||\
     (Sector == FLASH_SECTOR_3) || (Sector == FLASH_SECTOR_12) || (Sector == FLASH_SECTOR_13) ||\
     (Sector == FLASH_SECTOR_14) || (Sector == FLASH_SECTOR_15))
    {
        sectorsize = 16 * 1024;
    }
    else if((Sector == FLASH_SECTOR_4) || (Sector == FLASH_SECTOR_16))
    {
        sectorsize = 64 * 1024;
    }
    else
    {
        sectorsize = 128 * 1024;
    }
    return sectorsize;
}

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_SECTOR_7;
    }
    else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_SECTOR_8;
    }
    else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_SECTOR_9;
    }
    else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_SECTOR_10;
    }
    else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
    {
        sector = FLASH_SECTOR_11;
    }
    else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
    {
        sector = FLASH_SECTOR_12;
    }
    else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
    {
        sector = FLASH_SECTOR_13;
    }
    else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
    {
        sector = FLASH_SECTOR_14;
    }
    else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
    {
        sector = FLASH_SECTOR_15;
    }
    else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
    {
        sector = FLASH_SECTOR_16;
    }
    else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
    {
        sector = FLASH_SECTOR_17;
    }
    else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
    {
        sector = FLASH_SECTOR_18;
    }
    else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
    {
        sector = FLASH_SECTOR_19;
    }
    else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
    {
        sector = FLASH_SECTOR_20;
    } 
    else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
    {
        sector = FLASH_SECTOR_21;
    }
    else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
    {
        sector = FLASH_SECTOR_22;
    }
    else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
    {
        sector = FLASH_SECTOR_23;
    }

    return sector;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* Turn LED2 on */
    LOS_EvbLedControl(LOS_LED2, LED_ON);
}

#endif
