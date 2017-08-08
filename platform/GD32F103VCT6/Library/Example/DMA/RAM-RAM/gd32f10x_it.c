/**
  ******************************************************************************
  * @file    DMA/RAM_RAM/gd32f10x_it.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014   
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h"
#include "gd32f10x_dma.h"
#include "gd32f10x_usart.h"
#include "main.h"


/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t count;

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
 

}

void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetBitState(DMA1_INT_HT1))
    {     
        DMA_ClearIntBitState(DMA1_INT_HT1);
        count++;
        printf("TxHT11 \r\n");
    }
    if(DMA_GetBitState(DMA1_INT_TC1))
    {     
        count++;
        printf("TxTC11 \r\n");
        DMA_ClearIntBitState(DMA1_INT_GL1);
    }
}

void DMA1_Channel2_IRQHandler(void)
{
    if(DMA_GetBitState(DMA1_INT_HT2))
    {     
        DMA_ClearIntBitState(DMA1_INT_HT2);
        count++;
        printf("TxHT12 \r\n");
    }
    if(DMA_GetBitState(DMA1_INT_TC2))
    {     
        count++;
        printf("TxTC12 \r\n");
        DMA_ClearIntBitState(DMA1_INT_GL2);    
    }
}

void DMA1_Channel3_IRQHandler(void)
{
    if(DMA_GetBitState(DMA1_INT_HT3))
    {   
        DMA_ClearIntBitState(DMA1_INT_HT3);
        count++;
        printf("TxHT13 \r\n");
    }
    if(DMA_GetBitState(DMA1_INT_TC3))
    {     
        count++;
        printf("TxTC13 \r\n");
        DMA_ClearIntBitState(DMA1_INT_GL3);    
    }
}

void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetBitState(DMA1_INT_HT4))
    {     
        DMA_ClearIntBitState(DMA1_INT_HT4);
        count++;
        printf("TxHT14 \r\n");
    }
    if(DMA_GetBitState(DMA1_INT_TC4))
    {     
        count++;
        printf("TxTC14 \r\n");
        DMA_ClearIntBitState(DMA1_INT_GL4);    
    }
}


void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetBitState(DMA1_INT_HT5))
    {     
        DMA_ClearIntBitState(DMA1_INT_HT5);
        count++;
        printf("TxHT15 \r\n");
    }
    if(DMA_GetBitState(DMA1_INT_TC5))
    {     
        count++;
        printf("TxTC15 \r\n");
        DMA_ClearIntBitState(DMA1_INT_GL5);    
    }
}
/**
  * @brief  This function handles DMA1_Channel6_7 Handler.
  * @param  None
  * @retval None
  */
void DMA1_Channel6_IRQHandler(void)
{
    if(DMA_GetBitState(DMA1_INT_HT6))
    {     
        DMA_ClearIntBitState(DMA1_INT_HT6);
        count++;
        printf("TxHT16 \r\n");
    }
    if(DMA_GetBitState(DMA1_INT_TC6))
    {     
        count++;
        printf("TxTC16 \r\n");
        DMA_ClearIntBitState(DMA1_INT_GL6);    
    }

}
void DMA1_Channel7_IRQHandler(void)
{
    if(DMA_GetBitState(DMA1_INT_HT7))
    {     
        DMA_ClearIntBitState(DMA1_INT_HT7);
        count++;
        printf("TxHT17 \r\n");
    }
    if(DMA_GetBitState(DMA1_INT_TC7))
    {     
        printf("TxTC17 \r\n");
        count++;
        DMA_ClearIntBitState(DMA1_INT_GL7);    
    }
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/



/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
