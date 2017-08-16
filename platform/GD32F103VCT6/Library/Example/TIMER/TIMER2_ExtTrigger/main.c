/**
  ******************************************************************************
  * @file    TIMER/TIMER2_ExtTrigger/main.c 
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
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOC |
                                RCC_APB2PERIPH_AF , ENABLE);
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

    /* Configure PA0(TIMER2 CH1) as alternate function */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    /* Configure PA2(TIMER2 CH3) as alternate function */
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
    /* Timer with an external trigger -----------------------------------
    TIMER2 is configured as Slave Timer for an external Trigger connected
    to TIMER2 TI1 pin :
    - The TIMER2 TI1FP1 is used as Trigger Input
    - Rising edge is used to start the TIMER2: Trigger Mode.
    - TIMER2 is used PWM2 Mode 
    The starts of the TIMER2 counter are controlled by the 
    external trigger.  
    -------------------------------------------------------------------- */
    TIMER_BaseInitPara TIM_TimeBaseStructure;
    TIMER_OCInitPara   TIM_OCInitStructure;
    TIMER_ICInitPara   TIM_ICInitStructure;

    /* TIMER2 clock enable */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_TIMER2,ENABLE);

    /* TIMER2  configuration */
    TIMER_DeInit(TIMER2);
    TIM_TimeBaseStructure.TIMER_Prescaler     = 53999;
    TIM_TimeBaseStructure.TIMER_CounterMode   = TIMER_COUNTER_UP;
    TIM_TimeBaseStructure.TIMER_Period        = 7999;
    TIM_TimeBaseStructure.TIMER_ClockDivision = TIMER_CDIV_DIV1;
    TIMER_BaseInit(TIMER2,&TIM_TimeBaseStructure);

    /* CH3 Configuration in PWM2 mode */
    TIM_OCInitStructure.TIMER_OCMode       = TIMER_OC_MODE_PWM2;
    TIM_OCInitStructure.TIMER_OCPolarity   = TIMER_OC_POLARITY_HIGH;
    TIM_OCInitStructure.TIMER_OCNPolarity  = TIMER_OCN_POLARITY_HIGH;
    TIM_OCInitStructure.TIMER_OutputState  = TIMER_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.TIMER_OutputNState = TIMER_OUTPUTN_STATE_DISABLE;
    TIM_OCInitStructure.TIMER_OCIdleState  = TIMER_OC_IDLE_STATE_RESET;
    TIM_OCInitStructure.TIMER_OCNIdleState = TIMER_OCN_IDLE_STATE_RESET;
    TIM_OCInitStructure.TIMER_Pulse        = 3999;
    TIMER_OC3_Init(TIMER2, &TIM_OCInitStructure);
    TIMER_OC3_Preload(TIMER2,TIMER_OC_PRELOAD_DISABLE);

    /* TIMER2 Input Capture Configuration */
    TIM_ICInitStructure.TIMER_CH          = TIMER_CH_1;
    TIM_ICInitStructure.TIMER_ICPolarity  = TIMER_IC_POLARITY_RISING;
    TIM_ICInitStructure.TIMER_ICSelection = TIMER_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.TIMER_ICPrescaler = TIMER_IC_PSC_DIV1;
    TIM_ICInitStructure.TIMER_ICFilter    = 0x02;
    TIMER_ICInit( TIMER2, &TIM_ICInitStructure);
    
    /* Slave Mode selection : TIMER2 */
    /* TIMER2 Input trigger : External Trigger connected to TI1 */
    TIMER_SelectInputTrigger( TIMER2, TIMER_TS_TI1FP1);
    TIMER_SelectSlaveMode( TIMER2, TIMER_SLAVE_MODE_TRIGGER);
    /* Auto-reload preload enable */
    TIMER_CARLPreloadConfig(TIMER2,ENABLE);
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
