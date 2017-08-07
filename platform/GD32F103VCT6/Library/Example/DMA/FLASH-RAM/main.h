/**
  ******************************************************************************
  * @file    DMA/FLASH_RAM/main.h 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Headfie of main program.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include <stdio.h>

/* Private function prototypes -----------------------------------------------*/
void LED_config(void);


#ifdef __GNUC__

    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
