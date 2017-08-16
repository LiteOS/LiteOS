/**
  ******************************************************************************
  * @file    EXMC/EXMC_LCD/ili9320.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   LCD driver functions.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "EXMC_LCD.h"
#include "ili9320.h"
#include "ili9320_font.h"
#include "picture.h"

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Write to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @param  LCD_RegValue: value to write to the selected register.
  * @retval None
  */
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
    *(__IO uint16_t *) (Bank1_LCD_C)= LCD_Reg;
    *(__IO uint16_t *) (Bank1_LCD_D)= LCD_RegValue;
}

/**
  * @brief  Read the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval LCD register value
  */
uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
    uint16_t data;
    data = *(__IO uint16_t *) (Bank1_LCD_D); 
    return  data;
}

/**
  * @brief  Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
void LCD_WriteRAM_Prepare(void)
{
    *(__IO uint16_t *) (Bank1_LCD_C) = R34;
}

/**
  * @brief  Write to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  */
void LCD_WriteRAM(uint16_t RGB_Code)
{
    /* Write 16-bit GRAM Reg */
    *(__IO uint16_t *) (Bank1_LCD_D) = RGB_Code;
}

/**
  * @brief  Initize the LCD.
  * @param  None
  * @retval None
  */
void ili9320_Initializtion(void)
{
    uint16_t i;

    if(1)  /*!< if(DeviceCode == 0x8989) */
    {
        LCD_WriteReg(0x0000,0x0001);
        LCD_WriteReg(0x0003,0xA8A4);
        LCD_WriteReg(0x000C,0x0000);
        LCD_WriteReg(0x000D,0x080C);
        LCD_WriteReg(0x000E,0x2B00);
        LCD_WriteReg(0x001E,0x00B0);
        LCD_WriteReg(0x0001,0x2B3F);
        LCD_WriteReg(0x0002,0x0600);
        LCD_WriteReg(0x0010,0x0000);
        LCD_WriteReg(0x0011,0x6070);
        LCD_WriteReg(0x0005,0x0000);
        LCD_WriteReg(0x0006,0x0000);
        LCD_WriteReg(0x0016,0xEF1C);
        LCD_WriteReg(0x0017,0x0003);
        LCD_WriteReg(0x0007,0x0233);
        LCD_WriteReg(0x000B,0x0000);
        LCD_WriteReg(0x000F,0x0000);
        LCD_WriteReg(0x0041,0x0000);
        LCD_WriteReg(0x0042,0x0000);
        LCD_WriteReg(0x0048,0x0000);
        LCD_WriteReg(0x0049,0x013F);
        LCD_WriteReg(0x004A,0x0000);
        LCD_WriteReg(0x004B,0x0000);
        LCD_WriteReg(0x0044,0xEF00);
        LCD_WriteReg(0x0045,0x0000);
        LCD_WriteReg(0x0046,0x013F);
        LCD_WriteReg(0x0030,0x0707);
        LCD_WriteReg(0x0031,0x0204);
        LCD_WriteReg(0x0032,0x0204);
        LCD_WriteReg(0x0033,0x0502);
        LCD_WriteReg(0x0034,0x0507);
        LCD_WriteReg(0x0035,0x0204);
        LCD_WriteReg(0x0036,0x0204);
        LCD_WriteReg(0x0037,0x0502);
        LCD_WriteReg(0x003A,0x0302);
        LCD_WriteReg(0x003B,0x0302);
        LCD_WriteReg(0x0023,0x0000);
        LCD_WriteReg(0x0024,0x0000);
        LCD_WriteReg(0x0025,0x8000);
        LCD_WriteReg(0x004e,0);
        LCD_WriteReg(0x004f,0);
    }
    else
    {
        return;
    }
    for(i=50000;i>0;i--);
}

/**
  * @brief  Set the curson of LCD.
  * @param  x: the row-coordinate.
  * @param  y: the column-coordinate.
  * @retval None
  */
void ili9320_SetCursor(uint16_t x,uint16_t y)
{
    LCD_WriteReg(0x004e,x);
    LCD_WriteReg(0x004f,y);

}

/**
  * @brief  Clear the LCD screen to the specified color.
  * @param  Color: specified  the screen color.
  * @retval None
  */
void ili9320_Clear(uint16_t Color)
{
    uint32_t index=0;
    ili9320_SetCursor(0,0);
    /* Prepare to write GRAM */
    LCD_WriteRAM_Prepare();
    for(index=0;index<76800;index++)
    {
        *(__IO uint16_t *) (Bank1_LCD_D) = Color;
    }
}

/**
  * @brief  Set point according to the specified position and color.
  * @param  x: the row-coordinate.
  * @param  y: the column-coordinate.
  * @param  point: specified the color of  point.
  * @retval None
  */
void ili9320_SetPoint(uint16_t x,uint16_t y,uint16_t point)
{
    if ((x > 240)||(y > 320)) return;
    ili9320_SetCursor(x,y);
    LCD_WriteRAM_Prepare();
    LCD_WriteRAM(point);
}

/**
  * @brief  Set a rectangle according to the specified position and color.
  * @param  start_x,start_y,end_x,end_y: position of rectangle.
  * @param  point: specified the color of rectangle frame.
  * @retval None
  */
void ili9320_SetRectangle(uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y,uint16_t point)
{
    uint16_t x,y;
    x=start_x;
    y=start_y;
    /* Draw four lines */
    for(x=start_x;x<end_x;x++)
    {
        /* Draw a point */
        ili9320_SetPoint(x,y,point);
    }
    for(y=start_y;y<end_y;y++)
    {
        ili9320_SetPoint(x,y,point);
    }
    for(x=end_x;x>start_x;x--)
    {
        ili9320_SetPoint(x,y,point);
    }
    for(y=end_y;y>start_y;y--)
    {
        ili9320_SetPoint(x,y,point);
    }
}

/**
  * @brief  Draw picture on LCD screen.
  * @param  StartX,StartY,EndX,EndY: position of picture.
  * @param  pic: the picture pointer.
  * @retval None
  */
void ili9320_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t EndX,uint16_t EndY,uint16_t *pic)
{
    uint32_t i, total;
    uint16_t *picturepointer = pic;
    uint16_t x,y;

    x = StartX;
    y = StartY;

    total = (EndX - StartX + 1) * (EndY - StartY + 1);
    
    for(i = 0; i < total; i ++)
    {
        /* Set point according to the specified position and color */
        ili9320_SetPoint(x,y,*picturepointer++);
        x++;
        if(x > EndX)
        {
            y++;
            x = StartX;
        }
    }
}

/**
  * @brief  Put char on LCD screen.
  * @param  x,y: start position of char.
  * @param  c: the char.
  * @param  charColor: the color of char.
  * @param  bkColor: the color of backgroud.
  * @retval None
  */
void ili9320_PutChar(uint16_t x,uint16_t y,uint8_t c,uint16_t charColor,uint16_t bkColor)
{
    uint16_t i=0;
    uint16_t j=0;
    uint8_t tmp_char=0;

    if(HyalineBackColor == bkColor)
    {
        for (i=0;i<16;i++)
        {
            /* 8x16 ASCII */
            tmp_char=ascii_8x16[((c-0x20)*16)+i];
            for (j=0;j<8;j++)
            {
                if ( ((tmp_char >> (7-j)) & 0x01) == 0x01)
                {
                    /* Set point according to the specified position and character color */
                    ili9320_SetPoint(x+j,y+i,charColor);
                }
                else
                {
                /* Do nothing */
                }
            }
        }
    }
    else
    {
        for (i=0;i<16;i++)
        {
            /* 8x16 ASCII */
            tmp_char=ascii_8x16[((c-0x20)*16)+i];
            for (j=0;j<8;j++)
            {
                if ( ((tmp_char >> (7-j)) & 0x01) == 0x01)
                {
                    /* Set point according to the specified position and character color */
                    ili9320_SetPoint(x+j,y+i,charColor);
                }
                else
                {
                    /* Set point according to the specified position and background color */
                    ili9320_SetPoint(x+j,y+i,bkColor);
                }
            }
        }
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
