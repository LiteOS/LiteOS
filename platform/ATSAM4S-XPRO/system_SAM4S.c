/**************************************************************************//**
 * @file     system_SAM4S.c
 * @brief    CMSIS Cortex-M4 Device System Source File
 *           for Atmel SAM4S Device Series
 * @version  V1.10
 * @date     05. March 2013
 *
 * @note
 * Copyright (C) 2012-2013 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#include <stdint.h>
#include <SAM4S.h>

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

/*--------------------- Embedded Flash Controllers Configuration -------------
//
// <e0> Embedded Flash Controller  (EEFC)
//   <o1.8..11>  FWS: Flash Wait State <1-16><#-1>
//   <o1.24>     FAM: Flash Access Mode
//                 <0=> 128-bit in read mode (enhance speed)
//                 <1=> 64-bit in read mode (enhance power consumption)
// </e0>
*/
#define EEFC_SETUP      1               // Reset values:
#define EEFC_FMR_Val    0x00000300      // 0x00000000


/*--------------------- Power Management Controller Configuration ------------
//
// <e> Power Management Controller (PMC)
//   <h> System Clock Enable Register (PMC_SCER)
//     <o1.7>      UDP:  USB Device Port Clock Enable
//     <o1.8>      PCK0: Programmable Clock 0 Output Enable
//     <o1.9>      PCK1: Programmable Clock 1 Output Enable
//     <o1.10>     PCK2: Programmable Clock 2 Output Enable
//   </h>
//
//   <h> Peripheral Clock Enable Register 0 (PMC_PCER0)
//     <o2.2>      PID2:  Real Time Clock Clock Enable
//     <o2.3>      PID3:  Real Time Timer Clock Enable
//     <o2.4>      PID4:  Watchdog Timer Clock Enable
//     <o2.5>      PID5:  Power Management Controller Clock Enable
//     <o2.6>      PID6:  Enhanced Embedded Flash Controller Clock Enable
//     <o2.8>      PID8:  UART 0 Clock Enable
//     <o2.9>      PID9:  UART 1 Clock Enable Clock Enable
//     <o2.10>     PID10: Static Memory Controller Clock Enable
//     <o2.11>     PID11: Parallel I/O Controller A Clock Enable
//     <o2.12>     PID12: Parallel I/O Controller B Clock Enable
//     <o2.13>     PID13: Parallel I/O Controller C Clock Enable
//     <o2.14>     PID14: USART 0 Clock Enable
//     <o2.15>     PID15: USART 1 Clock Enable
//     <o2.18>     PID18: Multimedia Card Interface Clock Enable
//     <o2.19>     PID19: Two-wire Interface 0 Clock Enable
//     <o2.20>     PID20: Two-wire Interface 1 Clock Enable
//     <o2.21>     PID21: Serial Peripheral Interface Clock Enable
//     <o2.22>     PID22: Synchronous Serial Controller Clock Enable
//     <o2.23>     PID23: Timer Counter 0 Clock Enable
//     <o2.24>     PID24: Timer Counter 1 Clock Enable
//     <o2.25>     PID25: Timer Counter 2 Clock Enable
//     <o2.26>     PID26: Timer Counter 3 Clock Enable
//     <o2.27>     PID27: Timer Counter 4 Clock Enable
//     <o2.28>     PID28: Timer Counter 5 Clock Enable
//     <o2.29>     PID29: Analog-to-Digital Converter Clock Enable
//     <o2.30>     PID30: Digital-to-Analog Converter Clock Enable
//     <o2.31>     PID31: Pulse Width Modulation Clock Enable
//   </h>
//
//   <h> Peripheral Clock Enable Register 1 (PMC_PCER1)
//     <o3.0>      PID32: CRC Calculation Unit Clock Enable
//     <o3.1>      PID33: Analog Comparator Clock Enable
//     <o3.2>      PID34: USB Device Port Clock Enable
//   </h>
//
//   <h> Main Oscillator Register (CKGR_MOR)
//     <o4.0>      MOSCXTEN: Main Crystal Oscillator Enable
//     <o4.1>      MOSCXTBY: Main Crystal Oscillator Bypass
//     <o4.3>      MOSCRCEN: Main On-chip RC Oscillator Enable
//     <o4.4..6>   MOSCRCF: Main On-chip RC Oscillator Frequency Selection
//                   <0=> 4MHz <1=> 8 MHz <2=> 12 MHz <3=> Reserved
//     <o4.8..15>  MOSCXTST: Main Crystal Oscillator Startup Time <0-255>
//     <o4.24>     MOSCSEL: Main Oscillator Selection
//                   <0=> Main On-chip RC Oscillator <1=> Main Crystal Oscillator
//     <o4.25>     CFDEN: Clock Failure Detector Enable
//   </h>
//
//   <h> Clock Generator Phase Locked Loop A Register (CKGR_PLLAR)
//                   <i> PLL A Freq = (Main CLOCK Freq / DIVA) * (MULA + 1)
//                   <i> Example: XTAL = 12 MHz, DIVA = 1, MULA = 14  =>  PLLA =  168 MHz
//     <o5.0..7>   DIVA: PLL Divider A <0-255>
//                   <i> 0        - Divider output is 0
//                   <i> 1        - Divider is bypassed
//                   <i> 2 .. 255 - Divider output is the Main Clock divided by DIVA
//     <o5.8..13>  PLLACOUNT: PLL A Counter <0-63>
//                   <i> Number of Slow Clocks before the LOCKA bit is set in
//                   <i> PMC_SR after CKGR_PLLAR is written
//     <o5.16..26> MULA: PLL A Multiplier <0-2047>
//                   <i> 0         - The PLL A is deactivated
//                   <i> 1 .. 2047 - The PLL A Clock frequency is the PLL a input
//                   <i>             frequency multiplied by MULA + 1
//   </h>
//
//   <h> Clock Generator Phase Locked Loop B Register (CKGR_PLLBR)
//                   <i> PLL B Freq = (Main CLOCK Freq / DIVB) * (MULB + 1)
//                   <i> Example: XTAL = 12 MHz, DIVB = 1, MULB = 14  =>  PLLB =  168 MHz
//     <o6.0..7>   DIVB: PLL Divider B <0-255>
//                   <i> 0        - Divider output is 0
//                   <i> 1        - Divider is bypassed
//                   <i> 2 .. 255 - Divider output is the Main Clock divided by DIVB
//     <o6.8..13>  PLLBCOUNT: PLL A Counter <0-63>
//                   <i> Number of Slow Clocks before the LOCKA bit is set in
//                   <i> PMC_SR after CKGR_PLLBR is written
//     <o6.16..26> MULB: PLL B Multiplier <0-2047>
//                   <i> 0         - The PLL B is deactivated
//                   <i> 1 .. 2047 - The PLL B Clock frequency is the PLL a input
//                   <i>             frequency multiplied by MULB + 1
//   </h>
//
//   <h> Master Clock Register (PMC_MCKR)
//     <o7.0..1>   CSS: Master Clock Selection
//                   <0=> Slow Clock
//                   <1=> Main Clock
//                   <2=> PLL A Clock
//                   <3=> PLL B Clock
//     <o7.4..6>   PRES: Master Clock Prescaler
//                   <0=> Clock        <1=> Clock / 2
//                   <2=> Clock / 4    <3=> Clock / 8
//                   <4=> Clock / 16   <5=> Clock / 32
//                   <6=> Clock / 64   <7=> Clock / 3
//     <o7.12>     PLLADIV2: PLLA Divisor by 2
//     <o7.13>     PLLBDIV2: PLLB Divisor by 2
//   </h>
//
//   <h> USB Clock Register (PMC_USB)
//     <o8.0>      USBS: USB Input Clock Selection
//                   <0=> PLL A Clock
//                   <1=> PLL B Clock
//     <o8.8..11>  USBDIV: USB Input Clock Devider <0-15>
//                   <i>  USB Clock is Input clock divided by USBDIV+1
//   </h>
//
//   <h> Programmable Clock Register 0 (PMC_PCK0)
//     <o9.0..2>   CSS: Master Clock Selection
//                   <0=> Slow Clock
//                   <1=> Main Clock
//                   <2=> PLL A Clock
//                   <3=> PLL B Clock
//                   <4=> Master Clock
//                   <5=> Reserved
//                   <6=> Reserved
//                   <7=> Reserved
//     <o9.4..6>   PRES: Programmable Clock Prescaler
//                   <0=> Clock        <1=> Clock / 2
//                   <2=> Clock / 4    <3=> Clock / 8
//                   <4=> Clock / 16   <5=> Clock / 32
//                   <6=> Clock / 64   <7=> Reserved
//   </h>
//
//   <h> Programmable Clock Register 1 (PMC_PCK1)
//     <o10.0..2>  CSS: Master Clock Selection
//                   <0=> Slow Clock
//                   <1=> Main Clock
//                   <2=> PLL A Clock
//                   <3=> PLL B Clock
//                   <4=> Master Clock
//                   <5=> Reserved
//                   <6=> Reserved
//                   <7=> Reserved
//     <o10.4..6>  PRES: Programmable Clock Prescaler
//                   <0=> Clock        <1=> Clock / 2
//                   <2=> Clock / 4    <3=> Clock / 8
//                   <4=> Clock / 16   <5=> Clock / 32
//                   <6=> Clock / 64   <7=> Reserved
//   </h>
//
//   <h> Programmable Clock Register 2 (PMC_PCK2)
//     <o11.0..2>  CSS: Master Clock Selection
//                   <0=> Slow Clock
//                   <1=> Main Clock
//                   <2=> PLL A Clock
//                   <3=> PLL B Clock
//                   <4=> Master Clock
//                   <5=> Reserved
//                   <6=> Reserved
//                   <7=> Reserved
//     <o11.4..6>  PRES: Programmable Clock Prescaler
//                   <0=> Clock        <1=> Clock / 2
//                   <2=> Clock / 4    <3=> Clock / 8
//                   <4=> Clock / 16   <5=> Clock / 32
//                   <6=> Clock / 64   <7=> Reserved
//   </h>
// </e>
*/
#define PMC_SETUP       1               // Reset values:
#define PMC_SCER_Val    0x00000100      // WO register (0x00000001)
#define PMC_PCER0_Val   0x00003C00      // WO register (0x00000000)
#define PMC_PCER1_Val   0x00000000      // WO register (0x00000000)
#define CKGR_MOR_Val    0x01370F21      // 0x00000001
#define CKGR_PLLAR_Val  0x201F0604      // 0x00003F00
#define CKGR_PLLBR_Val  0x00170604      // 0x00003F00
#define PMC_MCKR_Val    0x00000002      // 0x00000001
#define PMC_USB_Val     0x00000101      // 0x00000000
#define PMC_PCK0_Val    0x00000002      // 0x00000000
#define PMC_PCK1_Val    0x00000000      // 0x00000000
#define PMC_PCK2_Val    0x00000000      // 0x00000000


/*--------------------- Watchdog Configuration -------------------------------
//
// <e> Watchdog Disable
// </e>
*/
#define WDT_SETUP       1               // Reset values:


/*
//-------- <<< end of configuration section >>> ------------------------------
*/

/*----------------------------------------------------------------------------
  Check the register settings
 *----------------------------------------------------------------------------*/
#define CHECK_RANGE(val, min, max)                ((val < min) || (val > max))
#define CHECK_RSVD(val, mask)                     (val & mask)

/* Embedded Flash Controllers Configuration ----------------------------------*/
#if (CHECK_RSVD((EEFC_FMR_Val),  ~0x01000F01))
   #error "EEFC_FMR: Invalid values of reserved bits!"
#endif

/* Power Management Controller Configuration ---------------------------------*/
#if (CHECK_RSVD((PMC_SCER_Val),   ~0x00000780))
   #error "PMC_SCER: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_PCER0_Val),   ~0xFFFFFFFC))
   #error "PMC_PCER0: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_PCER1_Val),   ~0x00000007))
   #error "PMC_PCER1: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((CKGR_MOR_Val),   ~0x03FFFF7F))
   #error "CKGR_MOR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((CKGR_PLLAR_Val), ~0x27FF3FFF))
   #error "CKGR_PLLAR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((CKGR_PLLBR_Val), ~0x07FF3FFF))
   #error "CKGR_PLLBR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_MCKR_Val),   ~0x00003073))
   #error "PMC_MCKR: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_USB_Val),   ~0x00000F01))
   #error "PMC_USB: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_PCK0_Val),   ~0x00000077))
   #error "PMC_PCK0: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_PCK1_Val),   ~0x00000077))
   #error "PMC_PCK1: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PMC_PCK2_Val),   ~0x00000077))
   #error "PMC_PCK2: Invalid values of reserved bits!"
#endif


/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define XTAL        (12000000UL)        /* Crystal frequency                  */
#define XTAL32      (   32768UL)        /* 32k crystal frequency              */
#define OSC_CLK     (      XTAL)        /* Main oscillator frequency          */
#define OSC32_CLK   (    XTAL32)        /* 32k oscillator frequency           */
#define ERC_OSC     (   32000UL)        /* Embedded RC oscillator freqquency  */
#define EFRC_OSC    ( 4000000UL)        /* Embedded fast RC oscillator freq   */

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = XTAL;/*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
  /* Determine clock frequency according to clock register values */
  switch (PMC->PMC_MCKR & (uint32_t) PMC_MCKR_CSS_Msk) {
  case PMC_MCKR_CSS_SLOW_CLK:  /* Slow clock */
    if (SUPC->SUPC_SR & SUPC_SR_OSCSEL) {
      SystemCoreClock = XTAL32;
    } else {
      SystemCoreClock = CHIP_FREQ_SLCK_RC;
    }
    break;
  case PMC_MCKR_CSS_MAIN_CLK:  /* Main clock */
    if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) {
      SystemCoreClock = XTAL;
    } else {
      SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

      switch (PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk) {
      case CKGR_MOR_MOSCRCF_4_MHz:
        break;
      case CKGR_MOR_MOSCRCF_8_MHz:
        SystemCoreClock *= 2U;
        break;
      case CKGR_MOR_MOSCRCF_12_MHz:
        SystemCoreClock *= 3U;
        break;
      default:
        break;
      }
    }
    break;
  case PMC_MCKR_CSS_PLLA_CLK:  /* PLLA clock */
  case PMC_MCKR_CSS_PLLB_CLK:  /* PLLB clock */
    if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) {
      SystemCoreClock = XTAL;
    } else {
      SystemCoreClock = CHIP_FREQ_MAINCK_RC_4MHZ;

      switch (PMC->CKGR_MOR & CKGR_MOR_MOSCRCF_Msk) {
      case CKGR_MOR_MOSCRCF_4_MHz:
        break;
      case CKGR_MOR_MOSCRCF_8_MHz:
        SystemCoreClock *= 2U;
        break;
      case CKGR_MOR_MOSCRCF_12_MHz:
        SystemCoreClock *= 3U;
        break;
      default:
        break;
      }
    }
    if ((uint32_t) (PMC->PMC_MCKR & (uint32_t) PMC_MCKR_CSS_Msk) == PMC_MCKR_CSS_PLLA_CLK) {
      SystemCoreClock *= ((((PMC->CKGR_PLLAR) & CKGR_PLLAR_MULA_Msk) >>
                                  CKGR_PLLAR_MULA_Pos) + 1U);
      SystemCoreClock /= ((((PMC->CKGR_PLLAR) & CKGR_PLLAR_DIVA_Msk) >>
                                  CKGR_PLLAR_DIVA_Pos));
    } else {
      SystemCoreClock *= ((((PMC->CKGR_PLLBR) & CKGR_PLLBR_MULB_Msk) >>
                                   CKGR_PLLBR_MULB_Pos) + 1U);
      SystemCoreClock /= ((((PMC->CKGR_PLLBR) & CKGR_PLLBR_DIVB_Msk) >>
                                       CKGR_PLLBR_DIVB_Pos));
    }
    break;
  default:
    break;
  }

  if ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) == PMC_MCKR_PRES_CLK_3) {
    SystemCoreClock /= 3U;
  } else {
    SystemCoreClock >>= ((PMC->PMC_MCKR & PMC_MCKR_PRES_Msk) >> PMC_MCKR_PRES_Pos);
  }
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
#if (EEFC_SETUP == 1)                   /* Embedded Flash Controller 0/1 Setup  */
  EFC0->EEFC_FMR = EEFC_FMR_Val;
#if defined(ID_EFC1)
  EFC1->EEFC_FMR = EEFC_FMR_Val;
#endif
#endif

  PMC->PMC_WPMR = 0x504D4300;           /* Disable write protect              */

#if (PMC_SETUP == 1)                    /* Power Management Controller Setup  */

  /* before we change the clocksetup we switch Master Clock Source to MAIN_CLK */
  PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));     /* Wait for MCKRDY                    */

#if (CKGR_MOR_Val & (CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN))             /* If MOSCRCEN or MOSCXTEN set        */
  PMC->CKGR_MOR  =  (PMC->CKGR_MOR &  CKGR_MOR_MOSCSEL) |                /* Keep the current MOSCSEL  */
                    (CKGR_MOR_Val  & ~CKGR_MOR_MOSCSEL) |                /* Set value except MOSCSEL  */
                    (CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN);             /* and enable bothe OSC      */
#if (CKGR_MOR_Val & CKGR_MOR_MOSCRCEN)
  while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));    /* Wait for MOSCRCS                   */
#endif
#if (CKGR_MOR_Val & CKGR_MOR_MOSCXTEN)
  while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));    /* Wait for MOSCXTS                   */
#endif
  PMC->CKGR_MOR  =  CKGR_MOR_Val;             /* set the desired selection          */
  while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));   /* Wait for MOSCSELS                  */
#endif

#if (CKGR_PLLAR_Val & CKGR_PLLAR_MULA_Msk)    /* If MULA != 0                       */
  PMC->CKGR_PLLAR = CKGR_PLLAR_Val | CKGR_PLLAR_ONE;
  while (!(PMC->PMC_SR & PMC_SR_LOCKA));      /* Wait for LOCKA                     */
#endif

#if (CKGR_PLLBR_Val & CKGR_PLLBR_MULB_Msk)    /* If MULB != 0                       */
  PMC->CKGR_PLLBR = CKGR_PLLBR_Val;
  while (!(PMC->PMC_SR & PMC_SR_LOCKB));      /* Wait for LOCKB                     */
#endif

  if ((PMC_MCKR_Val & 0x03) >= 2) {
    /* Write PRES field only                                                  */
    PMC->PMC_MCKR = (PMC->PMC_MCKR & ~0x70) | (PMC_MCKR_Val & 0x70);
  } else {
    /* Write CSS field only                                                   */
    PMC->PMC_MCKR = (PMC->PMC_MCKR & ~0x03) | (PMC_MCKR_Val & 0x03);
  }
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));     /* Wait for MCKRDY                    */
  PMC->PMC_MCKR   =  PMC_MCKR_Val;            /* Write all MCKR                     */
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY));     /* Wait for MCKRDY                    */

  PMC->PMC_USB   = PMC_USB_Val;
  PMC->PMC_SCER  = PMC_SCER_Val;
  PMC->PMC_PCER0 = PMC_PCER0_Val;
  PMC->PMC_PCER1 = PMC_PCER1_Val;

#if (PMC_SCER_Val & PMC_SCER_PCK0)
  PMC->PMC_PCK[0] = PMC_PCK0_Val;             /* Write PCK0                         */
  while (!(PMC->PMC_SR & PMC_SR_PCKRDY0));    /* Wait for PCKRDY0                   */
#endif
#if (PMC_SCER_Val & PMC_SCER_PCK1)
  PMC->PMC_PCK[1] = PMC_PCK1_Val;             /* Write PCK1                         */
  while (!(PMC->PMC_SR & PMC_SR_PCKRDY1));    /* Wait for PCKRDY1                   */
#endif
#if (PMC_SCER_Val & PMC_SCER_PCK2)
  PMC->PMC_PCK[2] = PMC_PCK2_Val;             /* Write PCK2                         */
  while (!(PMC->PMC_SR & PMC_SR_PCKRDY2));    /* Wait for PCKRDY2                   */
#endif

#endif
  PMC->PMC_WPMR = 0x504D4301;                 /* Enable write protect               */

#if (WDT_SETUP == 1)                          /* Watchdog Setup                     */
  WDT->WDT_MR = WDT_MR_WDDIS;
#endif
}
