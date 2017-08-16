/**
  ******************************************************************************
  * @file    ADC/TIMTrigger_AutoInsertion/gd32f10x_it.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h" 

/* Private variables ---------------------------------------------------------*/
extern __IO uint16_t ADC_InsertedConvertedValueTab[64];
__IO uint32_t Index;

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
  * @brief  This function handles ADC1 and ADC2 global interrupts requests.
  * @param  None
  * @retval None
  */
void ADC1_2_IRQHandler(void)
{
    /* Set PC10 pin */
    GPIO_WriteBit(GPIOC, GPIO_PIN_10, Bit_SET);
    /* Get inserted channel11 converted value */
    ADC_InsertedConvertedValueTab[Index++] = ADC_GetInsertedConversionValue(ADC1,ADC_INSERTEDCHANNEL_1);
    /* Clear ADC1 IEOC pending interrupt bit */
    ADC_ClearIntBitState(ADC1,ADC_INT_IEOC);
    /* Reset PC10 pin */
    GPIO_WriteBit(GPIOC, GPIO_PIN_10, Bit_RESET);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
