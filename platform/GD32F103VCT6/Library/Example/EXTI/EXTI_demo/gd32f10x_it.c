/**
  ******************************************************************************
  * @file    EXTI/EXTI_demo/gd32f10x_it.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-DEC-2014
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h"

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
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/**
  * @brief  This function handles EXTI line0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetIntBitState(EXTI_LINE0) != RESET)
    {
        GPIO_WriteBit(GPIOC, GPIO_PIN_0, 
                     (BitState)((1-GPIO_ReadOutputBit(GPIOC, GPIO_PIN_0))));
        EXTI_ClearIntBitState(EXTI_LINE0);
    }
}

/**
  * @brief  This function handles EXTI line14 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetIntBitState(EXTI_LINE14) != RESET)
    {
        GPIO_WriteBit(GPIOC, GPIO_PIN_2, 
                     (BitState)((1-GPIO_ReadOutputBit(GPIOC, GPIO_PIN_2))));
        EXTI_ClearIntBitState(EXTI_LINE14);
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
