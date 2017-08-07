/**
  ******************************************************************************
  * @file    ADC/TIMTrigger_AutoInsertion/main.c 
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

/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)

/* Private variables ---------------------------------------------------------*/
ADC_InitPara                ADC_InitStructure;
DMA_InitPara                DMA_InitStructure;
TIMER_BaseInitPara          TIM_TimeBaseStructure;
TIMER_OCInitPara            TIM_OCInitStructure;

__IO uint16_t ADC_RegularConvertedValueTab[64], ADC_InsertedConvertedValueTab[64];

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
    
    /* Config NVIC -----------------------------------------------------------*/
    NVIC_Configuration();
    
    /* Config GPIO ports -----------------------------------------------------*/
    GPIO_Configuration();
    
    /* Config TIMER2 -----------------------------------------------------------*/ 
    /* Config Time Base */
    TIMER_BaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIMER_Period = 256;          
    TIM_TimeBaseStructure.TIMER_Prescaler = 6;       
    TIM_TimeBaseStructure.TIMER_ClockDivision = 0x0;    
    TIM_TimeBaseStructure.TIMER_CounterMode = TIMER_COUNTER_UP;  
    TIMER_BaseInit(TIMER2, &TIM_TimeBaseStructure);
    
	/* Config TIMER2 channel2 in PWM mode */
    TIM_OCInitStructure.TIMER_OCMode = TIMER_OC_MODE_PWM1; 
    TIM_OCInitStructure.TIMER_OutputState = TIMER_OUTPUT_STATE_ENABLE;                
    TIM_OCInitStructure.TIMER_Pulse = 128; 
    TIM_OCInitStructure.TIMER_OCPolarity = TIMER_OC_POLARITY_LOW;         
    TIMER_OC2_Init(TIMER2, &TIM_OCInitStructure);
    
    /* Config DMA1 Channel1 --------------------------------------------------*/
    DMA_DeInit(DMA1_CHANNEL1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_RegularConvertedValueTab;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PERIPHERALSRC;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PERIPHERALDATASIZE_HALFWORD;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MEMORYDATASIZE_HALFWORD;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA1_CHANNEL1, &DMA_InitStructure);
    
    /* Enable DMA1 channel1 */
    DMA_Enable(DMA1_CHANNEL1, ENABLE);
    
    /* Config ADC1 -----------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode_Scan = ENABLE;
    ADC_InitStructure.ADC_Mode_Continuous = DISABLE;
    ADC_InitStructure.ADC_Trig_External  = ADC_EXTERNAL_TRIGGER_MODE_T2_CC2;
    ADC_InitStructure.ADC_Data_Align = ADC_DATAALIGN_RIGHT;
    ADC_InitStructure.ADC_Channel_Number = 1;
    ADC_Init(ADC1,&ADC_InitStructure);
    
    /* Config ADC1 regular channel14 */ 
    ADC_RegularChannel_Config(ADC1,ADC_CHANNEL_14, 1, ADC_SAMPLETIME_55POINT5);
    
    /* Set inserted sequencer length */
    ADC_InsertedSequencerLength_Config(ADC1,1);
    /* Config ADC1 inserted channel */ 
    ADC_InsertedChannel_Config(ADC1,ADC_CHANNEL_11, 1, ADC_SAMPLETIME_55POINT5);
    /* Config ADC1 inserted external trigger */
    ADC_ExternalTrigInsertedConv_Config(ADC1,ADC_EXTERNAL_TRIG_INSERTCONV_NONE);
    
    /* Enable automatic inserted conversion start after regular one */
    ADC_AutoInsertedConv_Enable(ADC1,ENABLE);
    
    /* Enable ADC1 DMA */
    ADC_DMA_Enable(ADC1,ENABLE);	 
    
    /* Enable ADC1 external trigger */ 
    ADC_ExternalTrigConv_Enable(ADC1, ENABLE);
    
    /* Enable IEOC interrupt */
    ADC_INTConfig(ADC1,ADC_INT_IEOC, ENABLE);
    
    /* Enable ADC1 */
    ADC_Enable(ADC1,ENABLE); 
    
    ADC_Calibration(ADC1);
    
    /* TIMER2 counter enable */
    TIMER_Enable(TIMER2, ENABLE);
    /* TIMER2 main Output Enable */
    TIMER_CtrlPWMOutputs(TIMER2, ENABLE);  
    
    /* Test on channel1 transfer complete flag */
    while(!DMA_GetBitState(DMA1_FLAG_TC1));
    /* Clear channel1 transfer complete flag */
    DMA_ClearBitState(DMA1_FLAG_TC1); 
    
    /* TIMER2 counter disable */
    TIMER_Enable(TIMER2, DISABLE);
    
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
    /* Configure the ADCCLK to 12MHz */
    RCC_ADCCLKConfig(RCC_ADCCLK_APB2_DIV6);
    
    /* Enable DMA1 clock */
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_DMA1, ENABLE);
	
    /* Enable ADC1 ,GPIO and TIMER1 clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_ADC1, ENABLE);
	RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOC, ENABLE);
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_TIMER2, ENABLE);
}

/**
  * @brief  Configure the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;
    
    /* Configure TIM2_CH2 (PA1) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    /* Configure PC6 as output push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* Configure PC1 and PC4 (ADC Channel11 and Channel14) as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_1 | GPIO_PIN_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  Configure NVIC and Vector Table base location.
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
