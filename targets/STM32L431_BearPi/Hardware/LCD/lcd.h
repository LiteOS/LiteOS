/********************************************************************************
    * 文件名称 ：lcd.h
    * 作     者：物联网俱乐部
    * 版     本：V1.0
    * 编写日期 ：2018-4-1
    * 功     能：LCD屏幕驱动
*********************************************************************************
    * 说    明 ：本例程配套物联网俱乐部BearPi开发板使用
    *
    * 淘     宝：https://shop128001708.taobao.com/
    * 论     坛：bbs.iot-club.cn
*********************************************************************************/
#ifndef __LCD_H
#define __LCD_H
#include "stm32l4xx_hal.h"


extern uint16_t	POINT_COLOR;		//Default brush color
extern uint16_t	BACK_COLOR;		//Default background color

//Width and height definitions of LCD
#define LCD_Width 	240
#define LCD_Height 	240

//Brush color
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0 //黄色
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)



/*
	LCD_PWR:	PB15
	LCD_RST:	PC7
	LCD_DC:		PC6	
*/
#define	LCD_PWR(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET))
#define	LCD_RST(n)		(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET))
#define	LCD_DC(n)		(n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET))


void LCD_Init(void);																	//Init
void LCD_DisplayOn(void);																//Open display
void LCD_DisplayOff(void);																//Close display
void LCD_Write_HalfWord(const uint16_t da);												//Write half a byte of data to LCD
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);				//Setting up the data display area
void LCD_Clear(uint16_t color);															//Clean screen
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);				//Filled monochrome
void LCD_Draw_Point(uint16_t x, uint16_t y);														//Draw points
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y,uint16_t color);										//Painting with color dots
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);										//Draw line
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);									//Draw rectangle
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);												//Circle drawing
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size);										//Display a character
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);									//Display a number
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);							//Display number
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p);					//display string
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);					//display picture

#endif


