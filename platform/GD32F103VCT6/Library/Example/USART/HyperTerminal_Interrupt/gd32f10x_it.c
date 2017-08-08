/**
  ******************************************************************************
  * @file    USART/HyperTerminal_Interrupt/gd32f10x_it.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h"

/* Private define ------------------------------------------------------------*/
#define TXBUFFERSIZE   (countof(TxBuffer) - 1)
#define RXBUFFERSIZE   0x20
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer[] = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal\
 communication using Interrupt\n\r";
uint8_t RxBuffer[RXBUFFERSIZE];
uint8_t NbrOfDataToTransfer = TXBUFFERSIZE;
uint8_t NbrOfDataToRead = RXBUFFERSIZE;
__IO uint8_t TxCount = 0; 
__IO uint16_t RxCount = 0; 

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles USART1 RBNE interrupt request and TBE interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
    if(USART_GetIntBitState( USART1, USART_INT_RBNE) != RESET)
    {
        /* Read one byte from the receive data register */
        RxBuffer[RxCount++] = (USART_DataReceive(USART1) & 0x7F);
    if(RxCount == NbrOfDataToRead)
    {
        /* Disable the USART1 Receive interrupt */
        USART_INT_Set(USART1, USART_INT_RBNE, DISABLE);
    }
    }

    if(USART_GetIntBitState(USART1, USART_INT_TBE) != RESET)
    {   
        /* Write one byte to the transmit data register */
        USART_DataSend(USART1, TxBuffer[TxCount++]);
    
    if(TxCount == NbrOfDataToTransfer)
    {
        /* Disable the USART1 Transmit interrupt */
        USART_INT_Set(USART1, USART_INT_TBE, DISABLE);
    }
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
