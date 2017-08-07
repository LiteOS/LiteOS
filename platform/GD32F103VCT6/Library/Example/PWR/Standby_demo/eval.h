/**
  ******************************************************************************
  * @file    PWR/Standby_demo/eval.h
  * @author  MCU SD
  * @version V1.0
  * @date    26-DEC-2014
  * @brief   None
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EVAL_H
#define __EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
     
/* Exported functions ------------------------------------------------------- */   
void LED_Init(void);
void RTC_Configuration(void);
void NVIC_Configuration(void);
void delay_ms(uint32_t nms);
void LED_Spark(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIOTOEXTI_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __EVAL_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
