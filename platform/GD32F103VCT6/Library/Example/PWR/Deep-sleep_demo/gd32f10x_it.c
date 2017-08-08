/**
  ******************************************************************************
  * @file    PWR/Deep-sleep_demo/gd32f10x_it.c 
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
  * @brief  This function handles RTC Alarm auto wake-up interrupt request.
  * @param  None
  * @retval None
  */
void RTCAlarm_IRQHandler(void)
{
    if (RTC_GetBitState(RTC_FLAG_AF) != RESET)
    {
        /* Clear the Alarm event flag */
        RTC_ClearBitState(RTC_FLAG_AF);
        
        /* Clear EXTI line17 bit */
        EXTI_ClearIntBitState(EXTI_LINE17);
    }
}

/**
  * @brief  This function handles EXTI_line13 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetIntBitState(EXTI_LINE13) != RESET)
    {
        /* Clear EXTI line13 bit */
        EXTI_ClearIntBitState(EXTI_LINE13);
        /* Recover the system clock to 72MHz */
        SystemInit();        
    }
} 

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
