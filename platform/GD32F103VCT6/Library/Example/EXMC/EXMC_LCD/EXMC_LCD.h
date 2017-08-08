/**
  ******************************************************************************
  * @file    EXMC/EXMC_LCD/EXMC_LCD.h
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   EXMC drive LCD header file.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef EXMC_LCD_H
#define EXMC_LCD_H

/* Exported macro ------------------------------------------------------------*/
#define Bank1_LCD_D    ((uint32_t)0x61000000)    /*!< LCD Data ADDR */
#define Bank1_LCD_C    ((uint32_t)0x60000000)    /*!< LCD Reg ADDR */

/* Exported functions ------------------------------------------------------- */
void LCD_GPIO_Configuration(void);
void EXMC_LCD_Init(void);

#endif/* EXMC_LCD_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
