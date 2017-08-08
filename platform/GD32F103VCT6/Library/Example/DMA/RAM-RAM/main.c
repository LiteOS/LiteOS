/**
  ******************************************************************************
  * @file    DMA/RAM_RAM/main.c 
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
#define BufferSize                  16
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
__IO uint32_t count = 0;
uint32_t m2m = 0,m2u =0;
DMA_InitPara  DMA_InitStructure;
__IO TestStatus TransferStatus = FAILED;
__IO TestStatus TransferStatus1 = FAILED;
__IO TestStatus TransferStatus2 = FAILED;
__IO TestStatus TransferStatus3 = FAILED;
__IO TestStatus TransferStatus4 = FAILED;
__IO TestStatus TransferStatus5 = FAILED;
__IO TestStatus TransferStatus6 = FAILED;
__IO TestStatus TransferStatus7 = FAILED;
const uint8_t SRC_Const_Buffer[BufferSize]= {
                                    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,
                                    0x0B,0x0C,0x0D,0x0E,0x0F,0x10};
uint8_t DST_Buffer1[BufferSize];
uint8_t DST_Buffer2[BufferSize];
uint8_t DST_Buffer3[BufferSize];
uint8_t DST_Buffer4[BufferSize];
uint8_t DST_Buffer5[BufferSize];
uint8_t DST_Buffer6[BufferSize];
uint8_t DST_Buffer7[BufferSize];

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
TestStatus Buffercmp(const uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength);
void USART_Configuration(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    int i = 0;
       
    /* System Clocks Configuration */
    RCC_Configuration();
                           
    /* NVIC configuration */
    NVIC_Configuration();
                         
    LED_config() ;
    GPIO_SetBits(GPIO_LED1_PORT, GPIO_LED1);
    GPIO_ResetBits(GPIO_LED3_PORT, GPIO_LED3);
    GPIO_ResetBits(GPIO_LED2_PORT, GPIO_LED2);
    GPIO_ResetBits(GPIO_LED4_PORT, GPIO_LED4);
   
    USART_Configuration();

    /* DMA1 channel1 configuration */
    DMA_DeInit(DMA1_CHANNEL1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer1;
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
    DMA_DeInit(DMA1_CHANNEL2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer2;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_Init(DMA1_CHANNEL2, &DMA_InitStructure);
        
    /* DMA1 channel3 configuration */
    DMA_DeInit(DMA1_CHANNEL3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer3;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_Init(DMA1_CHANNEL3, &DMA_InitStructure);
        
    /* DMA1 channel4 configuration */
    DMA_DeInit(DMA1_CHANNEL4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer4;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_Init(DMA1_CHANNEL4, &DMA_InitStructure);
        
    /* DMA1 channel5 configuration */
    DMA_DeInit(DMA1_CHANNEL5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer5;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_Init(DMA1_CHANNEL5, &DMA_InitStructure);
    
    /* DMA1 channel6 configuration */
    DMA_DeInit(DMA1_CHANNEL6);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer6;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_Init(DMA1_CHANNEL6, &DMA_InitStructure);
    
    /* DMA1 channel5 configuration */
    DMA_DeInit(DMA1_CHANNEL7);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer7;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_Init(DMA1_CHANNEL7, &DMA_InitStructure);

    DMA_INTConfig(DMA1_CHANNEL1, DMA_INT_TC, ENABLE);
    DMA_INTConfig(DMA1_CHANNEL2, DMA_INT_TC, ENABLE);
    DMA_INTConfig(DMA1_CHANNEL3, DMA_INT_TC, ENABLE);
    DMA_INTConfig(DMA1_CHANNEL4, DMA_INT_TC, ENABLE);
    DMA_INTConfig(DMA1_CHANNEL5, DMA_INT_TC, ENABLE);
    DMA_INTConfig(DMA1_CHANNEL6, DMA_INT_TC, ENABLE);
    DMA_INTConfig(DMA1_CHANNEL7, DMA_INT_TC, ENABLE);
    
    /* Enable DMA transfer */
    DMA_Enable(DMA1_CHANNEL1, ENABLE);
    DMA_Enable(DMA1_CHANNEL2, ENABLE);
    DMA_Enable(DMA1_CHANNEL3, ENABLE);
    DMA_Enable(DMA1_CHANNEL4, ENABLE);
    DMA_Enable(DMA1_CHANNEL5, ENABLE);
    DMA_Enable(DMA1_CHANNEL6, ENABLE);
    DMA_Enable(DMA1_CHANNEL7, ENABLE);
    
    for(i = 0; i < 200; i++)
    {
        if(count == 14)
        break;
    }    
    TransferStatus1 = Buffercmp(SRC_Const_Buffer, DST_Buffer1, BufferSize);
    TransferStatus2 = Buffercmp(SRC_Const_Buffer, DST_Buffer2, BufferSize);
    TransferStatus3 = Buffercmp(SRC_Const_Buffer, DST_Buffer3, BufferSize);
    TransferStatus4 = Buffercmp(SRC_Const_Buffer, DST_Buffer4, BufferSize);
    TransferStatus5 = Buffercmp(SRC_Const_Buffer, DST_Buffer5, BufferSize);    
    TransferStatus6 = Buffercmp(SRC_Const_Buffer, DST_Buffer6, BufferSize);
    TransferStatus7 = Buffercmp(SRC_Const_Buffer, DST_Buffer7, BufferSize);
            
    if(TransferStatus1 == PASSED)
    {
        printf("SUCESS1 \r\n");
    } 
    else
    {
        printf("ERROR1 \r\n");
    }
    if(TransferStatus2 == PASSED)
    {
        printf("SUCESS2 \r\n");
    } 
    else
    {
        printf("ERROR2 \r\n");
    }
    if(TransferStatus3 == PASSED)
    {
        printf("SUCESS3 \r\n");
    } 
    else
    {
        printf("ERROR3 \r\n");
    }
    if(TransferStatus4 == PASSED)
    {
        printf("SUCESS4 \r\n");
    } 
    else
    {
        printf("ERROR4 \r\n");
    }
    if(TransferStatus5 == PASSED)
    {
        printf("SUCESS5 \r\n");
    } 
    else
    {
        printf("ERROR5 \r\n");
    }
    if(TransferStatus6 == PASSED)
    {
        printf("SUCESS6 \r\n");
    } 
    else
    {
        printf("ERROR6 \r\n");
    }
    if(TransferStatus7 == PASSED)
    {
        printf("SUCESS7 \r\n");
    } 
    else
    {
        printf("TxERROR7 \r\n");
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
    NVIC_InitStructure.NVIC_IRQ = DMA1_Channel2_IRQn;
    NVIC_Init(&NVIC_InitStructure);  
    NVIC_InitStructure.NVIC_IRQ = DMA1_Channel3_IRQn;
    NVIC_Init(&NVIC_InitStructure);  
    NVIC_InitStructure.NVIC_IRQ = DMA1_Channel4_IRQn;
    NVIC_Init(&NVIC_InitStructure); 
    NVIC_InitStructure.NVIC_IRQ = DMA1_Channel5_IRQn;
    NVIC_Init(&NVIC_InitStructure); 
    NVIC_InitStructure.NVIC_IRQ = DMA1_Channel6_IRQn;
    NVIC_Init(&NVIC_InitStructure); 
    NVIC_InitStructure.NVIC_IRQ = DMA1_Channel7_IRQn;
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
