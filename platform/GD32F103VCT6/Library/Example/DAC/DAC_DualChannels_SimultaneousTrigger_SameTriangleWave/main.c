/**
  ******************************************************************************
  * @file    DAC/DAC_DualChannels_SimultaneousTrigger_SameTriangleWave/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The main function file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include <stdio.h>
/* Private functions ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static vu32 TimingDelay = 0;

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA ,ENABLE);
   /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
    connected to the DAC converter. In order to avoid parasitic consumption,
    the GPIO pin should be configured in analog */
    GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void DAC_Configuration(void)
{
    DAC_InitPara  DAC_InitStructure;

    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_DAC, ENABLE);
    /* DAC channel1 Configuration */
    DAC_InitStructure.DAC_Trigger = DAC_TRIGGER_T2_TRIG;
    DAC_InitStructure.DAC_WaveType = DAC_WAVEGENE_TRIANGLE;
    DAC_InitStructure.DAC_LFSRNoise_AmplitudeTriangle = DAC_AMPLITUDETRIANGLE_1023;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
    DAC_Init(DAC_CHANNEL_1, &DAC_InitStructure);
    /* DAC channel2 Configuration */ 
    DAC_InitStructure.DAC_LFSRNoise_AmplitudeTriangle = DAC_AMPLITUDETRIANGLE_1023; 
    DAC_Init(DAC_CHANNEL_2, &DAC_InitStructure);

    /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
    automatically connected to the DAC converter. */
    DAC_Enable(DAC_CHANNEL_1, ENABLE);
    /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is 
    automatically connected to the DAC converter. */
    DAC_Enable(DAC_CHANNEL_2, ENABLE);
    /* Set DAC dual channel DHR12RD register */
    DAC_SetDualChannelData(DAC_ALIGN_12B_R, 0x7F0, 0x7F0); 
}

void TIM2_Configuration(void)
{
    TIMER_BaseInitPara TIM_TimeBaseStructure;

    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_TIMER2, ENABLE);
    TIM_TimeBaseStructure.TIMER_Period = 0xF;
    TIM_TimeBaseStructure.TIMER_Prescaler = 0xF;
    TIM_TimeBaseStructure.TIMER_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIMER_CounterMode = TIMER_COUNTER_UP;
    TIMER_BaseInit(TIMER2, &TIM_TimeBaseStructure);
    /* TIM2 TRGO selection */
    TIMER_SelectOutputTrigger(TIMER2, TIMER_TRI_OUT_SRC_UPDATE);
    TIMER_Enable(TIMER2, ENABLE);  
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    GPIO_Configuration();
    DAC_Configuration();
    TIM2_Configuration();
    while (1)
    {
    }
}    
/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/  




















  
