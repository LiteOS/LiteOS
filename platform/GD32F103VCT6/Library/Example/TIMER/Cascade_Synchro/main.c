/**
  ******************************************************************************
  * @file    TIMER/Cascade_Synchro/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program.
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

    /* Configure PA0(TIMER2 CH1) as alternate function */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    /* Configure PA6(TIMER3 CH1) as alternate function */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    /* Configure PA8(TIMER1 CH1) as alternate function */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/**
  * @brief  Configure the TIMER peripherals.
  * @param  None
  * @retval None
  */
void TIM_Configuration(void)
{
    /* TIMERs synchronisation in cascade mode ----------------------------
       1/TIMER2 is configured as Master Timer:
       - PWM Mode is used
       - The TIMER2 Update event is used as Trigger Output  

       2/TIMER3 is slave for TIMER2 and Master for TIMER1,
       - PWM Mode is used
       - The ITR1(TIMER2) is used as input trigger 
       - External clock mode is used,the counter counts on the rising edges of
         the selected trigger.
       - The TIMER3 Update event is used as Trigger Output. 

       3/TIMER1 is slave for TIMER3,
       - PWM Mode is used
       - The ITR2(TIMER3) is used as input trigger
       - External clock mode is used,the counter counts on the rising edges of
         the selected trigger.

       The TIMERxCLK is fixed to 108 MHz, the TIMER2 counter clock is :
       108MHz/54000= 2 KHz.

       The Master Timer TIMER2 is running at TIMER2 frequency :
       TIMER2 frequency = (TIMER2 counter clock)/ (TIMER2 period + 1) = 0.5 Hz 
       and the duty cycle = TIMER2_CCR1/(TIMER2_ARR + 1) = 50%

       The TIMER3 is running:
       - At (TIMER2 frequency)/ (TIMER3 period + 1) = 0.25 Hz and a duty cycle
         equal to TIMER3_CCR1/(TIMER3_ARR + 1) = 50%

       The TIMER1 is running:
       - At (TIMER3 frequency)/ (TIMER1 period + 1) = 0.125 Hz and a duty cycle
         equal to TIMER1_CCR1/(TIMER1_ARR + 1) = 50%
      -------------------------------------------------------------------- */
    TIMER_BaseInitPara TIM_TimeBaseStructure;
    TIMER_OCInitPara   TIM_OCInitStructure;

    /* TIMERs clock enable */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_TIMER1,ENABLE);
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_TIMER2|RCC_APB1PERIPH_TIMER3,ENABLE);

    /* TIMER2  configuration */
    TIMER_DeInit(TIMER2);
    TIM_TimeBaseStructure.TIMER_Prescaler     = 53999;
    TIM_TimeBaseStructure.TIMER_CounterMode   = TIMER_COUNTER_UP;
    TIM_TimeBaseStructure.TIMER_Period        = 3999;
    TIM_TimeBaseStructure.TIMER_ClockDivision = TIMER_CDIV_DIV1;
    TIMER_BaseInit(TIMER2,&TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIMER_OCMode      = TIMER_OC_MODE_PWM1;
    TIM_OCInitStructure.TIMER_OCPolarity  = TIMER_OC_POLARITY_HIGH;
    TIM_OCInitStructure.TIMER_OutputState = TIMER_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.TIMER_OCIdleState = TIMER_OC_IDLE_STATE_RESET;
      
    TIM_OCInitStructure.TIMER_Pulse = 1999;
    TIMER_OC1_Init(TIMER2, &TIM_OCInitStructure);
    TIMER_OC1_Preload(TIMER2,TIMER_OC_PRELOAD_DISABLE);

    /* Auto-reload preload enable */
    TIMER_CARLPreloadConfig(TIMER2,ENABLE);
    /* Select the Master Slave Mode */
    TIMER_SelectMasterSlaveMode(TIMER2,TIMER_MASTER_SLAVE_MODE_ENABLE);
    /* TIMER2 Update event is used as Trigger Output */
    TIMER_SelectOutputTrigger(TIMER2,TIMER_TRI_OUT_SRC_UPDATE);

    /* TIMER3  configuration */
    TIMER_DeInit(TIMER3);
    TIM_TimeBaseStructure.TIMER_Prescaler     = 0;
    TIM_TimeBaseStructure.TIMER_CounterMode   = TIMER_COUNTER_UP;
    TIM_TimeBaseStructure.TIMER_Period        = 1;
    TIM_TimeBaseStructure.TIMER_ClockDivision = TIMER_CDIV_DIV1;
    TIMER_BaseInit(TIMER3,&TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIMER_OCMode      = TIMER_OC_MODE_PWM1;
    TIM_OCInitStructure.TIMER_OCPolarity  = TIMER_OC_POLARITY_HIGH;
    TIM_OCInitStructure.TIMER_OutputState = TIMER_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.TIMER_OCIdleState = TIMER_OC_IDLE_STATE_RESET;
      
    TIM_OCInitStructure.TIMER_Pulse = 1;
    TIMER_OC1_Init(TIMER3, &TIM_OCInitStructure);
    TIMER_OC1_Preload(TIMER3,TIMER_OC_PRELOAD_DISABLE);
    /* Auto-reload preload enable */
    TIMER_CARLPreloadConfig(TIMER3,ENABLE);
    /* Slave Mode selection: TIMER3 */
    TIMER_SelectSlaveMode(TIMER3,TIMER_SLAVE_MODE_EXTERNAL1);
    TIMER_SelectInputTrigger(TIMER3,TIMER_TS_ITR1);

    /* Select the Master Slave Mode */
    TIMER_SelectMasterSlaveMode(TIMER3,TIMER_MASTER_SLAVE_MODE_ENABLE);
    /* TIMER3 Update event is used as Trigger Output */
    TIMER_SelectOutputTrigger(TIMER3,TIMER_TRI_OUT_SRC_UPDATE);

    /* TIMER1  configuration */
    TIMER_DeInit(TIMER1);
    TIM_TimeBaseStructure.TIMER_Prescaler     = 0;
    TIM_TimeBaseStructure.TIMER_CounterMode   = TIMER_COUNTER_UP;
    TIM_TimeBaseStructure.TIMER_Period        = 1;
    TIM_TimeBaseStructure.TIMER_ClockDivision = TIMER_CDIV_DIV1;
    TIMER_BaseInit(TIMER1,&TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIMER_OCMode      = TIMER_OC_MODE_PWM1;
    TIM_OCInitStructure.TIMER_OCPolarity  = TIMER_OC_POLARITY_HIGH;
    TIM_OCInitStructure.TIMER_OutputState = TIMER_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.TIMER_OCIdleState = TIMER_OC_IDLE_STATE_RESET;
      
    TIM_OCInitStructure.TIMER_Pulse = 1;
    TIMER_OC1_Init(TIMER1, &TIM_OCInitStructure);
    TIMER_OC1_Preload(TIMER1,TIMER_OC_PRELOAD_DISABLE);
    /* Auto-reload preload enable */
    TIMER_CARLPreloadConfig(TIMER1,ENABLE);
    /* TIMER1 output enable */
    TIMER_CtrlPWMOutputs(TIMER1, ENABLE);
    /* Slave Mode selection: TIMER1 */
    TIMER_SelectSlaveMode(TIMER1,TIMER_SLAVE_MODE_EXTERNAL1);
    TIMER_SelectInputTrigger(TIMER1,TIMER_TS_ITR2);

    /* TIMERs enable counter*/
    TIMER_Enable( TIMER3, ENABLE );
    TIMER_Enable( TIMER1, ENABLE );
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
