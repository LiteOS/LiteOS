/**
  ******************************************************************************
  * @file    EXMC/EXMC_LCD/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "gd32f10x.h"
#include "EXMC_LCD.h"
#include "ili9320.h"
#include "picture.h"

/* Private variables ---------------------------------------------------------*/
uint8_t String[]={"www.gigadevice.com"} ;
uint16_t DeviceCode;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void Delay(uint16_t i);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    uint16_t i;

    /* Periph clock enable */
    RCC_Configuration();

    /* Config the EXMC access mode */
    EXMC_LCD_Init();

    /* Config the LCD GPIO */
    LCD_GPIO_Configuration();

    /* Read the LCD Controller device code:(0x8989) */
    DeviceCode = LCD_ReadReg(0x0000);

    /* Initize the LCD */
    ili9320_Initializtion();

    /* Clear the LCD screen */
    ili9320_Clear(White);
    
    /* Draw the picture of Gigadevice logo */
    /* if you don't want to draw the picture, you shoud modify the macro on 
       the line 423th of picture.c file and comment the next line */
    ili9320_DrawPicture(40,100,40+160-1,100+87-1,(uint16_t *)(picture + BmpHeadSize));

    /* Draw a rectangle */
    ili9320_SetRectangle(10,10,230,310,Blue);

    /* draw character on LCD screen */
    for (i = 0; i < sizeof(String)-1; i++)
    {
        ili9320_PutChar((50+8*i), 180+20, *(String+i), Blue, White);
    }
    while(1)
    {
    }
}

/**
  * @brief  Periph clock enable.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* Enable the GPIO Clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOD , ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOE , ENABLE);

    /* Enable the EXMC Clock */
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_EXMC, ENABLE);
}

/**
  * @brief  Delay function
  * @param  i
  * @retval None
  */
void Delay(uint16_t i)
{
    uint16_t k;
    for (k=0;k<i;k++);
}
/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
