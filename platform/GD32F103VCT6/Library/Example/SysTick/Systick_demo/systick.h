/**
  ******************************************************************************
  * @file    SysTick/Systick_demo/systick.h
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The header file of the SysTick configuration.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_TICK_H
#define __SYS_TICK_H

/* Exported functions ------------------------------------------------------- */
void SysTick_Configuration(void);
void Delay_1ms(uint32_t nTime);
void TimingDelay_Decrement(void);
void LED_Spark(void);

#endif

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
