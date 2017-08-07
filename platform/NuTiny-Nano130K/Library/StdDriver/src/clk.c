/**************************************************************************//**
 * @file     clk.c
 * @version  V1.00
 * $Revision: 29 $
 * $Date: 15/06/30 3:10p $
 * @brief    NANO100 series CLK driver source file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "Nano100Series.h"
/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_CLK_Driver CLK Driver
  @{
*/


/** @addtogroup NANO100_CLK_EXPORTED_FUNCTIONS CLK Exported Functions
  @{
*/

/**
  * @brief  This function disable frequency output function.
  * @param  None
  * @return None
  */
void CLK_DisableCKO(void)
{
    /* Disable CKO0 clock source */
    CLK->APBCLK &= (~CLK_APBCLK_FDIV_EN_Msk);
}

/**
  * @brief  This function enable frequency divider module clock,
  *         enable frequency divider clock function and configure frequency divider.
  * @param[in]  u32ClkSrc is frequency divider function clock source
  *         - \ref CLK_CLKSEL2_FRQDIV_S_HXT
  *         - \ref CLK_CLKSEL2_FRQDIV_S_LXT
  *         - \ref CLK_CLKSEL2_FRQDIV_S_HCLK
  *         - \ref CLK_CLKSEL2_FRQDIV_S_HIRC
  * @param[in]  u32ClkDiv is divider output frequency selection.
  * @return None
  *
  * @details    Output selected clock to CKO. The output clock frequency is divided by u32ClkDiv.
  *             The formula is:
  *                 CKO frequency = (Clock source frequency) / 2^(u32ClkDiv + 1)
  *             This function is just used to set CKO clock.
  *             User must enable I/O for CKO clock output pin by themselves.
  */
void CLK_EnableCKO(uint32_t u32ClkSrc, uint32_t u32ClkDiv)
{
    /* CKO = clock source / 2^(u32ClkDiv + 1) */
    CLK->FRQDIV = CLK_FRQDIV_FDIV_EN_Msk | u32ClkDiv ;

    /* Enable CKO clock source */
    CLK->APBCLK |= CLK_APBCLK_FDIV_EN_Msk;

    /* Select CKO clock source */
    CLK->CLKSEL2 = (CLK->CLKSEL2 & (~CLK_CLKSEL2_FRQDIV_S_Msk)) | u32ClkSrc;
}

/**
  * @brief  This function let system enter to Power-down mode.
  * @param  None
  * @return None
  */
void CLK_PowerDown(void)
{
    SCB->SCR = SCB_SCR_SLEEPDEEP_Msk;
    CLK->PWRCTL |= (CLK_PWRCTL_PD_EN_Msk | CLK_PWRCTL_WK_DLY_Msk );
    __WFI();
}

/**
  * @brief  This function let system enter to Idle mode
  * @return None
  */
void CLK_Idle(void)
{
    CLK->PWRCTL &= ~(CLK_PWRCTL_PD_EN_Msk );
    __WFI();
}

/**
  * @brief  This function get external high frequency crystal frequency. The frequency unit is Hz.
  * @param  None
  * @return None
  */
uint32_t CLK_GetHXTFreq(void)
{
    if(CLK->PWRCTL & CLK_PWRCTL_HXT_EN )
        return __HXT;
    else
        return 0;
}

/**
  * @brief  This function get external low frequency crystal frequency. The frequency unit is Hz.
  * @return LXT frequency
  */
uint32_t CLK_GetLXTFreq(void)
{
    if(CLK->PWRCTL & CLK_PWRCTL_LXT_EN )
        return __LXT;
    else
        return 0;
}

/**
  * @brief  This function get HCLK frequency. The frequency unit is Hz.
  * @param  None
  * @return HCLK frequency
  */
uint32_t CLK_GetHCLKFreq(void)
{
    SystemCoreClockUpdate();
    return SystemCoreClock;
}


/**
  * @brief  This function get CPU frequency. The frequency unit is Hz.
  * @param  None
  * @return CPU frequency
  */
uint32_t CLK_GetCPUFreq(void)
{
    SystemCoreClockUpdate();
    return SystemCoreClock;
}

/**
  * @brief  This function get PLL frequency. The frequency unit is Hz.
  * @param  None
  * @return PLL frequency
  */
uint32_t CLK_GetPLLClockFreq(void)
{
    uint32_t u32Freq =0, u32PLLSrc;
    uint32_t u32NO, u32NR, u32IN_DV, u32PllReg;

    u32PllReg = CLK->PLLCTL;

    if (u32PllReg & CLK_PLLCTL_PD)
        return 0;  /* PLL is in power down mode */

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
  * @brief  This function set HCLK frequency. The frequency unit is Hz. The range of u32Hclk is 24 ~ 42 MHz
  * @param[in]  u32Hclk is HCLK frequency
  * @return None
  */
uint32_t CLK_SetCoreClock(uint32_t u32Hclk)
{
    uint32_t u32HIRCSTB;
    /* Read HIRC clock source stable flag */
    u32HIRCSTB = CLK->CLKSTATUS & CLK_CLKSTATUS_HIRC_STB_Msk;

    if(u32Hclk==__HIRC12M) {
        CLK_EnableXtalRC(CLK_PWRCTL_HIRC_EN_Msk);
        CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC,CLK_HCLK_CLK_DIVIDER(1));
        return SystemCoreClock;
    }

    if(u32Hclk<FREQ_24MHZ) u32Hclk=FREQ_24MHZ;
    if(u32Hclk>FREQ_42MHZ) u32Hclk=FREQ_42MHZ;

    if(CLK->PWRCTL & CLK_PWRCTL_HXT_EN)
        CLK_EnablePLL(CLK_PLLCTL_PLL_SRC_HXT,u32Hclk*2);
    else {
        CLK_EnablePLL(CLK_PLLCTL_PLL_SRC_HIRC,u32Hclk*2);

        /* Read HIRC clock source stable flag */
        u32HIRCSTB = CLK->CLKSTATUS & CLK_CLKSTATUS_HIRC_STB_Msk;
    }
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_PLL,CLK_HCLK_CLK_DIVIDER(2));

    /* Disable HIRC if HIRC is disabled before setting core clock */
    if(u32HIRCSTB == 0)
        CLK->PWRCTL &= ~CLK_PWRCTL_HIRC_EN_Msk;

    return SystemCoreClock;
}

/**
  * @brief  This function set HCLK clock source and HCLK clock divider
  * @param[in]  u32ClkSrc is HCLK clock source. Including :
  *         - \ref CLK_CLKSEL0_HCLK_S_HXT
  *         - \ref CLK_CLKSEL0_HCLK_S_LXT
  *         - \ref CLK_CLKSEL0_HCLK_S_PLL
  *         - \ref CLK_CLKSEL0_HCLK_S_LIRC
  *         - \ref CLK_CLKSEL0_HCLK_S_HIRC
  * @param[in]  u32ClkDiv is HCLK clock divider. Including :
  *         - \ref CLK_HCLK_CLK_DIVIDER(x)
  * @return None
  */
void CLK_SetHCLK(uint32_t u32ClkSrc, uint32_t u32ClkDiv)
{
    uint32_t u32HIRCSTB;

    /* Read HIRC clock source stable flag */
    u32HIRCSTB = CLK->CLKSTATUS & CLK_CLKSTATUS_HIRC_STB_Msk;

    /* Switch to HIRC for Safe. Avoid HCLK too high when applying new divider. */
    CLK->PWRCTL |= CLK_PWRCTL_HIRC_EN_Msk;
    CLK_WaitClockReady(CLK_CLKSTATUS_HIRC_STB_Msk);
    CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLK_S_Msk)) | CLK_CLKSEL0_HCLK_S_HIRC;

    CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_HCLK_N_Msk) | u32ClkDiv;
    CLK->CLKSEL0 = (CLK->CLKSEL0 & ~CLK_CLKSEL0_HCLK_S_Msk) | u32ClkSrc;
    SystemCoreClockUpdate();

    /* Disable HIRC if HIRC is disabled before switching HCLK source */
    if(u32HIRCSTB == 0)
        CLK->PWRCTL &= ~CLK_CLKSTATUS_HIRC_STB_Msk;
}

/**
  * @brief  This function set selected module clock source and module clock divider
  * @param[in]  u32ModuleIdx is module index.
  * @param[in]  u32ClkSrc is module clock source.
  * @param[in]  u32ClkDiv is module clock divider.
  * @return None
  * @details Valid parameter combinations listed in following table:
  *
  * |Module index          |Clock source                       |Divider                      |
  * | :------------------- | :-------------------------------  | :-------------------------  |
  * |\ref GPIO_MODULE      | x                                 | x                           |
  * |\ref DMA_MODULE       | x                                 | x                           |
  * |\ref ISP_MODULE       | x                                 | x                           |
  * |\ref EBI_MODULE       | x                                 | x                           |
  * |\ref SRAM_MODULE      | x                                 | x                           |
  * |\ref TICK_MODULE      | x                                 | x                           |
  * |\ref SC2_MODULE       |\ref CLK_CLKSEL2_SC_S_HXT          |\ref CLK_SC2_CLK_DIVIDER(x)  |
  * |\ref SC2_MODULE       |\ref CLK_CLKSEL2_SC_S_PLL          |\ref CLK_SC2_CLK_DIVIDER(x)  |
  * |\ref SC2_MODULE       |\ref CLK_CLKSEL2_SC_S_HIRC         |\ref CLK_SC2_CLK_DIVIDER(x)  |
  * |\ref SC1_MODULE       |\ref CLK_CLKSEL2_SC_S_HXT          |\ref CLK_SC1_CLK_DIVIDER(x)  |
  * |\ref SC1_MODULE       |\ref CLK_CLKSEL2_SC_S_PLL          |\ref CLK_SC1_CLK_DIVIDER(x)  |
  * |\ref SC1_MODULE       |\ref CLK_CLKSEL2_SC_S_HIRC         |\ref CLK_SC1_CLK_DIVIDER(x)  |
  * |\ref SC0_MODULE       |\ref CLK_CLKSEL2_SC_S_HXT          |\ref CLK_SC0_CLK_DIVIDER(x)  |
  * |\ref SC0_MODULE       |\ref CLK_CLKSEL2_SC_S_PLL          |\ref CLK_SC0_CLK_DIVIDER(x)  |
  * |\ref SC0_MODULE       |\ref CLK_CLKSEL2_SC_S_HIRC         |\ref CLK_SC0_CLK_DIVIDER(x)  |
  * |\ref I2S_MODULE       |\ref CLK_CLKSEL2_I2S_S_HXT         |\ref CLK_I2S_CLK_DIVIDER(x)  |
  * |\ref I2S_MODULE       |\ref CLK_CLKSEL2_I2S_S_PLL         |\ref CLK_I2S_CLK_DIVIDER(x)  |
  * |\ref I2S_MODULE       |\ref CLK_CLKSEL2_I2S_S_HIRC        |\ref CLK_I2S_CLK_DIVIDER(x)  |
  * |\ref ADC_MODULE       |\ref CLK_CLKSEL1_ADC_S_HXT         |\ref CLK_ADC_CLK_DIVIDER(x)  |
  * |\ref ADC_MODULE       |\ref CLK_CLKSEL1_ADC_S_LXT         |\ref CLK_ADC_CLK_DIVIDER(x)  |
  * |\ref ADC_MODULE       |\ref CLK_CLKSEL1_ADC_S_PLL         |\ref CLK_ADC_CLK_DIVIDER(x)  |
  * |\ref ADC_MODULE       |\ref CLK_CLKSEL1_ADC_S_HIRC        |\ref CLK_ADC_CLK_DIVIDER(x)  |
  * |\ref USBD_MODULE      | x                                 |\ref CLK_USB_CLK_DIVIDER(x)  |
  * |\ref PWM1_CH23_MODULE |\ref CLK_CLKSEL2_PWM1_CH23_S_HXT   | x                           |
  * |\ref PWM1_CH23_MODULE |\ref CLK_CLKSEL2_PWM1_CH23_S_LXT   | x                           |
  * |\ref PWM1_CH23_MODULE |\ref CLK_CLKSEL2_PWM1_CH23_S_HCLK  | x                           |
  * |\ref PWM1_CH23_MODULE |\ref CLK_CLKSEL2_PWM1_CH23_S_HIRC  | x                           |
  * |\ref PWM1_CH01_MODULE |\ref CLK_CLKSEL2_PWM1_CH01_S_HXT   | x                           |
  * |\ref PWM1_CH01_MODULE |\ref CLK_CLKSEL2_PWM1_CH01_S_LXT   | x                           |
  * |\ref PWM1_CH01_MODULE |\ref CLK_CLKSEL2_PWM1_CH01_S_HCLK  | x                           |
  * |\ref PWM1_CH01_MODULE |\ref CLK_CLKSEL2_PWM1_CH01_S_HIRC  | x                           |
  * |\ref LCD_MODULE       |\ref CLK_CLKSEL1_LCD_S_LXT         | x                           |
  * |\ref PWM0_CH23_MODULE |\ref CLK_CLKSEL1_PWM0_CH23_S_HXT   | x                           |
  * |\ref PWM0_CH23_MODULE |\ref CLK_CLKSEL1_PWM0_CH23_S_LXT   | x                           |
  * |\ref PWM0_CH23_MODULE |\ref CLK_CLKSEL1_PWM0_CH23_S_HCLK  | x                           |
  * |\ref PWM0_CH23_MODULE |\ref CLK_CLKSEL1_PWM0_CH23_S_HIRC  | x                           |
  * |\ref PWM0_CH01_MODULE |\ref CLK_CLKSEL1_PWM0_CH01_S_HXT   | x                           |
  * |\ref PWM0_CH01_MODULE |\ref CLK_CLKSEL1_PWM0_CH01_S_LXT   | x                           |
  * |\ref PWM0_CH01_MODULE |\ref CLK_CLKSEL1_PWM0_CH01_S_HCLK  | x                           |
  * |\ref PWM0_CH01_MODULE |\ref CLK_CLKSEL1_PWM0_CH01_S_HIRC  | x                           |
  * |\ref UART1_MODULE     |\ref CLK_CLKSEL1_UART_S_HXT        |\ref CLK_UART_CLK_DIVIDER(x) |
  * |\ref UART1_MODULE     |\ref CLK_CLKSEL1_UART_S_LXT        |\ref CLK_UART_CLK_DIVIDER(x) |
  * |\ref UART1_MODULE     |\ref CLK_CLKSEL1_UART_S_PLL        |\ref CLK_UART_CLK_DIVIDER(x) |
  * |\ref UART1_MODULE     |\ref CLK_CLKSEL1_UART_S_HIRC       |\ref CLK_UART_CLK_DIVIDER(x) |
  * |\ref UART0_MODULE     |\ref CLK_CLKSEL1_UART_S_HXT        |\ref CLK_UART_CLK_DIVIDER(x) |
  * |\ref UART0_MODULE     |\ref CLK_CLKSEL1_UART_S_LXT        |\ref CLK_UART_CLK_DIVIDER(x) |
  * |\ref UART0_MODULE     |\ref CLK_CLKSEL1_UART_S_PLL        |\ref CLK_UART_CLK_DIVIDER(x) |
  * |\ref UART0_MODULE     |\ref CLK_CLKSEL1_UART_S_HIRC       |\ref CLK_UART_CLK_DIVIDER(x) |
  * |\ref SPI2_MODULE      |\ref CLK_CLKSEL2_SPI2_S_PLL        | x                           |
  * |\ref SPI2_MODULE      |\ref CLK_CLKSEL2_SPI2_S_HCLK       | x                           |
  * |\ref SPI1_MODULE      |\ref CLK_CLKSEL2_SPI1_S_PLL        | x                           |
  * |\ref SPI1_MODULE      |\ref CLK_CLKSEL2_SPI1_S_HCLK       | x                           |
  * |\ref SPI0_MODULE      |\ref CLK_CLKSEL2_SPI0_S_PLL        | x                           |
  * |\ref SPI0_MODULE      |\ref CLK_CLKSEL2_SPI0_S_HCLK       | x                           |
  * |\ref I2C1_MODULE      | x                                 | x                           |
  * |\ref I2C0_MODULE      | x                                 | x                           |
  * |\ref FDIV_MODULE      |\ref CLK_CLKSEL2_FRQDIV_S_HXT      | x                           |
  * |\ref FDIV_MODULE      |\ref CLK_CLKSEL2_FRQDIV_S_LXT      | x                           |
  * |\ref FDIV_MODULE      |\ref CLK_CLKSEL2_FRQDIV_S_HCLK     | x                           |
  * |\ref FDIV_MODULE      |\ref CLK_CLKSEL2_FRQDIV_S_HIRC     | x                           |
  * |\ref TMR3_MODULE      |\ref CLK_CLKSEL2_TMR3_S_HXT        | x                           |
  * |\ref TMR3_MODULE      |\ref CLK_CLKSEL2_TMR3_S_LXT        | x                           |
  * |\ref TMR3_MODULE      |\ref CLK_CLKSEL2_TMR3_S_LIRC       | x                           |
  * |\ref TMR3_MODULE      |\ref CLK_CLKSEL2_TMR3_S_EXT        | x                           |
  * |\ref TMR3_MODULE      |\ref CLK_CLKSEL2_TMR3_S_HIRC       | x                           |
  * |\ref TMR2_MODULE      |\ref CLK_CLKSEL2_TMR2_S_HXT        | x                           |
  * |\ref TMR2_MODULE      |\ref CLK_CLKSEL2_TMR2_S_LXT        | x                           |
  * |\ref TMR2_MODULE      |\ref CLK_CLKSEL2_TMR2_S_LIRC       | x                           |
  * |\ref TMR2_MODULE      |\ref CLK_CLKSEL2_TMR2_S_EXT        | x                           |
  * |\ref TMR2_MODULE      |\ref CLK_CLKSEL2_TMR2_S_HIRC       | x                           |
  * |\ref TMR1_MODULE      |\ref CLK_CLKSEL1_TMR1_S_HXT        | x                           |
  * |\ref TMR1_MODULE      |\ref CLK_CLKSEL1_TMR1_S_LXT        | x                           |
  * |\ref TMR1_MODULE      |\ref CLK_CLKSEL1_TMR1_S_LIRC       | x                           |
  * |\ref TMR1_MODULE      |\ref CLK_CLKSEL1_TMR1_S_EXT        | x                           |
  * |\ref TMR1_MODULE      |\ref CLK_CLKSEL1_TMR1_S_HIRC       | x                           |
  * |\ref TMR0_MODULE      |\ref CLK_CLKSEL1_TMR0_S_HXT        | x                           |
  * |\ref TMR0_MODULE      |\ref CLK_CLKSEL1_TMR0_S_LXT        | x                           |
  * |\ref TMR0_MODULE      |\ref CLK_CLKSEL1_TMR0_S_LIRC       | x                           |
  * |\ref TMR0_MODULE      |\ref CLK_CLKSEL1_TMR0_S_EXT        | x                           |
  * |\ref TMR0_MODULE      |\ref CLK_CLKSEL1_TMR0_S_HIRC       | x                           |
  * |\ref RTC_MODULE       | x                                 | x                           |
  * |\ref WDT_MODULE       | x                                 | x                           |
  *                                                                                          |
  */

void CLK_SetModuleClock(uint32_t u32ModuleIdx, uint32_t u32ClkSrc, uint32_t u32ClkDiv)
{
    uint32_t u32tmp=0,u32sel=0,u32div=0;

    if(MODULE_CLKDIV_Msk(u32ModuleIdx)!=MODULE_NoMsk) {
        u32div =(uint32_t)&CLK->CLKDIV0+((MODULE_CLKDIV(u32ModuleIdx))*4);
        u32tmp = *(volatile uint32_t *)(u32div);
        u32tmp = ( u32tmp & ~(MODULE_CLKDIV_Msk(u32ModuleIdx)<<MODULE_CLKDIV_Pos(u32ModuleIdx)) ) | u32ClkDiv;
        *(volatile uint32_t *)(u32div) = u32tmp;
    }

    if(MODULE_CLKSEL_Msk(u32ModuleIdx)!=MODULE_NoMsk) {
        u32sel = (uint32_t)&CLK->CLKSEL0+((MODULE_CLKSEL(u32ModuleIdx))*4);
        u32tmp = *(volatile uint32_t *)(u32sel);
        u32tmp = ( u32tmp & ~(MODULE_CLKSEL_Msk(u32ModuleIdx)<<MODULE_CLKSEL_Pos(u32ModuleIdx)) ) | u32ClkSrc;
        *(volatile uint32_t *)(u32sel) = u32tmp;
    }
}

/**
  * @brief  This function enable clock source
  * @param[in]  u32ClkMask is clock source mask. Including:
  *         - \ref CLK_PWRCTL_HXT_EN_Msk
  *         - \ref CLK_PWRCTL_LXT_EN_Msk
  *         - \ref CLK_PWRCTL_HIRC_EN_Msk
  *         - \ref CLK_PWRCTL_LIRC_EN_Msk
  * @return None
  */
void CLK_EnableXtalRC(uint32_t u32ClkMask)
{
    CLK->PWRCTL |= u32ClkMask;
    if(u32ClkMask & CLK_PWRCTL_HXT_EN_Msk)
        CLK_WaitClockReady(CLK_CLKSTATUS_HXT_STB_Msk);

    if(u32ClkMask & CLK_PWRCTL_LXT_EN_Msk)
        CLK_WaitClockReady(CLK_CLKSTATUS_LXT_STB_Msk);

    if(u32ClkMask & CLK_PWRCTL_HIRC_EN_Msk)
        CLK_WaitClockReady(CLK_CLKSTATUS_HIRC_STB_Msk);

    if(u32ClkMask & CLK_PWRCTL_LIRC_EN_Msk)
        CLK_WaitClockReady(CLK_CLKSTATUS_LIRC_STB_Msk);
}

/**
  * @brief  This function disable clock source
  * @param[in]  u32ClkMask is clock source mask. Including:
  *         - \ref CLK_PWRCTL_HXT_EN_Msk
  *         - \ref CLK_PWRCTL_LXT_EN_Msk
  *         - \ref CLK_PWRCTL_HIRC_EN_Msk
  *         - \ref CLK_PWRCTL_LIRC_EN_Msk
  * @return None
  */
void CLK_DisableXtalRC(uint32_t u32ClkMask)
{
    CLK->PWRCTL &= ~u32ClkMask;
}

/**
  * @brief  This function enable module clock
  * @param[in]  u32ModuleIdx is module index. Including :
  *         - \ref GPIO_MODULE
  *         - \ref DMA_MODULE
  *         - \ref ISP_MODULE
  *         - \ref EBI_MODULE
  *         - \ref SRAM_MODULE
  *         - \ref TICK_MODULE
  *         - \ref SC2_MODULE
  *         - \ref SC1_MODULE
  *         - \ref SC0_MODULE
  *         - \ref USBD_MODULE
  *         - \ref I2S_MODULE
  *         - \ref ADC_MODULE
  *         - \ref PWM1_CH23_MODULE
  *         - \ref PWM1_CH01_MODULE
  *         - \ref PWM0_CH23_MODULE
  *         - \ref PWM0_CH01_MODULE
  *         - \ref UART1_MODULE
  *         - \ref UART0_MODULE
  *         - \ref SPI2_MODULE
  *         - \ref SPI1_MODULE
  *         - \ref SPI0_MODULE
  *         - \ref I2C1_MODULE
  *         - \ref I2C0_MODULE
  *         - \ref FDIV_MODULE
  *         - \ref TMR3_MODULE
  *         - \ref TMR2_MODULE
  *         - \ref TMR1_MODULE
  *         - \ref TMR0_MODULE
  *         - \ref RTC_MODULE
  *         - \ref WDT_MODULE
  *         - \ref LCD_MODULE
  *         - \ref DAC_MODULE
  * @return None
  */
void CLK_EnableModuleClock(uint32_t u32ModuleIdx)
{
    *(volatile uint32_t *)((uint32_t)&CLK->AHBCLK+(MODULE_APBCLK(u32ModuleIdx)*4))  |= 1<<MODULE_IP_EN_Pos(u32ModuleIdx);
}

/**
  * @brief  This function disable module clock
  * @param[in]  u32ModuleIdx is module index. Including :
  *         - \ref GPIO_MODULE
  *         - \ref DMA_MODULE
  *         - \ref ISP_MODULE
  *         - \ref EBI_MODULE
  *         - \ref SRAM_MODULE
  *         - \ref TICK_MODULE
  *         - \ref SC2_MODULE
  *         - \ref SC1_MODULE
  *         - \ref SC0_MODULE
  *         - \ref USBD_MODULE
  *         - \ref I2S_MODULE
  *         - \ref ADC_MODULE
  *         - \ref PWM1_CH23_MODULE
  *         - \ref PWM1_CH01_MODULE
  *         - \ref PWM0_CH23_MODULE
  *         - \ref PWM0_CH01_MODULE
  *         - \ref UART1_MODULE
  *         - \ref UART0_MODULE
  *         - \ref SPI2_MODULE
  *         - \ref SPI1_MODULE
  *         - \ref SPI0_MODULE
  *         - \ref I2C1_MODULE
  *         - \ref I2C0_MODULE
  *         - \ref FDIV_MODULE
  *         - \ref TMR3_MODULE
  *         - \ref TMR2_MODULE
  *         - \ref TMR1_MODULE
  *         - \ref TMR0_MODULE
  *         - \ref RTC_MODULE
  *         - \ref WDT_MODULE
  *         - \ref LCD_MODULE
  *         - \ref DAC_MODULE
  * @return None
  */
void CLK_DisableModuleClock(uint32_t u32ModuleIdx)
{
    *(volatile uint32_t *)((uint32_t)&CLK->AHBCLK+(MODULE_APBCLK(u32ModuleIdx)*4))  &= ~(1<<MODULE_IP_EN_Pos(u32ModuleIdx));
}

/**
  * @brief  This function set PLL frequency
  * @param[in]  u32PllClkSrc is PLL clock source. Including :
  *         - \ref CLK_PLLCTL_PLL_SRC_HIRC
  *         - \ref CLK_PLLCTL_PLL_SRC_HXT
  * @param[in]  u32PllFreq is PLL frequency
  * @return None
  */
uint32_t CLK_EnablePLL(uint32_t u32PllClkSrc, uint32_t u32PllFreq)
{
    uint32_t u32ClkSrc,u32NR, u32NF,u32Register;
    uint32_t u32NRTable[4]= {2,4,8,16};
    int32_t i32NRVal;
    if ( u32PllFreq < FREQ_48MHZ)
        u32PllFreq=FREQ_48MHZ;
    else if(u32PllFreq > FREQ_120MHZ)
        u32PllFreq=FREQ_120MHZ;

    if(u32PllClkSrc!=CLK_PLLCTL_PLL_SRC_HIRC) {
        /* PLL source clock from HXT */
        u32Register = (0x0UL<<CLK_PLLCTL_PLL_SRC_Pos);
        u32ClkSrc = __HXT;
    } else {
        /* PLL source clock from HIRC */
        u32Register = (0x1UL<<CLK_PLLCTL_PLL_SRC_Pos);
        u32ClkSrc =__HIRC12M;
    }

    u32NF = u32PllFreq / 1000000;
    u32NR = u32ClkSrc / 1000000;
    if(u32ClkSrc%12==0) {
        u32NF=(u32NF/3)*4;
        u32NR=(u32NR/3)*4;
    }

    while( u32NR>16 || u32NF>(0x3F+32) ) {
        u32NR = u32NR>>1;
        u32NF = u32NF>>1;
    }

    for(i32NRVal=3; i32NRVal>=0; i32NRVal--)
        if(u32NR==u32NRTable[i32NRVal]) break;

    CLK->PLLCTL = u32Register | (i32NRVal<<8) | (u32NF - 32) ;

    CLK->PLLCTL &= ~CLK_PLLCTL_PD_Msk;

    CLK_WaitClockReady(CLK_CLKSTATUS_PLL_STB_Msk);

    return CLK_GetPLLClockFreq();

}

/**
  * @brief  This function disable PLL
  * @param  None
  * @return None
  */
void CLK_DisablePLL(void)
{
    CLK->PLLCTL |= CLK_PLLCTL_PD_Msk;
}

/**
  * @brief  This function execute delay function.
  * @param  us  Delay time. The Max value is 2^24 / CPU Clock(MHz). Ex:
  *                             50MHz => 335544us, 48MHz => 349525us, 28MHz => 699050us ...
  * @return None
  * @details    Use the SysTick to generate the delay time and the UNIT is in us.
  *             The SysTick clock source is from HCLK, i.e the same as system core clock.
  */
void CLK_SysTickDelay(uint32_t us)
{
    SysTick->LOAD = us * CyclesPerUs;
    SysTick->VAL  =  (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    SysTick->CTRL = 0;
}

/**
  * @brief      Enable System Tick counter
  * @param[in]  u32ClkSrc is System Tick clock source. Including:
  *             - \ref CLK_CLKSEL0_STCLKSEL_HCLK_DIV8
  *             - \ref CLK_CLKSEL0_STCLKSEL_HCLK
  * @param[in]  u32Count is System Tick reload value. It should be 0x1~0xFFFFFF.
  * @return     None
  * @details    This function set System Tick clock source, reload value, enable System Tick counter and interrupt.
  *                    The register write-protection function should be disabled before using this function.
  */
void CLK_EnableSysTick(uint32_t u32ClkSrc, uint32_t u32Count)
{
    SysTick->CTRL=0;
    if( u32ClkSrc== CLK_CLKSEL0_STCLKSEL_HCLK )    /* Set System Tick clock source */
        SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    else {
        SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
    }
    SysTick->LOAD  = u32Count;                /* Set System Tick reload value */
    SysTick->VAL = 0;                         /* Clear System Tick current value and counter flag  */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* Set System Tick counter enabled */
}

/**
  * @brief      Disable System Tick counter
  * @return     None
  * @details    This function disable System Tick counter.
  */
void CLK_DisableSysTick(void)
{
    SysTick->CTRL = 0;    /* Set System Tick counter disabled */
}

/**
  * @brief  This function check selected clock source status
  * @param[in]  u32ClkMask is selected clock source. Including
  *           - \ref CLK_CLKSTATUS_CLK_SW_FAIL_Msk
  *           - \ref CLK_CLKSTATUS_HIRC_STB_Msk
  *           - \ref CLK_CLKSTATUS_LIRC_STB_Msk
  *           - \ref CLK_CLKSTATUS_PLL_STB_Msk
  *           - \ref CLK_CLKSTATUS_LXT_STB_Msk
  *           - \ref CLK_CLKSTATUS_HXT_STB_Msk
  * @return   0  clock is not stable
  *           1  clock is stable
  *
  * @details  To wait for clock ready by specified CLKSTATUS bit or timeout (~5ms)
  */
uint32_t CLK_WaitClockReady(uint32_t u32ClkMask)
{
    int32_t i32TimeOutCnt;

    i32TimeOutCnt = __HSI / 200; /* About 5ms */

    while((CLK->CLKSTATUS & u32ClkMask) != u32ClkMask) {
        if(i32TimeOutCnt-- <= 0)
            return 0;
    }
    return 1;
}


/*@}*/ /* end of group NANO100_CLK_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_CLK_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/

