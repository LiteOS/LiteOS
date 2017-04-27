
#ifndef __LOS_BSP_FLASH_H
#define __LOS_BSP_FLASH_H

#ifdef LOS_FLASH

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "diskio.h"
#include "ff.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
//=============================================================================================
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

/* Base address of the Flash sectors Bank 2 */
#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_13     ((uint32_t)0x08104000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_14     ((uint32_t)0x08108000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0810C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08110000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08120000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08140000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08160000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08180000) /* Base @ of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_21     ((uint32_t)0x081A0000) /* Base @ of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_22     ((uint32_t)0x081C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000) /* Base @ of Sector 11, 128 Kbytes */

#define FLASH_FLAG_PGERR         ((uint32_t)0x00000004) /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPRTERR      ((uint32_t)0x00000010) /*!< FLASH Write protected error flag */

#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_5   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (ADDR_FLASH_SECTOR_11  +  GetSectorSize(ADDR_FLASH_SECTOR_11) -1) /* End @ of user Flash area : sector start address + sector size -1 */

/* Flash memory address of physics */
#define STM32_FLASH_ADDR        ((u32)0x08000000)
#define STM32_BLOCK_SIZE        ((u32)0x20000) //128K

/* Flash  memory parameters */
#define STM_FLASH_SECTOR_SIZE       ((u16)0x1000) /* 4096 bytes per sector w/o Spare Area */
#define STM_FLASH_SECTOR_COUNT      ((u16)0x0020) /* 128k = 32*4096 bytes sectors per block */
#define STM_FLASH_SECTOR_TOTAL      ((u16)((FLASH_USER_END_ADDR - FLASH_USER_START_ADDR +1)/STM_FLASH_SECTOR_SIZE)) /* (128/4)*7= 224*/

/* Flash memory status */
//#define STM_FLASH_VALID_ADDRESS       ((u32)0x00010000)
#define STM_FLASH_INVALID_ADDRESS       ((u32)0x00020000)
#define STM_FLASH_ERROR                 ((u32)0x00040000)
#define STM_FLASH_SUCCESS               ((u32)0x00080000)

/* Flash memory data computation */
#define DATA_1st(DATA)       (u8)((DATA) & 0x000000FF)          /* 1st data byte */
#define DATA_2nd(DATA)       (u8)(((DATA)& 0x0000FF00) >> 8)    /* 2nd data byte */
#define DATA_3rd(DATA)       (u8)(((DATA)& 0x00FF0000) >> 16)   /* 3rd data byte */
#define DATA_4th(DATA)       (u8)(((DATA)& 0xFF000000) >> 24)   /* 4th data byte */

/* Exported functions ------------------------------------------------------- */
u32 Flash_WritePage(u8 *pBuffer, u32 Address, u32 NumPageToWrite);
u32 Flash_ReadPage (u8 *pBuffer, u32 Address, u32 NumPageToRead);
u32 Flash_EraseBlock(u32 BlockNum);

DSTATUS TM_FATFS_FLASH_disk_initialize(void);
DSTATUS TM_FATFS_FLASH_disk_status(void);
DRESULT TM_FATFS_FLASH_disk_ioctl(BYTE cmd, char *buff);
DRESULT TM_FATFS_FLASH_disk_read(BYTE *buff, DWORD sector, UINT count);
DRESULT TM_FATFS_FLASH_disk_write(BYTE *buff, DWORD sector, UINT count);

uint32_t GetSectorSize(uint32_t Sector);

void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset);

#endif

#endif /* __LOS_BSP_FLASH_H */

