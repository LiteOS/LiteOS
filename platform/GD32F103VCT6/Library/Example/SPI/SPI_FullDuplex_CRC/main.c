/**
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_CRC/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Sep-2014
  * @brief   Main program body.
  ******************************************************************************  
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Private typedef ------------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize  10

/* Private variables ---------------------------------------------------------*/
uint8_t SPI1_Buffer_Tx[ BufferSize ] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA };
uint8_t SPI3_Buffer_Tx[ BufferSize ] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A };
uint8_t SPI1_Buffer_Rx[ BufferSize ], SPI3_Buffer_Rx[ BufferSize ];
uint32_t TxIdx = 0, RxIdx = 0;
__IO uint16_t CRC1Value = 0, CRC2Value = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

SPI_InitPara  SPI_InitStructure;
/* Private function prototypes -----------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private function ----------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{   
    GPIO_InitPara GPIO_InitStructure;
    /* Enable Peripheral clock */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOB | RCC_APB2PERIPH_GPIOC| RCC_APB2PERIPH_AF , ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_SPI1,ENABLE);
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_SPI3,ENABLE);
    GPIO_PinRemapConfig(GPIO_REMAP_SPI3, ENABLE);
    /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
    /* Confugure SCK and MOSI pins as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_5 | GPIO_PIN_7 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Confugure MISO pin as Input Floating  */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    /* Configure SPI3 pins: SCK, MISO and MOSI ---------------------------------*/
    /* Confugure SCK and MOSI pins as Input Floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10 | GPIO_PIN_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Confugure MISO pin as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* SPI1 configuration */
    SPI_InitStructure.SPI_TransType = SPI_TRANSTYPE_FULLDUPLEX ;
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
    SPI_InitStructure.SPI_FrameFormat = SPI_FRAMEFORMAT_8BIT ;
    SPI_InitStructure.SPI_SCKPL = SPI_SCKPL_LOW;
    SPI_InitStructure.SPI_SCKPH = SPI_SCKPH_2EDGE;
    SPI_InitStructure.SPI_SWNSSEN = SPI_SWNSS_SOFT; 
    SPI_InitStructure.SPI_PSC = SPI_PSC_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
    SPI_InitStructure.SPI_CRCPOL = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    /* SPI3 configuration ------------------------------------------------------*/
    SPI_InitStructure.SPI_Mode = SPI_MODE_SLAVE;
    SPI_Init(SPI3, &SPI_InitStructure);

    /* Enable SPI AND SPI3 CRC calculation */
    SPI_CRC_Enable(SPI1, ENABLE);
    SPI_CRC_Enable(SPI3, ENABLE);
    
    /* Enable SPI1 AND SPI3 */
    SPI_Enable(SPI1, ENABLE);
    SPI_Enable(SPI3, ENABLE);
 
    
    /* Transfer procedure */
    while (TxIdx < BufferSize - 1)
    {
        while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_TBE) == RESET);
        SPI_I2S_SendData(SPI3, SPI3_Buffer_Tx[ TxIdx ] );    
        SPI_I2S_SendData(SPI1, SPI1_Buffer_Tx[ TxIdx++ ]);
        while (SPI_I2S_GetBitState(SPI3, SPI_FLAG_RBNE) == RESET);
        SPI3_Buffer_Rx[ RxIdx ] = SPI_I2S_ReceiveData( SPI3 );           
        while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_RBNE) == RESET);
        SPI1_Buffer_Rx[ RxIdx++ ] = SPI_I2S_ReceiveData( SPI1 );

    }
    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_TBE) == RESET);
    while (SPI_I2S_GetBitState(SPI3, SPI_FLAG_TBE) == RESET);
   
    SPI_I2S_SendData(SPI3, SPI3_Buffer_Tx[TxIdx]);
    /* Set Next Data is CRC */
    SPI_SendCRCNext(SPI3);
    SPI_I2S_SendData(SPI1, SPI1_Buffer_Tx[TxIdx]);
    /* Set Next Data is CRC */
    SPI_SendCRCNext(SPI1);

    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_RBNE) == RESET);
    SPI1_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPI1);
    while (SPI_I2S_GetBitState(SPI3, SPI_FLAG_RBNE) == RESET);
    SPI3_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPI3);
    
    /* Wait for SPI1 data reception: CRC transmitted by SPI3 */
    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_RBNE) == RESET);
    /* Wait for SPI2 data reception: CRC transmitted by SPI1 */
    while (SPI_I2S_GetBitState(SPI3, SPI_FLAG_RBNE) == RESET);
    
    /* Compare the receive data with send data*/
    TransferStatus1 = Buffercmp(SPI3_Buffer_Rx, SPI1_Buffer_Tx, BufferSize);
    TransferStatus2 = Buffercmp(SPI1_Buffer_Rx, SPI3_Buffer_Tx, BufferSize);
    
    /* Check the SPI1 CRC Error flag */
    if ((SPI_I2S_GetBitState(SPI1, SPI_FLAG_CRCERR)) == SET)
    {
        TransferStatus2 = FAILED;
    }
    /* Check the SPI3 CRC Error flag */
    if ((SPI_I2S_GetBitState(SPI3, SPI_FLAG_CRCERR)) == SET)
    {
        TransferStatus1 = FAILED;
    }
    
    /* Read SPI1 received CRC value */
    CRC1Value = SPI_I2S_ReceiveData(SPI1);
    /* Read SPI3 received CRC value */
    CRC2Value = SPI_I2S_ReceiveData(SPI3);

    /* Disable SPIx and SPIx CRC*/
    SPI_CRC_Enable(SPI1, DISABLE);
    SPI_CRC_Enable(SPI3, DISABLE);
    SPI_Enable(SPI3, DISABLE);
    SPI_Enable(SPI1, DISABLE);    
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
