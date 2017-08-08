/**
  ******************************************************************************
  * @file    DMA/FLASH_RAM/main.c
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

#define BufferSize                       2048
#define FMC_PAGE_SIZE                    ((uint16_t)0x400)

#define BANK1_WRITE_START_ADDR           ((uint32_t)0x08004000)
#define BANK1_WRITE_END_ADDR             ((uint32_t)0x08004800)
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
DMA_InitPara  DMA_InitStructure;
__IO TestStatus TransferStatus = FAILED;
uint32_t DST_Buffer[512];
uint32_t *ptrd;
uint32_t m2m = 0,m2u =0;
__IO uint32_t count = 0;

uint32_t EraseCounter = 0x00, Address = 0x00 ,wrp = 0,wrp2 = 0;
uint32_t Data = 0x0123ABCD;
__IO uint32_t NbrOfPage = 0x00;
volatile FMC_State FMCStatus = FMC_READY;
volatile TestStatus MemoryProgramStatus = PASSED;

void RCC_Configuration(void);
void NVIC_Configuration(void);

void USART_Configuration(void);


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
       
    uint32_t i;
 
    TransferStatus = PASSED;
    
    /* System Clocks Configuration */
    RCC_Configuration();
         
    /* NVIC configuration */
    NVIC_Configuration();
    
    LED_config() ;
    GPIO_ResetBits(GPIO_LED1_PORT, GPIO_LED1);
    GPIO_ResetBits(GPIO_LED2_PORT, GPIO_LED3);
    GPIO_ResetBits(GPIO_LED3_PORT, GPIO_LED2);
    GPIO_ResetBits(GPIO_LED4_PORT, GPIO_LED4);

    /* Unlock the Flash Bank1 Program Erase controller */
    FMC_Unlock();
    
    /* Define the number of page to be erased */
    NbrOfPage = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR) / FMC_PAGE_SIZE;
    
    /* Clear All pending flags */
    FMC_ClearBitState(FMC_FLAG_EOP | FMC_FLAG_WERR | FMC_FLAG_PERR );
    
    /* Erase the FLASH pages */
    for(EraseCounter = 0; EraseCounter < NbrOfPage; EraseCounter++)
    {
        FMCStatus = FMC_ErasePage(BANK1_WRITE_START_ADDR + (FMC_PAGE_SIZE * EraseCounter));
        wrp += (FMCStatus == FMC_WRPERR);
        FMC_ClearBitState(FMC_FLAG_EOP | FMC_FLAG_WERR | FMC_FLAG_PERR );
    }

    FMC_Lock();
    
    ptrd = (uint32_t*)BANK1_WRITE_START_ADDR;
    for(i = 0; i < 512; i++)
    {
        if(*ptrd != 0xFFFFFFFF)
        { 
            TransferStatus = FAILED;
            goto fail;
        }
        ptrd++;
    }

    /* Unlock the Flash Bank1 Program Erase controller */
    FMC_Unlock();
    
    /* Define the number of page to be erased */
    NbrOfPage = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR) / FMC_PAGE_SIZE;
    
    /* Clear All pending flags */
    FMC_ClearBitState(FMC_FLAG_EOP | FMC_FLAG_WERR | FMC_FLAG_PERR );    
    
    /* Program Flash Bank1 */
    Address = BANK1_WRITE_START_ADDR;
    wrp2 = 0;
    while(Address < BANK1_WRITE_END_ADDR)
    {
        FMCStatus = FMC_ProgramWord(Address, Data);
        Address = Address + 4; 
        wrp2 += (FMCStatus == FMC_WRPERR);
        FMC_ClearBitState(FMC_FLAG_EOP | FMC_FLAG_WERR | FMC_FLAG_PERR );    
    }
    
    FMC_Lock();

    /* DMA1 channel1 configuration */
    DMA_DeInit(DMA1_CHANNEL1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)BANK1_WRITE_START_ADDR;
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
    
    DMA_INTConfig(DMA1_CHANNEL1, DMA_INT_TC, ENABLE);
    
    /* Enable DMA transfer */
    DMA_Enable(DMA1_CHANNEL1, ENABLE);

    for(i = 0; i < 10000; i++)
    {
        if(count)
        break;
    }    
    
    ptrd = DST_Buffer;
    for(i = 0; i < 512; i++)
    {
        if(*ptrd != Data)
        { 
            TransferStatus = FAILED;
            goto fail;
        }
        ptrd++;
    }
    
    if(TransferStatus != FAILED && count == 1)
    {
        GPIO_ResetBits(GPIO_LED1_PORT, GPIO_LED1);
        GPIO_ResetBits(GPIO_LED3_PORT, GPIO_LED3);
        GPIO_SetBits(GPIO_LED2_PORT, GPIO_LED2);
        GPIO_SetBits(GPIO_LED4_PORT, GPIO_LED4);
    }
    else
    {
fail:
        GPIO_ResetBits(GPIO_LED2_PORT, GPIO_LED2);
        GPIO_ResetBits(GPIO_LED4_PORT, GPIO_LED4);
        GPIO_SetBits(GPIO_LED1_PORT, GPIO_LED1);
        GPIO_SetBits(GPIO_LED3_PORT, GPIO_LED3);
    }
   
    while(1)
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
    /* Enable DMA1 clock */
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_DMA1, ENABLE);

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





/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
