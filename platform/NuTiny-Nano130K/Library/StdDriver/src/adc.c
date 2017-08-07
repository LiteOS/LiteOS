/**************************************************************************//**
 * @file     adc.c
 * @version  V1.00
 * $Revision: 7 $
 * $Date: 14/10/06 6:00p $
 * @brief    NANO100 series ADC driver source file
 *
 * @note
 * Copyright (C) 2013-2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "Nano100Series.h"

/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_ADC_Driver ADC Driver
  @{
*/


/** @addtogroup NANO100_ADC_EXPORTED_FUNCTIONS ADC Exported Functions
  @{
*/

/**
  * @brief This API configures ADC module to be ready for convert the input from selected channel
  * @param[in] adc Base address of ADC module
  * @param[in] u32InputMode Input mode (single-end/differential). Valid values are:
  *                 - \ref ADC_INPUT_MODE_SINGLE_END
  *                 - \ref ADC_INPUT_MODE_DIFFERENTIAL
  * @param[in] u32OpMode Operation mode (single/single cycle/continuous). Valid values are:
  *                 - \ref ADC_OPERATION_MODE_SINGLE
  *                 - \ref ADC_OPERATION_MODE_SINGLE_CYCLE
  *                 - \ref ADC_OPERATION_MODE_CONTINUOUS
  * @param[in] u32ChMask Channel enable bit. Each bit corresponds to a input channel. Bit 0 is channel 0, bit 1 is channel 1...
  * @return  None
  * @note This API does not turn on ADC power nor does trigger ADC conversion
  */
void ADC_Open(ADC_T *adc,
              uint32_t u32InputMode,
              uint32_t u32OpMode,
              uint32_t u32ChMask)
{

    ADC->CR = (ADC->CR & ~ADC_CR_DIFF_Msk) | u32InputMode;
    ADC->CR = (ADC->CR & ~ADC_CR_ADMD_Msk) | u32OpMode;
    ADC->CR = (ADC->CR & ~ADC_CR_REFSEL_Msk);
    ADC->CHEN  = u32ChMask;
    return;
}

/**
  * @brief Disable ADC module
  * @param[in] adc Base address of ADC module
  * @return None
  */
void ADC_Close(ADC_T *adc)
{
    SYS->IPRST_CTL2 |= SYS_IPRST_CTL2_ADC_RST_Msk;
    SYS->IPRST_CTL2 &= ~SYS_IPRST_CTL2_ADC_RST_Msk;
    return;

}

/**
  * @brief Configure the hardware trigger condition and enable hardware trigger
  * @param[in] adc Base address of ADC module
  * @param[in] u32Source Decides the hardware trigger source. Valid values are:
  *                 - \ref ADC_TRIGGER_BY_EXT_PIN
  * @param[in] u32Param While ADC trigger by external pin, this parameter
  *                     is used to set trigger condition. Valid values are:
  *                 - \ref ADC_LOW_LEVEL_TRIGGER
  *                 - \ref ADC_HIGH_LEVEL_TRIGGER
  *                 - \ref ADC_FALLING_EDGE_TRIGGER
  *                 - \ref ADC_RISING_EDGE_TRIGGER
  * @return None
  */
void ADC_EnableHWTrigger(ADC_T *adc,
                         uint32_t u32Source,
                         uint32_t u32Param)
{
    ADC->CR &= ~(ADC_CR_TRGE_Msk | ADC_CR_TRGCOND_Msk | ADC_CR_TRGS_Msk);
    ADC->CR |= u32Source | u32Param | ADC_CR_TRGE_Msk;
    return;
}

/**
  * @brief Disable hardware trigger ADC function.
  * @param[in] adc Base address of ADC module
  * @return None
  */
void ADC_DisableHWTrigger(ADC_T *adc)
{
    ADC->CR &= ~(ADC_CR_TRGS_Msk | ADC_CR_TRGCOND_Msk | ADC_CR_TRGE_Msk);
    return;
}

/**
  * @brief Config and enable timer trigger
  * @param[in] adc Base address of ADC module
  * @param[in] u32Source Decides which timer trigger ADC. Valid values are: 0 ~ 3
  * @param[in] u32PDMACnt When timer event occurred, PDMA will transfer u32PDMACnt+1 ADC result
  * @return None
  */
void ADC_EnableTimerTrigger(ADC_T *adc,
                            uint32_t u32Source,
                            uint32_t u32PDMACnt)
{
    ADC->CR &= ~(ADC_CR_TMPDMACNT_Msk | ADC_CR_TMSEL_Msk);
    ADC->CR |= (u32PDMACnt << ADC_CR_TMPDMACNT_Pos) | (u32Source << ADC_CR_TMSEL_Pos) | ADC_CR_TMTRGMOD_Msk;

    return;
}

/**
  * @brief Disable timer trigger ADC function.
  * @param[in] adc Base address of ADC module
  * @return None
  */
void ADC_DisableTimerTrigger(ADC_T *adc)
{
    ADC->CR &= ~ADC_CR_TMTRGMOD_Msk;

    return;
}

/**
  * @brief Configure the extended sampling time
  * @param[in] adc Base address of ADC module
  * @param[in] u32ChNum The channel number
  * @param[in] u32SampleTime Decides the extend sampling counter. Valid values are 0 ~ 15
  * @return None
  */
void ADC_SetExtraSampleTime(ADC_T *adc,
                            uint32_t u32ChNum,
                            uint32_t u32SampleTime)
{

    if (u32ChNum < 8)
        ADC->SMPLCNT0 = (ADC->SMPLCNT0 & ~(ADC_SMPLCNT0_CH0SAMPCNT_Msk << (u32ChNum * 4))) | (u32SampleTime << (u32ChNum * 4));
    else if (u32ChNum < 12)
        ADC->SMPLCNT1 = (ADC->SMPLCNT1 & ~(ADC_SMPLCNT1_CH8SAMPCNT_Msk << ((u32ChNum - 8) * 4))) | (u32SampleTime << ((u32ChNum - 8 ) * 4));
    else
        ADC->SMPLCNT1 = (ADC->SMPLCNT1 & ~ADC_SMPLCNT1_INTCHSAMPCNT_Msk) | (u32SampleTime << ADC_SMPLCNT1_INTCHSAMPCNT_Pos);
}

/**
  * @brief Enable the interrupt(s) selected by u32Mask parameter.
  * @param[in] adc Base address of ADC module
  * @param[in] u32Mask  The combination of interrupt status bits listed below. Each bit
  *                     corresponds to a interrupt status. This parameter decides which
  *                     interrupts will be enabled.
  *                     - \ref ADC_ADF_INT
  *                     - \ref ADC_CMP0_INT
  *                     - \ref ADC_CMP1_INT
  * @return None
  */
void ADC_EnableInt(ADC_T *adc, uint32_t u32Mask)
{
    if(u32Mask & ADC_ADF_INT)
        ADC->CR |= ADC_CR_ADIE_Msk;
    if(u32Mask & ADC_CMP0_INT)
        ADC->CMPR0 |= ADC_CMPR_CMPIE_Msk;
    if(u32Mask & ADC_CMP1_INT)
        ADC->CMPR1 |= ADC_CMPR_CMPIE_Msk;

    return;
}

/**
  * @brief Disable the interrupt(s) selected by u32Mask parameter.
  * @param[in] adc Base address of ADC module
  * @param[in] u32Mask  The combination of interrupt status bits listed below. Each bit
  *                     corresponds to a interrupt status. This parameter decides which
  *                     interrupts will be disabled.
  *                     - \ref ADC_ADF_INT
  *                     - \ref ADC_CMP0_INT
  *                     - \ref ADC_CMP1_INT
  * @return None
  */
void ADC_DisableInt(ADC_T *adc, uint32_t u32Mask)
{
    if(u32Mask & ADC_ADF_INT)
        ADC->CR &= ~ADC_CR_ADIE_Msk;
    if(u32Mask & ADC_CMP0_INT)
        ADC->CMPR0 &= ~ADC_CMPR_CMPIE_Msk;
    if(u32Mask & ADC_CMP1_INT)
        ADC->CMPR1 &= ~ADC_CMPR_CMPIE_Msk;

    return;
}



/*@}*/ /* end of group NANO100_ADC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_ADC_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2013-2014 Nuvoton Technology Corp. ***/
