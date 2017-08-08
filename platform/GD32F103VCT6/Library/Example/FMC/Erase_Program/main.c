/**
  ******************************************************************************
  * @file    FMC/Erase_Program/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h" 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize                   2048
#define FMC_PAGE_SIZE                ((uint16_t)0x400)
#define BANK1_WRITE_START_ADDR       ((uint32_t)0x08004000)
#define BANK1_WRITE_END_ADDR         ((uint32_t)0x08004800)
#define RCC_GPIO_LED                 RCC_AHBPeriph_GPIOC
#define GPIO_LED1_PORT               GPIOC
#define GPIO_LED2_PORT               GPIOC
#define GPIO_LED1                    GPIO_PIN_0
#define GPIO_LED2                    GPIO_PIN_2

#define RCC_GPIO4_LED                RCC_AHBPeriph_GPIOE
#define GPIO_LED3_PORT               GPIOE
#define GPIO_LED4_PORT               GPIOE
#define GPIO_LED3                    GPIO_PIN_0   
#define GPIO_LED4                    GPIO_PIN_1

/* Private variables ---------------------------------------------------------*/
uint32_t m2m = 0,m2u =0;
__IO TestStatus TransferStatus = FAILED;
uint32_t *ptrd;
uint32_t EraseCounter = 0x00, Address = 0x00 ,wrp = 0,wrp2 = 0;
uint32_t Data = 0x1234ABCD;
__IO uint32_t NbrOfPage = 0x00;
volatile FMC_State FMCStatus = FMC_READY;
volatile TestStatus MemoryProgramStatus = PASSED;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void LED_config(void);

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
    
    ptrd = (uint32_t*)BANK1_WRITE_START_ADDR;
    for(i = 0; i < 512; i++)
    {
        if(*ptrd != Data)
        { 
            TransferStatus = FAILED;
            goto fail;
        }
        ptrd++;
    }
    
    if(TransferStatus != FAILED)
    {
        GPIO_ResetBits(GPIO_LED1_PORT, GPIO_LED1);
        GPIO_ResetBits(GPIO_LED3_PORT, GPIO_LED3);
        GPIO_SetBits(GPIO_LED2_PORT, GPIO_LED2);
        GPIO_SetBits(GPIO_LED4_PORT, GPIO_LED4);
    }else{
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
  
void LED_config(void)
{
    GPIO_InitPara GPIO_InitStructure;
    
    /* Enable GPIOC, GPIOE */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_GPIOE, ENABLE);
      
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_LED1 | GPIO_LED2 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin =GPIO_LED3 | GPIO_LED4;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
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

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
