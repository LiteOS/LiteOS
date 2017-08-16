/**************************************************************************//**
 * @file     PWM.c
 * @version  V1.00
 * $Revision: 14 $
 * $Date: 14/09/04 11:58a $
 * @brief    NANO100 series PWM driver source file
 *
 * @note
 * Copyright (C) 2013-2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "Nano100Series.h"

/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_PWM_Driver PWM Driver
  @{
*/


/** @addtogroup NANO100_PWM_EXPORTED_FUNCTIONS PWM Exported Functions
  @{
*/

/**
 * @brief This function config PWM generator and get the nearest frequency in edge aligned auto-reload mode
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32Frequency Target generator frequency
 * @param[in] u32DutyCycle Target generator duty cycle percentage. Valid range are between 0 ~ 100. 10 means 10%, 20 means 20%...
 * @return Nearest frequency clock in nano second
 * @note Since every two channels, (0 & 1), (2 & 3), (4 & 5), shares a prescaler. Call this API to configure PWM frequency may affect
 *       existing frequency of other channel.
 */
uint32_t PWM_ConfigOutputChannel (PWM_T *pwm,
                                  uint32_t u32ChannelNum,
                                  uint32_t u32Frequency,
                                  uint32_t u32DutyCycle)
{
    uint32_t i;
    uint32_t u32ClkSrc;
    uint32_t u32PWM_Clock = SystemCoreClock;
    uint8_t  u8Divider = 1, u8Prescale = 0xFF;
    uint16_t u16CNR = 0xFFFF;

    if(pwm == PWM0)
        u32ClkSrc = (CLK->CLKSEL1 & (CLK_CLKSEL1_PWM0_CH01_S_Msk << (u32ChannelNum & 2))) >> (CLK_CLKSEL1_PWM0_CH01_S_Pos + (u32ChannelNum & 2));

    else
        u32ClkSrc = (CLK->CLKSEL2 & (CLK_CLKSEL2_PWM1_CH01_S_Msk << (u32ChannelNum & 2))) >> (CLK_CLKSEL2_PWM1_CH01_S_Pos + (u32ChannelNum & 2));

    switch (u32ClkSrc) {
    case 0:
        u32PWM_Clock = __HXT;
        break;
    case 1:
        u32PWM_Clock = __LXT;
        break;
    case 2:
        u32PWM_Clock = SystemCoreClock;
        break;
    case 3:
        u32PWM_Clock = __HIRC12M;
        break;
    }

    for(; u8Divider < 17; u8Divider <<= 1) {  // clk divider could only be 1, 2, 4, 8, 16
        i = (u32PWM_Clock / u32Frequency) / u8Divider;
        // If target value is larger than CNR * prescale, need to use a larger divider
        if(i > (0x10000 * 0x100))
            continue;

        // CNR = 0xFFFF + 1, get a prescaler that CNR value is below 0xFFFF
        u8Prescale = (i + 0xFFFF)/ 0x10000;

        // u8Prescale must at least be 2, otherwise the output stop
        if(u8Prescale < 3)
            u8Prescale = 2;

        i /= u8Prescale;

        if(i <= 0x10000) {
            if(i == 1)
                u16CNR = 1;     // Too fast, and PWM cannot generate expected frequency...
            else
                u16CNR = i;
            break;
        }

    }
    // Store return value here 'cos we're gonna change u8Divider & u8Prescale & u16CNR to the real value to fill into register
    i = u32PWM_Clock / (u8Prescale * u8Divider * u16CNR);

    u8Prescale -= 1;
    u16CNR -= 1;
    // convert to real register value
    if(u8Divider == 1)
        u8Divider = 4;
    else if (u8Divider == 2)
        u8Divider = 0;
    else if (u8Divider == 4)
        u8Divider = 1;
    else if (u8Divider == 8)
        u8Divider = 2;
    else // 16
        u8Divider = 3;

    // every two channels share a prescaler
    while((pwm->INTSTS & PWM_INTSTS_PRESSYNC_Msk ) == PWM_INTSTS_PRESSYNC_Msk);
    pwm->PRES = (pwm->PRES & ~(PWM_PRES_CP01_Msk << ((u32ChannelNum >> 1) * 8))) | (u8Prescale << ((u32ChannelNum >> 1) * 8));
    pwm->CLKSEL = (pwm->CLKSEL & ~(PWM_CLKSEL_CLKSEL0_Msk << (4 * u32ChannelNum))) | (u8Divider << (4 * u32ChannelNum));
    pwm->CTL |= (PWM_CTL_CH0MOD_Msk << (u32ChannelNum * 8));
    while((pwm->INTSTS & (PWM_INTSTS_DUTY0SYNC_Msk << u32ChannelNum)) == (PWM_INTSTS_DUTY0SYNC_Msk << u32ChannelNum));
    if(u32DutyCycle == 0)
        *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) &= ~PWM_DUTY_CM_Msk;
    else {
        *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) &= ~PWM_DUTY_CM_Msk;
        *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) |= ((u32DutyCycle * (u16CNR + 1) / 100 - 1) << PWM_DUTY_CM_Pos);
    }
    *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) &= ~PWM_DUTY_CN_Msk;
    *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) |= u16CNR;

    return(i);
}

/**
 * @brief This function config PWM capture and get the nearest unit time
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32UnitTimeNsec Unit time of counter
 * @param[in] u32CaptureEdge Condition to latch the counter
 * @return Nearest unit time in nano second
 * @note Since every two channels, (0 & 1), (2 & 3), (4 & 5), shares a prescaler. Call this API to configure PWM frequency may affect
 *       existing frequency of other channel.
 */
uint32_t PWM_ConfigCaptureChannel (PWM_T *pwm,
                                   uint32_t u32ChannelNum,
                                   uint32_t u32UnitTimeNsec,
                                   uint32_t u32CaptureEdge)
{
    uint32_t i;
    uint32_t u32ClkSrc;
    uint32_t u32PWM_Clock = SystemCoreClock;
    uint8_t  u8Divider = 1, u8Prescale = 0xFF;
    uint16_t u16CNR = 0xFFFF;

    if(pwm == PWM0)
        u32ClkSrc = (CLK->CLKSEL1 & (CLK_CLKSEL1_PWM0_CH01_S_Msk << (u32ChannelNum & 2))) >> (CLK_CLKSEL1_PWM0_CH01_S_Pos + (u32ChannelNum & 2));
    else
        u32ClkSrc = (CLK->CLKSEL2 & (CLK_CLKSEL2_PWM1_CH01_S_Msk << (u32ChannelNum & 2))) >> (CLK_CLKSEL2_PWM1_CH01_S_Pos + (u32ChannelNum & 2));

    switch (u32ClkSrc) {
    case 0:
        u32PWM_Clock = __HXT;
        break;
    case 1:
        u32PWM_Clock = __LXT;
        break;
    case 2:
        u32PWM_Clock = SystemCoreClock;
        break;
    case 3:
        u32PWM_Clock = __HIRC12M;
        break;
    }

    for(; u8Divider < 17; u8Divider <<= 1) {  // clk divider could only be 1, 2, 4, 8, 16
        i = ((long long)(u32PWM_Clock / u8Divider) * u32UnitTimeNsec) / 1000000000;

        // If target value is larger than 0xFF, need to use a larger divider
        if(i > (0xFF))
            continue;

        u8Prescale = i;

        // u8Prescale must at least be 2, otherwise the output stop
        if(u8Prescale < 3)
            u8Prescale = 2;

        break;
    }

    // Store return value here 'cos we're gonna change u8Divider & u8Prescale & u16CNR to the real value to fill into register
    i = (long long) (u8Prescale * u8Divider) * 1000000000 / u32PWM_Clock;

    u8Prescale -= 1;
    u16CNR -= 1;
    // convert to real register value
    if(u8Divider == 1)
        u8Divider = 4;
    else if (u8Divider == 2)
        u8Divider = 0;
    else if (u8Divider == 4)
        u8Divider = 1;
    else if (u8Divider == 8)
        u8Divider = 2;
    else // 16
        u8Divider = 3;

    // every two channels share a prescaler
    while((pwm->INTSTS & PWM_INTSTS_PRESSYNC_Msk ) == PWM_INTSTS_PRESSYNC_Msk);
    pwm->PRES = (pwm->PRES & ~(PWM_PRES_CP01_Msk << ((u32ChannelNum >> 1) * 8))) | (u8Prescale << ((u32ChannelNum >> 1) * 8));
    pwm->CLKSEL = (pwm->CLKSEL & ~(PWM_CLKSEL_CLKSEL0_Msk << (4 * u32ChannelNum))) | (u8Divider << (4 * u32ChannelNum));
    pwm->CTL |= (PWM_CTL_CH0MOD_Msk << (u32ChannelNum * 8));
    while((pwm->INTSTS & (PWM_INTSTS_DUTY0SYNC_Msk << u32ChannelNum)) == (PWM_INTSTS_DUTY0SYNC_Msk << u32ChannelNum));
    *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) &= ~PWM_DUTY_CN_Msk;
    *(__IO uint32_t *) (&pwm->DUTY0 + 3 * u32ChannelNum) |= u16CNR;

    return(i);
}

/**
 * @brief This function start PWM module
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelMask Combination of enabled channels. Each bit corresponds to a channel.
 *                           Bit 0 is channel 0, bit 1 is channel 1...
 * @return None
 */
void PWM_Start (PWM_T *pwm, uint32_t u32ChannelMask)
{
    uint8_t i;
    uint32_t u32Mask = 0;

    for (i = 0; i < PWM_CHANNEL_NUM; i++) {
        if ( u32ChannelMask  & (1 << i))
            u32Mask |= (PWM_CTL_CH0EN_Msk << (i * 8));
    }

    pwm->CTL |= u32Mask;
}

/**
 * @brief This function stop PWM module
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelMask Combination of enabled channels. Each bit corresponds to a channel.
 *                           Bit 0 is channel 0, bit 1 is channel 1...
 * @return None
 */
void PWM_Stop (PWM_T *pwm, uint32_t u32ChannelMask)
{
    uint32_t i;
    for(i = 0; i < PWM_CHANNEL_NUM; i ++) {
        if(u32ChannelMask & (1 << i)) {
            *(__IO uint32_t *) (&pwm->DUTY0 + 3 * i) &= ~PWM_DUTY_CN_Msk;
        }
    }

}

/**
 * @brief This function stop PWM generation immediately by clear channel enable bit
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelMask Combination of enabled channels. Each bit corresponds to a channel.
 *                           Bit 0 is channel 0, bit 1 is channel 1...
 * @return None
 */
void PWM_ForceStop (PWM_T *pwm, uint32_t u32ChannelMask)
{
    uint32_t i;
    for (i = 0; i < PWM_CHANNEL_NUM; i++) {
        if ( u32ChannelMask  & (1 << i))
            pwm->CTL &= ~(PWM_CTL_CH0EN_Msk << (i * 8));
    }
}

/**
 * @brief This function enables PWM capture of selected channels
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelMask Combination of enabled channels. Each bit corresponds to a channel.
 *                           Set bit 0 to 1 enables channel 0 output, set bit 1 to 1 enables channel 1 output...
 * @return None
 */
void PWM_EnableCapture (PWM_T *pwm, uint32_t u32ChannelMask)
{
    uint8_t i;
    uint32_t u32Mask = 0;

    for (i = 0; i < PWM_CHANNEL_NUM; i++) {
        if ( u32ChannelMask  & (1 << i)) {
            u32Mask |= ((PWM_CAPCTL_CAPCH0EN_Msk | PWM_CAPCTL_CAPCH0PADEN_Msk) << (i * 8));
        }
    }

    pwm->CAPCTL |= u32Mask;
}

/**
 * @brief This function disables PWM capture of selected channels
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelMask Combination of enabled channels. Each bit corresponds to a channel.
 *                           Set bit 0 to 1 enables channel 0 output, set bit 1 to 1 enables channel 1 output...
 * @return None
 */
void PWM_DisableCapture (PWM_T *pwm, uint32_t u32ChannelMask)
{
    uint8_t i;
    uint32_t u32CTLMask = 0;
    uint32_t u32CAPCTLMask = 0;

    for (i = 0; i < PWM_CHANNEL_NUM; i++) {
        if ( u32ChannelMask  & (1 << i)) {
            u32CTLMask |= (PWM_CTL_CH0EN_Msk << (i * 8));
            u32CAPCTLMask |= ((PWM_CAPCTL_CAPCH0EN_Msk | PWM_CAPCTL_CAPCH0PADEN_Msk) << (i * 8));
        }
    }

    pwm->CTL &= ~u32CTLMask;
    pwm->CAPCTL &= ~u32CAPCTLMask;
}

/**
 * @brief This function enables PWM output generation of selected channels
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelMask Combination of enabled channels. Each bit corresponds to a channel.
 *                           Set bit 0 to 1 enables channel 0 output, set bit 1 to 1 enables channel 1 output...
 * @return None
 */
void PWM_EnableOutput (PWM_T *pwm, uint32_t u32ChannelMask)
{
    pwm->OE |= u32ChannelMask;
}

/**
 * @brief This function disables PWM output generation of selected channels
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelMask Combination of enabled channels. Each bit corresponds to a channel
 *                           Set bit 0 to 1 disables channel 0 output, set bit 1 to 1 disables channel 1 output...
 * @return None
 */
void PWM_DisableOutput (PWM_T *pwm, uint32_t u32ChannelMask)
{
    pwm->OE &= ~u32ChannelMask;
}

/**
 * @brief This function enable Dead zone of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32Duration Dead Zone length in PWM clock count, valid values are between 0~0xFF, but 0 means there is no
 *                        dead zone.
 * @return None
 */
void PWM_EnableDeadZone (PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Duration)
{
    // every two channels shares the same setting
    u32ChannelNum >>= 1;
    // set duration
    pwm->PRES = (pwm->PRES & ~(PWM_PRES_DZ01_Msk << (8 * u32ChannelNum))) | ((u32Duration << PWM_PRES_DZ01_Pos ) << (8 * u32ChannelNum));
    // enable dead zone
    pwm->CTL |= (PWM_CTL_DZEN01_Msk << u32ChannelNum);
}

/**
 * @brief This function disable Dead zone of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @return None
 */
void PWM_DisableDeadZone (PWM_T *pwm, uint32_t u32ChannelNum)
{
    // every two channels shares the same setting
    u32ChannelNum >>= 1;
    // enable dead zone
    pwm->CTL &= ~(PWM_CTL_DZEN01_Msk << u32ChannelNum);
}

/**
 * @brief This function enable capture interrupt of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32Edge Capture interrupt type. It could be either
 *              - \ref PWM_RISING_LATCH_INT_ENABLE
 *              - \ref PWM_FALLING_LATCH_INT_ENABLE
 *              - \ref PWM_RISING_FALLING_LATCH_INT_ENABLE
 * @return None
 */
void PWM_EnableCaptureInt (PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Edge)
{
    // enable capture interrupt
    pwm->CAPINTEN |= (u32Edge << (u32ChannelNum * 8));
}

/**
 * @brief This function disable capture interrupt of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32Edge Capture interrupt type. It could be either
 *              - \ref PWM_RISING_LATCH_INT_ENABLE
 *              - \ref PWM_FALLING_LATCH_INT_ENABLE
 *              - \ref PWM_RISING_FALLING_LATCH_INT_ENABLE
 * @return None
 */
void PWM_DisableCaptureInt (PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Edge)
{
    // disable capture interrupt
    pwm->CAPINTEN &= ~(u32Edge << (u32ChannelNum * 8));
}

/**
 * @brief This function clear capture interrupt flag of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32Edge Capture interrupt type. It could be either
 *              - \ref PWM_RISING_LATCH_INT_FLAG
 *              - \ref PWM_FALLING_LATCH_INT_FLAG
 *              - \ref PWM_RISING_FALLING_LATCH_INT_FLAG
 * @return None
 */
void PWM_ClearCaptureIntFlag (PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32Edge)
{
    // disable capture interrupt flag
    pwm->CAPINTSTS = (u32Edge + 1)  << (u32ChannelNum * 8);
}

/**
 * @brief This function get capture interrupt flag of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @return Capture interrupt flag of specified channel
 * @retval 0 Capture interrupt did not occurred
 * @retval PWM_RISING_LATCH_INT_FLAG Rising edge latch interrupt occurred
 * @retval PWM_FALLING_LATCH_INT_FLAG Falling edge latch interrupt occurred
 * @retval PWM_RISING_FALLING_LATCH_INT_FLAG Rising and falling edge latch interrupt occurred
 */
uint32_t PWM_GetCaptureIntFlag (PWM_T *pwm, uint32_t u32ChannelNum)
{
    return ((pwm->CAPINTSTS >> (u32ChannelNum * 8)) & (PWM_RISING_FALLING_LATCH_INT_FLAG));
}

/**
 * @brief This function enable period interrupt of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @param[in] u32IntPeriodType This parameter is not used
 * @return None
 * @note All channels share the same period interrupt type setting.
 */
void PWM_EnablePeriodInt (PWM_T *pwm, uint32_t u32ChannelNum,  uint32_t u32IntPeriodType)
{
    // enable period interrupt
    pwm->INTEN |= (PWM_INTEN_TMIE0_Msk << u32ChannelNum);
}

/**
 * @brief This function disable period interrupt of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @return None
 */
void PWM_DisablePeriodInt (PWM_T *pwm, uint32_t u32ChannelNum)
{
    pwm->INTEN &= ~(PWM_INTEN_TMIE0_Msk << u32ChannelNum);
}

/**
 * @brief This function clear period interrupt of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @return None
 */
void PWM_ClearPeriodIntFlag (PWM_T *pwm, uint32_t u32ChannelNum)
{
    // write 1 clear
    pwm->INTSTS = (PWM_INTSTS_TMINT0_Msk << u32ChannelNum);
}

/**
 * @brief This function get period interrupt of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are between 0~5
 * @return Period interrupt flag of specified channel
 * @retval 0 Period interrupt did not occurred
 * @retval 1 Period interrupt occurred
 */
uint32_t PWM_GetPeriodIntFlag (PWM_T *pwm, uint32_t u32ChannelNum)
{
    return ((pwm->INTSTS & (PWM_INTSTS_TMINT0_Msk << u32ChannelNum)) ? 1 : 0);
}

/**
 * @brief This function enable capture PDMA of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are 0 and 2
 * @param[in] u32RisingFirst Order of captured data transferred by PDMA. It could be either
 *              - \ref PWM_CAP_PDMA_RFORDER_R
 *              - \ref PWM_CAP_PDMA_RFORDER_F
 * @param[in] u32Mode Captured data transferred by PDMA interrupt type. It could be either
 *              - \ref PWM_RISING_LATCH_PDMA_ENABLE
 *              - \ref PWM_FALLING_LATCH_PDMA_ENABLE
 *              - \ref PWM_RISING_FALLING_LATCH_PDMA_ENABLE
 * @return None
 */
void PWM_EnablePDMA(PWM_T *pwm, uint32_t u32ChannelNum, uint32_t u32RisingFirst, uint32_t u32Mode)
{
    if (u32ChannelNum == 0)
        pwm->CAPCTL = (pwm->CAPCTL & ~(PWM_CAPCTL_PDMACAPMOD0_Msk | PWM_CAPCTL_CH0RFORDER_Msk)) | u32Mode | u32RisingFirst | PWM_CAPCTL_CH0PDMAEN_Msk;
    else
        pwm->CAPCTL = (pwm->CAPCTL & ~(PWM_CAPCTL_PDMACAPMOD2_Msk | PWM_CAPCTL_CH2RFORDER_Msk)) | (u32Mode << 16)| (u32RisingFirst << 16)| PWM_CAPCTL_CH2PDMAEN_Msk;
}

/**
 * @brief This function disable capture PDMA of selected channel
 * @param[in] pwm The base address of PWM module
 * @param[in] u32ChannelNum PWM channel number. Valid values are 0 and 2
 * @return None
 */
void PWM_DisablePDMA(PWM_T *pwm, uint32_t u32ChannelNum)
{
    if (u32ChannelNum == 0)
        pwm->CAPCTL &= ~PWM_CAPCTL_CH0PDMAEN_Msk;
    else
        pwm->CAPCTL &= ~PWM_CAPCTL_CH2PDMAEN_Msk;
}

/*@}*/ /* end of group NANO100_PWM_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_PWM_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2013-2014 Nuvoton Technology Corp. ***/
