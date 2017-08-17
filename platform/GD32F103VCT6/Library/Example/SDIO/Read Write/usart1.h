/**
  ******************************************************************************
  * @file    SDIO/Read Write/usart1.h
  * @author  MCU SD
  * @version V1.0
  * @date    6-Sep-2014
  * @brief   usart1 head file.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART1_H
#define	__USART1_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "gd32f10x.h"

/* Exported functions ------------------------------------------------------- */
void USART1_Configuration(void);
int fputc(int ch, FILE *f);

#endif /* __USART1_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
