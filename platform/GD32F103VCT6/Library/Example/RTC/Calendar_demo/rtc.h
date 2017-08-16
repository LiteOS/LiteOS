/**
  ******************************************************************************
  * @file    RTC/Calendar_demo/rtc.h
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Headfile of RTC_CheckAndConfig,Time_Show and Time_Adjus.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "stdio.h"

/* Exported functions ------------------------------------------------------- */
void NVIC_Configuration(void);
void RTC_Configuration(void);
uint32_t Time_Regulate(void);
void Time_Adjust(void);
void Time_Display(uint32_t TimeVar);
void Time_Show(void);
uint8_t USART_Scanf(uint32_t value);
void USART1_Config(void);

#endif /* __RTC_H */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
