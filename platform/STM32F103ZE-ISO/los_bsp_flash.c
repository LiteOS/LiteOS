
#include "los_bsp_flash.h"

#ifdef LOS_STM32F103ZE
#include "stm32f10x_it.h"
#endif

/* Private variables ---------------------------------------------------------*/
static volatile DSTATUS TM_FATFS_FLASH_SPI_Stat = STA_NOINIT;	/* Physical drive status */
static u8 BackUpData[STM32_PAGE_SIZE];//[STM_FLASH_BLOCK_SIZE][STM_FLASH_SECTOR_SIZE];		

/******************************************************************************
* Function Name  : STM_Flash_WriteSector
* Description    : This routine is for writing one or several 512 Bytes Page size.
* Input          : u8 *pBuffer, DWORD uwSector, u32 ulNumSectorWrite
* Output         : None
* Return         : 
*******************************************************************************/
u32 STM_Flash_WriteSector(u8 *pBuffer, DWORD uwSector, u32 ulNumSectorWrite)
{
  FLASH_Status status = FLASH_COMPLETE;
  
  u32 STMFlashStatus = STM_FLASH_SUCCESS;
  u32 Address;
  u32 index, data, data32;
  
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  /* Clear All pending flags */
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
  
  while( (ulNumSectorWrite != 0x00) && (STMFlashStatus != STM_FLASH_INVALID_ADDRESS) )
  {
    Address = STM32_FLASH_ADDR + (STM32_PAGE_SIZE * STM32_START_PAGE)
		+ STM_FLASH_SECTOR_SIZE * uwSector;
    
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
				status = FLASH_ProgramWord(Address, data);
				Address += 4;
	
				if(status != FLASH_COMPLETE)
				{
					FLASH_Lock();
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
      if(uwSector >= (u32)(STM_FLASH_MAX_ZONE*STM_FLASH_ZONE_SIZE*STM_FLASH_BLOCK_SIZE))
	STMFlashStatus = STM_FLASH_INVALID_ADDRESS;
    }
  }
  
  FLASH_Lock();
  
  return STMFlashStatus;
}

/******************************************************************************
* Function Name  : STM_Flash_ReadSector
* Description    : This routine is for sequential read from one or several
* Input          : DWORD uwSector, u32 ulNumSectorRead
* Output         : u8 *pBuffer
* Return         : New status of the NAND operation. This parameter can be:
*******************************************************************************/
u32 STM_Flash_ReadSector(u8 *pBuffer, DWORD uwSector, u32 ulNumSectorRead)
{
  u32 STMFlashStatus = STM_FLASH_SUCCESS;
  u32 Address;
  u32 index, data;
	u8 tmpdata = 0x00;
  
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  /* Clear All pending flags */
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
  
  while( (ulNumSectorRead != 0x00) && (STMFlashStatus != STM_FLASH_INVALID_ADDRESS) )
  {
    Address = STM32_FLASH_ADDR + (STM32_PAGE_SIZE * STM32_START_PAGE)
		+ STM_FLASH_SECTOR_SIZE * uwSector;
    
    for(index=0; index < STM_FLASH_SECTOR_SIZE; index++)
    {
      switch(index%4)
      {
				case 0:
					data = *(u32*)Address;
					Address += 4;
				  
					tmpdata = DATA_1st_CYCLE(data);
				  *pBuffer = tmpdata;
				  pBuffer++;
					break;
				case 1:
					tmpdata = DATA_2nd_CYCLE(data);
				  *pBuffer = tmpdata;
				  pBuffer++;
					break;
				case 2:
					tmpdata = DATA_3rd_CYCLE(data);
				  *pBuffer = tmpdata;
				  pBuffer++;
					break;
				case 3:
					tmpdata = DATA_4th_CYCLE(data);
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
       if(uwSector >= (u32)(STM_FLASH_MAX_ZONE*STM_FLASH_ZONE_SIZE*STM_FLASH_BLOCK_SIZE))
	  STMFlashStatus = STM_FLASH_INVALID_ADDRESS;
    }
  }
  
  FLASH_Lock();
  
  return STMFlashStatus;
}

/******************************************************************************
* Function Name  : STM_Flash_EraseBlock
*******************************************************************************/
u32 STM_Flash_EraseBlock(u32 BlockNum)
{
  FLASH_Status status = FLASH_COMPLETE;
  u32 Address;
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  /* Clear All pending flags */
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
  
  Address = STM32_FLASH_ADDR + STM32_PAGE_SIZE * (STM32_START_PAGE + BlockNum);
  
  status = FLASH_ErasePage(Address);
  
  FLASH_Lock();
  
  if(status == FLASH_COMPLETE)
    return STM_FLASH_SUCCESS;
  else
    return STM_FLASH_ERROR;
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
* Function Name  : TM_FATFS_FLASH_SPI_disk_initialize
* Description    : Initializes the peripherals used by the SPI FLASH driver.
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
		case GET_SECTOR_SIZE :                    // Get R/W sector size (WORD)
			*(WORD * )buff = STM_FLASH_SECTOR_SIZE;  //flash最小写单元为2048，此处取512为一个读写单位
		break;
		case GET_SECTOR_COUNT:
			*(DWORD * )buff = STM_FLASH_MAX_ZONE * STM_FLASH_ZONE_SIZE * STM_FLASH_BLOCK_SIZE;		//sector数量
		break;
		case GET_BLOCK_SIZE :      // Get erase block size in unit of sector (DWORD)
			*(DWORD * )buff = STM_FLASH_BLOCK_SIZE;		 //STM_FLASH_BLOCK_SIZE flash以2048为最小擦除单位
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
	u32 result;
  
  if (!count)return RES_PARERR;
  
  result = STM_Flash_ReadSector(buff, sector, count);
  
  if(result == STM_FLASH_SUCCESS)
		return RES_OK;
  else 
		return RES_ERROR;
}

/*******************************************************************************
* Function Name  : TM_FATFS_FLASH_disk_write
* Description    : Write sector(s)
* Input          : buff
                   sector
                   count
* Output         : None
* Return         : RES_OK/RES_PARERR
*******************************************************************************/
DRESULT TM_FATFS_FLASH_disk_write(BYTE *buff, DWORD sector, UINT count)
{
  u32 WriteBlockNum;
  u16 IndexTmp, OffsetPage;
  
  if(!count)return RES_PARERR;
	
	//此函数实现的是STM_FLASH_SECTOR_SIZE比STM32_PAGE_SIZE小的情况。
	if(STM_FLASH_SECTOR_SIZE > STM32_PAGE_SIZE)
	return RES_PARERR;
  
  //块码：写数据所在的块
  WriteBlockNum = sector/STM_FLASH_BLOCK_SIZE;
  //页码：写数据所在块中的偏移页
  OffsetPage = sector%STM_FLASH_BLOCK_SIZE;
  //将数据拷贝到数组BackUpData
  STM_Flash_ReadSector((u8*)(&BackUpData[0]), WriteBlockNum*STM_FLASH_BLOCK_SIZE, STM_FLASH_BLOCK_SIZE);
  
  //擦除写所在块
  STM_Flash_EraseBlock(WriteBlockNum);
  
  // return write the block  with modify //
  for (IndexTmp = 0; IndexTmp < STM_FLASH_BLOCK_SIZE; IndexTmp++ )
  {
    if((IndexTmp>=OffsetPage)&&(IndexTmp < (OffsetPage+count)))
    {
      STM_Flash_WriteSector((u8 *)buff, WriteBlockNum*STM_FLASH_BLOCK_SIZE+IndexTmp, 1);
      buff = (u8 *)buff +  STM_FLASH_SECTOR_SIZE;
    }
    else
    {
      //将数组BackUpData中的数据写回STM Flash中:
      STM_Flash_WriteSector((u8 *)(&BackUpData[IndexTmp*STM_FLASH_SECTOR_SIZE]), WriteBlockNum*STM_FLASH_BLOCK_SIZE+IndexTmp, 1);
    }
  }
	
  return RES_OK;	
}


