
#include "los_bsp_flash.h"

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/


/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_initialize
* Description    : Initializes the peripherals used by the FLASH driver.
* Input          : None
* Output         : None
* Return         : RES_OK/RES_ERROR
*******************************************************************************/
DSTATUS TM_FATFS_FLASH_disk_initialize(void)
{
    //add you code here.
    
    return RES_OK;
}

/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_status
* Description    : Get disk status.
* Input          : None
* Output         : None
* Return         : RES_OK/RES_ERROR
*******************************************************************************/
DSTATUS TM_FATFS_FLASH_disk_status(void)
{
    //add you code here.
    
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
    //change the size.
    switch (cmd)
    {
        case GET_SECTOR_SIZE:                            // Get R/W sector size (WORD)
            *(WORD * )buff =  FAT_FLASH_SectorSize;      // flash read/write unit 4096
            break;
        case GET_SECTOR_COUNT:
            *(DWORD * )buff = FAT_FLASH_SectorCount ;    // sector num
            break;
        case GET_BLOCK_SIZE:                             // Get erase block size in unit of sector (DWORD)
            *(DWORD * )buff = FAT_FLASH_BlockSize ;      // Erase unit
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
    //add you code here.
    
    return RES_OK;
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
    //add you code here.
    
    return RES_OK;
}



