/**
  ******************************************************************************
  * @file    USART/Synchronous/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.  
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)
#define DYMMY_BYTE      0x00

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
USART_InitPara USART_InitStructure;
USART_ClockInitPara USART_ClockInitStructure; 

uint8_t TxBuffer1[] = "USART Synchronous Example: USART1 -> SPI1 using TXE and RXNE Flags";
uint8_t TxBuffer2[] = "USART Synchronous Example: SPI1 -> USART1 using TXE and RXNE Flags";
uint8_t RxBuffer1[TxBufferSize2];
uint8_t RxBuffer2[TxBufferSize1];
__IO uint8_t NbrOfDataToRead1 = TxBufferSize2;
__IO uint8_t NbrOfDataToRead2 = TxBufferSize1;
__IO uint8_t TxCounter1 = 0, RxCounter1 = 0;
__IO uint8_t TxCounter2 = 0, RxCounter2 = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED; 

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void SPI_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    /* System Clocks Configuration */
    RCC_Configuration();

    /* Configure the GPIO ports */
    GPIO_Configuration();

    /* Configure the SPI */
    SPI_Configuration();

    /* USART1 configuration */
    USART_ClockInitStructure.USART_CKEN = USART_CKEN_SET;
    USART_ClockInitStructure.USART_CPL = USART_CPL_HIGH;
    USART_ClockInitStructure.USART_CPH = USART_CPH_2EDGE;
    USART_ClockInitStructure.USART_LBCP= USART_LBCP_ENABLE;
    USART_ClockInit(USART1, &USART_ClockInitStructure);

    USART_InitStructure.USART_BRR = 115200;
    USART_InitStructure.USART_WL = USART_WL_8B;
    USART_InitStructure.USART_STBits = USART_STBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_RESET ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;
    USART_Init(USART1, &USART_InitStructure);
  
    /* Configure the USART1 */
    USART_Init(USART1, &USART_InitStructure);

    /* Enable the USART1 */
    USART_Enable(USART1, ENABLE);

    while(NbrOfDataToRead2--)
    {
        /* Write one byte in the USART1 Transmit Data Register */
        USART_DataSend(USART1, TxBuffer1[TxCounter1++]);
        /* Wait until end of transmit */
        while(USART_GetBitState(USART1, USART_FLAG_TC) == RESET)
        {
        }
        /* Wait the byte is entirely received by SPI1 */  
        while(SPI_I2S_GetBitState(SPI1, SPI_FLAG_RBNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer2 */
        RxBuffer2[RxCounter2++] = SPI_I2S_ReceiveData(SPI1);
    }

    /* Clear the USART1 Data Register */
    USART_DataReceive(USART1);

    while(NbrOfDataToRead1--)
    {
        /* Wait until end of transmit */
        while(SPI_I2S_GetBitState(SPI1, SPI_FLAG_TBE)== RESET)
        {
        }
        /* Write one byte in the SPI1 Transmit Data Register */
        SPI_I2S_SendData(SPI1, TxBuffer2[TxCounter2++]);

        /* Send a Dummy byte to generate clock to slave */ 
        USART_DataSend(USART1, DYMMY_BYTE);
        /* Wait until end of transmit */
        while(USART_GetBitState(USART1, USART_FLAG_TC) == RESET)
        {
        }
        /* Wait the byte is entirely received by USART1 */
        while(USART_GetBitState(USART1, USART_FLAG_RBNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer1 */
        RxBuffer1[RxCounter1++] = USART_DataReceive(USART1);
    }
  
    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
    /* TransferStatus = PASSED, if the data transmitted from USART1 and  
       received by SPI1 are the same */
    /* TransferStatus = FAILED, if the data transmitted from USART1 and 
       received by SPI1 are different */
    TransferStatus2 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);
    /* TransferStatus = PASSED, if the data transmitted from SPI1 and  
       received by USART1 are the same */
    /* TransferStatus = FAILED, if the data transmitted from SPI1 and 
       received by USART1 are different */

    while (1)
    {
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{   
    /* Enable GPIO clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA, ENABLE);
    /* Enable USART1 Clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_USART1, ENABLE); 
    /* Enable SPI1 Clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_SPI1, ENABLE);    
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    /* Configure USART1 TX and USART1 CK pins as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure SPI1 pins: SCK, MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin =   GPIO_PIN_6| GPIO_PIN_7|GPIO_PIN_5 ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 RX as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  Configures the SPI.
  * @param  None
  * @retval None
  */
void SPI_Configuration(void)
{
    SPI_InitPara SPI_InitStructure;

    SPI_ParaInit(&SPI_InitStructure);

    SPI_I2S_DeInit(SPI1);

    /* SPI1 Config */
    SPI_InitStructure.SPI_TransType =SPI_TRANSTYPE_FULLDUPLEX;
    SPI_InitStructure.SPI_Mode = SPI_MODE_SLAVE;
    SPI_InitStructure.SPI_FrameFormat = SPI_FRAMEFORMAT_8BIT;
    SPI_InitStructure.SPI_SCKPL = SPI_SCKPL_HIGH;
    SPI_InitStructure.SPI_SCKPH = SPI_SCKPH_2EDGE;
    SPI_InitStructure.SPI_SWNSSEN = SPI_SWNSS_SOFT;
    SPI_InitStructure.SPI_PSC=  SPI_PSC_32;
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_LSB;
  
    /* Configure SPI1 */
    SPI_Init(SPI1, &SPI_InitStructure);

    /* SPI1 enable */
    SPI_Enable(SPI1, ENABLE);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }
    return PASSED;
}


/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
