/**    
  ******************************************************************************    
  * @file    USART/HalfDuplex/main.c    
  * @author  MCU SD    
  * @version V1.0    
  * @date    26-Dec-2014    
  * @brief   The main function file.    
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

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/

USART_InitPara USART_InitStructure;
uint8_t TxBuffer1[] = "USART DMA Interrupt: USART1 -> USART2 using DMA Tx and Rx Flag";
uint8_t TxBuffer2[] = "USART DMA Interrupt: USART2 -> USART1 using DMA Tx and Rx Interrupt";
uint8_t RxBuffer1[TxBufferSize2];
uint8_t RxBuffer2[TxBufferSize1];
uint32_t NbrOfDataToRead1 = TxBufferSize2;
uint32_t NbrOfDataToRead2 = TxBufferSize1;
uint8_t TxCounter1 = 0, RxCounter1 = 0;
uint8_t TxCounter2 = 0, RxCounter2 = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED; 

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
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

    /* USART1 and USART2 configuration */

    USART_InitStructure.USART_BRR = 230400;
    USART_InitStructure.USART_WL = USART_WL_8B;
    USART_InitStructure.USART_STBits = USART_STBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_RESET;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;
    
    /* Configure USART1 */
    USART_Init(USART1, &USART_InitStructure);

    /* Configure USART2 */
    USART_Init(USART2, &USART_InitStructure);
    
    /* Enable USART1 */
    USART_Enable(USART1, ENABLE);

    /* Enable USART2 */
    USART_Enable(USART2, ENABLE);

    /* Enable USART1 Half Duplex Mode*/
    USART_HalfDuplex_Enable(USART1, ENABLE);
    /* Enable USART2 Half Duplex Mode*/
    USART_HalfDuplex_Enable(USART2, ENABLE);
    
    while(NbrOfDataToRead2--)
    {
        /* Wait until end of transmit */
        while(USART_GetBitState(USART1, USART_FLAG_TBE) == RESET)
        {
        }
        /* Write one byte in the USARTy Transmit Data Register */
        USART_DataSend(USART1, TxBuffer1[TxCounter1++]);

        /* Wait the byte is entirely received by USARTz */  
        while(USART_GetBitState(USART2, USART_FLAG_RBNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer2 */
        RxBuffer2[RxCounter2++] = USART_DataReceive(USART2);
    }

    /* Clear the USARTy Data Register */
    USART_DataReceive(USART1);

    while(NbrOfDataToRead1--)
    { 
        /* Wait until end of transmit */
        while(USART_GetBitState(USART2, USART_FLAG_TBE)== RESET)
        {
        }
        /* Write one byte in the USARTz Transmit Data Register */
        USART_DataSend(USART2, TxBuffer2[TxCounter2++]);

        /* Wait the byte is entirely received by USARTy */
        while(USART_GetBitState(USART1,USART_FLAG_RBNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer1 */
        RxBuffer1[RxCounter1++] = USART_DataReceive(USART1);
    }
  
    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);
    /* TransferStatus1 = PASSED, if the data transmitted from USART2 and    
       received by USART1 are the same */
    /* TransferStatus1 = FAILED, if the data transmitted from USART2 and 
       received by USART1 are different */
    TransferStatus2 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
    /* TransferStatus2 = PASSED, if the data transmitted from USART1 and    
       received by USART2 are the same */
    /* TransferStatus2 = FAILED, if the data transmitted from USART1 and 
       received by USART2 are different */

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
    /* DMA clock enable */
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_DMA1, ENABLE);

    /* Enable GPIO clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA , ENABLE);

    /* Enable USART1 Clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_USART1, ENABLE); 

    /* Enable USART2 Clock */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_USART2, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;
    
    /* Configure USART1 Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART2 Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
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
