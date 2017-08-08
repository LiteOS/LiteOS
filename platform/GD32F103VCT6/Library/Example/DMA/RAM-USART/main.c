/**
  ******************************************************************************
  * @file    DMA/RAM_USART/main.c
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
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize                  23
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
const uint8_t SRC_Const_Buffer[BufferSize]= "\n\r123456789ABCDEF\n\r";
uint8_t DST_Buffer[BufferSize];
/* Private variables ---------------------------------------------------------*/

__IO uint32_t count = 0;
__IO uint32_t int_count = 0;
DMA_InitPara  DMA_InitStructure;
__IO TestStatus TransferStatus = FAILED;

__IO uint32_t TimingDelay = 0;
void RCC_Configuration(void);
void NVIC_Configuration(void);
static void SysTickConfig(void);
void USART_Configuration(void);
TestStatus Buffercmp(const uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength);


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
                           
    /* NVIC configuration */
    NVIC_Configuration();

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
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PERIPHERALSRC;
    DMA_InitStructure.DMA_BufferSize = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_ENABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PERIPHERALDATASIZE_BYTE;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MEMORYDATASIZE_BYTE;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_ENABLE;
    DMA_Init(DMA1_CHANNEL1, &DMA_InitStructure);
    
    /* DMA1 channel2 configuration */
    DMA_DeInit(DMA1_CHANNEL4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x40013804;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PERIPHERALDST;
    DMA_InitStructure.DMA_BufferSize = BufferSize;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PERIPHERALDATASIZE_BYTE;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MEMORYDATASIZE_BYTE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA1_CHANNEL4, &DMA_InitStructure);
    
    DMA_INTConfig(DMA1_CHANNEL1, DMA_INT_TC, ENABLE);
    
    USART_DMA_Enable(USART1, USART_DMAREQ_TX, ENABLE);
    
    /* Enable DMA transfer */
    DMA_Enable(DMA1_CHANNEL1, ENABLE);
    DMA_Enable(DMA1_CHANNEL4, ENABLE);
    
    Delay(300); 
    
    TransferStatus = Buffercmp(SRC_Const_Buffer, DST_Buffer, BufferSize);
    if((TransferStatus == PASSED) && (int_count == 2) )
    {
        printf("\r\nSUCESS \r\n");
    } 
    else
    {
        printf("\r\nERROR \r\n");
    }
    
    
    while (1)
    {
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
    /* Enable DMA1¡¢DMA2 clock */
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_DMA1, ENABLE);
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_DMA2, ENABLE);

}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara NVIC_InitStructure;

    /* Configure three bit for preemption priority */
    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_1);
    /* Enable DMA1 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQ = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);  
         
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


TestStatus Buffercmp(const uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer != *pBuffer1)
        {
            return FAILED;
        }
        pBuffer++;
        pBuffer1++;
    }
    return PASSED;
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
