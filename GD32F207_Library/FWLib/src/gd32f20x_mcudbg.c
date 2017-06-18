/**
  ******************************************************************************
  * @file    gd32f20x_mcudbg.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   MCUDBG functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_mcudbg.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup MCUDBG 
  * @brief MCUDBG driver modules
  * @{
  */ 


/** @defgroup MCUDBG_Private_Defines
  * @{
  */

#define IDCODE_DEVID_MASK    ((uint32_t)0x00000FFF)
/**
  * @}
  */


/** @defgroup MCUDBG_Private_Functions
  * @{
  */

/**
  * @brief  Returns the device revision identifier.
  * @param  None
  * @retval Device revision identifier
  */
uint32_t MCUDBG_GetREVID(void)
{
   return(MCUDBG->IDR >> 16);
}

/**
  * @brief  Returns the device identifier.
  * @param  None
  * @retval Device identifier
  */
uint32_t MCUDBG_GetDEVID(void)
{
   return(MCUDBG->IDR & IDCODE_DEVID_MASK);
}

/**
  * @brief  Configure the specified peripheral and low power mode behavior
  *   when the MCU under Debug mode.
  * @param  MCUDBG_Periph: specifies the peripheral and low power mode.
  *   This parameter can be any combination of the following values:
  *     @arg MCUDBG_SLEEP: Keep debugger connection during SLEEP mode              
  *     @arg MCUDBG_DEEPSLEEP: Keep debugger connection during DEEPSLEEP mode               
  *     @arg MCUDBG_STANDBY: Keep debugger connection during STANDBY mode            
  *     @arg MCUDBG_IWDG_STOP: Debug IWDG stopped when Core is halted          
  *     @arg MCUDBG_WWDG_STOP: Debug WWDG stopped when Core is halted          
  *     @arg MCUDBG_TIMER1_STOP: TIMER1 counter stopped when Core is halted          
  *     @arg MCUDBG_TIMER2_STOP: TIMER2 counter stopped when Core is halted          
  *     @arg MCUDBG_TIMER3_STOP: TIMER3 counter stopped when Core is halted          
  *     @arg MCUDBG_TIMER4_STOP: TIMER4 counter stopped when Core is halted          
  *     @arg MCUDBG_CAN1_STOP: Debug CAN1 stopped when Core is halted           
  *     @arg MCUDBG_I2C1_SMBUS_TIMEOUT: I2C1 SMBUS timeout mode stopped when Core is halted
  *     @arg MCUDBG_I2C2_SMBUS_TIMEOUT: I2C2 SMBUS timeout mode stopped when Core is halted
  *     @arg MCUDBG_TIMER5_STOP: TIMER5 counter stopped when Core is halted          
  *     @arg MCUDBG_TIMER6_STOP: TIMER6 counter stopped when Core is halted          
  *     @arg MCUDBG_TIMER7_STOP: TIMER7 counter stopped when Core is halted          
  *     @arg MCUDBG_TIMER8_STOP: TIMER8 counter stopped when Core is halted
  *     @arg MCUDBG_CAN2_STOP: Debug CAN2 stopped when Core is halted 
  *     @arg MCUDBG_TIMER15_STOP: TIMER15 counter stopped when Core is halted
  *     @arg MCUDBG_TIMER16_STOP: TIMER16 counter stopped when Core is halted
  *     @arg MCUDBG_TIMER17_STOP: TIMER17 counter stopped when Core is halted                
  *     @arg MCUDBG_TIMER9_STOP: TIMER9 counter stopped when Core is halted
  *     @arg MCUDBG_TIMER10_STOP: TIMER10 counter stopped when Core is halted
  *     @arg MCUDBG_TIMER11_STOP: TIMER11 counter stopped when Core is halted
  *     @arg MCUDBG_TIMER12_STOP: TIMER12 counter stopped when Core is halted
  *     @arg MCUDBG_TIMER13_STOP: TIMER13 counter stopped when Core is halted
  *     @arg MCUDBG_TIMER14_STOP: TIMER14 counter stopped when Core is halted
  * @param  NewState: new state of the specified peripheral in Debug mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void MCUDBG_PeriphConfig(uint32_t MCUDBG_Periph, TypeState NewValue)
{
	if (NewValue != DISABLE)
	{
		MCUDBG->CTLR |= MCUDBG_Periph;
	}
	else
	{
		MCUDBG->CTLR &= ~MCUDBG_Periph;
	}
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2015 GIGADEVICE *****END OF FILE****/
