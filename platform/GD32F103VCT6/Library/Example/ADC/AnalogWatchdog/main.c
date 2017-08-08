/**
  ******************************************************************************
  * @file    ADC/AnalogWatchdog/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include <stdlib.h>
#include <stdio.h> 

/* Private variables ---------------------------------------------------------*/
ADC_InitPara  ADC_InitStructure;
    
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{ 
    /* Config System clocks --------------------------------------------------*/
    RCC_Configuration();
    
    /* NVIC configuration ----------------------------------------------------*/
    NVIC_Configuration();
    
    /* Config GPIO ports -----------------------------------------------------*/
    GPIO_Configuration();   
    
    /* Config ADC1  ----------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode_Scan = DISABLE;
    ADC_InitStructure.ADC_Mode_Continuous = ENABLE;
    ADC_InitStructure.ADC_Trig_External = ADC_EXTERNAL_TRIGGER_MODE_NONE;
    ADC_InitStructure.ADC_Data_Align = ADC_DATAALIGN_RIGHT;
    ADC_InitStructure.ADC_Channel_Number = 1;
    ADC_Init(ADC1,&ADC_InitStructure);
    
    /* Config ADC1 regular channel13 */ 
    ADC_RegularChannel_Config(ADC1,ADC_CHANNEL_13, 1, ADC_SAMPLETIME_13POINT5);
    
    /* Configure high and low analog watchdog thresholds */
    ADC_AnalogWatchdogThresholds_Config(ADC1,0x0A00, 0x0400);
    /* Configure channel11 as the single analog watchdog guarded channel */
    ADC_AnalogWatchdogSingleChannel_Config(ADC1,ADC_CHANNEL_13);
    /* Enable analog watchdog on one regular channel */
    ADC_AnalogWatchdog_Enable(ADC1,ADC_ANALOGWATCHDOG_SINGLEREGENABLE);
    
    /* Enable AWD interrupt */
    ADC_INTConfig(ADC1,ADC_INT_AWD, ENABLE);
    
    /* Enable ADC1 */
    ADC_Enable(ADC1,ENABLE);
    
    /* ADC1 calibration */   
    ADC_Calibration(ADC1);
    
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConv_Enable(ADC1,ENABLE);
    
    while (1)
    {
    }
}

/**
  * @brief  Configure system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* ADCCLK = PCLK2/6 */
    RCC_ADCCLKConfig(RCC_ADCCLK_APB2_DIV6); 
    /* Enable GPIOC clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC, ENABLE);
    /* Enable ADC1 and GPIO_LED clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_ADC1, ENABLE);
}

/**
  * @brief  Configure GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;
    
    /* Configure PC3 (ADC Channel13) as analog input -------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  Configure NVIC.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara NVIC_InitStructure;
    
    /* Configure and enable ADC interrupt */
    NVIC_InitStructure.NVIC_IRQ = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
