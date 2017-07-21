/**
  ******************************************************************************
  * @file    st7789h2.c
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    29-December-2016
  * @brief   This file includes the LCD driver for st7789h2 LCD.
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
#include "st7789h2.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @defgroup ST7789H2
  * @brief     This file provides a set of functions needed to drive the 
  *            FRIDA FRD154BP2901 LCD.
  * @{
  */

/** @defgroup ST7789H2_Private_TypesDefinitions ST7789H2 Private TypesDefinitions
  * @{
  */ 
typedef struct  {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} ST7789H2_Rgb888;

/**
  * @}
  */ 

/** @defgroup ST7789H2_Private_Defines ST7789H2 Private Defines
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup ST7789H2_Private_Macros ST7789H2 Private Macros
  * @{
  */
     
/**
  * @}
  */  

/** @defgroup ST7789H2_Private_Variables ST7789H2 Private Variables
  * @{
  */ 
LCD_DrvTypeDef   ST7789H2_drv = 
{
  ST7789H2_Init,
  ST7789H2_ReadID,
  ST7789H2_DisplayOn,
  ST7789H2_DisplayOff,
  ST7789H2_SetCursor,
  ST7789H2_WritePixel,
  ST7789H2_ReadPixel,
  ST7789H2_SetDisplayWindow,
  ST7789H2_DrawHLine,
  ST7789H2_DrawVLine,
  ST7789H2_GetLcdPixelWidth,
  ST7789H2_GetLcdPixelHeight,
  ST7789H2_DrawBitmap,
  ST7789H2_DrawRGBImage,  
};

static uint16_t WindowsXstart = 0;
static uint16_t WindowsYstart = 0;
static uint16_t WindowsXend = ST7789H2_LCD_PIXEL_WIDTH-1;
static uint16_t WindowsYend = ST7789H2_LCD_PIXEL_HEIGHT-1;
/**
  * @}
  */ 
  
/** @defgroup ST7789H2_Private_FunctionPrototypes ST7789H2 Private FunctionPrototypes
  * @{
  */
static ST7789H2_Rgb888 ST7789H2_ReadPixel_rgb888(uint16_t Xpos, uint16_t Ypos);
static void ST7789H2_DrawRGBHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint8_t *pdata);

/**
  * @}
  */ 
  
/** @defgroup ST7789H2_Private_Functions ST7789H2 Private Functions
  * @{
  */   

/**
  * @brief  Initialize the st7789h2 LCD Component.
  * @param  None
  * @retval None
  */
void ST7789H2_Init(void)
{
  uint8_t   parameter[14];
  
  /* Initialize st7789h2 low level bus layer ----------------------------------*/
  LCD_IO_Init();
  /* Sleep In Command */ 
  ST7789H2_WriteReg(ST7789H2_SLEEP_IN, (uint8_t*)NULL, 0); 
  /* Wait for 10ms */
  LCD_IO_Delay(10);  
  
  /* SW Reset Command */
  ST7789H2_WriteReg(0x01, (uint8_t*)NULL, 0); 
  /* Wait for 200ms */
  LCD_IO_Delay(200);
  
  /* Sleep Out Command */
  ST7789H2_WriteReg(ST7789H2_SLEEP_OUT, (uint8_t*)NULL, 0); 
  /* Wait for 120ms */
  LCD_IO_Delay(120); 

  /* Normal display for Driver Down side */
  parameter[0] = 0x00;     
  ST7789H2_WriteReg(ST7789H2_NORMAL_DISPLAY, parameter, 1);
 
  /* Color mode 16bits/pixel */
  parameter[0] = 0x05;     
  ST7789H2_WriteReg(ST7789H2_COLOR_MODE, parameter, 1);
  
  /* Display inversion On */
  ST7789H2_WriteReg(ST7789H2_DISPLAY_INVERSION, (uint8_t*)NULL, 0);     
  
  /* Set Column address CASET */  
  parameter[0] = 0x00;
  parameter[1] = 0x00;
  parameter[2] = 0x00;
  parameter[3] = 0xEF;
  ST7789H2_WriteReg(ST7789H2_CASET, parameter, 4);
  /* Set Row address RASET */  
  parameter[0] = 0x00;
  parameter[1] = 0x00;
  parameter[2] = 0x00;
  parameter[3] = 0xEF;
  ST7789H2_WriteReg(ST7789H2_RASET, parameter, 4);

  /*--------------- ST7789H2 Frame rate setting -------------------------------*/
  /* PORCH control setting */      
  parameter[0] = 0x0C;
  parameter[1] = 0x0C;
  parameter[2] = 0x00;
  parameter[3] = 0x33;
  parameter[4] = 0x33; 
  ST7789H2_WriteReg(ST7789H2_PORCH_CTRL, parameter, 5);
  
  /* GATE control setting */
  parameter[0] = 0x35; 
  ST7789H2_WriteReg(ST7789H2_GATE_CTRL, parameter, 1);
  
  /*--------------- ST7789H2 Power setting ------------------------------------*/
  /* VCOM setting */ 
  parameter[0] = 0x1F; 
  ST7789H2_WriteReg(ST7789H2_VCOM_SET, parameter, 1); 
  
  /* LCM Control setting */ 
  parameter[0] = 0x2C; 
  ST7789H2_WriteReg(ST7789H2_LCM_CTRL, parameter, 1);
  
  /* VDV and VRH Command Enable */ 
  parameter[0] = 0x01;
  parameter[1] = 0xC3;
  ST7789H2_WriteReg(ST7789H2_VDV_VRH_EN, parameter, 2);
  
  /* VDV Set */ 
  parameter[0] = 0x20; 
  ST7789H2_WriteReg(ST7789H2_VDV_SET, parameter, 1); 
  
  /* Frame Rate Control in normal mode */ 
  parameter[0] = 0x0F; 
  ST7789H2_WriteReg(ST7789H2_FR_CTRL, parameter, 1); 
  
  /* Power Control */     
  parameter[0] = 0xA4;
  parameter[1] = 0xA1;
  ST7789H2_WriteReg(ST7789H2_POWER_CTRL, parameter, 1); 
  
  /*--------------- ST7789H2 Gamma setting ------------------------------------*/
  /* Positive Voltage Gamma Control */ 
  parameter[0] = 0xD0;
  parameter[1] = 0x08;
  parameter[2] = 0x11;
  parameter[3] = 0x08;
  parameter[4] = 0x0C;
  parameter[5] = 0x15;
  parameter[6] = 0x39;
  parameter[7] = 0x33;
  parameter[8] = 0x50;
  parameter[9] = 0x36;
  parameter[10] = 0x13;
  parameter[11] = 0x14;
  parameter[12] = 0x29;
  parameter[13] = 0x2D;
  ST7789H2_WriteReg(ST7789H2_PV_GAMMA_CTRL, parameter, 14); 
  
  /* Negative Voltage Gamma Control */     
  parameter[0] = 0xD0;
  parameter[1] = 0x08;
  parameter[2] = 0x10;
  parameter[3] = 0x08;
  parameter[4] = 0x06;
  parameter[5] = 0x06;
  parameter[6] = 0x39;
  parameter[7] = 0x44;
  parameter[8] = 0x51;
  parameter[9] = 0x0B;
  parameter[10] = 0x16;
  parameter[11] = 0x14;
  parameter[12] = 0x2F;
  parameter[13] = 0x31;
  ST7789H2_WriteReg(ST7789H2_NV_GAMMA_CTRL, parameter, 14); 
  
  /* Display ON command */
  ST7789H2_DisplayOn();  
  
  /* Tearing Effect Line On: Option (00h:VSYNC Interface OFF, 01h:VSYNC Interface ON) */
  parameter[0] = 0x00;     
  ST7789H2_WriteReg(ST7789H2_TEARING_EFFECT, parameter, 1);

}

/**
  * @brief  Set the Display Orientation.
  * @param  orientation: ST7789H2_ORIENTATION_PORTRAIT, ST7789H2_ORIENTATION_LANDSCAPE
  *                      or ST7789H2_ORIENTATION_LANDSCAPE_ROT180  
  * @retval None
  */
void ST7789H2_SetOrientation(uint32_t orientation)
{
  uint8_t   parameter[6];

  if(orientation == ST7789H2_ORIENTATION_LANDSCAPE)
  {
    parameter[0] = 0x00;     
  }
  else if(orientation == ST7789H2_ORIENTATION_LANDSCAPE_ROT180)
  {
    /* Vertical Scrolling Definition */
    /* TFA describes the Top Fixed Area */
    parameter[0] = 0x00;
    parameter[1] = 0x00;
    /* VSA describes the height of the Vertical Scrolling Area */
    parameter[2] = 0x01;
    parameter[3] = 0xF0;
    /* BFA describes the Bottom Fixed Area */
    parameter[4] = 0x00;
    parameter[5] = 0x00; 
    ST7789H2_WriteReg(ST7789H2_VSCRDEF, parameter, 6);

    /* Vertical Scroll Start Address of RAM */
    /* GRAM row nbr (320) - Display row nbr (240) = 80 = 0x50 */
    parameter[0] = 0x00;
    parameter[1] = 0x50;
    ST7789H2_WriteReg(ST7789H2_VSCSAD, parameter, 2);
    
    parameter[0] = 0xC0; 
  }
  else
  {
    parameter[0] = 0x60;     
  }
  ST7789H2_WriteReg(ST7789H2_NORMAL_DISPLAY, parameter, 1);
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void ST7789H2_DisplayOn(void)
{
  /* Display ON command */
  ST7789H2_WriteReg(ST7789H2_DISPLAY_ON, (uint8_t*)NULL, 0);

  /* Sleep Out command */
  ST7789H2_WriteReg(ST7789H2_SLEEP_OUT, (uint8_t*)NULL, 0);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void ST7789H2_DisplayOff(void)
{
  uint8_t   parameter[1];
  parameter[0] = 0xFE;
  /* Display OFF command */
  ST7789H2_WriteReg(ST7789H2_DISPLAY_OFF, parameter, 1);  
  /* Sleep In Command */
  ST7789H2_WriteReg(ST7789H2_SLEEP_IN, (uint8_t*)NULL, 0); 
  /* Wait for 10ms */
  LCD_IO_Delay(10);  
}

/**
  * @brief  Get the LCD pixel Width.
  * @param  None
  * @retval The Lcd Pixel Width
  */
uint16_t ST7789H2_GetLcdPixelWidth(void)
{
 return (uint16_t)ST7789H2_LCD_PIXEL_WIDTH;
}

/**
  * @brief  Get the LCD pixel Height.
  * @param  None
  * @retval The Lcd Pixel Height
  */
uint16_t ST7789H2_GetLcdPixelHeight(void)
{
 return (uint16_t)ST7789H2_LCD_PIXEL_HEIGHT;
}

/**
  * @brief  Get the st7789h2 ID.
  * @param  None
  * @retval The st7789h2 ID 
  */
uint16_t ST7789H2_ReadID(void)
{
  LCD_IO_Init();
  
  return ST7789H2_ReadReg(ST7789H2_LCD_ID);
}

/**
  * @brief  Set Cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void ST7789H2_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  uint8_t   parameter[4];
  /* CASET: Comumn Addrses Set */
  parameter[0] = 0x00;     
  parameter[1] = 0x00 + Xpos;
  parameter[2] = 0x00;
  parameter[3] = 0xEF + Xpos;
  ST7789H2_WriteReg(ST7789H2_CASET, parameter, 4);
  /* RASET: Row Addrses Set */  
  parameter[0] = 0x00;
  parameter[1] = 0x00 + Ypos;
  parameter[2] = 0x00;
  parameter[3] = 0xEF + Ypos;
  ST7789H2_WriteReg(ST7789H2_RASET, parameter, 4);
}

/**
  * @brief  Write pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  RGBCode: the RGB pixel color in RGB565 format
  * @retval None
  */
void ST7789H2_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  /* Set Cursor */
  ST7789H2_SetCursor(Xpos, Ypos);

  /* Prepare to write to LCD RAM */
  ST7789H2_WriteReg(ST7789H2_WRITE_RAM, (uint8_t*)NULL, 0);   /* RAM write data command */

  /* Write RAM data */
  LCD_IO_WriteData(RGBCode);
}

/**
  * @brief  Read pixel.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval The RGB pixel color in RGB565 format
  */
uint16_t ST7789H2_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  ST7789H2_Rgb888 rgb888;
  uint8_t          r, g, b;
  uint16_t         rgb565;

  /* Set Cursor */
  ST7789H2_SetCursor(Xpos, Ypos);
  
  /* Read RGB888 data from LCD RAM */
  rgb888 = ST7789H2_ReadPixel_rgb888(Xpos, Ypos);
  
  /* Convert RGB888 to RGB565 */
  r = ((rgb888.red & 0xF8) >> 3);    /* Extract the red component 5 most significant bits */
  g = ((rgb888.green & 0xFC) >> 2);  /* Extract the green component 6 most significant bits */
  b = ((rgb888.blue & 0xF8) >> 3);   /* Extract the blue component 5 most significant bits */

  rgb565 = ((uint16_t)(r) << 11) + ((uint16_t)(g) << 5) + ((uint16_t)(b) << 0);
  
  return (rgb565);
}

/**
  * @brief  Writes to the selected LCD register.
  * @param  Command: command value (or register address as named in st7789h2 doc).
  * @param  Parameters: pointer on parameters value (if command uses one or several parameters).
  * @param  NbParameters: number of command parameters (0 if no parameter)
  * @retval None
  */
void ST7789H2_WriteReg(uint8_t Command, uint8_t *Parameters, uint8_t NbParameters)
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
  * @param  Command: command value (or register address as named in st7789h2 doc).
  * @retval Register Value.
  */
uint8_t ST7789H2_ReadReg(uint8_t Command)
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
void ST7789H2_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  if (Xpos < ST7789H2_LCD_PIXEL_WIDTH)
  {
    WindowsXstart = Xpos;
  }
  else
  {
    WindowsXstart = 0;
  }

  if (Ypos < ST7789H2_LCD_PIXEL_HEIGHT)
  {
    WindowsYstart = Ypos;
  }
  else
  {
    WindowsYstart = 0;
  }

  if (Width  + Xpos <= ST7789H2_LCD_PIXEL_WIDTH)
  {
    WindowsXend = Width  + Xpos - 1;
  }
  else
  {
    WindowsXend = ST7789H2_LCD_PIXEL_WIDTH - 1;
  }

  if (Height + Ypos <= ST7789H2_LCD_PIXEL_HEIGHT)
  {
    WindowsYend = Height + Ypos - 1;
  }
  else
  {
    WindowsYend = ST7789H2_LCD_PIXEL_HEIGHT-1;
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
void ST7789H2_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;
  
  /* Set Cursor */
  ST7789H2_SetCursor(Xpos, Ypos); 
  
  /* Prepare to write to LCD RAM */
  ST7789H2_WriteReg(ST7789H2_WRITE_RAM, (uint8_t*)NULL, 0);   /* RAM write data command */
  
  /* Sent a complete line */
  for(counter = 0; counter < Length; counter++)
  {
    LCD_IO_WriteData(RGBCode);
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
void ST7789H2_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;

  /* Set Cursor */
  ST7789H2_SetCursor(Xpos, Ypos);
  
  /* Prepare to write to LCD RAM */
  ST7789H2_WriteReg(ST7789H2_WRITE_RAM, (uint8_t*)NULL, 0);   /* RAM write data command */

  /* Fill a complete vertical line */
  for(counter = 0; counter < Length; counter++)
  {
    ST7789H2_WritePixel(Xpos, Ypos + counter, RGBCode);
  }
}

/**
  * @brief  Displays a bitmap picture.
  * @param  BmpAddress: Bmp picture address.
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD    
  * @retval None
  */
void ST7789H2_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
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
    ST7789H2_SetCursor(Xpos, posY - 1);

    /* Draw one line of the picture */
    ST7789H2_DrawRGBHLine(Xpos, posY - 1, Xsize, (pbmp + (nb_line * Xsize * 2)));
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
void ST7789H2_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{
  uint32_t posY;
  uint32_t nb_line = 0;

  for (posY = Ypos; posY < (Ypos + Ysize); posY ++)
  {
    /* Set Cursor */
    ST7789H2_SetCursor(Xpos, posY);

    /* Draw one line of the picture */
    ST7789H2_DrawRGBHLine(Xpos, posY, Xsize, (pdata + (nb_line * Xsize * 2)));
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
static ST7789H2_Rgb888 ST7789H2_ReadPixel_rgb888(uint16_t Xpos, uint16_t Ypos)
{
  ST7789H2_Rgb888 rgb888;
  uint16_t         rgb888_part1, rgb888_part2;

  /* In LCD RAM, pixels are 24 bits packed and read with 16 bits access
   * Here is the pixels components arrangement in memory :
   *       bits:  15 14 13 12 11 10 09 08 | 07 06 05 04 03 02 01 00
   * address 0 :     red pixel 0    X  X  |   green pixel 0   X  X
   * address 1 :    blue pixel 0    X  X  |     red pixel 1   X  X
   * address 2 :   green pixel 1    X  X  |    blue pixel 1   X  X
   */

  /* Set Cursor */
  ST7789H2_SetCursor(Xpos, Ypos);
  /* Prepare to read LCD RAM */
  ST7789H2_WriteReg(ST7789H2_READ_RAM, (uint8_t*)NULL, 0);   /* RAM read data command */
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
static void ST7789H2_DrawRGBHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint8_t *pdata)
{
  uint32_t i = 0;
  uint32_t posX;
  uint16_t *rgb565 = (uint16_t*)pdata;
  
  /* Prepare to write to LCD RAM */
  ST7789H2_WriteReg(ST7789H2_WRITE_RAM, (uint8_t*)NULL, 0);   /* RAM write data command */
  
  for (posX = Xpos; posX < (Xsize + Xpos); posX++)
  {
    if ((posX >= WindowsXstart) && (Ypos >= WindowsYstart) &&     /* Check we are in the defined window */
        (posX <= WindowsXend) && (Ypos <= WindowsYend))
    {
      if (posX != (Xsize + Xpos))     /* When writing last pixel when size is odd, the third part is not written */
      {
        LCD_IO_WriteData(rgb565[i]);        
      }      
      i++;
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
