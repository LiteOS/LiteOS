/**
  ******************************************************************************
  * @file    BKP/Tamper/gd32f10x_it.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-DEC-2014
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h"

/* Private function prototypes -----------------------------------------------*/
extern uint32_t IsBackupRegClear(void);

/* Private functions ---------------------------------------------------------*/
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
  * @brief  This function handles Tamper interrupt request.
  * @param  None
  * @retval None
  */
void TAMPER_IRQHandler(void)
{
   if(BKP_GetIntBitState() != RESET)
   {    
        /* A Tamper detection event occurred */

	    /* Check if Backup data registers are cleared */
	    if(IsBackupRegClear() == 0)
	    {	
	        /* Backup data registers are cleared */
			
			/* Turn on LED4 */
	        GPIO_SetBits(GPIOE, GPIO_PIN_0);
	    }
	    else
	    {	        
			/* Backup data registers are not cleared */
			
			/* Turn on LED5 */
	        GPIO_SetBits(GPIOE, GPIO_PIN_1);
	    }

	    /* Clear the interrupt bit flag of Tamper Interrupt */
	    BKP_ClearIntBitState();
			
	    /* Clear the bit flag of Tamper Event */
	    BKP_ClearBitState();
	
	    /* Disable the TAMPER pin */
		BKP_TamperPinConfig(BKP_TPAL_LOW, DISABLE);
			
		/* Configure the TAMPER pin active on low level, and enable the TAMPER pin */
		BKP_TamperPinConfig(BKP_TPAL_LOW, ENABLE);
	
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
