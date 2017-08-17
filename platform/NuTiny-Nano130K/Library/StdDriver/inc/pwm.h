/**************************************************************************//**
 * @file     pwm.h
 * @version  V1.00
 * $Revision: 12 $
 * $Date: 15/06/30 2:52p $
 * @brief    NANO100 series PWM driver header file
 *
 * @note
 * Copyright (C) 2013-2014 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_PWM_Driver PWM Driver
  @{
*/

/** @addtogroup NANO100_PWM_EXPORTED_CONSTANTS PWM Exported Constants
  @{
*/
#define PWM_CHANNEL_NUM                      (4)         /*!< PWM channel number  \hideinitializer */
#define PWM_CH0                              (0UL)       /*!< PWM channel 0  \hideinitializer */
#define PWM_CH1                              (1UL)       /*!< PWM channel 1  \hideinitializer */
#define PWM_CH2                              (2UL)       /*!< PWM channel 2  \hideinitializer */
#define PWM_CH3                              (3UL)       /*!< PWM channel 3  \hideinitializer */
#define PWM_CH_0_MASK                        (1UL)       /*!< PWM channel 0 mask  \hideinitializer */
#define PWM_CH_1_MASK                        (2UL)       /*!< PWM channel 1 mask  \hideinitializer */
#define PWM_CH_2_MASK                        (4UL)       /*!< PWM channel 2 mask  \hideinitializer */
#define PWM_CH_3_MASK                        (8UL)       /*!< PWM channel 3 mask  \hideinitializer */
#define PWM_CLK_DIV_1                        (4UL)       /*!< PWM clock divide by 1  \hideinitializer */
#define PWM_CLK_DIV_2                        (0UL)       /*!< PWM clock divide by 2  \hideinitializer */
#define PWM_CLK_DIV_4                        (1UL)       /*!< PWM clock divide by 4  \hideinitializer */
#define PWM_CLK_DIV_8                        (2UL)       /*!< PWM clock divide by 8  \hideinitializer */
#define PWM_CLK_DIV_16                       (3UL)       /*!< PWM clock divide by 16  \hideinitializer */
#define PWM_EDGE_ALIGNED                     (0UL)       /*!< PWM working in edge aligned type  \hideinitializer */
#define PWM_CENTER_ALIGNED                   (1UL)       /*!< PWM working in center aligned type  \hideinitializer */
#define PWM_RISING_LATCH_INT_ENABLE          (1UL)       /*!< PWM rising latch interrupt enable  \hideinitializer */
#define PWM_FALLING_LATCH_INT_ENABLE         (2UL)       /*!< PWM falling latch interrupt enable  \hideinitializer */
#define PWM_RISING_FALLING_LATCH_INT_ENABLE  (3UL)       /*!< PWM rising latch interrupt enable  \hideinitializer */
#define PWM_RISING_LATCH_INT_FLAG            (2UL)       /*!< PWM rising latch condition happened  \hideinitializer */
#define PWM_FALLING_LATCH_INT_FLAG           (4UL)       /*!< PWM falling latch condition happened  \hideinitializer */
#define PWM_RISING_FALLING_LATCH_INT_FLAG    (6UL)       /*!< PWM rising latch condition happened  \hideinitializer */
#define PWM_RISING_LATCH_PDMA_ENABLE         (0x10UL)    /*!< PWM rising latch PDMA enable  \hideinitializer */
#define PWM_FALLING_LATCH_PDMA_ENABLE        (0x20UL)    /*!< PWM falling latch PDMA enable  \hideinitializer */
#define PWM_RISING_FALLING_LATCH_PDMA_ENABLE (0x30UL)    /*!< PWM rising and falling latch PDMA enable  \hideinitializer */
#define PWM_CAP_PDMA_RFORDER_R               (0x1000UL)  /*!< PWM captured data transferred by PDMA is rising latch first  \hideinitializer */
#define PWM_CAP_PDMA_RFORDER_F               (0UL)       /*!< PWM captured data transferred by PDMA is falling latch first  \hideinitializer */

/*@}*/ /* end of group NANO100_PWM_EXPORTED_CONSTANTS */


/** @addtogroup NANO100_PWM_EXPORTED_FUNCTIONS PWM Exported Functions
  @{
*/

/**
 * @brief This macro enable output inverter of specified channel(s)
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelMask Combination of enabled channels. Each bit corresponds to a channel
 *                           Bit 0 represents channel 0, bit 1 represents channel 1...
 * @return None
 * \hideinitializer
 */
#define PWM_ENABLE_OUTPUT_INVERTER(pwm, u32ChannelMask)\
do { \
    uint8_t i; \
    (pwm)->CTL &= ~(PWM_CTL_CH0INV_Msk | PWM_CTL_CH1INV_Msk | PWM_CTL_CH2INV_Msk | PWM_CTL_CH3INV_Msk);\
    for (i = 0; i < PWM_CHANNEL_NUM; i++) { \
        if ( (u32ChannelMask)  & (1 << i)) { \
            (pwm)->CTL |= (PWM_CTL_CH0INV_Msk << (i * 8)); \
        } \
    } \
  }while(0)


/**
 * @brief This macro get captured rising data
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @return None
  * \hideinitializer
 */
#define PWM_GET_CAPTURE_RISING_DATA(pwm, u32ChannelNum) (*(__IO uint32_t *) (&pwm->CRL0 + 2 * u32ChannelNum))

/**
 * @brief This macro get captured falling data
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @return None
  * \hideinitializer
 */
#define PWM_GET_CAPTURE_FALLING_DATA(pwm, u32ChannelNum) (*(__IO uint32_t *) (&pwm->CFL0 + 2 * u32ChannelNum))

/**
 * @brief This macro set the prescaler of the selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32Prescaler Clock prescaler of specified channel. Valid values are between 1 ~ 0xFF
 * @return None
 * @note Every even channel N, and channel (N + 1) share a prescaler. So if channel 0 prescaler changed,
 *       channel 1 will also be affected.
 * \hideinitializer
 */
#define PWM_SET_PRESCALER(pwm, u32ChannelNum, u32Prescaler) \
    (pwm->PRES = (pwm->PRES & ~(PWM_PRES_CP01_Msk << (((u32ChannelNum) >> 1) * 8))) | ((u32Prescaler) << (((u32ChannelNum) >> 1) * 8)))

/**
 * @brief This macro set the divider of the selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32Divider Clock divider of specified channel. Valid values are
 *              - \ref PWM_CLK_DIV_1
 *              - \ref PWM_CLK_DIV_2
 *              - \ref PWM_CLK_DIV_4
 *              - \ref PWM_CLK_DIV_8
 *              - \ref PWM_CLK_DIV_16
 * @return None
 * \hideinitializer
 */
#define PWM_SET_DIVIDER(pwm, u32ChannelNum, u32Divider) \
    (pwm->CLKSEL = (pwm->CLKSEL & ~(PWM_CLKSEL_CLKSEL0_Msk << (4 * u32ChannelNum))) | (u32Divider << (4 * u32ChannelNum)))

/**
 * @brief This macro set the duty of the selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32CMR Duty of specified channel. Valid values are between 0~0xFFFF
 * @return None
 * @note This new setting will take effect on next PWM period
 * \hideinitializer
 */
#define PWM_SET_CMR(pwm, u32ChannelNum, u32CMR) \
do { \
    *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) &= ~PWM_DUTY_CM_Msk; \
    *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) |= (u32CMR << PWM_DUTY_CM_Pos); \
}while(0)

/**
 * @brief This macro set the period of the selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32CNR Period of specified channel. Valid values are between 0~0xFFFF
 * @return None
 * @note This new setting will take effect on next PWM period
 * @note PWM counter will stop if period length set to 0
 * \hideinitializer
 */
#define PWM_SET_CNR(pwm, u32ChannelNum, u32CNR) \
do { \
    *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) &= ~PWM_DUTY_CN_Msk; \
    *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) |= u32CNR; \
} while(0)

uint32_t PWM_ConfigOutputChannel(PWM_T *pwm,
                                 uint32_t u32ChannelNum,
                                 uint32_t u32Frequency,
                                 uint32_t u32DutyCycle);
uint32_t PWM_ConfigCaptureChannel (PWM_T *pwm,
                                   uint32_t u32ChannelNum,
                                   uint32_t u32UnitTimeNsec,
                                   uint32_t u32CaptureEdge);
void PWM_Start(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_Stop(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_ForceStop(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_EnableCapture(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_DisableCapture(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_EnableOutput(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_DisableOutput(PWM_T *pwm, uint32_t u32ChannelMask);
void PWM_EnableDeadZone(PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Duration);
void PWM_DisableDeadZone(PWM_T *pwm, uint32_t u32ChannelNum);
void PWM_EnableCaptureInt(PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Edge);
void PWM_DisableCaptureInt(PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Edge);
void PWM_ClearCaptureIntFlag(PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Edge);
uint32_t PWM_GetCaptureIntFlag(PWM_T *pwm, uint32_t u32ChannelNum);
void PWM_EnablePeriodInt(PWM_T *pwm, uint32_t u32ChannelNum,  uint32_t u32IntPeriodType);
void PWM_DisablePeriodInt(PWM_T *pwm, uint32_t u32ChannelNum);
void PWM_ClearPeriodIntFlag(PWM_T *pwm, uint32_t u32ChannelNum);
uint32_t PWM_GetPeriodIntFlag(PWM_T *pwm, uint32_t u32ChannelNum);
void PWM_EnablePDMA(PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32RisingFirst, uint32_t u32Mode);
void PWM_DisablePDMA(PWM_T *pwm, uint32_t u32ChannelNum);

/*@}*/ /* end of group NANO100_PWM_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_PWM_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__PWM_H__

/*** (C) COPYRIGHT 2013-2014 Nuvoton Technology Corp. ***/
