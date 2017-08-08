/**
  ******************************************************************************
  * @file    DMA/ADC1_DMA/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h" 
#include "main.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define RCC_GPIO_LED1               RCC_APB2PERIPH_GPIOC
#define GPIO_LED1_PORT              GPIOC
#define RCC_GPIO_LED2               RCC_APB2PERIPH_GPIOC
#define GPIO_LED2_PORT              GPIOC    
#define GPIO_LED1                   GPIO_PIN_0    
#define GPIO_LED2                   GPIO_PIN_2

#define RCC_GPIO_LED3               RCC_APB2PERIPH_GPIOE
#define GPIO_LED3_PORT              GPIOE
#define RCC_GPIO_LED4               RCC_APB2PERIPH_GPIOE
#define GPIO_LED4_PORT              GPIOE
#define GPIO_LED3                   GPIO_PIN_0    
#define GPIO_LED4                   GPIO_PIN_1

/* Private variables ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define ADC_DR_Address    ((u32)0x4001244C)

float AD_value;
__IO uint16_t ADC_ConvertedValue;

DMA_InitPara  DMA_InitStructure;

__IO uint32_t TimingDelay = 0;
void RCC_Configuration(void);
static void SysTickConfig(void);
void USART_Configuration(void);
void ADC_Configuration(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
       
    /* System Clocks Configuration */
    RCC_Configuration();
                           
    /* SysTick configuration */
    SysTickConfig();
                         
    LED_config() ;
    GPIO_SetBits(GPIO_LED1_PORT, GPIO_LED1);
    GPIO_ResetBits(GPIO_LED3_PORT, GPIO_LED3);
    GPIO_ResetBits(GPIO_LED2_PORT, GPIO_LED2);
    GPIO_ResetBits(GPIO_LED4_PORT, GPIO_LED4);
   
    USART_Configuration();

    /* DMA1 channel1 configuration */
    DMA_DeInit(DMA1_CHANNEL1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PERIPHERALSRC;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_DISABLE;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PERIPHERALDATASIZE_HALFWORD;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MEMORYDATASIZE_HALFWORD;
    DMA_InitStructure.DMA_Mode = DMA_MODE_CIRCULAR;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA1_CHANNEL1, &DMA_InitStructure);
            
    /* Enable DMA transfer */
    DMA_Enable(DMA1_CHANNEL1, ENABLE);
    ADC_Configuration();
    Delay(300);     
    while (1)
    {
        /***********************************************ADC²É¼¯×ª»»********************************************************/
         
        /* Printf message with AD value to serial port every 1 second */
        AD_value = ADC_ConvertedValue;
        AD_value = (AD_value/4096)*3.3;
        printf("The current AD value = %4.2fV \r\n", AD_value); 
        Delay(1000);   /* delay 1000ms */       
    }
}

/**
  * @brief  Led configure
  * @param  None
  * @retval None
  */
void LED_config(void)
{
    GPIO_InitPara GPIO_InitStructure;
    
    /* Enable the GPIO_LED Clock */
    RCC_APB2PeriphClock_Enable(RCC_GPIO_LED1 | RCC_GPIO_LED2 | RCC_GPIO_LED3 | RCC_GPIO_LED4, ENABLE);
    
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_LED1 | GPIO_LED2;    
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIO_LED1_PORT, &GPIO_InitStructure);
    GPIO_Init(GPIO_LED2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_LED3 | GPIO_LED4;    
    GPIO_Init(GPIO_LED3_PORT, &GPIO_InitStructure);
    GPIO_Init(GPIO_LED4_PORT, &GPIO_InitStructure);

}
/**
  * @brief  Configure the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* Enable peripheral clocks */
    /* Enable DMA1 clock */
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_DMA1, ENABLE);

}

void USART_Configuration(void)
 {
    GPIO_InitPara     GPIO_InitStructure;
    USART_InitPara    USART_InitStructure;

    /* Enable GPIO clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_AF, ENABLE);

    USART_InitStructure.USART_BRR = 115200;
    USART_InitStructure.USART_WL = USART_WL_8B;
    USART_InitStructure.USART_STBits = USART_STBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_RESET;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;

    /* Enable USART clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_USART1, ENABLE); 
    
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
     /* USART configuration */
    USART_Init(USART1, &USART_InitStructure);
    
    /* Enable USART */
    USART_Enable(USART1, ENABLE);
    USART_ClearBitState(USART1,USART_FLAG_TC); 
}



/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : Configure the ADC.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void ADC_Configuration(void)
{
    ADC_InitPara ADC_InitStructure;
    GPIO_InitPara GPIO_InitStructure;
    
    RCC_ADCCLKConfig(RCC_ADCCLK_APB2_DIV6);
    
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_ADC1 | RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_AF, ENABLE);
    
    /* Configure PC.03 (ADC12 Channel3)as analog input -------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
    
    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_MODE_INDEPENDENT;
    ADC_InitStructure.ADC_Mode_Scan = ENABLE;
    ADC_InitStructure.ADC_Mode_Continuous = ENABLE;
    ADC_InitStructure.ADC_Trig_External = ADC_EXTERNAL_TRIGGER_MODE_NONE;
    ADC_InitStructure.ADC_Data_Align = ADC_DATAALIGN_RIGHT;
    ADC_InitStructure.ADC_Channel_Number = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /* ADC1 regular channel 0 configuration */ 
    ADC_RegularChannel_Config(ADC1, ADC_CHANNEL_13, 1, ADC_SAMPLETIME_239POINT5); 
    
    /* Enable ADC1 DMA */
    ADC_DMA_Enable(ADC1, ENABLE);
    
    /* Enable ADC1 */
    ADC_Enable(ADC1, ENABLE);
    
    /* Enable ADC1 calibaration register */   
    ADC_Calibration(ADC1);
     
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConv_Enable(ADC1, ENABLE);
}

/**
* @brief  Configure a SysTick Base time to 1 ms.
* @param  None
* @retval None
*/
static void SysTickConfig(void)
{
    /* Setup SysTick Timer for 1ms interrupts */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
    
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
* @brief  Insert a delay time.
* @param  nTime: specify the delay time length.
* @retval None
*/
void Delay(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/**
* @brief  Decrement the TimingDelay variable.
* @param  None
* @retval None
*/
void TimingDelay_Decrement(void)
{
    if(TimingDelay != 0x00)
    { 
        TimingDelay --;
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

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
