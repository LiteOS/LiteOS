/**
  ******************************************************************************
  * @file    stm32412g_discovery_lcd.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    27-January-2017
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32412g_discovery_lcd.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32412G_DISCOVERY_LCD_H
#define __STM32412G_DISCOVERY_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32412g_discovery.h" 
#include "../Components/ls016b8uy/ls016b8uy.h"
#include "../Components/st7789h2/st7789h2.h"
#include "../../../Utilities/Fonts/fonts.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32412G_DISCOVERY
  * @{
  */
    
/** @addtogroup STM32412G_DISCOVERY_LCD
  * @{
  */ 

/** @defgroup STM32412G_DISCOVERY_LCD_Exported_Types STM32412G DISCOVERY LCD Exported Types
  * @{
  */
typedef struct 
{ 
  uint32_t TextColor;
  uint32_t BackColor;
  sFONT    *pFont; 
}LCD_DrawPropTypeDef;

typedef struct 
{
  int16_t X;
  int16_t Y;
}Point, * pPoint; 

/** 
  * @brief  Line mode structures definition
  */ 
typedef enum
{
  CENTER_MODE          = 0x01,    /* Center mode */
  RIGHT_MODE           = 0x02,    /* Right mode  */
  LEFT_MODE            = 0x03     /* Left mode   */
}Line_ModeTypdef;

/**
  * @}
  */ 

/** @defgroup STM32412G_DISCOVERY_LCD_Exported_Constants STM32412G DISCOVERY LCD Exported Constants
  * @{
  */
/** 
  * @brief  LCD status structure definition  
  */     
#define LCD_OK         ((uint8_t)0x00)
#define LCD_ERROR      ((uint8_t)0x01)
#define LCD_TIMEOUT    ((uint8_t)0x02)

/** 
  * @brief  LCD orientation  
  */ 
#define  LCD_ORIENTATION_PORTRAIT   ((uint8_t)0x00) /*!< Portrait orientation choice of LCD screen  */
#define  LCD_ORIENTATION_LANDSCAPE  ((uint8_t)0x01) /*!< Landscape orientation choice of LCD screen */
#define  LCD_ORIENTATION_LANDSCAPE_ROT180 ((uint8_t)0x02) /*!< Landscape rotated 180° orientation choice of LCD screen */

/** 
  * @brief  LCD color  
  */ 
#define LCD_COLOR_BLUE          ((uint16_t)0x001F)
#define LCD_COLOR_GREEN         ((uint16_t)0x07E0)
#define LCD_COLOR_RED           ((uint16_t)0xF800)
#define LCD_COLOR_CYAN          ((uint16_t)0x07FF)
#define LCD_COLOR_MAGENTA       ((uint16_t)0xF81F)
#define LCD_COLOR_YELLOW        ((uint16_t)0xFFE0)
#define LCD_COLOR_LIGHTBLUE     ((uint16_t)0x841F)
#define LCD_COLOR_LIGHTGREEN    ((uint16_t)0x87F0)
#define LCD_COLOR_LIGHTRED      ((uint16_t)0xFC10)
#define LCD_COLOR_LIGHTMAGENTA  ((uint16_t)0xFC1F)
#define LCD_COLOR_LIGHTYELLOW   ((uint16_t)0xFFF0)
#define LCD_COLOR_DARKBLUE      ((uint16_t)0x0010)
#define LCD_COLOR_DARKGREEN     ((uint16_t)0x0400)
#define LCD_COLOR_DARKRED       ((uint16_t)0x8000)
#define LCD_COLOR_DARKCYAN      ((uint16_t)0x0410)
#define LCD_COLOR_DARKMAGENTA   ((uint16_t)0x8010)
#define LCD_COLOR_DARKYELLOW    ((uint16_t)0x8400)
#define LCD_COLOR_WHITE         ((uint16_t)0xFFFF)
#define LCD_COLOR_LIGHTGRAY     ((uint16_t)0xD69A)
#define LCD_COLOR_GRAY          ((uint16_t)0x8410)
#define LCD_COLOR_DARKGRAY      ((uint16_t)0x4208)
#define LCD_COLOR_BLACK         ((uint16_t)0x0000)
#define LCD_COLOR_BROWN         ((uint16_t)0xA145)
#define LCD_COLOR_ORANGE        ((uint16_t)0xFD20)

/** 
  * @brief LCD default font 
  */ 
#define LCD_DEFAULT_FONT         Font12

/**
  * @brief LCD special pins
  */
/* LCD reset pin */
#define LCD_RESET_PIN                    GPIO_PIN_11
#define LCD_RESET_GPIO_PORT              GPIOD
#define LCD_RESET_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define LCD_RESET_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOD_CLK_DISABLE()

/* LCD tearing effect pin */
#define LCD_TE_PIN                       GPIO_PIN_3
#define LCD_TE_GPIO_PORT                 GPIOG
#define LCD_TE_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOG_CLK_ENABLE()
#define LCD_TE_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOG_CLK_DISABLE()

/* Backlight control pin */
#define LCD_BL_CTRL_PIN                  GPIO_PIN_5
#define LCD_BL_CTRL_GPIO_PORT            GPIOF
#define LCD_BL_CTRL_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define LCD_BL_CTRL_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOF_CLK_DISABLE()

/**
  * @}
  */

/** @defgroup STM32412G_DISCOVERY_LCD_Exported_Functions STM32412G DISCOVERY LCD Exported Functions
  * @{
  */   
uint8_t  BSP_LCD_Init(void);
uint8_t  BSP_LCD_InitEx(uint32_t orientation);
uint8_t  BSP_LCD_DeInit(void);
uint32_t BSP_LCD_GetXSize(void);
uint32_t BSP_LCD_GetYSize(void);
 
uint16_t BSP_LCD_GetTextColor(void);
uint16_t BSP_LCD_GetBackColor(void);
void     BSP_LCD_SetTextColor(__IO uint16_t Color);
void     BSP_LCD_SetBackColor(__IO uint16_t Color);
void     BSP_LCD_SetFont(sFONT *fonts);
sFONT    *BSP_LCD_GetFont(void);

void     BSP_LCD_Clear(uint16_t Color);
void     BSP_LCD_ClearStringLine(uint16_t Line);
void     BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr);
void     BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Line_ModeTypdef Mode);
void     BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

uint16_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void     BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);
void     BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount);
void     BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
void     BSP_LCD_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp);
void     BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount);
void     BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);

void     BSP_LCD_DisplayOff(void);
void     BSP_LCD_DisplayOn(void);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
__weak void BSP_LCD_MspInit(void);
__weak void BSP_LCD_MspDeInit(void);
 
/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* __STM32412G_DISCOVERY_LCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
