/**
  ******************************************************************************
  * @file    SysTick/Systick_demo/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The main function file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include <stdio.h>
#include "systick.h"

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure the GPIO ports.
  * @param  None
  * @retval None
  */
void LED_config(void)
{    
    GPIO_InitPara GPIO_InitStructure;

    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC,ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;

    GPIO_Init(GPIOC,&GPIO_InitStructure);
}


/**
  * @brief  Toggle the LED every 500ms.
  * @param  None
  * @retval None
  */
void LED_Spark(void)
{
    static __IO uint32_t TimingDelayLocal = 0;

    if (TimingDelayLocal != 0x00)
    {
        if(TimingDelayLocal < 500)
        {
           GPIO_ResetBits(GPIOC, GPIO_PIN_0);
        }
        else
        {
           GPIO_SetBits(GPIOC,GPIO_PIN_0);
        }
        TimingDelayLocal--;
    }
    else
    {
        TimingDelayLocal = 1000;
    }
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    LED_config();
    SysTick_Configuration();

    while (1)
    {
    }
}




/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/    
