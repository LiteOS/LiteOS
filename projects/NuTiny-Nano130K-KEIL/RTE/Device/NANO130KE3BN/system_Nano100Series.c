/******************************************************************************
 * @file     system_Nano100Series.c
 * @version  V1.00
 * $Revision: 4 $
 * $Date: 14/01/29 4:09p $
 * @brief    Nano100 series system clock init code and assert handler
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include <stdint.h>
#include "Nano100Series.h"


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock  = __HSI;              /*!< System Clock Frequency (Core Clock) */
uint32_t CyclesPerUs      = (__HSI / 1000000);  /*!< Cycles per micro second */

/**
  * @brief  Calculate current PLL clock frequency.
  * @param  None.
  * @return PLL clock frequency. The clock UNIT is in Hz.
  */
uint32_t SysGet_PLLClockFreq(void)
{
    uint32_t u32Freq =0, u32PLLSrc;
    uint32_t u32NO, u32NR, u32IN_DV, u32PllReg;

    u32PllReg = CLK->PLLCTL;

    if (u32PllReg & CLK_PLLCTL_PD)
        return 0;    /* PLL is in power down mode */

    if (u32PllReg & CLK_PLLCTL_PLL_SRC_Msk)
        u32PLLSrc = __HIRC12M;
    else
        u32PLLSrc = __HXT;

    u32NO = (u32PllReg & CLK_PLLCTL_OUT_DV) ? 2: 1;

    u32IN_DV = (u32PllReg & CLK_PLLCTL_IN_DV_Msk) >> 8;
    if (u32IN_DV == 0)
        u32NR = 2;
    else if (u32IN_DV == 1)
        u32NR = 4;
    else if (u32IN_DV == 2)
        u32NR = 8;
    else
        u32NR = 16;

    u32Freq = u32PLLSrc * ((u32PllReg & CLK_PLLCTL_FB_DV_Msk) +32) / u32NR / u32NO;

    return u32Freq;
}


/**
  * @brief  Get current HCLK clock frequency.
  * @param  None.
  * @return HCLK clock frequency. The clock UNIT is in Hz.
  */
uint32_t SysGet_HCLKFreq(void)
{

    uint32_t u32Freqout, u32AHBDivider, u32ClkSel;

    u32ClkSel = CLK->CLKSEL0 & CLK_CLKSEL0_HCLK_S_Msk;

    if (u32ClkSel == CLK_CLKSEL0_HCLK_S_HXT) {  /* external HXT crystal clock */
        u32Freqout = __HXT;
    } else if(u32ClkSel == CLK_CLKSEL0_HCLK_S_LXT) {    /* external LXT crystal clock */
        u32Freqout = __LXT;
    } else if(u32ClkSel == CLK_CLKSEL0_HCLK_S_PLL) {    /* PLL clock */
        u32Freqout = SysGet_PLLClockFreq();
    } else if(u32ClkSel == CLK_CLKSEL0_HCLK_S_LIRC) { /* internal LIRC oscillator clock */
        u32Freqout = __LIRC;
    } else {                                /* internal HIRC oscillator clock */
        u32Freqout = __HIRC12M;
    }
    u32AHBDivider = (CLK->CLKDIV0 & CLK_CLKDIV0_HCLK_N_Msk) + 1 ;

    return (u32Freqout/u32AHBDivider);
}


/**
  * @brief  This function is used to update the variable SystemCoreClock
  *   and must be called whenever the core clock is changed.
  * @param  None.
  * @retval None.
  */

void SystemCoreClockUpdate (void)
{

    SystemCoreClock = SysGet_HCLKFreq();
    CyclesPerUs = (SystemCoreClock + 500000) / 1000000;
}

#if USE_ASSERT

/**
 * @brief      Assert Error Message
 *
 * @param[in]  file  the source file name
 * @param[in]  line  line number
 *
 * @return     None
 *
 * @details    The function prints the source file name and line number where
 *             the ASSERT_PARAM() error occurs, and then stops in an infinite loop.
 */
void AssertError(uint8_t * file, uint32_t line)
{

    printf("[%s] line %d : wrong parameters.\r\n", file, line);

    /* Infinite loop */
    while(1) ;
}
#endif

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
