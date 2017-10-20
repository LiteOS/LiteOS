#include <stdio.h>
#include "los_bsp_flash.h"
#include "los_bsp_uart.h"
#include "los_bsp_led.h"

#ifdef STM32L476xx
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
uint32_t Address = 0, PAGEError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

/* Private function prototypes -----------------------------------------------*/
static volatile DSTATUS TM_FATFS_FLASH_SPI_Stat = STA_NOINIT;         /* Physical drive status */
static u8 BackUpData[STM_FLASH_BLOCK_COUNT][STM_FLASH_SECTOR_SIZE] = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//static void Error_Handler(void);
static uint32_t GetPage(uint32_t Address);
static uint32_t GetBank(uint32_t Address);
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
    assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG));

    /* Clear the flags */
    FLASH->SR = FLASH_FLAG;

    return;
}

/******************************************************************************
* Function Name  : Flash_WriteSector
* Description    : This routine is for writing one or several 512 Bytes Page size.
* Input          : pBuffer：pointer on the Buffer to read
                   uwSector
                   ulNumSectorWrite
* Output         : None
* Return         : STM_FLASH_SUCCESS、STM_FLASH_ERROR
*******************************************************************************/
u32 Flash_WriteSector(u8 *pBuffer, DWORD uwSector, u32 ulNumSectorWrite)
{
    HAL_StatusTypeDef status = HAL_OK;
    u32 STMFlashStatus = STM_FLASH_SUCCESS;
    u32 Address;
    u32 index, data, data32;

    /* Unlock the Flash Program Erase controller */
    HAL_FLASH_Unlock();
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

    while( (ulNumSectorWrite != 0x00) && (STMFlashStatus != STM_FLASH_INVALID_ADDRESS) )
    {
        Address = FLASH_USER_START_ADDR + uwSector*STM_FLASH_SECTOR_SIZE;

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
                    if(data != 0)
                    {
                        data = data;
                    }
                    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,Address, data);
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
        ulNumSectorWrite--;
        if(ulNumSectorWrite != 0)
        {
            uwSector++;

            /* Calculate Next Sector Address */
            if(uwSector >= STM_FLASH_SECTOR_TOTAL)
                STMFlashStatus = STM_FLASH_INVALID_ADDRESS;
        }
    }

    HAL_FLASH_Lock();

    return STMFlashStatus;
}

/******************************************************************************
* Function Name  : Flash_ReadSector
* Description    : This routine is for sequential read from one or several
* Input          : - uwSector：form which Sector to read
                   - ulNumSectorRead：read Sector num 
* Output         : - pBuffer: pointer on the Buffer to fill
* Return         : New status of the NAND operation. This parameter can be:
*******************************************************************************/
u32 Flash_ReadSector(u8 *pBuffer, DWORD uwSector, u32 ulNumSectorRead)
{
    u32 STMFlashStatus = STM_FLASH_SUCCESS;
    u32 Address;
    u32 index, data;
    u8 tmpdata = 0x00;

    /* Unlock the Flash Program Erase controller */
    HAL_FLASH_Unlock();
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

    while( (ulNumSectorRead != 0x00) && (STMFlashStatus != STM_FLASH_INVALID_ADDRESS) )
    {
        Address = FLASH_USER_START_ADDR + + uwSector*STM_FLASH_SECTOR_SIZE;

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
        ulNumSectorRead--;
        if(ulNumSectorRead != 0)
        {
            uwSector++;
            
            /* Calculate Next Sector Address */
            if(uwSector >= STM_FLASH_SECTOR_TOTAL)
                STMFlashStatus = STM_FLASH_INVALID_ADDRESS;
        }
    }
    HAL_FLASH_Lock();
    return STMFlashStatus;
}

/******************************************************************************
* Function Name  : Flash_EraseBlock
* Description    : Erase Block before write
* Input          : - BlockNum：form which Page to read
* Output         : None
* Return         : STM_FLASH_ERROR/STM_FLASH_SUCCESS
*******************************************************************************/
u32 Flash_EraseBlock(u32 BlockNum)
{
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    /* Clear OPTVERR bit set on virgin samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 
    /* Get the 1st page to erase */
    FirstPage = GetPage(FLASH_USER_START_ADDR);
    /* Get the number of pages to erase */
    NbOfPages = 1;
    /* Get the bank */
    BankNumber = GetBank(FLASH_USER_START_ADDR);
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks       = BankNumber;
    EraseInitStruct.Page        = FirstPage + BlockNum;
    EraseInitStruct.NbPages     = NbOfPages;

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        /*
          Error occurred while page erase.
          User can add here some code to deal with this error.
          PAGEError will contain the faulty page and then to know the code error on this page,
          user can call function 'HAL_FLASH_GetError()'
        */
        /* Infinite loop */
        while (1)
        {
            /* Make LED2 blink (100ms on, 2s off) to indicate error in Erase operation */
            BSP_LED_On(LED2);
            HAL_Delay(100);
            BSP_LED_Off(LED2);
            HAL_Delay(2000);
        }
    }
    
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
    for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
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
        case GET_SECTOR_SIZE:                         // Get R/W sector size (WORD)
            *(WORD * )buff = STM_FLASH_SECTOR_SIZE;   // flash read/write unit 512
            break;
        case GET_SECTOR_COUNT:
            *(DWORD * )buff = STM_FLASH_SECTOR_TOTAL; // sector num
            break;
        case GET_BLOCK_SIZE:                          // Get erase block size in unit of sector (DWORD)
            *(DWORD * )buff = STM_FLASH_BLOCK_COUNT;  // Erase unit
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

    result = Flash_ReadSector(buff, sector, count);

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

    /*此函数实现的是STM_FLASH_SECTOR_SIZE比STM32_PAGE_SIZE小的情况*/
    if(STM_FLASH_SECTOR_SIZE > STM32_PAGE_SIZE)
    return RES_PARERR;

    //data in witch Block
    WriteBlockNum = sector/STM_FLASH_BLOCK_COUNT;
    //data Offset Page of current Block
    OffsetPage = sector%STM_FLASH_BLOCK_COUNT;
    //Copy data to backup
    ulRet = Flash_ReadSector((u8*)(&BackUpData[0][0]), WriteBlockNum*STM_FLASH_BLOCK_COUNT, STM_FLASH_BLOCK_COUNT);
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
    for (IndexTmp = 0; IndexTmp < STM_FLASH_BLOCK_COUNT; IndexTmp++ )
    {
        if((IndexTmp>=OffsetPage)&&(IndexTmp < (OffsetPage+count)))
        {
            ulRet = Flash_WriteSector((u8 *)buff, WriteBlockNum*STM_FLASH_BLOCK_COUNT+IndexTmp, 1);
            if(STM_FLASH_SUCCESS != ulRet)
            {
                return RES_ERROR;
            }
            buff = (u8 *)buff + STM_FLASH_SECTOR_SIZE;
        }
        else
        {
            //write backup data to flash
            ulRet = Flash_WriteSector((u8 *)(&BackUpData[IndexTmp][0]), WriteBlockNum*STM_FLASH_BLOCK_COUNT+IndexTmp, 1);
            if(STM_FLASH_SUCCESS != ulRet)
            {
                return RES_ERROR;
            }
        }
    }
    return RES_OK;
}

#if 0
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


/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
    uint32_t page = 0;

    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
    }
    else
    {
        /* Bank 2 */
        page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
    }

    return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
    uint32_t bank = 0;

    if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
    {
    /* No Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
        bank = FLASH_BANK_1;
    }
    else
    {
        bank = FLASH_BANK_2;
    }
    }
    else
    {
    /* Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
        bank = FLASH_BANK_2;
    }
    else
    {
        bank = FLASH_BANK_1;
    }
    }
    return bank;
}

