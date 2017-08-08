/**************************************************************************//**
 * @file     dac.c
 * @version  V1.00
 * $Revision: 4 $
 * $Date: 14/09/08 12:31p $
 * @brief    NANO100 series DAC driver source file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "NANO100Series.h"

/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_DAC_Driver DAC Driver
  @{
*/

/** @addtogroup NANO100_DAC_EXPORTED_FUNCTIONS DAC Exported Functions
  @{
*/

/**
  * @brief This function make a DAC channel ready to convert.
  * @param[in] dac Base address of DAC module.
  * @param[in] u32Ch DAC channel number, could be 0 or 1
  * @param[in] u32TrgSrc Decides the trigger source of specified DAC channel. Valid options are:
  *                      - \ref DAC_WRITE_DAT_TRIGGER
  *                      - \ref DAC_PDMA_TRIGGER
  *                      - \ref DAC_TIMER0_TRIGGER
  *                      - \ref DAC_TIMER1_TRIGGER
  *                      - \ref DAC_TIMER2_TRIGGER
  *                      - \ref DAC_TIMER3_TRIGGER
  * @return None
  * @note This API also set DAC stable time to 2uc according to current PCLK
  */
void DAC_Open(DAC_T *dac, uint32_t u32Ch, uint32_t u32TrgSrc)
{
    uint32_t u32Delay;

    // DAC needs 6 us to stable after power on
    u32Delay = CLK_GetHCLKFreq() * 6 / 1000000;
    if(u32Delay == 0)
        u32Delay++;
    if(u32Ch == 0)
        DAC->CTL0 = (u32Delay << DAC_CTL_DACPWONSTBCNT_Pos) | u32TrgSrc | DAC_CTL_DACEN_Msk;
    else
        DAC->CTL1 = (u32Delay << DAC_CTL_DACPWONSTBCNT_Pos) | u32TrgSrc | DAC_CTL_DACEN_Msk;

    // DAC needs 2 us to stable after convert.
    u32Delay = CLK_GetHCLKFreq() * 2 / 1000000;
    if(u32Delay == 0)
        u32Delay++;
    DAC->COMCTL = (DAC->COMCTL & ~DAC_COMCTL_WAITDACCONV_Msk) | u32Delay;
}

/**
  * @brief Disable DAC analog power.
  * @param[in] dac Base address of DAC module.
  * @param[in] u32Ch DAC channel number, could be 0 or 1
  * @return None
  * @details Disable DAC analog power for saving power consumption.
  */
void DAC_Close(DAC_T *dac, uint32_t u32Ch)
{
    if(u32Ch == 0) {
        DAC->CTL0 &= ~DAC_CTL_DACEN_Msk;
    } else {
        DAC->CTL1 &= ~DAC_CTL_DACEN_Msk;
    }

}


/**
  * @brief Set delay time for DAC to become stable.
  * @param[in] dac Base address of DAC module.
  * @param[in] u32Delay Decides the DAC conversion settling time, Valid values are between 1~0xFF.
  * @return Success or failed
  * @retval 0 Success
  * @retval -1 Failed, the new setting will cause stable time less than 2us. So new setting is not applied.
  * @details For example, DAC controller clock speed is 12MHz and DAC conversion settling time is 3 us,
  *          u32Delay should be given the value 3 * 12 = 36.
  * @note User needs to write appropriate value to meet DAC conversion settling time base on
  *       PCLK (APB clock) speed. Minimum delay is 2 us.
  * @note This setting is shared by both DAC channels.
  */
int DAC_SetDelayTime(DAC_T *dac, uint32_t u32Delay)
{
    uint32_t u32Dly;

    // DAC needs 2 us to stable after DAC convert, calculate minimal setting
    u32Dly = CLK_GetHCLKFreq() * 2 / 1000000;
    if(u32Dly == 0)
        u32Dly++;

    if(u32Delay < u32Dly)  // return error id stable time is shorter than 2us
        return -1;
    DAC->COMCTL = (DAC->COMCTL & ~DAC_COMCTL_WAITDACCONV_Msk) | u32Delay;
    return 0;
}




/*@}*/ /* end of group NANO100_DAC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_DAC_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
