/**
  ******************************************************************************
  * @file    TIMER/TIMER2_PWMOutput/main.c 
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

    /* Configure PC0 (LED2) as PP output */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    /*Configure PA1 PA2 PA3(TIMER2 CH2 CH3 CH4) as alternate function*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
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
    /* -----------------------------------------------------------------------
    TIMER2 Configuration: generate 3 PWM signals with 3 different duty cycles:
    TIMER2CLK = SystemCoreClock / 108 = 1MHz

    TIMER2 Channel2 duty cycle = (2500/ 10000)* 100 = 25%
    TIMER2 Channel3 duty cycle = (5000/ 10000)* 100 = 50%
    TIMER2 Channel4 duty cycle = (7500/ 10000)* 100 = 75%
    ----------------------------------------------------------------------- */
    TIMER_BaseInitPara TIMER_TimeBaseStructure;
    TIMER_OCInitPara   TIMER_OCInitStructure;

    /* TIMER2 clock enable */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_TIMER2,ENABLE);

    /* TIMER2  configuration */
    TIMER_DeInit(TIMER2);
    TIMER_TimeBaseStructure.TIMER_Prescaler     = 107;
    TIMER_TimeBaseStructure.TIMER_CounterMode   = TIMER_COUNTER_UP;
    TIMER_TimeBaseStructure.TIMER_Period        = 9999;
    TIMER_TimeBaseStructure.TIMER_ClockDivision = TIMER_CDIV_DIV1;
    TIMER_BaseInit(TIMER2,&TIMER_TimeBaseStructure);

    /* CH2,CH3 and CH4 Configuration in PWM mode */
    TIMER_OCInitStructure.TIMER_OCMode      = TIMER_OC_MODE_PWM1;
    TIMER_OCInitStructure.TIMER_OCPolarity  = TIMER_OC_POLARITY_HIGH;
    TIMER_OCInitStructure.TIMER_OutputState = TIMER_OUTPUT_STATE_ENABLE;
    TIMER_OCInitStructure.TIMER_OCIdleState = TIMER_OC_IDLE_STATE_RESET;

    TIMER_OCInitStructure.TIMER_Pulse = 2499;
    TIMER_OC2_Init(TIMER2, &TIMER_OCInitStructure);
    TIMER_OC2_Preload(TIMER2,TIMER_OC_PRELOAD_DISABLE);

    TIMER_OCInitStructure.TIMER_Pulse = 4999;
    TIMER_OC3_Init(TIMER2, &TIMER_OCInitStructure);
    TIMER_OC3_Preload(TIMER2,TIMER_OC_PRELOAD_DISABLE);

    TIMER_OCInitStructure.TIMER_Pulse = 7499;
    TIMER_OC4_Init(TIMER2, &TIMER_OCInitStructure);
    TIMER_OC4_Preload(TIMER2,TIMER_OC_PRELOAD_DISABLE);

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
