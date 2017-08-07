/**
  ******************************************************************************
  * @file    SPI/SPI_Simplex_Interrupt/gd32f1x0_it.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The ISR file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h"

/* Private define ------------------------------------------------------------*/
#define BufferSize  32

/* Exported variables ------------------------------------------------------------*/
extern __IO uint8_t TxIdx, RxIdx;
extern uint8_t SPI_MASTER_Buffer_Tx[BufferSize], SPI_SLAVE_Buffer_Rx[BufferSize];

/* Exported functions ------------------------------------------------------- */
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

/**
  * @brief  This function handles SPI1 interrupt request.
  * @param  None
  * @retval None
  */
 void SPI1_IRQHandler(void)

{
    if (SPI_I2S_GetIntBitState(SPI1, SPI_I2S_INT_TBE) != RESET)
    {   
        /* Send SPI_MASTER data */
        while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_TBE) == RESET);
        SPI_I2S_SendData(SPI1, SPI_MASTER_Buffer_Tx[ TxIdx++ ]);
        if (TxIdx == BufferSize)
        {
            /* Disable SPI_MASTER TBE interrupt */
            SPI_I2S_INTConfig(SPI1, SPI_I2S_INT_TBE, DISABLE);
        }
    }
}

/**
  * @brief  This function handles SPI3 global interrupt request.
  * @param  None
  * @retval None
  */
void SPI3_IRQHandler(void)
{
    /* Store SPI_SLAVE received data */
    if (SPI_I2S_GetIntBitState(SPI3, SPI_I2S_INT_RBNE) == SET)
        SPI_SLAVE_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI3);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
