#ifndef __LOS_BSP_SPI_FLASH_H
#define __LOS_BSP_SPI_FLASH_H


#include "stm32f10x.h"
#include <stdio.h>

#include "diskio.h"
#include "integer.h"

//#define  sFLASH_ID              0xEF3015     //W25X16
//#define  sFLASH_ID              0xEF4015	   //W25Q16
#define  sFLASH_ID                0XEF4017      //W25Q64

/* Private typedef -----------------------------------------------------------*/
#define SPI_FLASH_SectorSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256
#define SPI_FAT_FLASH_SectorCount       1536
#define SPI_FAT_FLASH_BlockSize          1
#define SPI_FAT_FLASH_OffsetSector      512  //存放在后6M空间
/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0xFF


/************************** SPI Flash 连接引脚定义********************************/
#define      macSPIx                                     SPI1
#define      macSPI_APBxClock_FUN                        RCC_APB2PeriphClockCmd
#define      macSPI_CLK                                  RCC_APB2Periph_SPI1

#define      macSPI_CS_APBxClock_FUN                     RCC_APB2PeriphClockCmd
#define      macSPI_CS_CLK                               RCC_APB2Periph_GPIOC    
#define      macSPI_CS_PORT                              GPIOC
#define      macSPI_CS_PIN                               GPIO_Pin_0

#define      macSPI_SCK_APBxClock_FUN                    RCC_APB2PeriphClockCmd
#define      macSPI_SCK_CLK                              RCC_APB2Periph_GPIOA   
#define      macSPI_SCK_PORT                             GPIOA   
#define      macSPI_SCK_PIN                              GPIO_Pin_5

#define      macSPI_MISO_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define      macSPI_MISO_CLK                             RCC_APB2Periph_GPIOA    
#define      macSPI_MISO_PORT                            GPIOA 
#define      macSPI_MISO_PIN                             GPIO_Pin_6

#define      macSPI_MOSI_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define      macSPI_MOSI_CLK                             RCC_APB2Periph_GPIOA    
#define      macSPI_MOSI_PORT                            GPIOA 
#define      macSPI_MOSI_PIN                             GPIO_Pin_7

/************************** SPI Flash 函数宏定义********************************/
#define      macSPI_FLASH_CS_ENABLE()                       GPIO_ResetBits( macSPI_CS_PORT, macSPI_CS_PIN )
#define      macSPI_FLASH_CS_DISABLE()                      GPIO_SetBits( macSPI_CS_PORT, macSPI_CS_PIN )


/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         0
#define FLASH_DEBUG_FUNC_ON    0

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define FLASH_DEBUG_FUNC()               do{\
                                         if(FLASH_DEBUG_FUNC_ON)\
                                         printf("<<-FLASH-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)



DSTATUS TM_FATFS_FLASH_disk_initialize(void);
DSTATUS TM_FATFS_FLASH_disk_status(void) ;
DRESULT TM_FATFS_FLASH_disk_ioctl(BYTE cmd, char *buff) ;
DRESULT TM_FATFS_FLASH_disk_read(BYTE *buff, DWORD sector, UINT count) ;
DRESULT TM_FATFS_FLASH_disk_write(BYTE *buff, DWORD sector, UINT count) ;

void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, uint32_t NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
int GetGBKCode_from_EXFlash(unsigned char* pBuffer,const unsigned char * c);


#endif /* __SPI_FLASH_H */

