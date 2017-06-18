/**
  ******************************************************************************
  * @file    gd32f20x_rng.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   RNG functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_rng.h"
#include "gd32f20x_rcc.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup RNG 
  * @brief RNG driver modules
  * @{
  */ 

/* Private functions ---------------------------------------------------------*/

/** @defgroup RNG_Private_Functions
  * @{
  */ 

/**
  * @brief  Reset the RNG peripheral.
  * @param  None
  * @retval None
  */
void RNG_DeInit(void)
{
    /* Enable RNG reset state */
    RCC_AHB2PeriphClock_Enable(RCC_AHB2PERIPH_RNG, ENABLE);

    /* Release RNG from reset state */
    RCC_AHB2PeriphClock_Enable(RCC_AHB2PERIPH_RNG, DISABLE);
}

/**
  * @brief  Enable or disable the RNG peripheral.
  * @param  NewValue: new value of the RNG peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RNG_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the RNG */
        RNG->CTLR |= RNG_CTLR_RNGEN;
    }
    else
    {
        /* Disable the RNG */
        RNG->CTLR &= ~RNG_CTLR_RNGEN;
    }
}

/**
  * @brief  Return a 32-bit random number.
  * @param  None
  * @retval 32-bit random number.
  */
uint32_t RNG_GetRandomData(void)
{
    /* Return the 32 bit random number from the DR register */
    return RNG->DTR;
}

/**
  * @brief  Enable or disable the RNG interrupt.
  * @param  NewValue: new value of the RNG interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RNG_INTConfig(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the RNG interrupt */
        RNG->CTLR |= RNG_CTLR_IE;
    }
    else
    {
        /* Disable the RNG interrupt */
        RNG->CTLR &= ~RNG_CTLR_IE;
    }
}

/**
  * @brief  Get the bit flag of STR register.
  * @param  RNG_FLAG: the flag of STR register.
  *   This parameter can be one of the following values:
  *     @arg RNG_FLAG_DRDY: Data Ready flag.
  *     @arg RNG_FLAG_CECS: Clock Error Current flag.
  *     @arg RNG_FLAG_SECS: Seed Error Current flag.
  * @retval The new value of RNG_FLAG (SET or RESET).
  */
TypeState RNG_GetBitState(uint8_t RNG_FLAG)
{
    /* Check the status of the specified RNG flag */
    if ((RNG->STR & RNG_FLAG) != (uint8_t)RESET)
    {
        /* RNG_FLAG is set */
        return SET;
    }
    else
    {
        /* RNG_FLAG is reset */
        return RESET;
    }
}

/**
  * @brief  Clear the bit flag of STR registers.
  * @param  RNG_FLAG: the flag of STR register. 
  *   This parameter can be any combination of the following values:
  *     @arg RNG_FLAG_CECS: Clock Error Current flag.
  *     @arg RNG_FLAG_SECS: Seed Error Current flag.                        
  * @retval None
  */
void RNG_ClearBitState(uint8_t RNG_FLAG)
{
    /* Clear the selected RNG flags */
    RNG->STR = ~(uint32_t)(((uint32_t)RNG_FLAG) << 4);
}

/**
  * @brief  Get the interrupt bit flag.
  * @param  RNG_INT: the interrupt bit flag.
  *   This parameter can be one of the following values:
  *     @arg RNG_INT_CEI: Clock Error Interrupt.
  *     @arg RNG_INT_SEI: Seed Error Interrupt.                   
  * @retval The new value of RNG_INT (SET or RESET).
  */
TypeState RNG_GetIntBitState(uint8_t RNG_INT)
{
    /* Check the status of the specified RNG interrupt */
    if ((RNG->STR & RNG_INT) != (uint8_t)RESET)
    {
        /* RNG_INT is set */
        return SET;
    }
    else
    {
        /* RNG_INT is reset */
        return RESET;
    }
}


/**
  * @brief  Clear the interrupt bit flag.
  * @param  RNG_INT: the interrupt bit flag.
  *   This parameter can be any combination of the following values:
  *     @arg RNG_INT_CEI: Clock Error Interrupt.
  *     @arg RNG_INT_SEI: Seed Error Interrupt.
  * @retval None
  */
void RNG_ClearIntBitState(uint8_t RNG_INT)
{
    /* Clear the selected RNG interrupt pending bit */
    RNG->STR = (uint8_t)~RNG_INT;
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
