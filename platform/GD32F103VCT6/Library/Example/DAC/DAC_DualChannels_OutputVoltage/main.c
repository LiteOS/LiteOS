/**
  ******************************************************************************
  * @file    DAC/DAC_DualChannels_OutputVoltage/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The main function file
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include <stdio.h> 
/* Private functions ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define DAC_DHR12RD_Address 0x40007420
#define DAC_Out1_Code   0x7FF0
#define DAC_Out2_Code   0x1FF0
/**
  * @brief  Configures the GPIO.
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

/**
  * @brief  Configures the DAC.
  * @param  None
  * @retval None
  */
void DAC_Configuration(void)
{
    DAC_InitPara  DAC_InitStructure;

    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_DAC, ENABLE);
    /* DAC channel1 Configuration */
    DAC_InitStructure.DAC_Trigger = DAC_TRIGGER_NONE;
    DAC_InitStructure.DAC_WaveType = DAC_WAVEGENE_NONE;
    DAC_InitStructure.DAC_LFSRNoise_AmplitudeTriangle = DAC_LFSR_BITS8_0;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    DAC_Init(DAC_CHANNEL_1, &DAC_InitStructure);
    /* DAC channel2 Configuration */
    DAC_Init(DAC_CHANNEL_2, &DAC_InitStructure);
    /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
    automatically connected to the DAC converter. */
    DAC_Enable(DAC_CHANNEL_1, ENABLE);
    /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is 
    automatically connected to the DAC converter. */
    DAC_Enable(DAC_CHANNEL_2, ENABLE);
    /* Set DAC Channel1 DHR12L register */
    DAC_SetDualChannelData(DAC_ALIGN_12B_L,DAC_Out2_Code,DAC_Out1_Code);
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
    while (1)
    {
    }
}
/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/  




















  
