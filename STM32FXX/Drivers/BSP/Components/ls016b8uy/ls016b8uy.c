/**
  ******************************************************************************
  * @file    ls016b8uy.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-April-2016
  * @brief   This file includes the LCD driver for LS016B8UY LCD.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "ls016b8uy.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup ls016b8uy
  * @brief     This file provides a set of functions needed to drive the 
  *            LS016B8UY LCD.
  * @{
  */

/** @defgroup LS016B8UY_Private_TypesDefinitions
  * @{
  */ 
typedef struct  {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} LS016B8UY_Rgb888;

/**
  * @}
  */ 

/** @defgroup LS016B8UY_Private_Defines
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup LS016B8UY_Private_Macros
  * @{
  */
     
/**
  * @}
  */  

/** @defgroup LS016B8UY_Private_Variables
  * @{
  */ 
LCD_DrvTypeDef   ls016b8uy_drv = 
{
  ls016b8uy_Init,
  ls016b8uy_ReadID,
  ls016b8uy_DisplayOn,
  ls016b8uy_DisplayOff,
  ls016b8uy_SetCursor,
  ls016b8uy_WritePixel,
  ls016b8uy_ReadPixel,
  ls016b8uy_SetDisplayWindow,
  ls016b8uy_DrawHLine,
  ls016b8uy_DrawVLine,
  ls016b8uy_GetLcdPixelWidth,
  ls016b8uy_GetLcdPixelHeight,
  ls016b8uy_DrawBitmap,
  ls016b8uy_DrawRGBImage,  
};

static uint16_t WindowsXstart = 0;
static uint16_t WindowsYstart = 0;
static uint16_t WindowsXend = LS016B8UY_LCD_PIXEL_WIDTH-1;
static uint16_t WindowsYend = LS016B8UY_LCD_PIXEL_HEIGHT-1;
/**
  * @}
  */ 
  
/** @defgroup LS016B8UY_Private_FunctionPrototypes
  * @{
  */
static LS016B8UY_Rgb888 ls016b8uy_ReadPixel_rgb888(uint16_t Xpos, uint16_t Ypos);
static void ls016b8uy_DrawRGBHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint8_t *pdata);

/**
  * @}
  */ 
  
/** @defgroup LS016B8UY_Private_Functions
  * @{
  */   

/**
  * @brief  Initialize the LS016B8UY LCD Component.
  * @param  None
  * @retval None
  */
void ls016b8uy_Init(void)
{
  uint8_t   parameter[4];
  uint32_t  i;

  /* Initialize LS016B8UY low level bus layer ----------------------------------*/
  LCD_IO_Init();
  
  parameter[0] = 0x00;     /* VSYNC output */
  ls016b8uy_WriteReg(LCD_CMD_VSYNC_OUTPUT, parameter, 1);
  parameter[0] = 0x06;     /* 18 bits color mode */
  ls016b8uy_WriteReg(LCD_CMD_COLOR_MODE, parameter, 1);
  parameter[0] = 0x01;     /* Panel setting command <No.1 UN-Lock> */
  parameter[1] = 0xFE;
  ls016b8uy_WriteReg(LCD_CMD_PANEL_SETTING_1, parameter, 2);
  parameter[0] = 0xDE;     /* Panel setting command <No.2 UN-Lock> */
  parameter[1] = 0x21;
  ls016b8uy_WriteReg(LCD_CMD_PANEL_SETTING_2, parameter, 2);
  parameter[0] = 0x05;     /* V-Porch setting */
  parameter[1] = 0x33;
  ls016b8uy_WriteReg(LCD_CMD_PANEL_V_PORCH, parameter, 2);
  parameter[0] = 0x05;     /* Idle mode V-Porch setting */
  parameter[1] = 0x33;
  ls016b8uy_WriteReg(LCD_CMD_PANEL_IDLE_V_PORCH, parameter, 2);
  parameter[0] = 0x04;     /* panel timing setting */
  parameter[1] = 0x03;
  ls016b8uy_WriteReg(LCD_CMD_PANEL_TIMING_1, parameter, 2);
  parameter[0] = 0x5E;     /* panel timing setting */
  parameter[1] = 0x08;
  ls016b8uy_WriteReg(LCD_CMD_PANEL_TIMING_2, parameter, 2);
  parameter[0] = 0x0A;     /* panel timing setting */
  parameter[1] = 0x0C;
  parameter[2] = 0x02;
  ls016b8uy_WriteReg(LCD_CMD_PANEL_TIMING_3, parameter, 3);
  parameter[0] = 0x03;     /* panel timing setting */
  parameter[1] = 0x04;
  ls016b8uy_WriteReg(LCD_CMD_PANEL_TIMING_4, parameter, 2);
  parameter[0] = 0x0C;     /* panel power setting */
  ls016b8uy_WriteReg(LCD_CMD_PANEL_POWER, parameter, 1);
  parameter[0] = 0x02;     /* Oscillator Setting = 2MHz */
  ls016b8uy_WriteReg(LCD_CMD_OSCILLATOR, parameter, 1);
  parameter[0] = 0x53;     /* GVDD = 4.76V setting */
  ls016b8uy_WriteReg(LCD_CMD_GVDD, parameter, 1);
  parameter[0] = 0x00;     /* Reload MTP After SLPOUT */
  parameter[1] = 0x45;     /* VCOMH=3.76V */
  ls016b8uy_WriteReg(LCD_CMD_RELOAD_MTP_VCOMH, parameter, 2);
  parameter[0] = 0x03;     /* OP-Amp Ability. (Normal) */
  parameter[1] = 0x12;     /* Step-up Cycle for AVDD Booster Freq./4 */
  ls016b8uy_WriteReg(LCD_CMD_OPAMP, parameter, 2);
  parameter[0] = 0x00;     /* Tearing Effect Option (00h:VSYNC Interface OFF, 01h:VSYNC Interface ON) */
  ls016b8uy_WriteReg(LCD_CMD_TEARING_EFFECT, parameter, 1);
  ls016b8uy_WriteReg(LCD_CMD_PANEL_SETTING_LOCK, parameter, 0);   /* Panel setting command <Lock> */
  ls016b8uy_WriteReg(LCD_CMD_SLEEP_OUT, parameter, 0);            /* Sleep Out Command */
  LCD_IO_Delay(150);   /* Wait for 150ms */

  parameter[0] = 0x00;     /* CASET */
  parameter[1] = 0x1E;
  parameter[2] = 0x00;
  parameter[3] = 0xD1;
  ls016b8uy_WriteReg(LCD_CMD_CASET, parameter, 4);
  parameter[0] = 0x00;     /* RASET */
  parameter[1] = 0x00;
  parameter[2] = 0x00;
  parameter[3] = 0xB3;
  ls016b8uy_WriteReg(LCD_CMD_RASET, parameter, 4);
  parameter[0] = 0x83;     /* Normal display for Driver Down side */
  ls016b8uy_WriteReg(LCD_CMD_NORMAL_DISPLAY, parameter, 1);

  ls016b8uy_DisplayOn();   /* Display ON command */

  /* Fill LCD frame memory with black pixels */
  ls016b8uy_WriteReg(LCD_CMD_WRITE_RAM, parameter, 0);   /* RAM Write Data */

  for(i = 0; i < (LS016B8UY_LCD_PIXEL_WIDTH * LS016B8UY_LCD_PIXEL_HEIGHT * 3)/2; i++)
  {
    LCD_IO_WriteData(0x0000);
  }

  LCD_IO_Delay(20);   /* Wait for 20ms */
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void ls016b8uy_DisplayOn(void)
{
  ls016b8uy_WriteReg(LCD_CMD_DISPLAY_ON, (uint8_t*)NULL, 0);    /* Display ON command */
  ls016b8uy_WriteReg(LCD_CMD_IDLE_MODE_OFF, (uint8_t*)NULL, 0); /* Idle mode OFF command */
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void ls016b8uy_DisplayOff(void)
{
  ls016b8uy_WriteReg(LCD_CMD_DISPLAY_OFF, (uint8_t*)NULL, 0);   /* Display OFF command */
  LCD_IO_Delay(20);   /* Wait for 20ms */
  ls016b8uy_WriteReg(LCD_CMD_SLEEP_IN, (uint8_t*)NULL, 0);      /* Sleep In Command */
  LCD_IO_Delay(150);   /* Wait for 150ms */
}

/**
  * @brief  Get the LCD pixel Width.
  * @param  None
  * @retval The Lcd Pixel Width
  */
uint16_t ls016b8uy_GetLcdPixelWidth(void)
{
 return (uint16_t)LS016B8UY_LCD_PIXEL_WIDTH;
}

/**
  * @brief  Get the LCD pixel Height.
  * @param  None
  * @retval The Lcd Pixel Height
  */
uint16_t ls016b8uy_GetLcdPixelHeight(void)
{
 return (uint16_t)LS016B8UY_LCD_PIXEL_HEIGHT;
}

/**
  * @brief  Get the LS016B8UY ID.
  * @param  None
  * @retval The LS016B8UY ID 
  */
uint16_t ls016b8uy_ReadID(void)
{
  LCD_IO_Init(); 
  /* TODO : LCD read ID command not known for now, so assumption that the connected LCD is LS016B8UY */
  return (LS016B8UY_ID);
}

/**
  * @brief  Set Cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void ls016b8uy_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  uint8_t   parameter[4];

  parameter[0] = 0x00;     /* CASET */
  parameter[1] = 0x1E + Xpos;
  parameter[2] = 0x00;
  parameter[3] = 0xD1 + Xpos;
  ls016b8uy_WriteReg(LCD_CMD_CASET, parameter, 4);
  parameter[0] = 0x00;     /* RASET */
  parameter[1] = 0x00 + Ypos;
  parameter[2] = 0x00;
  parameter[3] = 0xB3 + Ypos;
  ls016b8uy_WriteReg(LCD_CMD_RASET, parameter, 4);
}

/**
  * @brief  Write pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  RGBCode: the RGB pixel color in RGB565 format
  * @retval None
  */
void ls016b8uy_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  uint16_t r, g, b;
  uint16_t rgb888_part1, rgb888_part2;

  r = (RGBCode & 0xF800) >> 11;        /* Extract red component from RGB565 pixel data */
  g = (RGBCode & 0x07E0) >> 5;         /* Extract green component from RGB565 pixel data  */
  b = (RGBCode & 0x001F) >> 0;         /* Extract blue component from RGB565 pixel data  */

  /* Prepare data to write with new pixel components and read old pixel component */
  rgb888_part1 = (r << 11) + (g << 2);
  rgb888_part2 = (b << 11);

  /* Set Cursor */
  ls016b8uy_SetCursor(Xpos, Ypos);

  /* Prepare to write to LCD RAM */
  ls016b8uy_WriteReg(LCD_CMD_WRITE_RAM, (uint8_t*)NULL, 0);   /* RAM write data command */

  /* Write RAM data */
  LCD_IO_WriteData(rgb888_part1);
  LCD_IO_WriteData(rgb888_part2);
}

/**
  * @brief  Read pixel.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval The RGB pixel color in RGB565 format
  */
uint16_t ls016b8uy_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  LS016B8UY_Rgb888 rgb888;
  uint8_t          r, g, b;
  uint16_t         rgb565;

  /* Set Cursor */
  ls016b8uy_SetCursor(Xpos, Ypos);
  
  /* Read RGB888 data from LCD RAM */
  rgb888 = ls016b8uy_ReadPixel_rgb888(Xpos, Ypos);
  
  /* Convert RGB888 to RGB565 */
  r = ((rgb888.red & 0xF8) >> 3);    /* Extract the red component 5 most significant bits */
  g = ((rgb888.green & 0xFC) >> 2);  /* Extract the green component 6 most significant bits */
  b = ((rgb888.blue & 0xF8) >> 3);   /* Extract the blue component 5 most significant bits */

  rgb565 = ((uint16_t)(r) << 11) + ((uint16_t)(g) << 5) + ((uint16_t)(b) << 0);
  
  return (rgb565);
}

/**
  * @brief  Writes to the selected LCD register.
  * @param  Command: command value (or register address as named in LS016B8UY doc).
  * @param  Parameters: pointer on parameters value (if command uses one or several parameters).
  * @param  NbParameters: number of command parameters (0 if no parameter)
  * @retval None
  */
void ls016b8uy_WriteReg(uint8_t Command, uint8_t *Parameters, uint8_t NbParameters)
{
  uint8_t   i;

  /* Send command */
  LCD_IO_WriteReg(Command);
  
  /* Send command's parameters if any */
  for (i=0; i<NbParameters; i++)
  {
    LCD_IO_WriteData(Parameters[i]);
  }
}

/**
  * @brief  Reads the selected LCD Register.
  * @param  Command: command value (or register address as named in LS016B8UY doc).
  * @retval Register Value.
  */
uint8_t ls016b8uy_ReadReg(uint8_t Command)
{
  /* Send command */
  LCD_IO_WriteReg(Command);

  /* Read dummy data */
  LCD_IO_ReadData();
  
  /* Read register value */
  return (LCD_IO_ReadData());
}

/**
  * @brief  Sets a display window
  * @param  Xpos:   specifies the X bottom left position.
  * @param  Ypos:   specifies the Y bottom left position.
  * @param  Height: display window height.
  * @param  Width:  display window width.
  * @retval None
  */
void ls016b8uy_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  if (Xpos < LS016B8UY_LCD_PIXEL_WIDTH)
  {
    WindowsXstart = Xpos;
  }
  else
  {
    WindowsXstart = 0;
  }

  if (Ypos < LS016B8UY_LCD_PIXEL_HEIGHT)
  {
    WindowsYstart = Ypos;
  }
  else
  {
    WindowsYstart = 0;
  }

  if (Width  + Xpos <= LS016B8UY_LCD_PIXEL_WIDTH)
  {
    WindowsXend = Width  + Xpos - 1;
  }
  else
  {
    WindowsXend = LS016B8UY_LCD_PIXEL_WIDTH - 1;
  }

  if (Height + Ypos <= LS016B8UY_LCD_PIXEL_HEIGHT)
  {
    WindowsYend = Height + Ypos - 1;
  }
  else
  {
    WindowsYend = LS016B8UY_LCD_PIXEL_HEIGHT-1;
  }
}

/**
  * @brief  Draw vertical line.
  * @param  RGBCode: Specifies the RGB color in RGB565 format
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Length:   specifies the Line length.  
  * @retval None
  */
void ls016b8uy_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;
  uint16_t r, g, b;
  uint16_t rgb888_part1, rgb888_part2, rgb888_part3;
  
  r = (RGBCode & 0xF800) >> 11;        /* Extract red component from RGB565 pixel data */
  g = (RGBCode & 0x07E0) >> 5;         /* Extract green component from RGB565 pixel data  */
  b = (RGBCode & 0x001F) >> 0;         /* Extract blue component from RGB565 pixel data  */

  rgb888_part1 = (r << 11) + (g << 2); /* Build pattern first part to write in LCD RAM */
  rgb888_part2 = (b << 11) + (r << 3); /* Build pattern second part to write in LCD RAM */
  rgb888_part3 = (g << 10) + (b << 3); /* Build pattern third part to write in LCD RAM */

  /* Set Cursor */
  ls016b8uy_SetCursor(Xpos, Ypos); 
  
  /* Prepare to write to LCD RAM */
  ls016b8uy_WriteReg(LCD_CMD_WRITE_RAM, (uint8_t*)NULL, 0);   /* RAM write data command */

  /* Sent a complete line */
  for(counter = 0; counter < Length; counter+=2)
  {
    /* Write 2 pixels at a time by performing 3 access (pixels coded on 24 bits in LCD RAM whereas access are coded on 16 bits) */
    LCD_IO_WriteData(rgb888_part1);
    LCD_IO_WriteData(rgb888_part2);
    if (counter != (Length-1))  /* When writing last pixel when Length is odd, the third part is not written */
    {
      LCD_IO_WriteData(rgb888_part3);
    }
  }  
}

/**
  * @brief  Draw vertical line.
  * @param  RGBCode: Specifies the RGB color    
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Length:   specifies the Line length.  
  * @retval None
  */
void ls016b8uy_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;

  /* Set Cursor */
  ls016b8uy_SetCursor(Xpos, Ypos);
  
  /* Prepare to write to LCD RAM */
  ls016b8uy_WriteReg(LCD_CMD_WRITE_RAM, (uint8_t*)NULL, 0);   /* RAM write data command */

  /* Fill a complete vertical line */
  for(counter = 0; counter < Length; counter++)
  {
    ls016b8uy_WritePixel(Xpos, Ypos + counter, RGBCode);
  }
}

/**
  * @brief  Displays a bitmap picture.
  * @param  BmpAddress: Bmp picture address.
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD    
  * @retval None
  */
void ls016b8uy_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, size = 0;
  uint32_t posY;
  uint32_t nb_line = 0;
  uint16_t Xsize = WindowsXend - WindowsXstart + 1;
  uint16_t Ysize = WindowsYend - WindowsYstart + 1;

  /* Read bitmap size */
  size = *(volatile uint16_t *) (pbmp + 2);
  size |= (*(volatile uint16_t *) (pbmp + 4)) << 16;
  /* Get bitmap data address offset */
  index = *(volatile uint16_t *) (pbmp + 10);
  index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;
  size = (size - index)/2;
  pbmp += index;

  for (posY = (Ypos + Ysize); posY > Ypos; posY--)  /* In BMP files the line order is inverted */
  {
    /* Set Cursor */
    ls016b8uy_SetCursor(Xpos, posY - 1);

    /* Draw one line of the picture */
    ls016b8uy_DrawRGBHLine(Xpos, posY - 1, Xsize, (pbmp + (nb_line * Xsize * 2)));
    nb_line++;
  }
}

/**
  * @brief  Displays picture.
  * @param  pdata: picture address.
  * @param  Xpos: Image X position in the LCD
  * @param  Ypos: Image Y position in the LCD
  * @param  Xsize: Image X size in the LCD
  * @param  Ysize: Image Y size in the LCD
  * @retval None
  */
void ls016b8uy_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{
  uint32_t posY;
  uint32_t nb_line = 0;

  for (posY = Ypos; posY < (Ypos + Ysize); posY ++)
  {
    /* Set Cursor */
    ls016b8uy_SetCursor(Xpos, posY);

    /* Draw one line of the picture */
    ls016b8uy_DrawRGBHLine(Xpos, posY, Xsize, (pdata + (nb_line * Xsize * 2)));
    nb_line++;
  }
}

/******************************************************************************
                            Static Functions
*******************************************************************************/

/**
  * @brief  Read pixel from LCD RAM in RGB888 format
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval Each RGB pixel color components in a structure
  */
static LS016B8UY_Rgb888 ls016b8uy_ReadPixel_rgb888(uint16_t Xpos, uint16_t Ypos)
{
  LS016B8UY_Rgb888 rgb888;
  uint16_t         rgb888_part1, rgb888_part2;

  /* In LCD RAM, pixels are 24 bits packed and read with 16 bits access
   * Here is the pixels components arrangement in memory :
   *       bits:  15 14 13 12 11 10 09 08 | 07 06 05 04 03 02 01 00
   * address 0 :     red pixel 0    X  X  |   green pixel 0   X  X
   * address 1 :    blue pixel 0    X  X  |     red pixel 1   X  X
   * address 2 :   green pixel 1    X  X  |    blue pixel 1   X  X
   */

  /* Set Cursor */
  ls016b8uy_SetCursor(Xpos, Ypos);
  /* Prepare to read LCD RAM */
  ls016b8uy_WriteReg(LCD_CMD_READ_RAM, (uint8_t*)NULL, 0);   /* RAM read data command */
  /* Dummy read */
  LCD_IO_ReadData();
  /* Read first part of the RGB888 data */
  rgb888_part1 = LCD_IO_ReadData();
  /* Read first part of the RGB888 data */
  rgb888_part2 = LCD_IO_ReadData();

  /* red component */
  rgb888.red   = (rgb888_part1 & 0xFC00) >> 8;
  /* green component */
  rgb888.green = (rgb888_part1 & 0x00FC) >> 0;
  /* blue component */
  rgb888.blue  = (rgb888_part2 & 0xFC00) >> 8;

  return rgb888;
}

/**
  * @brief  Displays a single picture line.
  * @param  pdata: picture address.
  * @param  Xpos: Image X position in the LCD
  * @param  Ypos: Image Y position in the LCD
  * @param  Xsize: Image X size in the LCD
  * @retval None
  */
static void ls016b8uy_DrawRGBHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint8_t *pdata)
{
  uint32_t i = 0;
  uint32_t posX;
  uint16_t r, g, b;
  uint16_t rgb888_part;
  uint16_t *rgb565 = (uint16_t*)pdata;

  /* Prepare to write to LCD RAM */
  ls016b8uy_WriteReg(LCD_CMD_WRITE_RAM, (uint8_t*)NULL, 0);   /* RAM write data command */

  for (posX = Xpos; posX < (Xsize + Xpos); posX += 2)
  {
    if ((posX >= WindowsXstart) && (Ypos >= WindowsYstart) &&     /* Check we are in the defined window */
        (posX <= WindowsXend) && (Ypos <= WindowsYend))
    {
      /* Write pixels in LCD RAM after RGB565 -> RGB888 conversion */
      /* As data in LCD RAM are 24bits packed, three 16 bits writes access are needed to transmit 2 pixels data */

      r = (rgb565[i] & 0xF800) >> 11;      /* Extract red component from first RGB565 pixel data */
      g = (rgb565[i] & 0x07E0) >> 5;       /* Extract green component from first RGB565 pixel data  */
      rgb888_part = (r << 11) + (g << 2);  /* Build data to be written in LCD RAM */
      LCD_IO_WriteData(rgb888_part);

      b = (rgb565[i] & 0x001F) >> 0;       /* Extract blue component from first RGB565 pixel data  */
      r = (rgb565[i+1] & 0xF800) >> 11;    /* Extract red component from second RGB565 pixel data  */
      rgb888_part = (b << 11) + (r << 3);  /* Build data to be written in LCD RAM */
      LCD_IO_WriteData(rgb888_part);

      if (posX != (Xsize + Xpos - 1))     /* When writing last pixel when size is odd, the third part is not written */
      {
        g = (rgb565[i+1] & 0x07E0) >> 5;     /* Extract green component from second RGB565 pixel data */
        b = (rgb565[i+1] & 0x001F) >> 0;     /* Extract blue component from second RGB565 pixel data */
        rgb888_part = (g << 10) + (b << 3);  /* Build data to be written in LCD RAM */
        LCD_IO_WriteData(rgb888_part);
      }

      i += 2;
    }
  }
}

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
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
