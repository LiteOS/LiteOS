/**
  ******************************************************************************
  * @file    TIMER/TIMER1_DMA/main.c 
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

/* Private define ------------------------------------------------------------*/
#define TIMER1_CHCC1  ((uint32_t)0x040012c34)

/* Private variables ---------------------------------------------------------*/
uint16_t buffer[3]={249,499,749};

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM_Configuration(void);
void DMA_Configuration(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* GPIOA, GPIOB, GPIOC and AF clocks enable */
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

    /*Configure PA8(TIMER1 CH1) as alternate function*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/**
  * @brief  Configure the DMA peripheral.
  * @param  None
  * @retval None
  */
void DMA_Configuration(void)
{
    DMA_InitPara DMA_InitStructure;

    /* DMA1 clock enable */
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_DMA1,ENABLE);

    /* DMA1 Channel5 Config */
    DMA_DeInit(DMA1_CHANNEL5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = TIMER1_CHCC1;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t) buffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PERIPHERALDST;
    DMA_InitStructure.DMA_BufferSize         = 3;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PERIPHERALDATASIZE_HALFWORD;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MEMORYDATASIZE_HALFWORD;
    DMA_InitStructure.DMA_Mode               = DMA_MODE_CIRCULAR;
    DMA_InitStructure.DMA_Priority           = DMA_PRIORITY_HIGH;
    DMA_InitStructure.DMA_MTOM               = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA1_CHANNEL5, &DMA_InitStructure);
    
    /* DMA1 Channel5 enable */
    DMA_Enable(DMA1_CHANNEL5, ENABLE);
}

/**
  * @brief  Configure the TIMER peripheral.
  * @param  None
  * @retval None
  */
void TIM_Configuration(void)
{
    /* TIMER1 DMA Transfer example -------------------------------------------------
    TIMER1CLK = 108MHz, Prescaler = 108 
    TIMER1 counter clock = SystemCoreClock/108 = 1MHz.

    The objective is to configure TIMER1 channel 1 to generate PWM
    signal with a frequency equal to 1KHz and a variable duty cycle(25%,50%,75%) that
    is changed by the DMA after a specific number of Update DMA request.

    The number of this repetitive requests is defined by the TIMER1 Repetition counter,
    each 2 Update Requests, the TIMER1 Channel 1 Duty Cycle changes to the next new 
    value defined by the buffer . 
    -----------------------------------------------------------------------------*/
    TIMER_BaseInitPara TIM_TimeBaseStructure;
    TIMER_OCInitPara   TIM_OCInitStructure;

    /* TIMER1 clock enable */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_TIMER1,ENABLE);

    /* TIMER1  configuration */
    TIMER_DeInit(TIMER1);
    TIM_TimeBaseStructure.TIMER_Prescaler         = 107;
    TIM_TimeBaseStructure.TIMER_CounterMode       = TIMER_COUNTER_UP;
    TIM_TimeBaseStructure.TIMER_Period            = 999;
    TIM_TimeBaseStructure.TIMER_ClockDivision     = TIMER_CDIV_DIV1;
    TIM_TimeBaseStructure.TIMER_RepetitionCounter = 1;
    TIMER_BaseInit(TIMER1,&TIM_TimeBaseStructure);

    /* CH1 Configuration in PWM mode */
    TIM_OCInitStructure.TIMER_OCMode       = TIMER_OC_MODE_PWM1;
    TIM_OCInitStructure.TIMER_OCPolarity   = TIMER_OC_POLARITY_HIGH;
    TIM_OCInitStructure.TIMER_OCNPolarity  = TIMER_OCN_POLARITY_HIGH;
    TIM_OCInitStructure.TIMER_OutputState  = TIMER_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.TIMER_OutputNState = TIMER_OUTPUTN_STATE_ENABLE;
    TIM_OCInitStructure.TIMER_OCIdleState  = TIMER_OC_IDLE_STATE_SET;
    TIM_OCInitStructure.TIMER_OCNIdleState = TIMER_OCN_IDLE_STATE_RESET;
     
    TIM_OCInitStructure.TIMER_Pulse = buffer[0];

    TIMER_OC1_Init(TIMER1, &TIM_OCInitStructure);
    TIMER_OC1_Preload(TIMER1,TIMER_OC_PRELOAD_DISABLE);

    /* TIMER1 output enable */
    TIMER_CtrlPWMOutputs(TIMER1,ENABLE);
    /* Auto-reload preload enable */
    TIMER_CARLPreloadConfig(TIMER1,ENABLE);
    /* TIMER1 Update DMA Request enable */
    TIMER_DMACmd( TIMER1, TIMER_DMA_UPDATE, ENABLE);
    /* TIMER enable counter*/
    TIMER_Enable( TIMER1, ENABLE );
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
    DMA_Configuration();
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
