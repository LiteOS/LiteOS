
#ifndef __LOS_SPI_FLASH_H
#define __LOS_SPI_FLASH_H


/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "diskio.h"
#include "ff.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* FAT Flash  memory parameters */
#define FAT_FLASH_SECTOR_SIZE       ((u16)0x1000)   /* 256£¨page size£©* 16 = 4096 bytes per sector  w/o Spare Area */
#define FAT_FLASH_SECTOR_COUNT      256             /* 256 * 4096 = 1M*/
#define FAT_FLASH_BLOCK_SIZE        1               /* erase block size in unit of sector*/

/* STM32 Flash memory data computation */
#define DATA_1st_CYCLE(DATA)       (u8)((DATA)& 0xFF)               /* 1st data cycle */
#define DATA_2nd_CYCLE(DATA)       (u8)(((DATA)& 0xFF00) >> 8)      /* 2nd data cycle */
#define DATA_3rd_CYCLE(DATA)       (u8)(((DATA)& 0xFF0000) >> 16)   /* 3rd data cycle */
#define DATA_4th_CYCLE(DATA)       (u8)(((DATA)& 0xFF000000) >> 24) /* 4th data cycle */


DSTATUS TM_FATFS_FLASH_disk_initialize(void);
DSTATUS TM_FATFS_FLASH_disk_status(void);
DRESULT TM_FATFS_FLASH_disk_ioctl(BYTE cmd, char *buff);
DRESULT TM_FATFS_FLASH_disk_write(BYTE *buff, DWORD sector, UINT count);
DRESULT TM_FATFS_FLASH_disk_read(BYTE *buff, DWORD sector, UINT count);


#endif /* __LOS_SPI_FLASH_H */

