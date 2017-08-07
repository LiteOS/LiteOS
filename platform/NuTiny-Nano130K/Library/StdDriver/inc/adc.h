/**************************************************************************//**
 * @file     adc.h
 * @version  V1.00
 * $Revision: 10 $
 * $Date: 15/06/30 2:50p $
 * @brief    NANO100 series ADC driver header file
 *
 * @note
 * Copyright (C) 2013-2014 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_ADC_Driver ADC Driver
  @{
*/

/** @addtogroup NANO100_ADC_EXPORTED_CONSTANTS ADC Exported Constants
  @{
*/

#define ADC_CH_0_MASK                    (1UL << 0)                  /*!< ADC channel 0 mask */
#define ADC_CH_1_MASK                    (1UL << 1)                  /*!< ADC channel 1 mask */
#define ADC_CH_2_MASK                    (1UL << 2)                  /*!< ADC channel 2 mask */
#define ADC_CH_3_MASK                    (1UL << 3)                  /*!< ADC channel 3 mask */
#define ADC_CH_4_MASK                    (1UL << 4)                  /*!< ADC channel 4 mask */
#define ADC_CH_5_MASK                    (1UL << 5)                  /*!< ADC channel 5 mask */
#define ADC_CH_6_MASK                    (1UL << 6)                  /*!< ADC channel 6 mask */
#define ADC_CH_7_MASK                    (1UL << 7)                  /*!< ADC channel 7 mask */
#define ADC_CH_8_MASK                    (1UL << 8)                  /*!< ADC channel 8 mask */
#define ADC_CH_9_MASK                    (1UL << 9)                  /*!< ADC channel 9 mask */
#define ADC_CH_10_MASK                   (1UL << 10)                 /*!< ADC channel 10 mask */
#define ADC_CH_11_MASK                   (1UL << 11)                 /*!< ADC channel 11 mask */
#define ADC_CH_12_MASK                   (1UL << 12)                 /*!< ADC channel 12 mask */
#define ADC_CH_13_MASK                   (1UL << 13)                 /*!< ADC channel 13 mask */
#define ADC_CH_14_MASK                   (1UL << 14)                 /*!< ADC channel 14 mask */
#define ADC_CH_15_MASK                   (1UL << 15)                 /*!< ADC channel 15 mask */
#define ADC_CH_16_MASK                   (1UL << 16)                 /*!< ADC channel 16 mask */
#define ADC_CH_17_MASK                   (1UL << 17)                 /*!< ADC channel 17 mask */
#define ADC_CHEN_Msk                     (0x3FFFF)                   /*!< ADC channel 0 ~ 17 mask */
#define ADC_PDMADATA_AD_PDMA_Msk         (0xFFF)                     /*!< ADC PDMA current transfer data */
#define ADC_CMP_LESS_THAN                (0UL)                       /*!< ADC compare condition less than */
#define ADC_CMP_GREATER_OR_EQUAL_TO      (ADC_CMPR_CMPCOND_Msk)      /*!< ADC compare condition greater or equal to */
#define ADC_TRIGGER_BY_EXT_PIN           (0UL)                       /*!< ADC trigger by STADC (P3.2) pin */
#define ADC_LOW_LEVEL_TRIGGER            (0UL << ADC_CR_TRGCOND_Pos) /*!< External pin low level trigger ADC */
#define ADC_HIGH_LEVEL_TRIGGER           (1UL << ADC_CR_TRGCOND_Pos) /*!< External pin high level trigger ADC */
#define ADC_FALLING_EDGE_TRIGGER         (2UL << ADC_CR_TRGCOND_Pos) /*!< External pin falling edge trigger ADC */
#define ADC_RISING_EDGE_TRIGGER          (3UL << ADC_CR_TRGCOND_Pos) /*!< External pin rising edge trigger ADC */
#define ADC_ADF_INT                      (ADC_SR_ADF_Msk)            /*!< ADC convert complete interrupt */
#define ADC_CMP0_INT                     (ADC_SR_CMPF0_Msk)          /*!< ADC comparator 0 interrupt */
#define ADC_CMP1_INT                     (ADC_SR_CMPF1_Msk)          /*!< ADC comparator 1 interrupt */
#define ADC_INPUT_MODE_SINGLE_END        (0UL << ADC_CR_DIFF_Pos)    /*!< ADC input mode set to single end */
#define ADC_INPUT_MODE_DIFFERENTIAL      (1UL << ADC_CR_DIFF_Pos)    /*!< ADC input mode set to differential */
#define ADC_OPERATION_MODE_SINGLE        (0UL << ADC_CR_ADMD_Pos)    /*!< ADC operation mode set to single conversion */
#define ADC_OPERATION_MODE_SINGLE_CYCLE  (2UL << ADC_CR_ADMD_Pos)    /*!< ADC operation mode set to single cycle scan */
#define ADC_OPERATION_MODE_CONTINUOUS    (3UL << ADC_CR_ADMD_Pos)    /*!< ADC operation mode set to continuous scan */
#define ADC_DMODE_OUT_FORMAT_UNSIGNED    (0UL << ADC_CR_DIFF_Pos)    /*!< ADC differential mode output format with unsigned */
#define ADC_DMODE_OUT_FORMAT_2COMPLEMENT (1UL << ADC_CR_DIFF_Pos)    /*!< ADC differential mode output format with 2's complement */
#define ADC_RESSEL_6_BIT                 (0UL << ADC_CR_RESSEL_Pos)  /*!< ADC resolution selection set to 6 bit */
#define ADC_RESSEL_8_BIT                 (1UL << ADC_CR_RESSEL_Pos)  /*!< ADC resolution selection set to 8 bit */
#define ADC_RESSEL_10_BIT                (2UL << ADC_CR_RESSEL_Pos)  /*!< ADC resolution selection set to 10 bit */
#define ADC_RESSEL_12_BIT                (3UL << ADC_CR_RESSEL_Pos)  /*!< ADC resolution selection set to 12 bit */
#define ADC_REFSEL_POWER                 (0UL << ADC_CR_REFSEL_Pos)  /*!< ADC reference voltage source selection set to power */
#define ADC_REFSEL_INT_VREF              (1UL << ADC_CR_REFSEL_Pos)  /*!< ADC reference voltage source selection set to Int_VREF */
#define ADC_REFSEL_VREF                  (2UL << ADC_CR_REFSEL_Pos)  /*!< ADC reference voltage source selection set to VREF */

/*@}*/ /* end of group NANO100_ADC_EXPORTED_CONSTANTS */


/** @addtogroup NANO100_ADC_EXPORTED_FUNCTIONS ADC Exported Functions
  @{
*/

/**
  * @brief Get the latest ADC conversion data
  * @param[in] adc Base address of ADC module
  * @param[in] u32ChNum Channel number
  * @return  Latest ADC conversion data
  * \hideinitializer
  */
#define ADC_GET_CONVERSION_DATA(adc, u32ChNum) (ADC->RESULT[u32ChNum] & ADC_RESULT_RSLT_Msk)

/**
  * @brief Return the user-specified interrupt flags
  * @param[in] adc Base address of ADC module
  * @param[in] u32Mask The combination of following interrupt status bits. Each bit corresponds to a interrupt status.
  *                     - \ref ADC_ADF_INT
  *                     - \ref ADC_CMP0_INT
  *                     - \ref ADC_CMP1_INT
  * @return  User specified interrupt flags
  * \hideinitializer
  */
#define ADC_GET_INT_FLAG(adc, u32Mask) (ADC->SR & (u32Mask))

/**
  * @brief This macro clear the selected interrupt status bits
  * @param[in] adc Base address of ADC module
  * @param[in] u32Mask The combination of following interrupt status bits. Each bit corresponds to a interrupt status.
  *                     - \ref ADC_ADF_INT
  *                     - \ref ADC_CMP0_INT
  *                     - \ref ADC_CMP1_INT
  * @return  None
  * \hideinitializer
  */
#define ADC_CLR_INT_FLAG(adc, u32Mask) (ADC->SR = (ADC->SR & ~(ADC_SR_ADF_Msk | \
                                                                       ADC_SR_CMPF0_Msk | \
                                                                       ADC_SR_CMPF1_Msk)) | (u32Mask))

/**
  * @brief Get the busy state of ADC
  * @param[in] adc Base address of ADC module
  * @return busy state of ADC
  * @retval 0 ADC is not busy
  * @retval 1 ADC is busy
  * \hideinitializer
  */
#define ADC_IS_BUSY(adc) (ADC->SR & ADC_SR_BUSY_Msk ? 1 : 0)

/**
  * @brief Check if the ADC conversion data is over written or not
  * @param[in] adc Base address of ADC module
  * @param[in] u32ChNum Currently not used
  * @return Over run state of ADC data
  * @retval 0 ADC data is not overrun
  * @retval 1 ADC data us overrun
  * \hideinitializer
  */
#define ADC_IS_DATA_OVERRUN(adc, u32ChNum) (ADC->RESULT[u32ChNum] & ADC_RESULT_OVERRUN_Msk ? 1 : 0)

/**
  * @brief Check if the ADC conversion data is valid or not
  * @param[in] adc Base address of ADC module
  * @param[in] u32ChNum Currently not used
  * @return Valid state of ADC data
  * @retval 0 ADC data is not valid
  * @retval 1 ADC data us valid
  * \hideinitializer
  */
#define ADC_IS_DATA_VALID(adc, u32ChNum) (ADC->RESULT[u32ChNum] & ADC_RESULT_VALID_Msk ? 1 : 0)

/**
  * @brief Power down ADC module
  * @param[in] adc Base address of ADC module
  * @return None
  * \hideinitializer
  */
#define ADC_POWER_DOWN(adc) (ADC->CR &= ~ADC_CR_ADEN_Msk)

/**
  * @brief Power on ADC module
  * @param[in] adc Base address of ADC module
  * @return None
  * \hideinitializer
  */
#define ADC_POWER_ON(adc) \
do { \
  ADC->CR |= ADC_CR_ADEN_Msk; \
  while ((!(ADC->SR & ADC_SR_INITRDY_Msk)) || (!(ADC->PWRCTL & ADC_PWRCTL_PWUPRDY_Msk))); \
} while(0)

/**
  * @brief Configure the comparator 0 and enable it
  * @param[in] adc Base address of ADC module
  * @param[in] u32ChNum  Specifies the source channel, valid value are from 0 to 7
  * @param[in] u32Condition Specifies the compare condition
  *                     - \ref ADC_CMP_LESS_THAN
  *                     - \ref ADC_CMP_GREATER_OR_EQUAL_TO
  * @param[in] u32Data Specifies the compare value. Valid value are between 0 ~ 0x3FF
  * @param[in] u32MatchCount Specifies the match count setting, valid values are between 1~16
  * @return None
  * @details For example, ADC_ENABLE_CMP0(ADC, 5, ADC_CMP_GREATER_OR_EQUAL_TO, 0x800, 10);
  *          Means ADC will assert comparator 0 flag if channel 5 conversion result is
  *          greater or equal to 0x800 for 10 times continuously.
  * \hideinitializer
  */
#define ADC_ENABLE_CMP0(adc, \
                        u32ChNum, \
                        u32Condition, \
                        u32Data, \
                        u32MatchCount) (ADC->CMPR0 = ((u32ChNum) << ADC_CMPR_CMPCH_Pos) | \
                                                                   (u32Condition) | \
                                                                   ((u32Data) << ADC_CMPR_CMPD_Pos) | \
                                                                   (((u32MatchCount) - 1) << ADC_CMPR_CMPMATCNT_Pos) |\
                                                                   ADC_CMPR_CMPEN_Msk)

/**
  * @brief Disable comparator 0
  * @param[in] adc Base address of ADC module
  * @return None
  * \hideinitializer
  */
#define ADC_DISABLE_CMP0(adc) (ADC->CMPR0 = 0)

/**
  * @brief Configure the comparator 1 and enable it
  * @param[in] adc Base address of ADC module
  * @param[in] u32ChNum  Specifies the source channel, valid value are from 0 to 7
  * @param[in] u32Condition Specifies the compare condition
  *                     - \ref ADC_CMP_LESS_THAN
  *                     - \ref ADC_CMP_GREATER_OR_EQUAL_TO
  * @param[in] u32Data Specifies the compare value. Valid value are between 0 ~ 0x3FF
  * @param[in] u32MatchCount Specifies the match count setting, valid values are between 1~16
  * @return None
  * @details For example, ADC_ENABLE_CMP1(ADC, 5, ADC_CMP_GREATER_OR_EQUAL_TO, 0x800, 10);
  *          Means ADC will assert comparator 1 flag if channel 5 conversion result is
  *          greater or equal to 0x800 for 10 times continuously.
  * \hideinitializer
  */
#define ADC_ENABLE_CMP1(adc, \
                        u32ChNum, \
                        u32Condition, \
                        u32Data, \
                        u32MatchCount) (ADC->CMPR1 = ((u32ChNum) << ADC_CMPR_CMPCH_Pos) | \
                                                                   (u32Condition) | \
                                                                   ((u32Data) << ADC_CMPR_CMPD_Pos) | \
                                                                   ((u32MatchCount - 1) << ADC_CMPR_CMPMATCNT_Pos) |\
                                                                   ADC_CMPR_CMPEN_Msk)

/**
  * @brief Disable comparator 1
  * @param[in] adc Base address of ADC module
  * @return None
  * \hideinitializer
  */
#define ADC_DISABLE_CMP1(adc) (ADC->CMPR1 = 0)

/**
  * @brief Set ADC input channel. Enabled channel will be converted while ADC starts.
  * @param[in] adc Base address of ADC module
  * @param[in] u32Mask  Channel enable bit. Each bit corresponds to a input channel. Bit 0 is channel 0, bit 1 is channel 1...
  * @return None
  * \hideinitializer
  */
#define ADC_SET_INPUT_CHANNEL(adc, u32Mask) (ADC->CHEN = (ADC->CHEN & ~ADC_CHEN_Msk) | (u32Mask))

/**
  * @brief Start the A/D conversion.
  * @param[in] adc Base address of ADC module
  * @return None
  * \hideinitializer
  */
#define ADC_START_CONV(adc) (ADC->CR |= ADC_CR_ADST_Msk)

/**
  * @brief Stop the A/D conversion.
  * @param[in] adc Base address of ADC module
  * @return None
  * \hideinitializer
  */
#define ADC_STOP_CONV(adc) (ADC->CR &= ~ADC_CR_ADST_Msk)

/**
  * @brief Set the output format in differential input mode.
  * @param[in] adc Base address of ADC module
  * @param[in] u32Format Differential input mode output format. Valid values are:
  *                 - \ref ADC_DMODE_OUT_FORMAT_UNSIGNED
  *                 - \ref ADC_DMODE_OUT_FORMAT_2COMPLEMENT
  * @return None
  * \hideinitializer
  */
#define ADC_SET_DMOF(adc, u32Format) (ADC->CR = (ADC->CR & ~ADC_CR_DIFF_Msk) | u32Format)

/**
  * @brief Set the resolution of conversion result.
  * @param[in] adc Base address of ADC module
  * @param[in] u32Resolution The resolution of conversion result. Valid values are:
  *                 - \ref ADC_RESSEL_6_BIT
  *                 - \ref ADC_RESSEL_8_BIT
  *                 - \ref ADC_RESSEL_10_BIT
  *                 - \ref ADC_RESSEL_12_BIT
  * @return None
  * \hideinitializer
  */
#define ADC_SET_RESOLUTION(adc, u32Resolution) (ADC->CR = (ADC->CR & ~ADC_CR_RESSEL_Msk) | u32Resolution)

/**
  * @brief Set the reference voltage selection.
  * @param[in] adc Base address of ADC module
  * @param[in] u32Ref The reference voltage selection. Valid values are:
  *                 - \ref ADC_REFSEL_POWER
  *                 - \ref ADC_REFSEL_INT_VREF
  *                 - \ref ADC_REFSEL_VREF
  * @return None
  * \hideinitializer
  */
#define ADC_SET_REF_VOLTAGE(adc, u32Ref) (ADC->CR = (ADC->CR & ~ADC_CR_REFSEL_Msk) | u32Ref)

/**
  * @brief Set power down mode.
  * @param[in] adc Base address of ADC module
  * @param[in] u32Mode The power down mode. 0: power down mode, 2: standby mode
  * @param[in] u32CalEn Do calibration when power up.
  * @return None
  * \hideinitializer
  */
#define ADC_SET_POWERDOWN_MODE(adc, u32Mode, u32CalEn) \
    ADC->PWRCTL = (ADC->PWRCTL & ~(ADC_PWRCTL_PWDMOD_Msk | ADC_PWRCTL_PWDCALEN_Msk)) \
    | (u32Mode << ADC_PWRCTL_PWDMOD_Pos) | (u32CalEn << ADC_PWRCTL_PWDCALEN_Pos)

/**
  * @brief Enable PDMA transfer.
  * @param[in] adc Base address of ADC module
  * @return None
  * \hideinitializer
  */
#define ADC_ENABLE_PDMA(adc) (ADC->CR |= ADC_CR_PTEN_Msk)

/**
  * @brief Disable PDMA transfer.
  * @param[in] adc Base address of ADC module
  * @return None
  * \hideinitializer
  */
#define ADC_DISABLE_PDMA(adc) (ADC->CR &= ~ADC_CR_PTEN_Msk)

/**
  * @brief Get PDMA current transfer data
  * @param[in] adc Base address of ADC module
  * @return  PDMA current transfer data
  * \hideinitializer
  */
#define ADC_GET_PDMA_DATA(adc) (ADC->PDMA & ADC_PDMADATA_AD_PDMA_Msk)

void ADC_Open(ADC_T *adc,
              uint32_t u32InputMode,
              uint32_t u32OpMode,
              uint32_t u32ChMask);
void ADC_Close(ADC_T *adc);
void ADC_EnableHWTrigger(ADC_T *adc,
                         uint32_t u32Source,
                         uint32_t u32Param);
void ADC_DisableHWTrigger(ADC_T *adc);
void ADC_EnableTimerTrigger(ADC_T *adc,
                            uint32_t u32Source,
                            uint32_t u32PDMACnt);
void ADC_DisableTimerTrigger(ADC_T *adc);
void ADC_SetExtraSampleTime(ADC_T *adc,
                            uint32_t u32ChNum,
                            uint32_t u32SampleTime);
void ADC_EnableInt(ADC_T *adc, uint32_t u32Mask);
void ADC_DisableInt(ADC_T *adc, uint32_t u32Mask);



/*@}*/ /* end of group NANO100_ADC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_ADC_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__ADC_H__

/*** (C) COPYRIGHT 2013-2014 Nuvoton Technology Corp. ***/
