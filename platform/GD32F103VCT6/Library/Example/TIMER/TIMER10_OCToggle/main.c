/**
  ******************************************************************************
  * @file    TIMER/TIMER10_OCToggle/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <gd32f10x.h>
#include "systick.h"
#include <stdio.h>

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM_Configuration(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configures system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* GPIOB ,GPIOC and AF clocks enable */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOB | RCC_APB2PERIPH_GPIOC |
                                RCC_APB2PERIPH_AF, ENABLE);
}

/**
  * @brief  Configure the GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    /* Configure PC6 as PP output */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    /* Configure PB8(TIMER10 CH1) as alternate function */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}

/**
  * @brief  Configure the TIMER peripheral.
  * @param  None
  * @retval None
  */
void TIM_Configuration(void)
{
    /* ---------------------------------------------------------------------------
    TIMER10 Configuration: Output Compare Toggle Mode:
    TIMER10CLK = SystemCoreClock / 54000=2K
    TIMER10 CH1 update rate = TIMER10 counter clock / CHCC1 = 2000/2000 = 1 Hz
    ----------------------------------------------------------------------------*/
    TIMER_BaseInitPara TIM_TimeBaseStructure;
    TIMER_OCInitPara  TIM_OCInitStructure;

    /* TIMER10 clock enable */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_TIMER10,ENABLE);

    /* TIMER10  configuration */
    TIMER_DeInit(TIMER10);
    TIM_TimeBaseStructure.TIMER_Prescaler     = 53999;
    TIM_TimeBaseStructure.TIMER_CounterMode   = TIMER_COUNTER_UP;
    TIM_TimeBaseStructure.TIMER_Period        = 1999;
    TIM_TimeBaseStructure.TIMER_ClockDivision = TIMER_CDIV_DIV1;
    TIMER_BaseInit(TIMER10,&TIM_TimeBaseStructure);

    /* TIMER10 CH1 Configuration in OC_TOGGLE mode */
    TIM_OCInitStructure.TIMER_OCMode      = TIMER_OC_MODE_TOGGLE;
    TIM_OCInitStructure.TIMER_OCPolarity  = TIMER_OC_POLARITY_HIGH;
    TIM_OCInitStructure.TIMER_OutputState = TIMER_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.TIMER_OCIdleState = TIMER_OC_IDLE_STATE_RESET;
    TIM_OCInitStructure.TIMER_Pulse       = 999;
    TIMER_OC1_Init(TIMER10, &TIM_OCInitStructure);
    TIMER_OC1_Preload(TIMER10,TIMER_OC_PRELOAD_DISABLE);

    /* Auto-reload preload enable */
    TIMER_CARLPreloadConfig(TIMER10,ENABLE);
    /* TIMER10 enable counter*/
    TIMER_Enable(TIMER10, ENABLE);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main()
{
    RCC_Configuration();
    GPIO_Configuration();
    SysTick_Configuration();
    TIM_Configuration();

    while(1)
    {
        GPIO_SetBits(GPIOC,GPIO_PIN_0);
        Delay_1ms(1000);
        GPIO_ResetBits(GPIOC, GPIO_PIN_0);
        Delay_1ms(1000);
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
