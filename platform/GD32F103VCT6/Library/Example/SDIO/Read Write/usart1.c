/**
  ******************************************************************************
  * @file    SDIO/Read Write/usart1.c
  * @author  MCU SD
  * @version V1.0
  * @date    6-Sep-2014
  * @brief   Usart1 driver.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart1.h"
#include <stdio.h>

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure the USART peripheral.
  * @param  None
  * @retval None
  */
void USART1_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;
    USART_InitPara USART_InitStructure;

    /* Enable USART1 and GPIOA clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_USART1 | RCC_APB2PERIPH_GPIOA, ENABLE);

    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART1  configuration */
    USART_InitStructure.USART_BRR = 115200;
    USART_InitStructure.USART_WL = USART_WL_8B;
    USART_InitStructure.USART_STBits = USART_STBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_RESET ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;
    USART_Init(USART1, &USART_InitStructure);

    /* Enable USART1 */
    USART_Enable(USART1, ENABLE);
}

/**
  * @brief  Retarget the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
    /* Loop until transmit data register is empty */
    while (USART_GetBitState ( USART1 , USART_FLAG_TC ) == RESET);

    /* Place your implementation of fputc here */
    USART_DataSend( USART1, (uint8_t) ch );

    return ch;
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
