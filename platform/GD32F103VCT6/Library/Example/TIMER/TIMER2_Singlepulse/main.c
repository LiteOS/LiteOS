/**
  ******************************************************************************
  * @file    TIMER/TIMER2_Singlepulse/main.c 
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

    /* Configure PA0 (TIMER2 CH1 ) as alternate function */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    /* Configure PA1 (TIMER2 CH2) as alternate function */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_1;
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
    TIMER2 Configuration: Single Pulse Mode
    The external signal is connected to TIMER2 CH1 pin (PA0) and the Rising 
    edge is used as active edge.
    The Single Pulse signal is output on TIMER2 CH2 pin (PA1).

    The TIMER2CLK frequency is set to SystemCoreClock (Hz),the Prescaler is 
    4,so the TIMER2 counter clock is 27MHz.

    Single Pulse value = (TIMER2_Period - TIMER2_Pulse) / TIMER2 counter clock
                       = (65535 - 11535) / 27M = 2.0 ms.

    ----------------------------------------------------------------------- */

    TIMER_BaseInitPara TIMER_TimeBaseStructure;
    TIMER_OCInitPara   TIMER_OCInitStructure;
    TIMER_ICInitPara   TIM_ICInitStructure;

    /* TIMER2 clock enable */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_TIMER2,ENABLE);

    /* TIMER2 configuration */
    TIMER_DeInit(TIMER2);
    TIMER_TimeBaseStructure.TIMER_Prescaler     = 3;
    TIMER_TimeBaseStructure.TIMER_CounterMode   = TIMER_COUNTER_UP;
    TIMER_TimeBaseStructure.TIMER_Period        = 65535;
    TIMER_TimeBaseStructure.TIMER_ClockDivision = TIMER_CDIV_DIV1;
    TIMER_BaseInit(TIMER2,&TIMER_TimeBaseStructure);

    /* TIMER2 CH2 Configuration in PWM2 mode */
    TIMER_OCInitStructure.TIMER_OCMode      = TIMER_OC_MODE_PWM2;
    TIMER_OCInitStructure.TIMER_OCPolarity  = TIMER_OC_POLARITY_HIGH;
    TIMER_OCInitStructure.TIMER_OutputState = TIMER_OUTPUT_STATE_ENABLE;
    TIMER_OCInitStructure.TIMER_OCIdleState = TIMER_OC_IDLE_STATE_RESET;

    TIMER_OCInitStructure.TIMER_Pulse = 11535;
    TIMER_OC2_Init(TIMER2, &TIMER_OCInitStructure);
    TIMER_OC2_Preload(TIMER2,TIMER_OC_PRELOAD_DISABLE);

    /* TIMER2 CH1 Input Capture Configuration */
    TIM_ICInitStructure.TIMER_CH          = TIMER_CH_1;
    TIM_ICInitStructure.TIMER_ICPolarity  = TIMER_IC_POLARITY_FALLING;
    TIM_ICInitStructure.TIMER_ICSelection = TIMER_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.TIMER_ICPrescaler = TIMER_IC_PSC_DIV1;
    TIM_ICInitStructure.TIMER_ICFilter    = 0x00;
    TIMER_ICInit( TIMER2, &TIM_ICInitStructure);

    /* Single Pulse Mode selection */
    TIMER_SinglePulseMode(TIMER2,TIMER_SP_MODE_SINGLE);

    /* Slave Mode selection : TIMER4 */
    /* TIMER2 Input trigger : External Trigger connected to TI1 */
    TIMER_SelectInputTrigger( TIMER2, TIMER_TS_TI1FP1);
    TIMER_SelectSlaveMode( TIMER2, TIMER_SLAVE_MODE_TRIGGER);
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
