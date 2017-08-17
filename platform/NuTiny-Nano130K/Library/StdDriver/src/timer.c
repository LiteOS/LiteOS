/**************************************************************************//**
 * @file     timer.c
 * @version  V1.00
 * $Revision: 11 $
 * $Date: 15/06/23 5:15p $
 * @brief    Nano100 series TIMER driver source file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "Nano100Series.h"

/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_TIMER_Driver TIMER Driver
  @{
*/


/** @addtogroup NANO100_TIMER_EXPORTED_FUNCTIONS TIMER Exported Functions
  @{
*/

/**
  * @brief This API is used to configure timer to operate in specified mode
  *        and frequency. If timer cannot work in target frequency, a closest
  *        frequency will be chose and returned.
  * @param[in] timer The base address of Timer module
  * @param[in] u32Mode Operation mode. Possible options are
  *                 - \ref TIMER_ONESHOT_MODE
  *                 - \ref TIMER_PERIODIC_MODE
  *                 - \ref TIMER_TOGGLE_MODE
  *                 - \ref TIMER_CONTINUOUS_MODE
  * @param[in] u32Freq Target working frequency
  * @return Real Timer working frequency
  * @note After calling this API, Timer is \b NOT running yet. But could start timer running be calling
  *       \ref TIMER_Start macro or program registers directly
  */
uint32_t TIMER_Open(TIMER_T *timer, uint32_t u32Mode, uint32_t u32Freq)
{
    uint32_t u32Clk = TIMER_GetModuleClock(timer);
    uint32_t u32Cmpr = 0, u32Prescale = 0;

    // Fastest possible timer working freq is u32Clk / 2. While cmpr = 2, pre-scale = 0
    if(u32Freq > (u32Clk / 2)) {
        u32Cmpr = 2;
    } else {
        if(u32Clk >= 0x2000000) {
            u32Prescale = 3;    // real prescaler value is 4
            u32Clk >>= 2;
        } else if(u32Clk >= 0x1000000) {
            u32Prescale = 1;    // real prescaler value is 2
            u32Clk >>= 1;
        }
        u32Cmpr = u32Clk / u32Freq;
    }
    timer->CMPR = u32Cmpr;
    timer->PRECNT = u32Prescale;
    timer->CTL = u32Mode;


    return(u32Clk / (u32Cmpr * (u32Prescale + 1)));
}

/**
  * @brief This API stops Timer counting and disable the Timer interrupt function
  * @param[in] timer The base address of Timer module
  * @return None
  */
void TIMER_Close(TIMER_T *timer)
{
    timer->CTL = 0;
    timer->IER = 0;
}

/**
  * @brief This API is used to create a delay loop for u32usec micro seconds
  * @param[in] timer The base address of Timer module
  * @param[in] u32Usec Delay period in micro seconds with 10 usec every step. Valid values are between 10~1000000 (10 micro second ~ 1 second)
  * @return None
  * @note This API overwrites the register setting of the timer used to count the delay time.
  * @note This API use polling mode. So there is no need to enable interrupt for the timer module used to generate delay
  */
void TIMER_Delay(TIMER_T *timer, uint32_t u32Usec)
{
    uint32_t u32Clk = TIMER_GetModuleClock(timer);
    uint32_t u32Prescale = 0, delay = SystemCoreClock / u32Clk;
    long long u64Cmpr;

    // Clear current timer configuration
    timer->CTL = 0;

    if(u32Clk == 10000) {         // min delay is 100us if timer clock source is LIRC 10k
        u32Usec = ((u32Usec + 99) / 100) * 100;
    } else {    // 10 usec every step
        u32Usec = ((u32Usec + 9) / 10) * 10;
    }

    if(u32Clk >= 0x2000000) {
        u32Prescale = 3;    // real prescaler value is 4
        u32Clk >>= 2;
    } else if(u32Clk >= 0x1000000) {
        u32Prescale = 1;    // real prescaler value is 2
        u32Clk >>= 1;
    }

    // u32Usec * u32Clk might overflow if using uint32_t
    u64Cmpr = ((long long)u32Usec * (long long)u32Clk) / (long long)1000000;

    timer->CMPR = (uint32_t)u64Cmpr;
    timer->PRECNT = u32Prescale;
    timer->CTL = TIMER_CTL_TMR_EN_Msk; // one shot mode

    // When system clock is faster than timer clock, it is possible timer active bit cannot set in time while we check it.
    // And the while loop below return immediately, so put a tiny delay here allowing timer start counting and raise active flag.
    for(; delay > 0; delay--) {
        __NOP();
    }

    while(timer->CTL & TIMER_CTL_TMR_ACT_Msk);

}

/**
  * @brief This API is used to enable timer capture function with specified mode and capture edge
  * @param[in] timer The base address of Timer module
  * @param[in] u32CapMode Timer capture mode. Could be
  *                 - \ref TIMER_CAPTURE_FREE_COUNTING_MODE
  *                 - \ref TIMER_CAPTURE_TRIGGER_COUNTING_MODE
  *                 - \ref TIMER_CAPTURE_COUNTER_RESET_MODE
  * @param[in] u32Edge Timer capture edge. Possible values are
  *                 - \ref TIMER_CAPTURE_FALLING_EDGE
  *                 - \ref TIMER_CAPTURE_RISING_EDGE
  *                 - \ref TIMER_CAPTURE_FALLING_THEN_RISING_EDGE
  *                 - \ref TIMER_CAPTURE_RISING_THEN_FALLING_EDGE
  * @return None
  * @note Timer frequency should be configured separately by using \ref TIMER_Open API, or program registers directly
  */
void TIMER_EnableCapture(TIMER_T *timer, uint32_t u32CapMode, uint32_t u32Edge)
{

    timer->CTL = (timer->CTL & ~(TIMER_CTL_TCAP_MODE_Msk |
                                 TIMER_CTL_TCAP_CNT_MODE_Msk |
                                 TIMER_CTL_TCAP_EDGE_Msk)) |
                 u32CapMode | u32Edge | TIMER_CTL_TCAP_EN_Msk;
}

/**
  * @brief This API is used to disable the Timer capture function
  * @param[in] timer The base address of Timer module
  * @return None
  */
void TIMER_DisableCapture(TIMER_T *timer)
{
    timer->CTL &= ~TIMER_CTL_TCAP_EN_Msk;

}

/**
  * @brief This function is used to enable the Timer counter function with specify detection edge
  * @param[in] timer The base address of Timer module
  * @param[in] u32Edge Detection edge of counter pin. Could be ether
  *             - \ref TIMER_COUNTER_RISING_EDGE, or
  *             - \ref TIMER_COUNTER_FALLING_EDGE
  * @return None
  * @note Timer compare value should be configured separately by using \ref TIMER_SET_CMP_VALUE macro or program registers directly
  */
void TIMER_EnableEventCounter(TIMER_T *timer, uint32_t u32Edge)
{
    timer->CTL = (timer->CTL & ~TIMER_CTL_EVENT_EDGE_Msk) | u32Edge;
    timer->CTL |= TIMER_CTL_EVENT_EN_Msk;
}

/**
  * @brief This API is used to disable the Timer event counter function.
  * @param[in] timer The base address of Timer module
  * @return None
  */
void TIMER_DisableEventCounter(TIMER_T *timer)
{
    timer->CTL &= ~TIMER_CTL_EVENT_EN_Msk;
}

/**
  * @brief This API is used to get the clock frequency of Timer
  * @param[in] timer The base address of Timer module
  * @return Timer clock frequency
  * @note This API cannot return correct clock rate if timer source is external clock input.
  */
uint32_t TIMER_GetModuleClock(TIMER_T *timer)
{
    uint32_t u32Src;
    const uint32_t au32Clk[] = {__HXT, __LXT, __LIRC, 0};   // we don't know actual clock if external pin is clock source, set to 0 here

    if(timer == TIMER0)
        u32Src = (CLK->CLKSEL1 & CLK_CLKSEL1_TMR0_S_Msk) >> CLK_CLKSEL1_TMR0_S_Pos;
    else if(timer == TIMER1)
        u32Src = (CLK->CLKSEL1 & CLK_CLKSEL1_TMR1_S_Msk) >> CLK_CLKSEL1_TMR1_S_Pos;
    else if(timer == TIMER2)
        u32Src = (CLK->CLKSEL2 & CLK_CLKSEL2_TMR2_S_Msk) >> CLK_CLKSEL2_TMR2_S_Pos;
    else // Timer 3
        u32Src = (CLK->CLKSEL2 & CLK_CLKSEL2_TMR3_S_Msk) >> CLK_CLKSEL2_TMR3_S_Pos;

    if(u32Src < 4)
        return au32Clk[u32Src];
    else
        return __HIRC;

}

/**
  * @brief This function is used to enable the Timer frequency counter function
  * @param[in] timer The base address of Timer module. Can be \ref TIMER0 or \ref TIMER2
  * @param[in] u32DropCount This parameter has no effect in Nano100 series BSP
  * @param[in] u32Timeout This parameter has no effect in Nano100 series BSP
  * @param[in] u32EnableInt Enable interrupt assertion after capture complete or not. Valid values are TRUE and FALSE
  * @return None
  * @details This function is used to calculate input event frequency. After enable
  *          this function, a pair of timers, TIMER0 and TIMER1, or TIMER2 and TIMER3
  *          will be configured for this function. The mode used to calculate input
  *          event frequency is mentioned as "Inter Timer Trigger Mode" in Technical
  *          Reference Manual
  */
void TIMER_EnableFreqCounter(TIMER_T *timer,
                             uint32_t u32DropCount,
                             uint32_t u32Timeout,
                             uint32_t u32EnableInt)
{
    TIMER_T *t;    // store the timer base to configure compare value

    t = (timer == TIMER0) ? TIMER1 : TIMER3;

    t->CMPR = 0xFFFFFF;
    t->IER = u32EnableInt ? TIMER_IER_TCAP_IE_Msk : 0;
    timer->CTL = TIMER_CTL_INTR_TRG_EN_Msk | TIMER_CTL_TMR_EN_Msk;

    return;
}
/**
  * @brief This function is used to disable the Timer frequency counter function.
  * @param[in] timer The base address of Timer module
  * @return None
  */
void TIMER_DisableFreqCounter(TIMER_T *timer)
{
    timer->CTL &= ~TIMER_CTL_INTR_TRG_EN_Msk;
}

/**
  * @brief This function is used to select the interrupt source used to trigger other modules.
  * @param[in] timer The base address of Timer module
  * @param[in] u32Src Selects the interrupt source to trigger other modules. Could be:
  *              - \ref TIMER_TIMEOUT_TRIGGER
  *              - \ref TIMER_CAPTURE_TRIGGER
  * @return None
  */
void TIMER_SetTriggerSource(TIMER_T *timer, uint32_t u32Src)
{
    timer->CTL = (timer->CTL & ~TIMER_CTL_CAP_TRG_EN_Msk) | u32Src;
}

/**
  * @brief This function is used to set modules trigger by timer interrupt
  * @param[in] timer The base address of Timer module
  * @param[in] u32Mask The mask of modules (ADC, DAC and PDMA) trigger by timer. Is the combination of
  *             - \ref TIMER_CTL_PDMA_TEEN_Msk,
  *             - \ref TIMER_CTL_ADC_TEEN_Msk, and
  *             - \ref TIMER_CTL_DAC_TEEN_Msk,
  * @return None
  */
void TIMER_SetTriggerTarget(TIMER_T *timer, uint32_t u32Mask)
{
    timer->CTL = (timer->CTL & ~(TIMER_CTL_PDMA_TEEN_Msk | TIMER_CTL_DAC_TEEN_Msk | TIMER_CTL_ADC_TEEN_Msk)) | u32Mask;
}

/*@}*/ /* end of group NANO100_TIMER_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_TIMER_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
