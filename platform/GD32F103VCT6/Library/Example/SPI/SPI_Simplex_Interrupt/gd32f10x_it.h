/**
  ******************************************************************************
  * @file    SPI/SPI_Simplex_Interrupt/gd32f1x0_it.h
  * @author  MCU SD
  * @version V1.0
  * @date    6-Sep-2014
  * @brief   The header file of the ISR.
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_IT_H
#define __GD32F10X_IT_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Exported functions --------------------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void SPI1_IRQHandler(void);
void SPI3_IRQHandler(void);

#endif /* __GD32F10X_IT_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
