/**
  ******************************************************************************
  * @file    ADC/Multi_channel_regular/main.c 
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
#define DR_ADDRESS        ((uint32_t)0x4001244C) 

#define ADC1_DRH          0x0B66	   /*!< equal to 2.35V */
#define ADC1_DRL          0x08BA	   /*!< equal to 1.80V */

/* Private variables ---------------------------------------------------------*/
float AD_value;
uint16_t ADC_ConvertedValue[1024];	 
uint16_t ADC_Channel1[256];	
uint16_t ADC_Channel2[256];	 
uint16_t ADC_Channel3[256];	 
uint16_t ADC_Channel4[256];	 
int AD_I=0;
int i;

/* Private function prototypes -----------------------------------------------*/
void ADC_Configuration(void);

#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */ 

static void Delay_ARMJISHU(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    ADC_Configuration();
    GPIO_ResetBits(GPIOC,GPIO_PIN_0);
    GPIO_ResetBits(GPIOC,GPIO_PIN_2);
    
    while (1)
    {	
        for(AD_I=0;AD_I<256;AD_I++)
        {
            ADC_Channel1[AD_I]=ADC_ConvertedValue[4*AD_I];	
            ADC_Channel2[AD_I]=ADC_ConvertedValue[4*AD_I+1];
            ADC_Channel3[AD_I]=ADC_ConvertedValue[4*AD_I+2];
            ADC_Channel4[AD_I]=ADC_ConvertedValue[4*AD_I+3];
            
            if(ADC1_DRH<ADC_Channel1[AD_I]||ADC_Channel1[AD_I]<ADC1_DRL)
            {
                GPIO_SetBits(GPIOC,GPIO_PIN_0);
                GPIO_ResetBits(GPIOC,GPIO_PIN_2);	
            }
            else
            {
                GPIO_SetBits(GPIOC,GPIO_PIN_0);
                GPIO_ResetBits(GPIOC,GPIO_PIN_2);
            }
            Delay_ARMJISHU(8000000);	
        }
    }    
}

/**
  * @brief  Retarget the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_DataSend(USART1, (uint8_t) ch);
    
    /* Loop until the end of transmission */
    while (USART_GetBitState(USART1, USART_FLAG_TC) == RESET)
    {}
    
    return ch;
}

/**
  * @brief  Configure the different GPIO ports.
  * @param  None
  * @retval None
  */

void ADC_Configuration(void)
{	  
    GPIO_InitPara GPIO_InitStructure;
    DMA_InitPara DMA_InitStructure;
    ADC_InitPara ADC_InitStructure;
    
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_ADC1, ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC|RCC_APB2PERIPH_GPIOF, ENABLE);  
    RCC_ADCCLKConfig(RCC_ADCCLK_APB2_DIV6);
    /*********************************ADC*****************/
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_DMA1, ENABLE);		     
    
    GPIO_InitStructure.GPIO_Pin 	= GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Init( GPIOC , &GPIO_InitStructure );    
    
    GPIO_InitStructure.GPIO_Pin 	= GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_Init( GPIOC , &GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_1;     
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AIN;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);     
    
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_2;     
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AIN;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);     
       
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_3;     
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AIN;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);      
      
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_4;     
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AIN;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);     
    
    /* DMA channel1 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_CHANNEL1); 
    DMA_InitStructure.DMA_PeripheralBaseAddr = DR_ADDRESS; 
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue; 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PERIPHERALSRC;  
    DMA_InitStructure.DMA_BufferSize = 1024;	
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;	
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PERIPHERALDATASIZE_HALFWORD; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MEMORYDATASIZE_HALFWORD; 
    DMA_InitStructure.DMA_Mode = DMA_MODE_CIRCULAR; 
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE; 
    DMA_Init(DMA1_CHANNEL1, &DMA_InitStructure);	 
    
    /* Enable DMA1 channel1 */
    DMA_Enable(DMA1_CHANNEL1, ENABLE);  
    
    ADC_DeInit(ADC1,&ADC_InitStructure);
    ADC_InitStructure.ADC_Mode_Scan = ENABLE;		 
    ADC_InitStructure.ADC_Mode_Continuous = ENABLE;  
    ADC_InitStructure.ADC_Trig_External = ADC_EXTERNAL_TRIGGER_MODE_NONE;  
    ADC_InitStructure.ADC_Data_Align = ADC_DATAALIGN_RIGHT;	
    ADC_InitStructure.ADC_Channel_Number = 4;  
    ADC_Init(ADC1,&ADC_InitStructure);  
    
    /* ADC1 regular channels configuration */ 
    ADC_RegularChannel_Config(ADC1,ADC_CHANNEL_11, 1, ADC_SAMPLETIME_55POINT5);
    ADC_RegularChannel_Config(ADC1,ADC_CHANNEL_12, 2, ADC_SAMPLETIME_55POINT5);
    ADC_RegularChannel_Config(ADC1,ADC_CHANNEL_13, 3, ADC_SAMPLETIME_55POINT5);
    ADC_RegularChannel_Config(ADC1,ADC_CHANNEL_14, 4, ADC_SAMPLETIME_55POINT5);
    
    /* Enable ADC1 DMA */
    ADC_DMA_Enable(ADC1,ENABLE);	 
    
    /* Enable ADC1 */
    ADC_Enable(ADC1,ENABLE); 
    ADC_Calibration(ADC1);
    
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConv_Enable(ADC1,ENABLE);  
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
