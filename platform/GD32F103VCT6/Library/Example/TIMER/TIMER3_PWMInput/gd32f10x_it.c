/**
  ******************************************************************************
  * @file    TIMER/TIMER3_PWMInput/gd32f10x_it.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h"
#include "systick.h"
#include <stdio.h>


/* Private variables ---------------------------------------------------------*/
__IO uint16_t IC1Value = 0,IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO float Frequency = 0;

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
    TimingDelay_Decrement();
}

/**
  * @brief  This function handles TIMER3 interrupt request.
  * @param  None
  * @retval None
  */ 
void TIMER3_IRQHandler()
{
    if(TIMER_GetIntBitState(TIMER3,TIMER_INT_CH1)==ENABLE)
    {
        TIMER_ClearIntBitState(TIMER3,TIMER_INT_CH1);

        IC1Value = TIMER_GetCapture1(TIMER3);

        if (IC1Value != 0)
        {
            IC2Value  = TIMER_GetCapture2(TIMER3)+1;
            DutyCycle = ( IC2Value * 100) / IC1Value;
            Frequency = (float)2000 / IC1Value;
            printf("the value1 is %d,the value2 is %d\n",IC1Value,IC2Value);
            printf("the count is %ld\n",IC1Value-IC2Value);
            printf("the dutycycle is %d\n",DutyCycle);
            printf("the frequence is %f\n",Frequency);
        }
        else
        {
            DutyCycle = 0;
            Frequency = 0;
        }
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
