
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

/* STM Flash memory address of physics */
#define STM32_FLASH_ADDR		((u32)0x08000000)
#define STM32_PAGE_SIZE			((u32)0x0800)//2k
#define STM32_START_PAGE		((u32)150)
#define STM32_END_PAGE			((u32)255)

/* STM Flash  memory parameters */
#define STM_FLASH_SECTOR_SIZE		((u16)0x0200)			/* 512 bytes per sector w/o Spare Area */
#define STM_FLASH_BLOCK_SIZE    ((u16)0x0004)			/* 4x512 bytes pages per block */
#define STM_FLASH_ZONE_SIZE		((u16)(STM32_END_PAGE-STM32_START_PAGE))	/* 256-200=56 */
#define STM_FLASH_MAX_ZONE		((u16)0x0001)			/* 1 zones of 1024 block */

/* memory status */
//#define STM_FLASH_VALID_ADDRESS		((u32)0x00010000)
#define STM_FLASH_INVALID_ADDRESS	((u32)0x00020000)
#define STM_FLASH_ERROR			((u32)0x00040000)
#define STM_FLASH_SUCCESS		((u32)0x00080000)

/* STM32 Flash memory data computation */
#define DATA_1st_CYCLE(DATA)       (u8)((DATA)& 0xFF)			/* 1st data cycle */
#define DATA_2nd_CYCLE(DATA)       (u8)(((DATA)& 0xFF00) >> 8)		/* 2nd data cycle */
#define DATA_3rd_CYCLE(DATA)       (u8)(((DATA)& 0xFF0000) >> 16)	/* 3rd data cycle */
#define DATA_4th_CYCLE(DATA)       (u8)(((DATA)& 0xFF000000) >> 24)	/* 4th data cycle */


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u32 STM_Flash_WriteSector(u8 *pBuffer, DWORD uwSector, u32 ulNumSectorWrite);
u32 STM_Flash_ReadSector (u8 *pBuffer, DWORD uwSector, u32 ulNumSectorRead);
u32 STM_Flash_EraseBlock(u32 BlockNum);

DSTATUS TM_FATFS_FLASH_disk_initialize(void);
DSTATUS TM_FATFS_FLASH_disk_status(void);
DRESULT TM_FATFS_FLASH_disk_ioctl(BYTE cmd, char *buff);
DRESULT TM_FATFS_FLASH_disk_read(BYTE *buff, DWORD sector, UINT count);
DRESULT TM_FATFS_FLASH_disk_write(BYTE *buff, DWORD sector, UINT count);

void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset);


void flash_buffer_write(uint8_t* pbuffer, uint32_t write_addr, uint16_t num_byte_to_write);

#endif /* __LOS_SPI_FLASH_H */

