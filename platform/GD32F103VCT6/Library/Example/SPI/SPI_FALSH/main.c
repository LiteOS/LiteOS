/**    
  ******************************************************************************    
  * @file    SPI/SPI_FALSH/main.c    
  * @author  MCU SD    
  * @version V1.0    
  * @date    26-Dec-2014    
  * @brief   The main function file.    
  ******************************************************************************    
  */

/* Includes ------------------------------------------------------------------*/    
#include "gd32f10x.h"   
#include "GD25Qxx.h"
/* Private typedef ------------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize  ((sizeof(Buffer_Tx)/sizeof((*Buffer_Tx)))-1) 
#define  sFLASH_ID              0xC84015          /* GigaDevice SPI_Flash  GD25Q16B */
#define  FLASH_WriteAddress     0x000000
#define  FLASH_ReadAddress      FLASH_WriteAddress 

/* Private variables ---------------------------------------------------------*/
uint8_t Buffer_Tx[  ] = {"this is spi flash demo"};
uint8_t Buffer_Rx[ BufferSize ];
uint32_t FlashID  = 0;
uint32_t Index=0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;

/* Private function prototypes -----------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**    
  * @brief  Flash GPIO configuration.    
  * @param  None    
  * @retval None    
  */   
void SPI1_Flash_GPIO_Configuration(void)
{
  
    GPIO_InitPara  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_5 |GPIO_PIN_7; 
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;                      
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_6 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_3;                              
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    SPI_FLASH_CS_HIGH();        
}
/**    
  * @brief  Flash SPI configuration.    
  * @param  None    
  * @retval None    
  */
void SPI1_Flash_Configuration(void)
{
    SPI_InitPara  SPI_InitStructure;
    SPI_InitStructure.SPI_TransType = SPI_TRANSTYPE_FULLDUPLEX;
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
    SPI_InitStructure.SPI_FrameFormat = SPI_FRAMEFORMAT_8BIT;
    SPI_InitStructure.SPI_SCKPL = SPI_SCKPL_LOW;
    SPI_InitStructure.SPI_SCKPH = SPI_SCKPH_1EDGE;
    SPI_InitStructure.SPI_SWNSSEN = SPI_SWNSS_SOFT;
    SPI_InitStructure.SPI_PSC = SPI_PSC_32; 
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
    SPI_InitStructure.SPI_CRCPOL = 7;
  
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Enable(SPI1, ENABLE);      

}
/**    
  * @brief  RCC configuration.    
  * @param  None    
  * @retval None    
  */ 
void RCC_Configuration()
{
     RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOB 
                               | RCC_APB2PERIPH_GPIOC |RCC_APB2PERIPH_GPIOE, ENABLE);
     RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_SPI1,ENABLE);
}

/**    
  * @brief  Main program.    
  * @param  None    
  * @retval None    
  */
int main(void)    
{   
    RCC_Configuration();
    SPI1_Flash_GPIO_Configuration();
    SPI1_Flash_Configuration();
    /* Get Flash ID */
    FlashID =SPI_FLASH_ReadID();
    if(sFLASH_ID== FlashID  )
    {
        /* Erase SPI FLASH Sector */
        SPI_FLASH_SectorErase(FLASH_WriteAddress);
        /* Write Buffer_Tx to SPI FLASH */
        SPI_FLASH_BufferWrite(Buffer_Tx,FLASH_WriteAddress,BufferSize);
        /* Read data from SPI FLASH */
        SPI_FLASH_BufferRead (Buffer_Rx,FLASH_ReadAddress,BufferSize); 
        /* if TransferStatus1 =PASSED, the Flash write and read operation are correct£¬
           if TransferStatus1 =FAILED, the Flash write and read operation are failed */
        TransferStatus1 = Buffercmp(Buffer_Tx, Buffer_Rx, BufferSize);   
   
        /* Erase SPI FLASH Sector */
        SPI_FLASH_SectorErase(FLASH_WriteAddress); 
        /* Read data from SPI FLASH */
        SPI_FLASH_BufferRead(Buffer_Rx, FLASH_ReadAddress, BufferSize);  
        for (Index = 0; Index < BufferSize; Index++)
       {
          if (Buffer_Rx[Index] != 0xFF)
          {
            /* if TransferStatus2 =PASSED, the Flash erase and read operation are correct£¬
               if TransferStatus2 =FAILED, the Flash erase and read operation are failed */  
            TransferStatus2 = FAILED;
          }
       }
        
    }
    
    while (1)    
    {    
    }        
}    
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1: buffers to be compared.
  * @param  pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }
        pBuffer1++;
        pBuffer2++;
    }
    return PASSED;
}


/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/    





