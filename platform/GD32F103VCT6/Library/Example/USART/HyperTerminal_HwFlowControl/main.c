/**    
  ******************************************************************************    
  * @file    USART/HyperTerminal_HwFlowControl/main.c    
  * @author  MCU SD    
  * @version V1.0    
  * @date    26-Dec-2014    
  * @brief   The main function file.    
  ******************************************************************************    
  */    

/* Includes ------------------------------------------------------------------*/    
#include "gd32f10x.h"    
#include <stdio.h>    

/* Private define ------------------------------------------------------------*/
#define TxBufferSize   (countof(TxBuffer) - 1)
#define RxBufferSize   0xFF

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
USART_InitPara USART_InitStructure;
uint8_t TxBuffer[] = "\n\rUSART Hyperterminal Hardware Flow Control Example: USART - \
Hyperterminal communication using hardware flow control\n\r";
uint8_t RxBuffer[RxBufferSize];
uint8_t NbrOfDataToTransfer = TxBufferSize;
uint8_t TxCounter = 0;
uint8_t RxCounter = 0;
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);

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

   /* USART1 configuration */
    USART_InitStructure.USART_BRR = 115200;;
    USART_InitStructure.USART_WL = USART_WL_8B;
    USART_InitStructure.USART_STBits = USART_STBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_RESET;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_RTS_CTS;;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;
    USART_Init(USART1, &USART_InitStructure);

    /* Enable USART1 */
    USART_Enable(USART1, ENABLE);

    /* Communication hyperterminal-USART1 using hardware flow control */
    /* Send a buffer from USART to hyperterminal */  
    while(NbrOfDataToTransfer--)
    {
        USART_DataSend(USART1, TxBuffer[TxCounter++]);
        while(USART_GetBitState(USART1, USART_FLAG_TBE) == RESET);
    }
  
    /* Receive a string (Max RxBufferSize bytes) from the Hyperterminal ended by '\r' (Enter key) */
    do
    { 
        if((USART_GetBitState(USART1, USART_FLAG_RBNE) != RESET)&&(RxCounter < RxBufferSize))
        {
            RxBuffer[RxCounter] = USART_DataReceive(USART1);
            USART_DataSend(USART1, RxBuffer[RxCounter++]);
        }   
 
    }while((RxBuffer[RxCounter - 1] != '\r')&&(RxCounter != RxBufferSize));

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
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_11 |GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART1 Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_12 |GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
 
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
