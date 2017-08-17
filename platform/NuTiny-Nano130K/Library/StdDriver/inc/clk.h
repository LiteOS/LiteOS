/**************************************************************************//**
 * @file     clk.h
 * @version  V1.00
 * $Revision: 20 $
 * $Date: 15/07/08 10:00a $
 * @brief    Nano100 series CLK driver header file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#ifndef __CLK_H__
#define __CLK_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_CLK_Driver CLK Driver
  @{
*/

/** @addtogroup NANO100_CLK_EXPORTED_CONSTANTS CLK Exported Constants
  @{
*/


#define FREQ_128MHZ       128000000
#define FREQ_120MHZ       120000000
#define FREQ_48MHZ         48000000
#define FREQ_42MHZ         42000000
#define FREQ_32MHZ         32000000
#define FREQ_24MHZ         24000000
#define FREQ_12MHZ         12000000

/********************* Bit definition of PWRCTL register **********************/
#define CLK_PWRCTL_HXT_EN         (0x1UL<<CLK_PWRCTL_HXT_EN_Pos)      /*!<Enable high speed crystal */
#define CLK_PWRCTL_LXT_EN         (0x1UL<<CLK_PWRCTL_LXT_EN_Pos)      /*!<Enable low speed crystal */
#define CLK_PWRCTL_HIRC_EN        (0x1UL<<CLK_PWRCTL_HIRC_EN_Pos)     /*!<Enable internal high speed oscillator */
#define CLK_PWRCTL_LIRC_EN        (0x1UL<<CLK_PWRCTL_LIRC_EN_Pos)     /*!<Enable internal low speed oscillator */
#define CLK_PWRCTL_DELY_EN        (0x1UL<<CLK_PWRCTL_WK_DLY_Pos)      /*!<Enable the wake-up delay counter */
#define CLK_PWRCTL_WAKEINT_EN     (0x1UL<<CLK_PWRCTL_PD_WK_IE_Pos)    /*!<Enable the wake-up interrupt */
#define CLK_PWRCTL_PWRDOWN_EN     (0x1UL<<CLK_PWRCTL_PD_EN_Pos)       /*!<Power down enable bit */
#define CLK_PWRCTL_HXT_SELXT      (0x1UL<<CLK_PWRCTL_HXT_SELXT_Pos)   /*!<High frequency crystal loop back path Enabled */
#define CLK_PWRCTL_HXT_GAIN       (0x1UL<<CLK_PWRCTL_HXT_GAIN_Pos)    /*!<High frequency crystal Gain control Enabled */
#define CLK_PWRCTL_LXT_SCNT       (0x1UL<<CLK_PWRCTL_LXT_SCNT_Pos)    /*!<Delay 8192 LXT before LXT output */


/********************* Bit definition of AHBCLK register **********************/
#define CLK_AHBCLK_GPIO_EN        (0x1UL<<CLK_AHBCLK_GPIO_EN_Pos)      /*!<GPIO clock enable */
#define CLK_AHBCLK_DMA_EN         (0x1UL<<CLK_AHBCLK_DMA_EN_Pos)       /*!<DMA clock enable */
#define CLK_AHBCLK_ISP_EN         (0x1UL<<CLK_AHBCLK_ISP_EN_Pos)       /*!<Flash ISP controller clock enable */
#define CLK_AHBCLK_EBI_EN         (0x1UL<<CLK_AHBCLK_EBI_EN_Pos)       /*!<EBI clock enable */
#define CLK_AHBCLK_SRAM_EN        (0x1UL<<CLK_AHBCLK_SRAM_EN_Pos)      /*!<SRAM Controller Clock Enable */
#define CLK_AHBCLK_TICK_EN        (0x1UL<<CLK_AHBCLK_TICK_EN_Pos)      /*!<System Tick Clock Enable */

/********************* Bit definition of APBCLK register **********************/
#define CLK_APBCLK_WDT_EN         (0x1UL<<CLK_APBCLK_WDT_EN_Pos)       /*!<Watchdog clock enable */
#define CLK_APBCLK_RTC_EN         (0x1UL<<CLK_APBCLK_RTC_EN_Pos)       /*!<RTC clock enable */
#define CLK_APBCLK_TMR0_EN        (0x1UL<<CLK_APBCLK_TMR0_EN_Pos)      /*!<Timer 0 clock enable */
#define CLK_APBCLK_TMR1_EN        (0x1UL<<CLK_APBCLK_TMR1_EN_Pos)      /*!<Timer 1 clock enable */
#define CLK_APBCLK_TMR2_EN        (0x1UL<<CLK_APBCLK_TMR2_EN_Pos)      /*!<Timer 2 clock enable */
#define CLK_APBCLK_TMR3_EN        (0x1UL<<CLK_APBCLK_TMR3_EN_Pos)      /*!<Timer 3 clock enable */
#define CLK_APBCLK_FDIV_EN        (0x1UL<<CLK_APBCLK_FDIV_EN_Pos)      /*!<Frequency Divider Output clock enable */
#define CLK_APBCLK_SC2_EN         (0x1UL<<CLK_APBCLK_SC2_EN_Pos)       /*!<SmartCard 2 Clock Enable Control */
#define CLK_APBCLK_I2C0_EN        (0x1UL<<CLK_APBCLK_I2C0_EN_Pos)      /*!<I2C 0 clock enable */
#define CLK_APBCLK_I2C1_EN        (0x1UL<<CLK_APBCLK_I2C1_EN_Pos)      /*!<I2C 1 clock enable */
#define CLK_APBCLK_SPI0_EN        (0x1UL<<CLK_APBCLK_SPI0_EN_Pos)      /*!<SPI 0 clock enable */
#define CLK_APBCLK_SPI1_EN        (0x1UL<<CLK_APBCLK_SPI1_EN_Pos)      /*!<SPI 1 clock enable */
#define CLK_APBCLK_SPI2_EN        (0x1UL<<CLK_APBCLK_SPI2_EN_Pos)      /*!<SPI 2 clock enable */
#define CLK_APBCLK_UART0_EN       (0x1UL<<CLK_APBCLK_UART0_EN_Pos)     /*!<UART 0 clock enable */
#define CLK_APBCLK_UART1_EN       (0x1UL<<CLK_APBCLK_UART1_EN_Pos)     /*!<UART 1 clock enable */
#define CLK_APBCLK_PWM0_CH01_EN   (0x1UL<<CLK_APBCLK_PWM0_CH01_EN_Pos) /*!<PWM0 Channel 0 and Channel 1 Clock Enable Control */
#define CLK_APBCLK_PWM0_CH23_EN   (0x1UL<<CLK_APBCLK_PWM0_CH23_EN_Pos) /*!<PWM0 Channel 2 and Channel 3 Clock Enable Control */
#define CLK_APBCLK_PWM1_CH01_EN   (0x1UL<<CLK_APBCLK_PWM1_CH01_EN_Pos) /*!<PWM1 Channel 0 and Channel 1 Clock Enable Control */
#define CLK_APBCLK_PWM1_CH23_EN   (0x1UL<<CLK_APBCLK_PWM1_CH23_EN_Pos) /*!<PWM1 Channel 2 and Channel 3 Clock Enable Control */
#define CLK_APBCLK_DAC_EN         (0x1UL<<CLK_APBCLK_DAC_EN_Pos)       /*!<DAC Clock Enable Control */
#define CLK_APBCLK_LCD_EN         (0x1UL<<CLK_APBCLK_LCD_EN_Pos)       /*!<LCD Clock Enable Control */
#define CLK_APBCLK_USBD_EN        (0x1UL<<CLK_APBCLK_USBD_EN_Pos)      /*!<USB device clock enable */
#define CLK_APBCLK_ADC_EN         (0x1UL<<CLK_APBCLK_ADC_EN_Pos)       /*!<ADC clock enable */
#define CLK_APBCLK_I2S_EN         (0x1UL<<CLK_APBCLK_I2S_EN_Pos)       /*!<I2S clock enable */
#define CLK_APBCLK_SC0_EN         (0x1UL<<CLK_APBCLK_SC0_EN_Pos)       /*!<SmartCard 0 Clock Enable Control */
#define CLK_APBCLK_SC1_EN         (0x1UL<<CLK_APBCLK_SC1_EN_Pos)       /*!<SmartCard 1 Clock Enable Control */

/********************* Bit definition of CLKSTATUS register **********************/
#define CLK_CLKSTATUS_HXT_STB     (0x1UL<<CLK_CLKSTATUS_HXT_STB_Pos)       /*!<External high speed crystal clock source stable flag */
#define CLK_CLKSTATUS_LXT_STB     (0x1UL<<CLK_CLKSTATUS_LXT_STB_Pos)       /*!<External low speed crystal clock source stable flag */
#define CLK_CLKSTATUS_PLL_STB     (0x1UL<<CLK_CLKSTATUS_PLL_STB_Pos)       /*!<Internal PLL clock source stable flag */
#define CLK_CLKSTATUS_LIRC_STB    (0x1UL<<CLK_CLKSTATUS_LIRC_STB_Pos)      /*!<Internal low speed oscillator clock source stable flag */
#define CLK_CLKSTATUS_HIRC_STB    (0x1UL<<CLK_CLKSTATUS_HIRC_STB_Pos)      /*!<Internal high speed oscillator clock source stable flag */
#define CLK_CLKSTATUS_CLK_SW_FAIL (0x1UL<<CLK_CLKSTATUS_CLK_SW_FAIL_Pos)   /*!<Clock switch fail flag */


/********************* Bit definition of CLKSEL0 register **********************/
#define CLK_CLKSEL0_HCLK_S_HXT    (0UL<<CLK_CLKSEL0_HCLK_S_Pos)     /*!<Select HCLK clock source from high speed crystal */
#define CLK_CLKSEL0_HCLK_S_LXT    (1UL<<CLK_CLKSEL0_HCLK_S_Pos)     /*!<Select HCLK clock source from low speed crystal */
#define CLK_CLKSEL0_HCLK_S_PLL    (2UL<<CLK_CLKSEL0_HCLK_S_Pos)     /*!<Select HCLK clock source from PLL */
#define CLK_CLKSEL0_HCLK_S_LIRC   (3UL<<CLK_CLKSEL0_HCLK_S_Pos)     /*!<Select HCLK clock source from low speed oscillator */
#define CLK_CLKSEL0_HCLK_S_HIRC   (7UL<<CLK_CLKSEL0_HCLK_S_Pos)     /*!<Select HCLK clock source from high speed oscillator */

/********************* Bit definition of CLKSEL1 register **********************/
#define CLK_CLKSEL1_LCD_S_LXT     (0x0UL<<CLK_CLKSEL1_LCD_S_Pos)      /*!<Select LCD clock source from low speed crystal */

#define CLK_CLKSEL1_TMR1_S_HXT    (0x0UL<<CLK_CLKSEL1_TMR1_S_Pos)     /*!<Select TMR1 clock source from high speed crystal */
#define CLK_CLKSEL1_TMR1_S_LXT    (0x1UL<<CLK_CLKSEL1_TMR1_S_Pos)     /*!<Select TMR1 clock source from low speed crystal */
#define CLK_CLKSEL1_TMR1_S_LIRC   (0x2UL<<CLK_CLKSEL1_TMR1_S_Pos)     /*!<Select TMR1 clock source from low speed oscillator  */
#define CLK_CLKSEL1_TMR1_S_EXT    (0x3UL<<CLK_CLKSEL1_TMR1_S_Pos)     /*!<Select TMR1 clock source from external trigger */
#define CLK_CLKSEL1_TMR1_S_HIRC   (0x4UL<<CLK_CLKSEL1_TMR1_S_Pos)     /*!<Select TMR1 clock source from high speed oscillator */

#define CLK_CLKSEL1_TMR0_S_HXT    (0x0UL<<CLK_CLKSEL1_TMR0_S_Pos)     /*!<Select TMR0 clock source from high speed crystal */
#define CLK_CLKSEL1_TMR0_S_LXT    (0x1UL<<CLK_CLKSEL1_TMR0_S_Pos)     /*!<Select TMR0 clock source from low speed crystal */
#define CLK_CLKSEL1_TMR0_S_LIRC   (0x2UL<<CLK_CLKSEL1_TMR0_S_Pos)     /*!<Select TMR0 clock source from low speed oscillator */
#define CLK_CLKSEL1_TMR0_S_EXT    (0x3UL<<CLK_CLKSEL1_TMR0_S_Pos)     /*!<Select TMR0 clock source from external trigger */
#define CLK_CLKSEL1_TMR0_S_HIRC   (0x4UL<<CLK_CLKSEL1_TMR0_S_Pos)     /*!<Select TMR0 clock source from high speed oscillator */

#define CLK_CLKSEL1_PWM0_CH01_S_HXT   (0x0UL<<CLK_CLKSEL1_PWM0_CH01_S_Pos)  /*!<Select PWM0_CH01 clock source from high speed crystal */
#define CLK_CLKSEL1_PWM0_CH01_S_LXT   (0x1UL<<CLK_CLKSEL1_PWM0_CH01_S_Pos)  /*!<Select PWM0_CH01 clock source from low speed crystal */
#define CLK_CLKSEL1_PWM0_CH01_S_HCLK  (0x2UL<<CLK_CLKSEL1_PWM0_CH01_S_Pos)  /*!<Select PWM0_CH01 clock source from HCLK */
#define CLK_CLKSEL1_PWM0_CH01_S_HIRC  (0x3UL<<CLK_CLKSEL1_PWM0_CH01_S_Pos)  /*!<Select PWM0_CH01 clock source from high speed oscillator */

#define CLK_CLKSEL1_PWM0_CH23_S_HXT   (0x0UL<<CLK_CLKSEL1_PWM0_CH23_S_Pos)  /*!<Select PWM0_CH23 clock source from high speed crystal */
#define CLK_CLKSEL1_PWM0_CH23_S_LXT   (0x1UL<<CLK_CLKSEL1_PWM0_CH23_S_Pos)  /*!<Select PWM0_CH23 clock source from low speed crystal */
#define CLK_CLKSEL1_PWM0_CH23_S_HCLK  (0x2UL<<CLK_CLKSEL1_PWM0_CH23_S_Pos)  /*!<Select PWM0_CH23 clock source from HCLK */
#define CLK_CLKSEL1_PWM0_CH23_S_HIRC  (0x3UL<<CLK_CLKSEL1_PWM0_CH23_S_Pos)  /*!<Select PWM0_CH23 clock source from high speed oscillator */

#define CLK_CLKSEL1_ADC_S_HXT     (0x0UL<<CLK_CLKSEL1_ADC_S_Pos)      /*!<Select ADC clock source from high speed crystal */
#define CLK_CLKSEL1_ADC_S_LXT     (0x1UL<<CLK_CLKSEL1_ADC_S_Pos)      /*!<Select ADC clock source from low speed crystal */
#define CLK_CLKSEL1_ADC_S_PLL     (0x2UL<<CLK_CLKSEL1_ADC_S_Pos)      /*!<Select ADC clock source from PLL */
#define CLK_CLKSEL1_ADC_S_HIRC    (0x3UL<<CLK_CLKSEL1_ADC_S_Pos)      /*!<Select ADC clock source from high speed oscillator */

#define CLK_CLKSEL1_UART_S_HXT    (0x0UL<<CLK_CLKSEL1_UART_S_Pos)     /*!<Select UART clock source from high speed crystal */
#define CLK_CLKSEL1_UART_S_LXT    (0x1UL<<CLK_CLKSEL1_UART_S_Pos)     /*!<Select UART clock source from low speed crystal */
#define CLK_CLKSEL1_UART_S_PLL    (0x2UL<<CLK_CLKSEL1_UART_S_Pos)     /*!<Select UART clock source from PLL */
#define CLK_CLKSEL1_UART_S_HIRC   (0x3UL<<CLK_CLKSEL1_UART_S_Pos)     /*!<Select UART clock source from high speed oscillator */

/********************* Bit definition of CLKSEL2 register **********************/
#define CLK_CLKSEL2_SPI2_S_PLL    (0x0UL<<CLK_CLKSEL2_SPI2_S_Pos)     /*!<Select SPI2 clock source from PLL */
#define CLK_CLKSEL2_SPI2_S_HCLK   (0x1UL<<CLK_CLKSEL2_SPI2_S_Pos)     /*!<Select SPI2 clock source from HCLK */

#define CLK_CLKSEL2_SPI1_S_PLL    (0x0UL<<CLK_CLKSEL2_SPI1_S_Pos)     /*!<Select SPI1 clock source from PLL */
#define CLK_CLKSEL2_SPI1_S_HCLK   (0x1UL<<CLK_CLKSEL2_SPI1_S_Pos)     /*!<Select SPI1 clock source from HCLK */

#define CLK_CLKSEL2_SPI0_S_PLL    (0x0UL<<CLK_CLKSEL2_SPI0_S_Pos)     /*!<Select SPI0 clock source from PLL */
#define CLK_CLKSEL2_SPI0_S_HCLK   (0x1UL<<CLK_CLKSEL2_SPI0_S_Pos)     /*!<Select SPI0 clock source from HCLK */

#define CLK_CLKSEL2_SC_S_HXT      (0x0UL<<CLK_CLKSEL2_SC_S_Pos)       /*!<Select SmartCard clock source from HXT */
#define CLK_CLKSEL2_SC_S_PLL      (0x1UL<<CLK_CLKSEL2_SC_S_Pos)       /*!<Select smartCard clock source from PLL */
#define CLK_CLKSEL2_SC_S_HIRC     (0x2UL<<CLK_CLKSEL2_SC_S_Pos)       /*!<Select SmartCard clock source from HIRC */

#define CLK_CLKSEL2_I2S_S_HXT      (0x0UL<<CLK_CLKSEL2_I2S_S_Pos)       /*!<Select I2S clock source from HXT */
#define CLK_CLKSEL2_I2S_S_PLL      (0x1UL<<CLK_CLKSEL2_I2S_S_Pos)       /*!<Select I2S clock source from PLL */
#define CLK_CLKSEL2_I2S_S_HIRC     (0x2UL<<CLK_CLKSEL2_I2S_S_Pos)       /*!<Select I2S clock source from HIRC */

#define CLK_CLKSEL2_TMR3_S_HXT    (0x0UL<<CLK_CLKSEL2_TMR3_S_Pos)     /*!<Select TMR3 clock source from high speed crystal */
#define CLK_CLKSEL2_TMR3_S_LXT    (0x1UL<<CLK_CLKSEL2_TMR3_S_Pos)     /*!<Select TMR3 clock source from low speed crystal */
#define CLK_CLKSEL2_TMR3_S_LIRC   (0x2UL<<CLK_CLKSEL2_TMR3_S_Pos)     /*!<Select TMR3 clock source from low speed oscillator  */
#define CLK_CLKSEL2_TMR3_S_EXT    (0x3UL<<CLK_CLKSEL2_TMR3_S_Pos)     /*!<Select TMR3 clock source from external trigger */
#define CLK_CLKSEL2_TMR3_S_HIRC   (0x4UL<<CLK_CLKSEL2_TMR3_S_Pos)     /*!<Select TMR3 clock source from high speed oscillator */

#define CLK_CLKSEL2_TMR2_S_HXT    (0x0UL<<CLK_CLKSEL2_TMR2_S_Pos)     /*!<Select TMR2 clock source from high speed crystal */
#define CLK_CLKSEL2_TMR2_S_LXT    (0x1UL<<CLK_CLKSEL2_TMR2_S_Pos)     /*!<Select TMR2 clock source from low speed crystal */
#define CLK_CLKSEL2_TMR2_S_LIRC   (0x2UL<<CLK_CLKSEL2_TMR2_S_Pos)     /*!<Select TMR2 clock source from low speed oscillator */
#define CLK_CLKSEL2_TMR2_S_EXT    (0x3UL<<CLK_CLKSEL2_TMR2_S_Pos)     /*!<Select TMR2 clock source from external trigger */
#define CLK_CLKSEL2_TMR2_S_HIRC   (0x4UL<<CLK_CLKSEL2_TMR2_S_Pos)     /*!<Select TMR2 clock source from high speed oscillator */

#define CLK_CLKSEL2_PWM1_CH01_S_HXT   (0x0UL<<CLK_CLKSEL2_PWM1_CH01_S_Pos)  /*!<Select PWM1_CH01 clock source from high speed crystal */
#define CLK_CLKSEL2_PWM1_CH01_S_LXT   (0x1UL<<CLK_CLKSEL2_PWM1_CH01_S_Pos)  /*!<Select PWM1_CH01 clock source from low speed crystal */
#define CLK_CLKSEL2_PWM1_CH01_S_HCLK  (0x2UL<<CLK_CLKSEL2_PWM1_CH01_S_Pos)  /*!<Select PWM1_CH01 clock source from HCLK */
#define CLK_CLKSEL2_PWM1_CH01_S_HIRC  (0x3UL<<CLK_CLKSEL2_PWM1_CH01_S_Pos)  /*!<Select PWM1_CH01 clock source from high speed oscillator */

#define CLK_CLKSEL2_PWM1_CH23_S_HXT   (0x0UL<<CLK_CLKSEL2_PWM1_CH23_S_Pos)  /*!<Select PWM1_CH23 clock source from high speed crystal */
#define CLK_CLKSEL2_PWM1_CH23_S_LXT   (0x1UL<<CLK_CLKSEL2_PWM1_CH23_S_Pos)  /*!<Select PWM1_CH23 clock source from low speed crystal */
#define CLK_CLKSEL2_PWM1_CH23_S_HCLK  (0x2UL<<CLK_CLKSEL2_PWM1_CH23_S_Pos)  /*!<Select PWM1_CH23 clock source from HCLK */
#define CLK_CLKSEL2_PWM1_CH23_S_HIRC  (0x3UL<<CLK_CLKSEL2_PWM1_CH23_S_Pos)  /*!<Select PWM1_CH23 clock source from high speed oscillator */

#define CLK_CLKSEL2_FRQDIV_S_HXT      (0x0UL<<CLK_CLKSEL2_FRQDIV_S_Pos)     /*!<Select FRQDIV clock source from HXT */
#define CLK_CLKSEL2_FRQDIV_S_LXT      (0x1UL<<CLK_CLKSEL2_FRQDIV_S_Pos)     /*!<Select FRQDIV clock source from LXT */
#define CLK_CLKSEL2_FRQDIV_S_HCLK     (0x2UL<<CLK_CLKSEL2_FRQDIV_S_Pos)     /*!<Select FRQDIV clock source from HCLK */
#define CLK_CLKSEL2_FRQDIV_S_HIRC     (0x3UL<<CLK_CLKSEL2_FRQDIV_S_Pos)     /*!<Select FRQDIV clock source from HIRC */

/********************* Bit definition of CLKDIV0 register **********************/
#define CLK_HCLK_CLK_DIVIDER(x)     (((x-1)<< CLK_CLKDIV0_HCLK_N_Pos) & CLK_CLKDIV0_HCLK_N_Msk)  /*!< CLKDIV0 Setting for HCLK clock divider. It could be 1~16 */
#define CLK_USB_CLK_DIVIDER(x)      (((x-1)<< CLK_CLKDIV0_USB_N_Pos) & CLK_CLKDIV0_USB_N_Msk)    /*!< CLKDIV0 Setting for HCLK clock divider. It could be 1~16 */
#define CLK_UART_CLK_DIVIDER(x)     (((x-1)<< CLK_CLKDIV0_UART_N_Pos) & CLK_CLKDIV0_UART_N_Msk)  /*!< CLKDIV0 Setting for UART clock divider. It could be 1~16 */
#define CLK_ADC_CLK_DIVIDER(x)      (((x-1)<< CLK_CLKDIV0_ADC_N_Pos)  & CLK_CLKDIV0_ADC_N_Msk)   /*!< CLKDIV0 Setting for ADC clock divider. It could be 1~256 */
#define CLK_SC0_CLK_DIVIDER(x)      (((x-1)<< CLK_CLKDIV0_SC0_N_Pos)  & CLK_CLKDIV0_SC0_N_Msk)   /*!< CLKDIV0 Setting for SmartCard0 clock divider. It could be 1~16 */
#define CLK_I2S_CLK_DIVIDER(x)      (((x-1)<< CLK_CLKDIV0_I2S_N_Pos)  & CLK_CLKDIV0_I2S_N_Msk)   /*!< CLKDIV0 Setting for I2S clock divider. It could be 1~16 */

/********************* Bit definition of CLKDIV1 register **********************/
#define CLK_SC2_CLK_DIVIDER(x)      (((x-1)<< CLK_CLKDIV1_SC2_N_Pos ) & CLK_CLKDIV1_SC2_N_Msk)   /*!< CLKDIV1 Setting for SmartCard2 clock divider. It could be 1~16 */
#define CLK_SC1_CLK_DIVIDER(x)      (((x-1)<< CLK_CLKDIV1_SC1_N_Pos ) & CLK_CLKDIV1_SC1_N_Msk)   /*!< CLKDIV1 Setting for SmartCard1 clock divider. It could be 1~16 */

/********************* Bit definition of SysTick register **********************/
#define CLK_CLKSEL0_STCLKSEL_HCLK         (1)     /*!< Setting systick clock source as external HCLK */ 
#define CLK_CLKSEL0_STCLKSEL_HCLK_DIV8    (2)     /*!< Setting systick clock source as external HCLK/8 */ 

/********************* Bit definition of PLLCTL register **********************/
#define CLK_PLLCTL_OUT_DV         (0x1UL<<CLK_PLLCTL_OUT_DV_Pos)     /*!<PLL Output Divider Control */
#define CLK_PLLCTL_PD             (0x1UL<<CLK_PLLCTL_PD_Pos)         /*!<PLL Power down mode */
#define CLK_PLLCTL_PLL_SRC_HIRC   (0x1UL<<CLK_PLLCTL_PLL_SRC_Pos)    /*!<PLL clock source from high speed oscillator */
#define CLK_PLLCTL_PLL_SRC_HXT    (0x0UL<<CLK_PLLCTL_PLL_SRC_Pos)    /*!<PLL clock source from high speed crystal */

#define CLK_PLLCTL_NR_2        0x000         /*!< For PLL input divider is  2 */
#define CLK_PLLCTL_NR_4        0x100         /*!< For PLL input divider is  4 */
#define CLK_PLLCTL_NR_8        0x200         /*!< For PLL input divider is  8 */
#define CLK_PLLCTL_NR_16       0x300         /*!< For PLL input divider is 16 */
#define CLK_PLLCON_NF(x)      ((x)-32)       /*!< x must be constant and 32 <= x <= 95.) */

#define CLK_PLLCON_NO_1        0x0000UL      /*!< For PLL output divider is 1 */
#define CLK_PLLCON_NO_2        0x1000UL      /*!< For PLL output divider is 2 */

#if (__HXT == 12000000)
#define CLK_PLLCTL_120MHz_HXT  (CLK_PLLCTL_PLL_SRC_HXT  | CLK_PLLCON_NO_1 | CLK_PLLCTL_NR_4 | CLK_PLLCON_NF(40) ) /*!< Predefined PLLCTL setting for 120MHz PLL output with 12MHz X'tal */
#define CLK_PLLCTL_96MHz_HXT   (CLK_PLLCTL_PLL_SRC_HXT  | CLK_PLLCON_NO_1 | CLK_PLLCTL_NR_8 | CLK_PLLCON_NF(64) ) /*!< Predefined PLLCTL setting for  96MHz PLL output with 12MHz X'tal */
#define CLK_PLLCTL_48MHz_HXT   (CLK_PLLCTL_PLL_SRC_HXT  | CLK_PLLCON_NO_1 | CLK_PLLCTL_NR_16| CLK_PLLCON_NF(64) ) /*!< Predefined PLLCTL setting for  48MHz PLL output with 12MHz X'tal */
#define CLK_PLLCTL_84MHz_HXT   (CLK_PLLCTL_PLL_SRC_HXT  | CLK_PLLCON_NO_1 | CLK_PLLCTL_NR_8 | CLK_PLLCON_NF(56) ) /*!< Predefined PLLCTL setting for  84MHz PLL output with 12MHz X'tal */
#define CLK_PLLCTL_42MHz_HXT   (CLK_PLLCTL_PLL_SRC_HXT  | CLK_PLLCON_NO_1 | CLK_PLLCTL_NR_16| CLK_PLLCON_NF(56) ) /*!< Predefined PLLCTL setting for  42MHz PLL output with 12MHz X'tal */
#else
# error "The PLL pre-definitions are only valid when external crystal is 12MHz"
#endif
#define CLK_PLLCTL_120MHz_HIRC (CLK_PLLCTL_PLL_SRC_HIRC | CLK_PLLCON_NO_1 | CLK_PLLCTL_NR_4 | CLK_PLLCON_NF(40) ) /*!< Predefined PLLCTL setting for 120MHz PLL output with 12MHz IRC */
#define CLK_PLLCTL_96MHz_HIRC  (CLK_PLLCTL_PLL_SRC_HIRC | CLK_PLLCON_NO_1 | CLK_PLLCTL_NR_8 | CLK_PLLCON_NF(64) ) /*!< Predefined PLLCTL setting for  96MHz PLL output with 12MHz IRC */
#define CLK_PLLCTL_48MHz_HIRC  (CLK_PLLCTL_PLL_SRC_HIRC | CLK_PLLCON_NO_1 | CLK_PLLCTL_NR_16| CLK_PLLCON_NF(64) ) /*!< Predefined PLLCTL setting for  48MHz PLL output with 12MHz IRC */
#define CLK_PLLCTL_84MHz_HIRC  (CLK_PLLCTL_PLL_SRC_HIRC | CLK_PLLCON_NO_1 | CLK_PLLCTL_NR_8 | CLK_PLLCON_NF(56) ) /*!< Predefined PLLCTL setting for  84MHz PLL output with 12MHz IRC */
#define CLK_PLLCTL_42MHz_HIRC  (CLK_PLLCTL_PLL_SRC_HIRC | CLK_PLLCON_NO_1 | CLK_PLLCTL_NR_16| CLK_PLLCON_NF(56) ) /*!< Predefined PLLCTL setting for  42MHz PLL output with 12MHz IRC */

/********************* Bit definition of FRQDIV register **********************/
#define CLK_FRQDIV_EN         (0x1UL<<CLK_FRQDIV_FDIV_EN_Pos)        /*!<Frequency divider enable bit */

/********************* Bit definition of WK_INTSTS register **********************/
#define CLK_WK_INTSTS_IS      (0x1UL<<CLK_WK_INTSTS_PD_WK_IS_Pos)     /*!<Wake-up Interrupt Status in chip Power-down Mode */

/********************* Bit definition of MCLKO register **********************/
#define CLK_MCLKO_MCLK_SEL_ISP_CLK 	(0x00<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output ISP_CLK */
#define CLK_MCLKO_MCLK_SEL_HIRC			(0x01<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output HIRC clock */
#define CLK_MCLKO_MCLK_SEL_HXT			(0x02<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output HXT clock */
#define CLK_MCLKO_MCLK_SEL_LXT			(0x03<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output LXT clock */
#define CLK_MCLKO_MCLK_SEL_LIRC			(0x04<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output LIRC clock */
#define CLK_MCLKO_MCLK_SEL_PLLO			(0x05<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output PLL input */
#define CLK_MCLKO_MCLK_SEL_PLLI			(0x06<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output PLL input */
#define CLK_MCLKO_MCLK_SEL_SYSTICK	(0x07<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output system tick */
#define CLK_MCLKO_MCLK_SEL_HCLK			(0x08<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output HCLK clock */
#define CLK_MCLKO_MCLK_SEL_PCLK			(0x0A<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output PCLK clock */
#define CLK_MCLKO_MCLK_SEL_TMR0			(0x20<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output TMR0 clock */
#define CLK_MCLKO_MCLK_SEL_TMR1			(0x21<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output TMR1 clock */
#define CLK_MCLKO_MCLK_SEL_UART0		(0x22<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output UART0 clock */
#define CLK_MCLKO_MCLK_SEL_USB			(0x23<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output USB clock */
#define CLK_MCLKO_MCLK_SEL_ADC 			(0x24<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output ADC clock */
#define CLK_MCLKO_MCLK_SEL_WDT 			(0x25<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output WDT clock */
#define CLK_MCLKO_MCLK_SEL_PWM0CH01	(0x26<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output PWM0CH01 clock */
#define CLK_MCLKO_MCLK_SEL_PWM0CH23	(0x27<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output PWM0CH23 clock */
#define CLK_MCLKO_MCLK_SEL_LCD			(0x29<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output LCD clock */
#define CLK_MCLKO_MCLK_SEL_TMR2			(0x38<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output TMR2 clock */
#define CLK_MCLKO_MCLK_SEL_TMR3			(0x39<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output TMR3 clock */
#define CLK_MCLKO_MCLK_SEL_UART1		(0x3A<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output UART1 clock */
#define CLK_MCLKO_MCLK_SEL_PWM1CH01	(0x3B<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output PWM1CH01 clock */
#define CLK_MCLKO_MCLK_SEL_PWM1CH23	(0x3C<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output PWM1CH23 clock */
#define CLK_MCLKO_MCLK_SEL_I2S			(0x3D<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output I2S clock */
#define CLK_MCLKO_MCLK_SEL_SC0			(0x3E<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output SC0 clock */
#define CLK_MCLKO_MCLK_SEL_SC1			(0x3F<<CLK_MCLKO_MCLK_SEL_Pos) /*!<Select MCLK clock output SC1 clock */


/*---------------------------------------------------------------------------------------------------------*/
/*  MODULE constant definitions.                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define MODULE_APBCLK(x)                   ((x >>31) & 0x1)    /*!< Calculate APBCLK offset on MODULE index */
#define MODULE_CLKSEL(x)                   ((x >>29) & 0x3)    /*!< Calculate CLKSEL offset on MODULE index */
#define MODULE_CLKSEL_Msk(x)               ((x >>25) & 0xf)    /*!< Calculate CLKSEL mask offset on MODULE index */
#define MODULE_CLKSEL_Pos(x)               ((x >>20) & 0x1f)   /*!< Calculate CLKSEL position offset on MODULE index */
#define MODULE_CLKDIV(x)                   ((x >>18) & 0x3)    /*!< Calculate APBCLK CLKDIV on MODULE index */
#define MODULE_CLKDIV_Msk(x)               ((x >>10) & 0xff)   /*!< Calculate CLKDIV mask offset on MODULE index */
#define MODULE_CLKDIV_Pos(x)               ((x >>5 ) & 0x1f)   /*!< Calculate CLKDIV position offset on MODULE index */
#define MODULE_IP_EN_Pos(x)                ((x >>0 ) & 0x1f)   /*!< Calculate APBCLK offset on MODULE index */
#define MODULE_NoMsk                       0x0                 /*!< Not mask on MODULE index */
#define NA                                 MODULE_NoMsk        /*!< Not Available */

#define MODULE_APBCLK_ENC(x)        (((x) & 0x01) << 31)   /*!< MODULE index, 0x0:AHBCLK, 0x1:APBCLK */
#define MODULE_CLKSEL_ENC(x)        (((x) & 0x03) << 29)   /*!< CLKSEL offset on MODULE index, 0x0:CLKSEL0, 0x1:CLKSEL1 0x3 CLKSEL2*/
#define MODULE_CLKSEL_Msk_ENC(x)    (((x) & 0x0f) << 25)   /*!< CLKSEL mask offset on MODULE index */
#define MODULE_CLKSEL_Pos_ENC(x)    (((x) & 0x1f) << 20)   /*!< CLKSEL position offset on MODULE index */
#define MODULE_CLKDIV_ENC(x)        (((x) & 0x03) << 18)   /*!< APBCLK CLKDIV on MODULE index, 0x0:CLKDIV */
#define MODULE_CLKDIV_Msk_ENC(x)    (((x) & 0xff) << 10)   /*!< CLKDIV mask offset on MODULE index */
#define MODULE_CLKDIV_Pos_ENC(x)    (((x) & 0x1f) <<  5)   /*!< CLKDIV position offset on MODULE index */
#define MODULE_IP_EN_Pos_ENC(x)     (((x) & 0x1f) <<  0)   /*!< APBCLK offset on MODULE index */
/*-------------------------------------------------------------------------------------------------------------------------------*/
/*   APBCLK(1) | CLKSEL(2) | CLKSEL_Msk(4) |  CLKSEL_Pos(5) | CLKDIV(2) | CLKDIV_Msk(8) |  CLKDIV_Pos(5)  |  IP_EN_Pos(5)        */
/*-------------------------------------------------------------------------------------------------------------------------------*/
#define TICK_MODULE      ((0UL<<31)|(3<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(1<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_AHBCLK_TICK_EN_Pos     ) /*!< TICK Module */
#define SRAM_MODULE      ((0UL<<31)|(3<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(1<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_AHBCLK_SRAM_EN_Pos     ) /*!< SRAM Module */
#define EBI_MODULE       ((0UL<<31)|(3<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(1<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_AHBCLK_EBI_EN_Pos      ) /*!< EBI Module */
#define ISP_MODULE       ((0UL<<31)|(3<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(1<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_AHBCLK_ISP_EN_Pos      ) /*!< ISP Module */
#define DMA_MODULE       ((0UL<<31)|(3<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_AHBCLK_DMA_EN_Pos      ) /*!< DMA Module */
#define GPIO_MODULE      ((0UL<<31)|(3<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_AHBCLK_GPIO_EN_Pos     ) /*!< GPIO Module */

#define SC2_MODULE       ((1UL<<31)|(2<<29)|(3<<25)           |(18<<20)|(1<<18)|(0xF<<10)         |( 4<<5)|CLK_APBCLK_SC2_EN_Pos      ) /*!< SmartCard2 Module */
#define SC1_MODULE       ((1UL<<31)|(2<<29)|(3<<25)           |(18<<20)|(1<<18)|(0xF<<10)         |( 0<<5)|CLK_APBCLK_SC1_EN_Pos      ) /*!< SmartCard1 Module */
#define SC0_MODULE       ((1UL<<31)|(2<<29)|(3<<25)           |(18<<20)|(0<<18)|(0xF<<10)         |(28<<5)|CLK_APBCLK_SC0_EN_Pos      ) /*!< SmartCard0 Module */
#define I2S_MODULE       ((1UL<<31)|(2<<29)|(3<<25)           |(16<<20)|(0<<18)|(0xF<<10)         |(12<<5)|CLK_APBCLK_I2S_EN_Pos      ) /*!< I2S Module */
#define ADC_MODULE       ((1UL<<31)|(1<<29)|(3<<25)           |( 2<<20)|(0<<18)|(0xFF<<10)        |(16<<5)|CLK_APBCLK_ADC_EN_Pos      ) /*!< ADC Module */
#define USBD_MODULE      ((1UL<<31)|(1<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(0<<18)|(0xF<<10)         |( 4<<5)|CLK_APBCLK_USBD_EN_Pos     ) /*!< USBD Module */
#define PWM1_CH23_MODULE ((1UL<<31)|(2<<29)|(3<<25)           |( 6<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_PWM1_CH23_EN_Pos) /*!< PWM1 Channel2 and Channel3 Module */
#define PWM1_CH01_MODULE ((1UL<<31)|(2<<29)|(3<<25)           |( 4<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_PWM1_CH01_EN_Pos) /*!< PWM1 Channel0 and Channel1 Module */
#define PWM0_CH23_MODULE ((1UL<<31)|(1<<29)|(3<<25)           |( 6<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_PWM0_CH23_EN_Pos) /*!< PWM0 Channel2 and Channel3 Module */
#define PWM0_CH01_MODULE ((1UL<<31)|(1<<29)|(3<<25)           |( 4<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_PWM0_CH01_EN_Pos) /*!< PWM0 Channel0 and Channel1 Module */
#define UART1_MODULE     ((1UL<<31)|(1<<29)|(3<<25)           |( 0<<20)|(0<<18)|(0xF<<10)         |( 8<<5)|CLK_APBCLK_UART1_EN_Pos    ) /*!< UART1 Module */
#define UART0_MODULE     ((1UL<<31)|(1<<29)|(3<<25)           |( 0<<20)|(0<<18)|(0xF<<10)         |( 8<<5)|CLK_APBCLK_UART0_EN_Pos    ) /*!< UART0 Module */
#define SPI2_MODULE      ((1UL<<31)|(2<<29)|(1<<25)           |(22<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_SPI2_EN_Pos     ) /*!< SPI0 Module */
#define SPI1_MODULE      ((1UL<<31)|(2<<29)|(1<<25)           |(21<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_SPI1_EN_Pos     ) /*!< SPI1 Module */
#define SPI0_MODULE      ((1UL<<31)|(2<<29)|(1<<25)           |(20<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_SPI0_EN_Pos     ) /*!< SPI0 Module */
#define I2C1_MODULE      ((1UL<<31)|(0<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_I2C1_EN_Pos     ) /*!< I2C1 Module */
#define I2C0_MODULE      ((1UL<<31)|(0<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_I2C0_EN_Pos     ) /*!< I2C0 Module */
#define FDIV_MODULE      ((1UL<<31)|(2<<29)|(3<<25)           |( 2<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_FDIV_EN_Pos     ) /*!< Frequency Divider0 Output Module */
#define TMR3_MODULE      ((1UL<<31)|(2<<29)|(7<<25)           |(12<<20)|(1<<18)|(0xF<<10)         |(20<<5)|CLK_APBCLK_TMR3_EN_Pos     ) /*!< Timer3 Module */
#define TMR2_MODULE      ((1UL<<31)|(2<<29)|(7<<25)           |( 8<<20)|(1<<18)|(0xF<<10)         |(16<<5)|CLK_APBCLK_TMR2_EN_Pos     ) /*!< Timer2 Module */
#define TMR1_MODULE      ((1UL<<31)|(1<<29)|(7<<25)           |(12<<20)|(1<<18)|(0xF<<10)         |(12<<5)|CLK_APBCLK_TMR1_EN_Pos     ) /*!< Timer1 Module */
#define TMR0_MODULE      ((1UL<<31)|(1<<29)|(7<<25)           |( 8<<20)|(1<<18)|(0xF<<10)         |( 8<<5)|CLK_APBCLK_TMR0_EN_Pos     ) /*!< Timer0 Module */
#define RTC_MODULE       ((1UL<<31)|(3<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_RTC_EN_Pos      ) /*!< Real-Time-Clock Module */
#define WDT_MODULE       ((1UL<<31)|(3<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_WDT_EN_Pos      ) /*!< Watchdog Timer Module */
#define LCD_MODULE       ((1UL<<31)|(1<<29)|(1<<25)           |(18<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_LCD_EN_Pos      ) /*!< LCD Module */
#define DAC_MODULE       ((1UL<<31)|(3<<29)|(MODULE_NoMsk<<25)|( 0<<20)|(0<<18)|(MODULE_NoMsk<<10)|( 0<<5)|CLK_APBCLK_DAC_EN_Pos      ) /*!< DAC Module */
/*@}*/ /* end of group NANO100_CLK_EXPORTED_CONSTANTS */


/** @addtogroup NANO100_CLK_EXPORTED_FUNCTIONS CLK Exported Functions
  @{
*/
void CLK_DisableCKO(void);
void CLK_EnableCKO(uint32_t u32ClkSrc, uint32_t u32ClkDiv);
void CLK_PowerDown(void);
void CLK_Idle(void);
uint32_t CLK_GetHXTFreq(void);
uint32_t CLK_GetLXTFreq(void);
uint32_t CLK_GetHCLKFreq(void);
uint32_t CLK_GetCPUFreq(void);
uint32_t CLK_GetPLLClockFreq(void);
uint32_t CLK_SetCoreClock(uint32_t u32Hclk);
void CLK_SetHCLK(uint32_t u32ClkSrc, uint32_t u32ClkDiv);
void CLK_SetModuleClock(uint32_t u32ModuleIdx, uint32_t u32ClkSrc, uint32_t u32ClkDiv);
void CLK_EnableXtalRC(uint32_t u32ClkMask);
void CLK_DisableXtalRC(uint32_t u32ClkMask);
void CLK_EnableModuleClock(uint32_t u32ModuleIdx);
void CLK_DisableModuleClock(uint32_t u32ModuleIdx);
uint32_t CLK_EnablePLL(uint32_t u32PllClkSrc, uint32_t u32PllFreq);
void CLK_DisablePLL(void);
void CLK_SysTickDelay(uint32_t us);
void CLK_EnableSysTick(uint32_t u32ClkSrc, uint32_t u32Count);
void CLK_DisableSysTick(void);
uint32_t CLK_WaitClockReady(uint32_t u32ClkMask);

/*@}*/ /* end of group NANO100_CLK_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_CLK_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__CLK_H__

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
