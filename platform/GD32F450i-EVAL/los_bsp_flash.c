
#include <stdio.h>
#include "los_bsp_flash.h"
#include "los_bsp_uart.h"

#ifdef GD32F4XX
#include "gd32f4xx.h"
#include "systick.h"
#include "gd25qxx.h"
#include "gd32f450i_eval.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SFLASH_ID                0xC84015

uint32_t flash_id = 0;
static volatile DSTATUS TM_FATFS_FLASH_SPI_Stat = STA_NOINIT;   /* Physical drive status */
/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_initialize
* Description    : Initializes the peripherals used by the FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
DSTATUS TM_FATFS_FLASH_disk_initialize(void)
{
    /* configure SPI5 GPIO and parameter */
    spi_flash_init();
    
 /* GD32450Z-EVAL start up */
    LOS_EvbUartPrintf("\n\r###############################################################################\n\r");
    LOS_EvbUartPrintf("\n\rGD32450I-EVAL System is Starting up...\n\r");
    LOS_EvbUartPrintf("\n\rGD32450I-EVAL Flash:%dK\n\r",*(__IO uint16_t*)(0x1FFFF7E0));
    
  /* get flash id */
    flash_id = spi_flash_read_id();
    LOS_EvbUartPrintf("\n\rThe Flash_ID:0x%X\n\r\n\r",flash_id);

    /* flash id is correct */
    if(SFLASH_ID == flash_id){
         LOS_EvbUartPrintf("\n\rflash_id right\n\r",flash_id);
         TM_FATFS_FLASH_SPI_Stat &= ~STA_NOINIT;
    }
    else
    {
         LOS_EvbUartPrintf("\n\rflash_id error\n\r",flash_id);
         TM_FATFS_FLASH_SPI_Stat |= STA_NOINIT;
    }
    
    return TM_FATFS_FLASH_SPI_Stat;
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
  DSTATUS status = STA_NOINIT;
    
    /* SPI Flash状态检测：读取SPI Flash 设备ID */
    if(SFLASH_ID == spi_flash_read_id())
    {
        /* 设备ID读取结果正确 */
        status &= ~STA_NOINIT;
    }
    
    return status;
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
        case GET_SECTOR_SIZE :       // Get R/W sector size (WORD)
            *(WORD * )buff = FAT_FLASH_SECTOR_SIZE; 
        break;
        case GET_SECTOR_COUNT:
            *(DWORD * )buff = FAT_FLASH_SECTOR_COUNT;     //the total number of sector
        break;
        case GET_BLOCK_SIZE :       // Get erase block size in unit of sector (DWORD)
            *(DWORD * )buff = FAT_FLASH_BLOCK_SIZE;
        break;
        case CTRL_SYNC :
        break;
        default:break;
    }

    return RES_OK;
}

/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_read
* Description    : Read sector(s)
* Input          : sector
                   count
* Output         : buff
* Return         : RES_OK
*******************************************************************************/
DRESULT TM_FATFS_FLASH_disk_read(BYTE *buff, DWORD sector, UINT count)
{
    uint32_t read_addr;
    
    if ((TM_FATFS_FLASH_SPI_Stat & STA_NOINIT)) {
        return RES_NOTRDY;
    }
    
    read_addr = sector;
    
    spi_flash_buffer_read(buff,read_addr<<12, count<<12);//*4096
    
    return RES_OK;
}

/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_write
* Description    : Write sector(s)
* Input          : buff
                   sector
                   count
* Output         : None
* Return         : RES_OK
*******************************************************************************/
DRESULT TM_FATFS_FLASH_disk_write(BYTE *buff, DWORD sector, UINT count)
{
    uint32_t write_addr;
    
    write_addr = sector;
    
  spi_flash_sector_erase(sector<<12);/*乘以4090等于<<12*/

    spi_flash_buffer_write(buff,write_addr<<12,count<<12);//*4096
    
    return RES_OK;
}


