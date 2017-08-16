/**
  ******************************************************************************
  * @file    TIMER/TIMER2_OCActive/main.c
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
  * @brief  Configure system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* GPIOA, GPIOC and AF clocks enable */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOC |
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

    /* Configure PC0 as PP output */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    /*Configure PA0(TIMER2 CH1) as alternate function*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    /*Configure PA1(TIMER2 CH2) as alternate function*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    /*Configure PA2(TIMER2 CH3) as alternate function*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/**
  * @brief  Configure the TIMER peripheral.
  * @param  None
  * @retval None
  */
void TIM_Configuration(void)
{
    /* ---------------------------------------------------------------
    TIMER2 Configuration: 
    TIMER2CLK = SystemCoreClock / 54000 = 2K,
    And generate 3 signals with 3 different delays:
    TIMER2_CH1 delay = 4000/2000  = 2s
    TIMER2_CH2 delay = 8000/2000  = 4s
    TIMER2_CH3 delay = 12000/2000 = 6s
    --------------------------------------------------------------- */
    TIMER_BaseInitPara TIM_TimeBaseStructure;
    TIMER_OCInitPara  TIM_OCInitStructure;

    /* TIMER2 clock enable */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_TIMER2, ENABLE);

    /* TIMER2  configuration */
    TIMER_DeInit(TIMER2);
    TIM_TimeBaseStructure.TIMER_Prescaler     = 53999;
    TIM_TimeBaseStructure.TIMER_CounterMode   = TIMER_COUNTER_UP;
    TIM_TimeBaseStructure.TIMER_Period        = 19999;
    TIM_TimeBaseStructure.TIMER_ClockDivision = TIMER_CDIV_DIV1;
    TIMER_BaseInit(TIMER2,&TIM_TimeBaseStructure);

    /* CH1,CH2 and CH3 Configuration in OC_ACTIVE mode */
    TIM_OCInitStructure.TIMER_OCMode      = TIMER_OC_MODE_ACTIVE;
    TIM_OCInitStructure.TIMER_OCPolarity  = TIMER_OC_POLARITY_HIGH;
    TIM_OCInitStructure.TIMER_OutputState = TIMER_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.TIMER_OCIdleState = TIMER_OC_IDLE_STATE_RESET;
      
    TIM_OCInitStructure.TIMER_Pulse = 3999;
    TIMER_OC1_Init(TIMER2, &TIM_OCInitStructure);
    TIMER_OC1_Preload(TIMER2,TIMER_OC_PRELOAD_DISABLE);

    TIM_OCInitStructure.TIMER_Pulse = 7999;
    TIMER_OC2_Init(TIMER2, &TIM_OCInitStructure);
    TIMER_OC2_Preload(TIMER2,TIMER_OC_PRELOAD_DISABLE);

    TIM_OCInitStructure.TIMER_Pulse = 11999;
    TIMER_OC3_Init(TIMER2, &TIM_OCInitStructure);
    TIMER_OC3_Preload(TIMER2,TIMER_OC_PRELOAD_DISABLE);

    /* Auto-reload preload enable */
    TIMER_CARLPreloadConfig(TIMER2,ENABLE);
    /* TIMER enable counter*/
    TIMER_Enable( TIMER2, ENABLE );
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
