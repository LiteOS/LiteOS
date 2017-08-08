/**
  ******************************************************************************
  * @file    ADC/RegSimul_DualMode/main.c 
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


__IO uint16_t ADC_DualConvertedValueTab[16];

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
  
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
      
    /* Config GPIO ports -----------------------------------------------------*/
    GPIO_Configuration();
    
    /* Config DMA1 Channel1 --------------------------------------------------*/
    DMA_DeInit(DMA1_CHANNEL1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PERIPHERALSRC;
    DMA_InitStructure.DMA_BufferSize = 16;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PERIPHERALDATASIZE_WORD;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MEMORYDATASIZE_WORD;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA1_CHANNEL1, &DMA_InitStructure);
    
    /* Enable DMA1 channel1 */
    DMA_Enable(DMA1_CHANNEL1, ENABLE);
    
    /* Config ADC1 -----------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode=ADC_MODE_REGSIMULT;
    ADC_InitStructure.ADC_Mode_Scan = ENABLE;
    ADC_InitStructure.ADC_Mode_Continuous = ENABLE;
    ADC_InitStructure.ADC_Trig_External  = ADC_EXTERNAL_TRIGGER_MODE_NONE;
    ADC_InitStructure.ADC_Data_Align = ADC_DATAALIGN_RIGHT;
    ADC_InitStructure.ADC_Channel_Number = 2;
    ADC_Init(ADC1,&ADC_InitStructure);
    
    /* Config ADC1 regular channel */ 
    ADC_RegularChannel_Config(ADC1,ADC_CHANNEL_14, 1, ADC_SAMPLETIME_239POINT5);
    ADC_RegularChannel_Config(ADC1,ADC_CHANNEL_17, 2, ADC_SAMPLETIME_239POINT5);

    /* Enable ADC1 DMA */
    ADC_DMA_Enable(ADC1,ENABLE);

    /* Config ADC2 -----------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode=ADC_MODE_REGSIMULT;
    ADC_InitStructure.ADC_Mode_Scan = ENABLE;
    ADC_InitStructure.ADC_Mode_Continuous = ENABLE;
    ADC_InitStructure.ADC_Trig_External  = ADC_EXTERNAL_TRIGGER_MODE_NONE;
    ADC_InitStructure.ADC_Data_Align = ADC_DATAALIGN_RIGHT;
    ADC_InitStructure.ADC_Channel_Number = 2;
    ADC_Init(ADC2,&ADC_InitStructure);
    
    /* Config ADC2 regular channel */ 
    ADC_RegularChannel_Config(ADC2,ADC_CHANNEL_11, 1, ADC_SAMPLETIME_239POINT5);
    ADC_RegularChannel_Config(ADC2,ADC_CHANNEL_12, 2, ADC_SAMPLETIME_239POINT5);

    /* Enable ADC2 external trigger conversion */
    ADC_ExternalTrigConv_Enable(ADC2, ENABLE);

    /* Enable ADC1 */
    ADC_Enable(ADC1,ENABLE); 
    /* Enable Vrefint channel17 */
    ADC_TempSensorVrefint_Enable(ENABLE);

    ADC_Calibration(ADC1);

    /* Enable ADC2 */
    ADC_Enable(ADC2, ENABLE);

    ADC_Calibration(ADC2);

    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConv_Enable(ADC1, ENABLE);

    /* Test on DMA1 channel1 transfer complete flag */
    while(!DMA_GetBitState(DMA1_FLAG_TC1));
    /* Clear DMA1 channel1 transfer complete flag */
    DMA_ClearBitState(DMA1_FLAG_TC1);

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
	
    /* Enable ADC1 ,GPIO and TIMER2 clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_ADC1 | RCC_APB2PERIPH_ADC2, ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC, ENABLE);

}

/**
  * @brief  Configure the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;
    
    /* Configure PC1,PC2 and PC4 as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}



/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
