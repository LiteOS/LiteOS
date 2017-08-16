/**
  ******************************************************************************
  * @file    I2C/Interrupt_MasterReceiver&SlaveTransmitter/gd32f10x_it.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h"
#include "I2C_IE.h"

/* Private function ----------------------------------------------------------*/
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
  * @brief  This function handles I2C1 event interrupt request exception.
  * @param  None
  * @retval None
  */
void I2C1_EV_IRQHandler(void)
{
    I2C1_EventIRQ_Handler();
}

/**
  * @brief  This function handles I2C1 error interrupt request exception.
  * @param  None
  * @retval None
  */
void I2C1_ER_IRQHandler(void)
{
    I2C1_ErrorIRQ_Handler();
}

/**
  * @brief  This function handles I2C2 event interrupt request exception.
  * @param  None
  * @retval None
  */
void I2C2_EV_IRQHandler(void)
{
    I2C2_EventIRQ_Handler();
}

/**
  * @brief  This function handles I2C2 error interrupt request exception.
  * @param  None
  * @retval None
  */
void I2C2_ER_IRQHandler(void)
{
    I2C2_ErrorIRQ_Handler();
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
