/**
  ******************************************************************************
  * @file    IWDG/IWDG-Demo/gd32f20x_it.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   The header file of the ISR.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20X_IT_H
#define __GD32F20X_IT_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/* Exported functions ------------------------------------------------------- */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif /* __GD32F20X_IT_H */

/******************* (C) COPYRIGHT 2015 GIGADEVICE *****END OF FILE****/
