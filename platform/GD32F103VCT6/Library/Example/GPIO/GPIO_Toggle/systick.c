/**
  ******************************************************************************
  * @file    GPIO/GPIO_Toggle/systick.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The SysTick configuration file
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "systick.h"

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure SysTick
  * @param  None
  * @retval None
  */
void SysTick_Configuration(void)
{
    /* Setup SysTick Timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
