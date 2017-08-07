/**
  ******************************************************************************
  * @file    TIMER/TIMER9_PWMOutput/main.c 
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

    /* Configure PC6 (LED2) as PP output */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    /*Configure PA2 PA3(TIMER9 CH1 CH2) as alternate function*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_2|GPIO_PIN_3;
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
    TIMER9 Configuration: generate 2 PWM signals with 2 different duty cycles:
    TIMER9CLK = SystemCoreClock / 108 = 1MHz

    TIMER9 Channel1,Channel2 PWM Period is 10000/1MHz = 10ms.
    TIMER9 Channel1 duty cycle = (2500/ 10000)* 100 = 25%
    TIMER9 Channel2 duty cycle = (5000/ 10000)* 100 = 50%
    ----------------------------------------------------------------------- */
    TIMER_BaseInitPara TIMER_TimeBaseStructure;
    TIMER_OCInitPara   TIMER_OCInitStructure;

    /* TIMER9 clock enable */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_TIMER9,ENABLE);

    /* TIMER9  configuration */
    TIMER_DeInit(TIMER9);
    TIMER_TimeBaseStructure.TIMER_Prescaler     = 107;
    TIMER_TimeBaseStructure.TIMER_CounterMode   = TIMER_COUNTER_UP;
    TIMER_TimeBaseStructure.TIMER_Period        = 9999;
    TIMER_TimeBaseStructure.TIMER_ClockDivision = TIMER_CDIV_DIV1;
    TIMER_BaseInit(TIMER9,&TIMER_TimeBaseStructure);

    /* TIMER9 CH1,CH2 Configuration in PWM mode */
    TIMER_OCInitStructure.TIMER_OCMode      = TIMER_OC_MODE_PWM1;
    TIMER_OCInitStructure.TIMER_OCPolarity  = TIMER_OC_POLARITY_HIGH;
    TIMER_OCInitStructure.TIMER_OutputState = TIMER_OUTPUT_STATE_ENABLE;
    TIMER_OCInitStructure.TIMER_OCIdleState = TIMER_OC_IDLE_STATE_RESET;

    TIMER_OCInitStructure.TIMER_Pulse = 2499;
    TIMER_OC1_Init(TIMER9, &TIMER_OCInitStructure);
    TIMER_OC1_Preload(TIMER9,TIMER_OC_PRELOAD_DISABLE);

    TIMER_OCInitStructure.TIMER_Pulse = 4999;
    TIMER_OC2_Init(TIMER9, &TIMER_OCInitStructure);
    TIMER_OC2_Preload(TIMER9,TIMER_OC_PRELOAD_DISABLE);

    /* Auto-reload preload enable */
    TIMER_CARLPreloadConfig(TIMER9,ENABLE);
    /* TIMER enable counter*/
    TIMER_Enable( TIMER9, ENABLE );
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
        GPIO_SetBits(GPIOC,GPIO_PIN_6);
        Delay_1ms(1000);
        GPIO_ResetBits(GPIOC, GPIO_PIN_6);
        Delay_1ms(1000);
    }    
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
