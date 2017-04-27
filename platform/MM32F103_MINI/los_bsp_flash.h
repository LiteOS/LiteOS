
#ifndef __LOS_BSP_FLASH_H
#define __LOS_BSP_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "diskio.h"
#include "ff.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define FAT_FLASH_SectorSize            4096
#define FAT_FLASH_SectorCount           256
#define FAT_FLASH_BlockSize             1


/* Exported functions ------------------------------------------------------- */

DSTATUS TM_FATFS_FLASH_disk_initialize(void);
DSTATUS TM_FATFS_FLASH_disk_status(void);
DRESULT TM_FATFS_FLASH_disk_ioctl(BYTE cmd, char *buff);
DRESULT TM_FATFS_FLASH_disk_read(BYTE *buff, DWORD sector, UINT count);
DRESULT TM_FATFS_FLASH_disk_write(BYTE *buff, DWORD sector, UINT count);

#endif /* __LOS_BSP_FLASH_H */
