/**
  ******************************************************************************
  * @file    GPIO/GPIO_Toggle/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The main function file
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include <stdio.h>
#include "systick.h"

/* Private functions ---------------------------------------------------------*/
#define LED1(a) if (a)  \
                    GPIO_SetBits(GPIOC,GPIO_PIN_0);\
                    else    \
                    GPIO_ResetBits(GPIOC,GPIO_PIN_0)

#define LED2(a) if (a)  \
                    GPIO_SetBits(GPIOC,GPIO_PIN_2);\
                    else    \
                    GPIO_ResetBits(GPIOC,GPIO_PIN_2)

#define LED3(a) if (a)  \
                    GPIO_SetBits(GPIOE,GPIO_PIN_0);\
                    else    \
                    GPIO_ResetBits(GPIOE,GPIO_PIN_0)

#define LED4(a) if (a)  \
                    GPIO_SetBits(GPIOE,GPIO_PIN_1);\
                    else    \
                    GPIO_ResetBits(GPIOE,GPIO_PIN_1)

/* Private variables ---------------------------------------------------------*/
static vu32 TimingDelay = 0;

/**
  * @brief  Configure the GPIO ports
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{    
    GPIO_InitPara GPIO_InitStructure;
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC |RCC_APB2PERIPH_GPIOE,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_Init(GPIOE,&GPIO_InitStructure);
}

/**
  * @brief  Delay time
  * @param  data: The size of the delay time
  * @retval None
  */
void Delay(uint32_t data)
{
    TimingDelay = data;
    while(TimingDelay != 0)
    {
    }
}


/**
  * @brief  Decrements the TimingDelay variable
  * @param  None
  * @retval None
  */ 
void Decrement_TimingDelay(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    GPIO_Configuration();
    SysTick_Configuration();

    while (1)
    {
        LED1(1);
        LED2(1);
        LED3(1);
        LED4(1);
        Delay(1000);

        LED1(0);
        LED2(0);
        LED3(0);
        LED4(0);
        Delay(1000);

        LED1(1);
        LED4(0);
        Delay(1000);

        LED2(1);
        LED1(0);
        Delay(1000);

        LED3(1);
        LED2(0);
        Delay(1000);

        LED4(1);
        LED3(0);
        Delay(1000);
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
