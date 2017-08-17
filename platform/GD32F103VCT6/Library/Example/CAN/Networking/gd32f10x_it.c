/**
  ******************************************************************************
  * @file    CAN/Networking/gd32f1x0_it.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h"

/* Private variables ---------------------------------------------------------*/
CanRxMessage RxMessage;
TypeState g_recvflag = RESET; 
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
  * @brief  This function handles PendSVC exception.
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
  * @brief  This function handles CAN1 Handler.
  * @param  None
  * @retval None
  */
#ifndef GD32F10X_CL
void USB_LP_CAN1_RX0_IRQHandler(void)
#else
void CAN1_RX0_IRQHandler(void)
#endif
{
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
    if ((RxMessage.StdId == 0x321)&&(RxMessage.FF == CAN_FF_STANDARD) && (RxMessage.DLC == 1))
    {
        g_recvflag = SET;
    }
}

/**
  * @brief  This function handles CAN2 Handler.
  * @param  None
  * @retval None
  */
#ifdef GD32F10X_CL
void CAN2_RX0_IRQHandler(void)
{
    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
    if ((RxMessage.StdId == 0x321)&&(RxMessage.FF == CAN_FF_STANDARD) && (RxMessage.DLC == 1))
    {
        g_recvflag = SET;
    }
}
#endif

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
