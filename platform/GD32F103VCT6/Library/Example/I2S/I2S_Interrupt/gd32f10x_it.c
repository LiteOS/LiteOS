/**
  ******************************************************************************
  * @file    I2S/I2S_Interrupt/gd32f10x_it.c
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
extern const uint16_t I2S3_Buffer_Tx[32];
extern uint16_t I2S2_Buffer_Rx[32];
extern  __IO uint32_t TxIdx, RxIdx;

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
  * @brief  This function handles SPI3 global interrupt request.
  * @param  None
  * @retval None
  */
void SPI3_IRQHandler(void)
{
    /* Check the interrupt source */
    if (SPI_I2S_GetIntBitState(SPI3, SPI_I2S_INT_TBE) == SET)
    {
        /* Send a data from I2S3 */
        SPI_I2S_SendData(SPI3, I2S3_Buffer_Tx[TxIdx++]);
    }

    /* Check the end of buffer transfer */
    if (RxIdx == 32)
    {
        /* Disable the I2S3 TXE interrupt to end the communication */
        SPI_I2S_INTConfig(SPI3, SPI_I2S_INT_TBE, DISABLE);
    }
}

/**
  * @brief  This function handles SPI2 global interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{
    /* Check the interrupt source */
    if (SPI_I2S_GetIntBitState(SPI2, SPI_I2S_INT_RBNE) == SET)
    {
        /* Store the I2S2 received data in the relative data table */
        I2S2_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI2);
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
