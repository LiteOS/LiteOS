/*
 *****************************************************************************
   @file:    adi_pwr_config.h
   @brief:   Configuration options for PWR driver.
             This is specific to the PWR driver and will be included by the source file.
             It is not required for the application to include this header file.
  -----------------------------------------------------------------------------

Copyright (c) 2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifndef ADI_PWR_CONFIG_H
#define ADI_PWR_CONFIG_H
#include <adi_global_config.h>
#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions.
*
* Pm009 (rule 5.1): identifiers shall not rely on significance of more than 31 characters.
*   The YODA-generated headers rely on more. The IAR compiler supports that.
*/
#pragma diag_suppress=Pm009
#endif /* __ICCARM__ */

/** @addtogroup PWR_Driver_Config Static Configuration
 *  @ingroup Power_Driver
 *  @{
 */

/*! Enable the code to support input clock through the GPIO pin
    0 - No support for input clock through the GPIO pin.
    1 - Support for input clock through the GPIO pin.

*/
#define ADI_PWR_CFG_ENABLE_CLOCK_SOURCE_GPIO                    0

/*-------------------------------------------------------------------------------
           Set of MACROs for configuring the clock
--------------------------------------------------------------------------------*/
/* Oscillator Control Register */

/*!
    32 KHz clock select mux.  This clock connects to beeper, RTC.\n
   0 -  Internal 32 KHz oscillator is selected.\n
   1 -  External 32 KHz crystal is selected..
*/
#define ADI_PWR_LF_CLOCK_MUX                                    0


/*!
    High frequency internal oscillator enable\n
   0 -  The HFOSC oscillator is disabled and placed in a low power state\n
   1 -  The HFOSC oscillator is enabled.
*/
#define ADI_PWR_HFOSC_CLOCK_ENABLE                              1

/*!
   Low  frequency external oscillator enable and placed in a low power state\n
   0 -  The LFXTAL oscillator is disabled\n
   1 -  The LFXTAL oscillator is enabled.
*/
#define ADI_PWR_LFXTAL_CLOCK_ENABLE                             0

/*!
    High frequency external  oscillator enable\n
   0 -  The HFXTAL  oscillator is disabled and placed in a low power state\n
   1 -  The HFXTAL  oscillator is enabled.
*/
#define ADI_PWR_HFXTAL_CLOCK_ENABLE                              0

/*!
   Low frequency external clock fail interrupt enable \n
   0 -  The LFXTAL clock monitor and clock fail interrupt disabled \n
   1 -  The LFXTAL clock monitor and clock fail interrupt enabled.
*/
#define ADI_PWR_LFXTAL_CLOCK_MON_ENABLE                         0

/*!
   Automatic switching of the LF Mux to LF Oscillator on LFXTAL failure.  \n
   0 -  Disables Automatic switching of LF Mux to LF Oscillator on LFXTAL failure \n
   1 -  Disables Automatic switching of LF Mux to LF Oscillator on LFXTAL failure.
*/
#define ADI_PWR_LFXTAL_FAIL_AUTO_SWITCH_ENABLE                  0

/*!
   Low frequency crystal Robust mode enable. The Robust mode enables the LFXTAL oscillator to work also when an
   additional resistive load is placed between the crystal pins and GND. \n
   0 -  Selects Normal mode \n
   1 -  Selects Robust mode
*/
#define ADI_PWR_LFXTAL_ROBUST_MODE_ENABLE                       0

/*!
   Low frequency crystal Robust mode load select. The amount of loading tolerated when robust mode is enabled. \n
   0 -  No Trim, and big resistive loads not tolerated. \n
   1 -  20 Mohm load mode, greater than 20 Mohm load allowed. \n
   2 -  10 Mohm load mode, greater than 10 Mohm load allowed. \n
   3 -  5 Mohm load mode, 5 Mohm load allowed on both IO pins.
*/
#define ADI_PWR_LFXTAL_ROBUST_LOAD_SELECT                       0


/*!
   Root clock monitor and Clock Fail interrupt enable.
   0 - Disable Root Clock Monitor and Clock Fail interrupt.
   1 - Enable Root Clock Monitor and Clock Fail interrupt.
*/
#define ADI_PWR_ROOT_CLOCK_MON_INT_ENABLE                       0


/*!
   Enable Auto switch to High Frequency Oscillator (HFOSC) when Root Clock Fails.
   0 - Disable Automatic switching of the Root Clock.
   1 - Enable Automatic switching of the Root Clock.
*/
#define ADI_PWR_ROOT_CLOCK_FAIL_AUTOSWITCH_ENABLE               0


/********** Miscellaneous clock setting register CTL0 *************/

/*!
    Selecting the input clock for  Root Clock mux.  Determines which single shared clock source
    is used by the PCLK,  and HCLK dividers. \n
    0 -  HFOSC High frequency internal oscillator \n
    1 -  HFXTAL High frequency external oscillator\n
    2 -  SPLL  Output of System PLL is selected\n
    3 -  External GPIO port is selected
*/
#define ADI_PWR_INPUT_TO_ROOT_CLOCK_MUX                         0

/*!
    GPIO clock out select. Selects the clock to be routed to the GPIO clock out pin. \n
    0  -  Root Clock (ROOT_CLK)\n
    1  -  Low Frequency Clock (LF_CLK) \n
    2  -  ADC Clock (ACLK) \n
    3  -  HCLK_BUS \n
    4  -  HCLK_CORE \n
    5  -  Peripheral Clock (PCLK)
    6  -  Reference Clock for Flash controller timer (RCLK)\n
    7  -  Mux of HFOSC, HFXTAL clock (RHP_CLK)\n
    8  -  GP Timer 0 clock (GPT0_CLK)\n
    9  -  GP Timer 1 clock (GPT1_CLK)\n
    10 -  Peripherals operating at HCLK (HCLK_P)\n
    11 -  PLL Clock out (PCLK)\n
    12 -  RTC0 Clock \n
    13 -  HP Buck Clock (HPBUCK_CLK)\n
    14 -  HP Buck Non overlap clock\n
    15 -  RTC1 generated clock
*/
#define ADI_PWR_GPIO_CLOCK_OUT_SELECT                           0

/*!
    Flash reference clock and HPBUCK clock source mux.  \n
    0 -  sourcing from HFOSC (High frequency internal oscillator) \n
    2 -  sourcing from external HFXTAL(  High frequency external oscillator 26M Hz )\n
    3 -  sourcing from external HFXTAL(  High frequency external oscillator 16M Hz )

*/
#define ADI_PWR_INPUT_TO_RCLK_MUX                               0

/*!
    Selecting the input clock for the system PLL clock.  \n
    0 -  sourcing from  HFOSC (High frequency internal oscillator) \n
    1 -  sourcing from  HFXTAL(High frequency external oscillator) \n
    2 -  GPIO Input clock. \n
    3 -  GPIO Input clock.
*/
#define ADI_PWR_INPUT_TO_SPLL_MUX                               0

/*!
    External Low frequency crystal interrupt enable.\n
    0 -  Disable the interrupt for LF clock \n
    1 -  Enable the interrupt for LF clock
*/
#define ADI_PWR_LFXTAL_CLOCK_INTERRUPT_ENABLE                   0

/*!
    External Hight frequency crystal interrupt enable.\n
    0 -  Disable the interrupt for HFXTAL clock \n
    1 -  Enable the interrupt for  HFXTAL clock
*/
#define ADI_PWR_HFXTAL_CLOCK_INTERRUPT_ENABLE                    0



/********** Clock divider register CTL1 ***************/

/*!
    HCLK divide count.Determines the HCLK rate based on the following equation: HCLK = ROOT_CLK/HCLKDIVCNT.
    0 - 63 is valid range.
*/
#define ADI_PWR_HCLK_DIVIDE_COUNT                               4

/*!
    PCLK divide count.Determines the PCLK rate based on the following equation: PCLK = ROOT_CLK/PCLKDIVCNT.
    0 - 63 is valid range.
*/
#define ADI_PWR_PCLK_DIVIDE_COUNT                               4

/*!
    ACLK divide count.Determines the ACLK rate based on the following equation: ACLK = ROOT_CLK/ACLKDIVCNT.
    0 - 63 is valid range.
*/
#define ADI_PWR_ACLK_DIVIDE_COUNT                               16


/************* HF Oscillator divide clock select register CTL2 ***********/

/*!
    HF Oscillator auto divide by one clock selection during wakeup from Flexi power mode.

    When enabled enabled (Set to 1), the frequency undivided 26MHz HF oscillator clock itself will be used during the wake up.
    The undivided HFOSC clock is selected automatically by clearing  the HFOSCDIVCLKSEL register content to 0, which selects the HFOSC/1 clock.This updated divided by 1 clock selection will remain same until the new divider value is written to this register.

    When disabled (Set to 0), this fast wake up feature will be disabled and the HFOSCDIVCLKSEL register will remain unchanged
    during the wakeup.

    0 -  Auto select HFOSC/1 clock during wakeup from Flexi mode is disable.
    1 -  Auto select HFOSC/1 clock during wakeup from Flexi mode is enabled.
*/
#define ADI_PWR_HFOSC_AUTO_DIV_BY_1                              0

/*!
    HF Oscillator divide select.
    0 -  HFOSC/1. \n
    1 -  HFOSC/2. \n
    2 -  HFOSC/4. \n
    3 -  HFOSC/8. \n
    4 -  HFOSC/16. \n
    5 -  HFOSC/32.
*/
#define ADI_PWR_HFOSC_DIVIDE_SELECT                              0



/****** System PLL Register CTL3 *****/
/*!
    System PLL N multiplier(SPLL_NSEL).  Sets the N value used to obtain the multiplication
    factor N/M of the PLL.
    8 - 31 is valid range.
*/
#define ADI_PWR_SPLL_MUL_FACTOR                                 26

/*!
    System PLL division by 2.  Controls if an optional divide by two is placed on the PLL output.\n
    0 -  The System PLL is not divided. Its output frequency equals that selected by the N/M ratio \n
    1 -  The System PLL is divided by two. Its output frequency equals that selected by the N/M ratio
         with an additional divide by 2
*/
#define ADI_PWR_SPLL_ENABLE_DIV2                                0

/*!
    System PLL enable.  Controls if the PLL should be enabled or placed in its low power state. \n
    0 - The system PLL is disabled and is in its power down state\n
    1 - The system  PLL is enabled.
*/
#define ADI_PWR_SPLL_ENABLE                                     0

/*!
    System PLL interrupt enable.Controls if the core should be interrupted on a PLL lock/PLL unlock or no interrupt generated.\n
    0 - Disable the SPLL interrupt generation\n
    1 - Enable the SPLL interrupt generation
*/
#define ADI_PWR_SPLL_INTERRUPT_ENABLE                           0

/*!
    System PLL M Divider(SPLL_MSEL).  Sets the M value used to obtain the multiplication
    factor N/M of the PLL.
    2 - 15 is valid range.
*/
#define ADI_PWR_SPLL_DIV_FACTOR                                 13

/*!
    system PLL multiply by 2.  This bit is used to configure if the VCO clock frequency should be multiplied by 2 or 1.\n
    0 -  The System PLL is multiplied by 1.\n
    1 -  The System PLL is multiplied by 2.
*/
#define ADI_PWR_SPLL_ENABLE_MUL2                                0


/********** User Clock Gating Control CTL5 ********************/

/*!
    This can be  used to enable/disable clock to GPT0.  \n
    0 - Disable  the clock to GPT0\n
    1 - Enable the clock to GPT0
*/
#define ADI_PWR_GPT0_CLOCK_ENABLE                               1

/*!
    This can be  used to enable/disable clock to GPT1.  \n
    0 - Disable  the clock to GPT1\n
    1 - Enable the clock to   GPT1
*/
#define ADI_PWR_GPT1_CLOCK_ENABLE                               1
/*!
    This can be  used to enable/disable clock to GPT2.  \n
    0 - Disable  the clock to GPT2\n
    1 - Enable the clock to   GPT2
*/
#define ADI_PWR_GPT2_CLOCK_ENABLE                               1

/*!
    This can be  used to enable/disable clock to I2C.  \n
    0 - Disable  the clock to I2C\n
    1 - Enable the clock to   I2C
*/
#define ADI_PWR_I2C_CLOCK_ENABLE                                1

/*!
    This can be used to enable/disable clock to GPIO.  \n
    0 - Disable the clock to GPIO\n
    1 - Enable the clock to  GPIO
*/
#define ADI_PWR_GPIO_CLOCK_ENABLE                               1


/*!
    This can be used to enable/disable all clocks connected to peripherals.  \n
    0 - Disable  the Clock supply to  peripherals\n
    1 - Enable the Clock supply to  peripherals
*/
#define ADI_PWR_PCLK_ENABLE                                     0


/*!
    This can be used to enable/disable clocks to Timer RGB.  \n
    0 - Disable the Clock supply to Timer RGB \n
    1 - Enable the Clock supply to Timer RGB
*/
#define ADI_PWR_TIMER_RGB_ENABLE                                 1

/*-------------------------------------------------------------------------------
           Set of macros for configuring the power management module
--------------------------------------------------------------------------------*/

/********* Interrupt enable register IEN ********/

/*!
    Enabling the interrupt if the  Battery  voltage falls below 1.8V.\n
   0 -  Disable Battery voltage interrupt \n
   1 -  Enable Battery voltage interrupt.
*/
#define ADI_PWR_ENABLE_VBAT_INTERRUPT                           0

/*!
    Enabling the interrupt for under VREG voltage (i.e less than 1V).\n
   0 -  Disable VREG under voltage  interrupt \n
   1 -  Enable VREG under voltage  interrupt.
*/
#define ADI_PWR_ENABLE_VREG_UNDER_VOLTAGE_INTERRUPT             0

/*!
    Enabling the interrupt for over  VREG voltage (i.e above  than 1.32V).\n
   0 -  Disable VREG over  voltage  interrupt \n
   1 -  Enable VREG  over voltage  interrupt.
*/
#define ADI_PWR_ENABLE_VREG_OVER_VOLTAGE_INTERRUPT              0

/*!
    Enabling the interrupt for Battery range.\n
   0 -  Disable battery voltage range  interrupt \n
   1 -  Enable battery voltage range  interrupt
*/
#define ADI_PWR_ENABLE_BATTERY_VOLTAGE_RANGE_INTERRUPT          0

/*!
    Battery voltage range for generating the interrupt.\n
   0 - Configure to generate interrupt if VBAT > 2.75V \n
   1 - Configure to generate interrupt if VBAT is between 2.75 and 1.6V  \n
   2 - Configure to generate interrupt if VBAT is between 2.3V and 1.6V
*/
#define ADI_PWR_BATTERY_VOLTAGE_RANGE_FOR_INTERRUPT             0

/********* HP Buck control register CTL1 ********/
/*!
    Enable or disable HP Buck.\n
    0 - Disable HP Buck.
    1 - Enable HP Buck.
*/
#define ADI_PWR_HP_BUCK_ENABLE                                  0

/*!
    HP Buck Load mode.\n
    0 - HP Buck low load mode. Can be set when the system is running at
        less than 26 Mhz. \n
    1 - HP Buck High load mode. Can be set when the system is running at
        more than 26 Mh.
*/
#define ADI_PWR_HP_BUCK_LOAD_MODE                               0

/*!
    HP Buck low power mode.\n
    The HPBUCK Low Power mode can be selected, when the  Chip is in Flexi Power mode
    and low power modules such as Timer, Beeper only are enabled

    0 - HPBUCK Low power mode is disabled. \n
    1 - HPBUCK Low power mode is enabled.
*/
#define ADI_PWR_HP_BUCK_LOW_POWER_MODE                           0


/********* Power mode register ********/

/*!
    Enable or disable monitoring battery voltage (VBAT) during HIBERNATE Mode. \n
    0 - Battery voltage monitoring is enabled.
    1 - Battery voltage monitoring is disabled.

    By default battery voltage monitoring during hibernate is enabled.
*/
#define ADI_PWR_ENABLE_BATTERY_VOLTAGE_MONITORING                0


/*******************************************************************************
                        M A C R O   V A L I D A T I O N
*******************************************************************************/

#if ( ADI_PWR_CFG_ENABLE_CLOCK_SOURCE_GPIO > 1 )
#error "Invalid configuration set for ADI_PWR_CFG_ENABLE_CLOCK_SOURCE_GPIO"
#endif

#if ( ADI_PWR_LF_CLOCK_MUX > 1 )
#error "Invalid configuration set for ADI_PWR_LF_CLOCK_MUX"
#endif

#if ( ADI_PWR_HFOSC_CLOCK_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_HFOSC_CLOCK_ENABLE"
#endif

#if ( ADI_PWR_LFXTAL_CLOCK_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_LFXTAL_CLOCK_ENABLE"
#endif

#if ( ADI_PWR_HFXTAL_CLOCK_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_HFXTAL_CLOCK_ENABLE"
#endif

#if ( ADI_PWR_LFXTAL_CLOCK_MON_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_LFXTAL_CLOCK_MON_ENABLE"
#endif

#if ( ADI_PWR_LFXTAL_FAIL_AUTO_SWITCH_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_LFXTAL_FAIL_AUTO_SWITCH_ENABLE"
#endif

#if ( ADI_PWR_LFXTAL_ROBUST_MODE_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_LFXTAL_ROBUST_MODE_ENABLE"
#endif

#if ( ADI_PWR_LFXTAL_ROBUST_LOAD_SELECT > 3 )
#error "Invalid configuration set for ADI_PWR_LFXTAL_ROBUST_LOAD_SELECT"
#endif

#if ( ADI_PWR_ROOT_CLOCK_MON_INT_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_ROOT_CLOCK_MON_INT_ENABLE"
#endif

#if ( ADI_PWR_ROOT_CLOCK_FAIL_AUTOSWITCH_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_ROOT_CLOCK_FAIL_AUTOSWITCH_ENABLE"
#endif

#if ( ADI_PWR_INPUT_TO_ROOT_CLOCK_MUX > 3 )
#error "Invalid configuration set for ADI_PWR_INPUT_TO_ROOT_CLOCK_MUX"
#endif

#if ( ADI_PWR_GPIO_CLOCK_OUT_SELECT > 15 )
#error "Invalid configuration set for ADI_PWR_GPIO_CLOCK_OUT_SELECT"
#endif

#if ( ADI_PWR_INPUT_TO_RCLK_MUX > 3 )
#error "Invalid configuration set for ADI_PWR_INPUT_TO_RCLK_MUX"
#endif

#if ( ADI_PWR_INPUT_TO_SPLL_MUX > 3 )
#error "Invalid configuration set for ADI_PWR_INPUT_TO_SPLL_MUX"
#endif

#if ( ADI_PWR_LFXTAL_CLOCK_INTERRUPT_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_LFXTAL_CLOCK_INTERRUPT_ENABLE"
#endif

#if ( ADI_PWR_HFXTAL_CLOCK_INTERRUPT_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_HFXTAL_CLOCK_INTERRUPT_ENABLE"
#endif

#if ( ADI_PWR_HCLK_DIVIDE_COUNT > 63 )
#error "Invalid configuration set for ADI_PWR_HCLK_DIVIDE_COUNT"
#endif

#if ( ADI_PWR_PCLK_DIVIDE_COUNT > 63 )
#error "Invalid configuration set for ADI_PWR_PCLK_DIVIDE_COUNT"
#endif

#if ( ADI_PWR_ACLK_DIVIDE_COUNT > 63 )
#error "Invalid configuration set for ADI_PWR_ACLK_DIVIDE_COUNT"
#endif

#if ( ADI_PWR_HFOSC_AUTO_DIV_BY_1 > 1 )
#error "Invalid configuration set for ADI_PWR_HFOSC_AUTO_DIV_BY_1"
#endif

#if ( ADI_PWR_HFOSC_DIVIDE_SELECT > 5 )
#error "Invalid configuration set for ADI_PWR_HFOSC_DIVIDE_SELECT"
#endif

#if ( ADI_PWR_SPLL_MUL_FACTOR < 8 || ADI_PWR_SPLL_MUL_FACTOR > 31 )
#error "Invalid configuration set for ADI_PWR_SPLL_MUL_FACTOR"
#endif

#if ( ADI_PWR_SPLL_ENABLE_DIV2 > 1 )
#error "Invalid configuration set for ADI_PWR_SPLL_ENABLE_DIV2"
#endif

#if ( ADI_PWR_SPLL_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_SPLL_ENABLE"
#endif

#if ( ADI_PWR_SPLL_INTERRUPT_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_SPLL_INTERRUPT_ENABLE"
#endif

#if ( ADI_PWR_SPLL_DIV_FACTOR < 2 || ADI_PWR_SPLL_DIV_FACTOR > 15 )
#error "Invalid configuration set for ADI_PWR_SPLL_DIV_FACTOR"
#endif

#if ( ADI_PWR_SPLL_ENABLE_MUL2 > 1 )
#error "Invalid configuration set for ADI_PWR_SPLL_ENABLE_MUL2"
#endif

#if ( ADI_PWR_GPT0_CLOCK_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_GPT0_CLOCK_ENABLE"
#endif

#if ( ADI_PWR_GPT1_CLOCK_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_GPT1_CLOCK_ENABLE"
#endif

#if ( ADI_PWR_GPT2_CLOCK_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_GPT2_CLOCK_ENABLE"
#endif

#if ( ADI_PWR_I2C_CLOCK_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_I2C_CLOCK_ENABLE"
#endif

#if ( ADI_PWR_GPIO_CLOCK_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_GPIO_CLOCK_ENABLE"
#endif

#if ( ADI_PWR_PCLK_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_PCLK_ENABLE"
#endif

#if ( ADI_PWR_TIMER_RGB_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_TIMER_RGB_ENABLE"
#endif

#if ( ADI_PWR_ENABLE_VBAT_INTERRUPT > 1 )
#error "Invalid configuration set for ADI_PWR_ENABLE_VBAT_INTERRUPT"
#endif

#if ( ADI_PWR_ENABLE_VREG_UNDER_VOLTAGE_INTERRUPT > 1 )
#error "Invalid configuration set for ADI_PWR_ENABLE_VREG_UNDER_VOLTAGE_INTERRUPT"
#endif

#if ( ADI_PWR_ENABLE_VREG_OVER_VOLTAGE_INTERRUPT > 1 )
#error "Invalid configuration set for ADI_PWR_ENABLE_VREG_OVER_VOLTAGE_INTERRUPT"
#endif

#if ( ADI_PWR_ENABLE_BATTERY_VOLTAGE_RANGE_INTERRUPT > 1 )
#error "Invalid configuration set for ADI_PWR_ENABLE_BATTERY_VOLTAGE_RANGE_INTERRUPT"
#endif

#if ( ADI_PWR_BATTERY_VOLTAGE_RANGE_FOR_INTERRUPT > 2 )
#error "Invalid configuration set for ADI_PWR_BATTERY_VOLTAGE_RANGE_FOR_INTERRUPT"
#endif

#if ( ADI_PWR_HP_BUCK_ENABLE > 1 )
#error "Invalid configuration set for ADI_PWR_HP_BUCK_ENABLE"
#endif

#if ( ADI_PWR_HP_BUCK_LOAD_MODE > 1 )
#error "Invalid configuration set for ADI_PWR_HP_BUCK_LOAD_MODE"
#endif

#if ( ADI_PWR_HP_BUCK_LOW_POWER_MODE > 1 )
#error "Invalid configuration set for ADI_PWR_HP_BUCK_LOW_POWER_MODE"
#endif

#if ( ADI_PWR_ENABLE_BATTERY_VOLTAGE_MONITORING > 1 )
#error "Invalid configuration set for ADI_PWR_ENABLE_BATTERY_VOLTAGE_MONITORING"
#endif



/*! @} */

#ifdef __ICCARM__
#pragma diag_default=Pm009
#endif /* __ICCARM__ */

#endif /* ADI_PWR_CONFIG_H */
