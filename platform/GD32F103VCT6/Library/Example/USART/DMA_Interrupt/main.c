/**    
  ******************************************************************************    
  * @file    USART/DMA_Interrupt/main.c    
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
uint8_t NbrOfDataToRead = TxBufferSize1;

volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void DMA_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    uint32_t index = 0;
    /* System Clocks Configuration */
    RCC_Configuration();

    /* NVIC configuration */
    NVIC_Configuration();

    /* Configure the GPIO ports */
    GPIO_Configuration();

    /* Configure the DMA */
    DMA_Configuration();

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

    /* Enable USART1 DMA TX request */
    USART_DMA_Enable(USART1, USART_DMAREQ_TX, ENABLE);

    /* Enable USART2 DMA TX request */
    USART_DMA_Enable(USART2, USART_DMAREQ_TX, ENABLE);

    /* Enable the USART2 Receive Interrupt */
    USART_INT_Set(USART2, USART_INT_RBNE, ENABLE);
  
    /* Enable USART1 */
    USART_Enable(USART1, ENABLE);

    /* Enable USART2 */
    USART_Enable(USART2, ENABLE);

    /* Enable USART1 DMA TX Channel */
    DMA_Enable(DMA1_CHANNEL4, ENABLE);

    /* Enable USART2 DMA TX Channel */
    DMA_Enable(DMA1_CHANNEL7, ENABLE);

    /* Receive the TxBuffer2 */
    while(index < TxBufferSize2)
    {
         while(USART_GetBitState(USART1, USART_FLAG_RBNE) == RESET)
         {
         }
         RxBuffer1[index++] = USART_DataReceive(USART1);
    }

    /* Wait until USART1 TX DMA1 Channel Transfer Complete */
    while (DMA_GetBitState(DMA1_FLAG_TC4) == RESET)
    {
    }
    /* Wait until USART2 TX DMA1 Channel Transfer Complete */
    while (DMA_GetBitState(DMA1_FLAG_TC7) == RESET)
    {
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

    /* Configure USART1 Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Configure USART2 Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    
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
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara NVIC_InitStructure;

    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQ = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the DMA.
  * @param  None
  * @retval None
  */
void DMA_Configuration(void)
{
    DMA_InitPara DMA_InitStructure;

    /* USART1_Tx_DMA_Channel (triggered by USART1 Tx event) Config */
    DMA_DeInit(DMA1_CHANNEL4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer1;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PERIPHERALDST;
    DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PERIPHERALDATASIZE_BYTE;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MEMORYDATASIZE_BYTE;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA1_CHANNEL4, &DMA_InitStructure);
    
    /* USART2_Tx_DMA_Channel (triggered by USART2 Tx event) Config */
    DMA_DeInit(DMA1_CHANNEL7);
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40004404;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer2;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PERIPHERALDST;
    DMA_InitStructure.DMA_BufferSize = TxBufferSize2;
    DMA_Init(DMA1_CHANNEL7, &DMA_InitStructure);
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
