/**
  ******************************************************************************
  * @file    gd32f20x_bkp.c
  * @author  MCU SD 
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   BKP functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_bkp.h"
#include "gd32f20x_rcc.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */
  
/** @defgroup BKP
  * @brief BKP driver modules
  * @{
  */

/** @defgroup BKP_Private_Defines
  * @{
  */
/* RTCOUTPUT bit is masked in BKP_RCCR register */
#define RTCOUTPUT_MASK           ((uint16_t)0xFC7F)

#define TAMPER1_TEF_SHIFT        8
#define TAMPER1_TIF_SHIFT        8
#define TAMPER2_TEF_SHIFT        9
#define TAMPER2_TIF_SHIFT        9

/**
  * @}
  */ 

/** @defgroup BKP_Private_Functions
  * @{
  */

/**
  * @brief  Reset the BKP peripheral registers.
  * @param  None
  * @retval None
  */
void BKP_DeInit(void)
{
    RCC_BackupReset_Enable(ENABLE);
    RCC_BackupReset_Enable(DISABLE);
}

/**
  * @brief  Write user data to the BKP_DRx register.
  * @param  BKP_DR: the Backup Data Register.
  *   This parameter can be BKP_DRx where x can be (1..42)
  * @param  Data: data to write
  * @retval None
  */
void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data)
{
    __IO uint32_t temp = 0;

    temp = (uint32_t)BKP_BASE;
    temp += BKP_DR;

    /* Store the write data */
    *(__IO uint16_t *) temp = Data;
}

/**
  * @brief  Read data from the BKP_DRx register.
  * @param  BKP_DR: The Backup Data Register.
  *   This parameter can be BKP_DRx where x can be (1..42)
  * @retval The content of the BKP_DRx register.
  */
uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR)
{
    __IO uint32_t temp = 0;

    temp = (uint32_t)BKP_BASE; 
    temp += BKP_DR;

    /* Return the BKP_DRx register value */
    return (*(__IO uint16_t *) temp);
}

/**
  * @brief  Configure the RTC output on the TAMPER_1 pin.
  * @param  BKP_RTCOUTPUT: the RTC output.
  *   This parameter can be one of the following values:
  *     @arg BKP_RTCOUTPUT_NULL: no RTC output on the TAMPER_1 pin.
  *     @arg BKP_RTCOUTPUT_CLKCAL: output the RTC clock with frequency
  *                                divided by 64 on the TAMPER_1 pin.
  *     @arg BKP_RTCOUTPUT_ALARM: output the RTC Alarm pulse signal on
  *                               the TAMPER_1 pin.
  *     @arg BKP_RTCOUTPUT_SECOND: output the RTC Second pulse signal on
  *                                the TAMPER_1 pin.  
  * @retval None
  */
void BKP_RTCOutputConfig(uint16_t BKP_RTCOUTPUT)
{
    uint16_t temp = 0;

    temp = BKP->RCCR;
    
    /* Clear RCCOE, ROE and ROS bits */
    temp &= RTCOUTPUT_MASK;
  
    /* Set RCCOE, ROE and ROS bits according to BKP_RTCOUTPUT value */
    temp |= BKP_RTCOUTPUT;
    
    /* Store the new value */
    BKP->RCCR = temp;
}

/**
  * @brief  Set RTC clock calibration direction.
  * @param  BKP_RTCCLKCALDIR: the RTC clock calibration direction.
  *   This parameter can be one of the following values:
  *     @arg BKP_RTCCLKCALDIR_UP: RTC clock calibration direction is speed up.
  *     @arg BKP_RTCCLKCALDIR_DOWN: RTC clock calibration direction is slowed down.
  * @retval None
  */
void BKP_SetRTCCalibrationDir(uint16_t BKP_RTCCLKCALDIR)
{
    uint16_t temp = 0;

    temp = BKP->RCCR;
    
    /* Clear CALDIR bit */
    temp &= ~BKP_RCCR_CALDIR;
    
    /* Set CALDIR bit according to BKP_RTCCLKCALDIR value */
    temp |= BKP_RTCCLKCALDIR;
    
    /* Store the new value */
    BKP->RCCR = temp;
}

/**
  * @brief  Set RTC Clock Calibration value.
  * @param  CalibrationValue: the RTC Clock Calibration value.
  *   This parameter must be a number between 0 and 0x7F.
  * @retval None
  */
void BKP_SetRTCCalibrationValue(uint8_t CalibrationValue)
{
    uint16_t temp = 0;

    temp = BKP->RCCR;
    
    /* Clear RCCV[6:0] bits */
    temp &= ~BKP_RCCR_RCCV;
    
    /* Set CAL[6:0] bits according to CalibrationValue */
    temp |= CalibrationValue;
    
    /* Store the new value */
    BKP->RCCR = temp;
}

/**
  * @brief  Configure the RTC clock output selection.
  * @param  BKP_RTCCLKOUTPUTSEL: the RTC clock output selection.
  *   This parameter can be one of the following values:
  *     @arg BKP_RTCCLKOUTPUTSEL_DIV64: RTC clock output is RTCCLK/64.
  *     @arg BKP_RTCCLKOUTPUTSEL_DIV1: RTC clock output is RTCCLK.
  * @retval None
  */
void BKP_RTCCLKOutputConfig(uint16_t BKP_RTCCLKOUTPUTSEL)
{
    uint16_t temp = 0;

    temp = BKP->RCCR;
    
    /* Clear CCOSEL bit */
    temp &= ~BKP_RCCR_CCOSEL;
    
    /* Set CCOSEL bit according to BKP_RTCCLKOUTPUTSEL value */
    temp |= BKP_RTCCLKOUTPUTSEL;
    
    /* Store the new value */
    BKP->RCCR = temp;
}

/**
  * @brief  Configure the TAMPER_1 pin active level.
  * @param  BKP_TPAL1: the TAMPER_1 pin active level.
  *   This parameter can be one of the following values:
  *     @arg BKP_TPAL1_HIGH: TAMPER_1 pin active on high level
  *     @arg BKP_TPAL1_LOW: TAMPER_1 pin active on low level
  * @param  NewValue: New value of the TAMPER_1 pin state.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void BKP_Tamper_1PinConfig(uint16_t BKP_TPAL1, TypeState NewValue)
{
    uint16_t temp1 = 0,temp2 = 0;
    
    temp1 = BKP->TPCR1;
    
    /* Clear TPE_1 bit */
    temp1 &= ~((uint16_t)BKP_TPCR1_TPE_1);
    
    /* Configure TPAL_1 bit according to BKP_TPAL1 value */
    temp1 |= BKP_TPAL1;
    
    /* Store the new value */
    BKP->TPCR1 = temp1;

    temp2 = BKP->TPCR2;

    /* Clear TPM_1 and TPM_2 bits */
    temp2 &= ~((uint16_t)(BKP_TPCR2_TPM_1 | BKP_TPCR2_TPM_2));

    /* Store the new value */
    BKP->TPCR2 = temp2;
    
    /* Enable the TAMPER_1 pin */
    if (NewValue != DISABLE)
    {
        /* The TAMPER_1 pin is dedicated for the Backup Reset function */
        BKP->TPCR1 |= BKP_TPCR1_TPE_1;
    }
    else
    {
        /* The TAMPER_1 pin is free for GPIO functions */
        BKP->TPCR1 &= ~((uint16_t)BKP_TPCR1_TPE_1);
    }
}

/**
  * @brief  Configure the TAMPER_2 pin active level.
  * @param  BKP_TPAL2: the TAMPER_2 pin active level.
  *   This parameter can be one of the following values:
  *     @arg BKP_TPAL2_HIGH: TAMPER_2 pin active on high level
  *     @arg BKP_TPAL2_LOW: TAMPER_2 pin active on low level
  * @param  NewValue: New value of the TAMPER_2 pin state.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void BKP_Tamper_2PinConfig(uint16_t BKP_TPAL2, TypeState NewValue)
{
    uint16_t temp1 = 0,temp2 = 0;
    
    temp1 = BKP->TPCR2;
    
    /* Clear TPE_2 bit */
    temp1 &= ~((uint16_t)BKP_TPCR2_TPE_2);
    
    /* Configure TPAL_2 bit according to BKP_TPAL2 value */
    temp1 |= BKP_TPAL2;
    
    /* Store the new value */
    BKP->TPCR2 = temp1;

    temp2 = BKP->TPCR2;

    /* Clear TPM_1 and TPM_2 bits */
    temp2 &= ~((uint16_t)(BKP_TPCR2_TPM_1 | BKP_TPCR2_TPM_2));

    /* Store the new value */
    BKP->TPCR2 = temp2;
    
    /* Enable the TAMPER_2 pin */
    if (NewValue != DISABLE)
    {
        /* The TAMPER_2 pin is dedicated for the Backup Reset function */
        BKP->TPCR2 |= BKP_TPCR2_TPE_2;
    }
    else
    {
        /* The TAMPER_2 pin is free for GPIO functions */
        BKP->TPCR2 &= ~((uint16_t)BKP_TPCR2_TPE_2);
    }
}

/**
  * @brief  Enable or disable the Tamper_1 Interrupt.
  * @param  NewValue: New value of the Tamper_1 Interrupt state.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void BKP_Tamper_1INT_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the Tamper_1 interrupt */
        BKP->TIER |= BKP_TIER_TIE_1;
    }
    else
    {
        /* Disable the Tamper_1 interrupt */
        BKP->TIER &= ~BKP_TIER_TIE_1;
    }
}

/**
  * @brief  Enable or disable the Tamper_2/Waveform Detect Interrupt.
  * @param  NewValue: New value of the Tamper_2/Waveform Detect Interrupt state.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void BKP_Tamper_2INT_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the Tamper_2 or Waveform detect interrupt */
        BKP->TIER |= BKP_TIER_TIE_2;
    }
    else
    {
        /* Disable the Tamper_2 or Waveform detect interrupt */
        BKP->TIER &= ~BKP_TIER_TIE_2;
    }
}

/**
  * @brief  Enable or disable the Waveform detection function.
  * @param  BKP_WAVEFORMDETECT: the Waveform detection.
  *   This parameter can be one of the following values:
  *     @arg BKP_WAVEFORMDETECT_1
  *     @arg BKP_WAVEFORMDETECT_2
  * @param  NewValue: New value of the Waveform detection state.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void BKP_WaveformDetect_Enable(uint16_t BKP_WAVEFORMDETECT, TypeState NewValue)
{
    uint32_t temp_1 = 0,temp_2 = 0,temp_3 = 0;
    
    temp_1 = BKP->TPCR1;
    temp_2 = BKP->TPCR2;
  
    /* Clear TPE_1 and TPE_2 bits */
    temp_1 &= ~((uint16_t)BKP_TPCR1_TPE_1);
    temp_2 &= ~((uint16_t)BKP_TPCR2_TPE_2);

    temp_3 = BKP->RCCR;

    /* Clear CCOSEL bit */
    temp_3 &= ~((uint16_t)BKP_RCCR_CCOSEL);

    /* Store the new value */
    BKP->TPCR1 = temp_1;
    BKP->TPCR2 = temp_2;
    BKP->RCCR = temp_3;

    if (NewValue != DISABLE)
    {
        /* Enable the Waveform detection function */
        BKP->TPCR2 |= BKP_WAVEFORMDETECT;
    }
    else
    {
        /* Disable the Waveform detection function */
        BKP->TPCR2 &= ~BKP_WAVEFORMDETECT;
    }
}

/**
  * @brief  Get the bit flag of Tamper or Waveform Detection Event.
  * @param  BKP_FLAG: the flag of BKP_TIER registers.
  *   This parameter can be one of the following values:
  *     @arg BKP_FLAG_TAMPER1: Tamper_1 event flag.
  *     @arg BKP_FLAG_TAMPER2: Tamper_2/Waveform detection event flag.
  * @retval The new value of Tamper Event flag (SET or RESET).
  */
TypeState BKP_GetBitState(uint16_t BKP_FLAG)
{
    /* Check and get the Tamper Event flag */
    if ((BKP->TIER & BKP_FLAG) != (uint16_t)RESET)
    {
        /* Tamper or Waveform Detection Event occured */
        return SET;
    }
    else 
    {
        /* No Tamper or Waveform Detection Event occured */
        return RESET;
    }
}

/**
  * @brief  Clear the bit flag of Tamper or Waveform Detection Event.
  * @param  BKP_FLAG: the flag of BKP_TIER registers.
  *   This parameter can be one of the following values:
  *     @arg BKP_FLAG_TAMPER1: Tamper_1 event flag.
  *     @arg BKP_FLAG_TAMPER2: Tamper_2/Waveform detection event flag.
  * @retval None
  */
void BKP_ClearBitState(uint16_t BKP_FLAG)
{
    /* Check the TAMPER1 or TAMPER2 */
    if(BKP_FLAG == BKP_FLAG_TAMPER1) 
    {
        /* Clear the Tamper_1 event flag */
        BKP->TIER |= BKP_FLAG >> TAMPER1_TEF_SHIFT;
    }
    else
    {
        /* Clear the Tamper_2 or Waveform detection event flag */
        BKP->TIER |= BKP_FLAG >> TAMPER2_TEF_SHIFT;
    }
}

/**
  * @brief  Get the interrupt bit flag of Tamper or Waveform Detection Interrupt.
  * @param  BKP_INT: the interrupt flag of BKP_TIER registers.
  *   This parameter can be one of the following values:
  *     @arg BKP_INT_TAMPER1: Tamper_1 interrupt flag.
  *     @arg BKP_INT_TAMPER2: Tamper_2/Waveform detection interrupt flag.
  * @retval The new value of the Tamper Interrupt flag (SET or RESET).
  */
TypeState BKP_GetIntBitState(uint16_t BKP_INT)
{
    /* Check and get the Tamper Interrupt flag */
    if ((BKP->TIER & BKP_INT) != (uint16_t)RESET)
    {
        /* Tamper or Waveform Detection Interrupt occured */
        return SET;
    }
    else 
    {
        /* No Tamper or Waveform Detection Interrupt occured */
        return RESET;
    }
}

/**
  * @brief  Clear the interrupt bit flag of Tamper or Waveform Detection Interrupt.
  * @param  BKP_INT: the interrupt flag of BKP_TIER registers.
  *   This parameter can be one of the following values:
  *     @arg BKP_INT_TAMPER1: Tamper_1 interrupt flag.
  *     @arg BKP_INT_TAMPER2: Tamper_2/Waveform detection interrupt flag.
  * @retval None
  */
void BKP_ClearIntBitState(uint16_t BKP_INT)
{
    /* Check the TAMPER1 or TAMPER2 */
    if(BKP_INT == BKP_INT_TAMPER1) 
    {
        /* Clear the Tamper_1 interrupt flag */
        BKP->TIER |= BKP_INT >> TAMPER1_TIF_SHIFT;
    }
    else
    {
        /* Clear the Tamper_2 or Waveform detection interrupt flag */
        BKP->TIER |= BKP_INT >> TAMPER2_TIF_SHIFT;
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
