/**
  ******************************************************************************
  * @file    PWR/Standby_demo/gd32f10x_it.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-DEC-2014
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h"

/* Private variables ---------------------------------------------------------*/
extern uint32_t Time_Delay;

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
    if(Time_Delay)
        Time_Delay--;
}

/**
  * @brief  This function handles External lines 10 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetIntBitState(EXTI_LINE14) != RESET)
    {
        /* Clear EXTI line14 bit */
        EXTI_ClearIntBitState(EXTI_LINE14);
        
        /* Turn on LED2 */
        GPIO_SetBits(GPIOC, GPIO_PIN_0);
        
        /* Wait till RTC Second event occurs */
        RTC_ClearBitState(RTC_FLAG_SF);
        while(RTC_GetBitState(RTC_FLAG_SF) == RESET);
    
        /* Set alarm in 5 second */
        RTC_SetAlarm(RTC_GetCounter()+ 5);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitLWOFF();
        
        /*Enter Standby mode*/
        PWR_STDBYMode_Entry(PWR_STDBYENTRY_WFI);       
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
