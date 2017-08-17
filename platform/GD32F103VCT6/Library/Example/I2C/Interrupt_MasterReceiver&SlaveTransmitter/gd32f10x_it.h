/**
  ******************************************************************************
  * @file    I2C/Interrupt_MasterReceiver&SlaveTransmitter/gd32f10x_it.h 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The header file of the Interrupt Service Routines.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_IT_H
#define __GD32F10X_IT_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Exported functions ------------------------------------------------------- */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);

void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);

void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);

#endif /* __GD32F10X_IT_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
