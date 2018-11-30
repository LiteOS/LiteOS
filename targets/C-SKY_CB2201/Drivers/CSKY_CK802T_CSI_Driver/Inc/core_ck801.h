/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     core_ck801.h
 * @brief    CSI CK801 Core Peripheral Access Layer Header File
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#ifndef __CORE_CK801_H_GENERIC
#define __CORE_CK801_H_GENERIC

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *                 CSI definitions
 ******************************************************************************/
/**
  \ingroup Ck801
  @{
 */

/*  CSI CK801 definitions */
#define __CK801_CSI_VERSION_MAIN  (0x04U)                                      /*!< [31:16] CSI HAL main version */
#define __CK801_CSI_VERSION_SUB   (0x1EU)                                      /*!< [15:0]  CSI HAL sub version */
#define __CK801_CSI_VERSION       ((__CK801_CSI_VERSION_MAIN << 16U) | \
                                   __CK801_CSI_VERSION_SUB           )        /*!< CSI HAL version number */

#ifndef __CK80X
#define __CK80X                   (0x01U)                                         /*!< CK80X Core */
#endif

/** __FPU_USED indicates whether an FPU is used or not.
    This core does not support an FPU at all
*/
#define __FPU_USED       0U

#if defined ( __GNUC__ )
#if defined (__VFP_FP__) && !defined(__SOFTFP__)
#error "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CORE_CK801_H_GENERIC */

#ifndef __CSI_GENERIC

#ifndef __CORE_CK801_H_DEPENDANT
#define __CORE_CK801_H_DEPENDANT

#ifdef __cplusplus
extern "C" {
#endif

/* check device defines and use defaults */
#ifndef __CK801_REV
#define __CK801_REV               0x0000U
#endif

#ifndef __VIC_PRIO_BITS
#define __VIC_PRIO_BITS           2U
#endif

#ifndef __Vendor_SysTickConfig
#define __Vendor_SysTickConfig    1U
#endif

#ifndef __GSR_GCR_PRESENT
#define __GSR_GCR_PRESENT         0U
#endif

#ifndef __MPU_PRESENT
#define __MPU_PRESENT             1U
#endif

#include <csi_gcc.h>

/* IO definitions (access restrictions to peripheral registers) */
/**
    \defgroup CSI_glob_defs CSI Global Defines

    <strong>IO Type Qualifiers</strong> are used
    \li to specify the access to peripheral variables.
    \li for automatic generation of peripheral register debug information.
*/
#ifdef __cplusplus
#define     __I      volatile             /*!< Defines 'read only' permissions */
#else
#define     __I      volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O      volatile             /*!< Defines 'write only' permissions */
#define     __IO     volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const       /*! Defines 'read only' structure member permissions */
#define     __OM     volatile             /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile             /*! Defines 'read / write' structure member permissions */

/*@} end of group CK801 */

/*******************************************************************************
 *                 Register Abstraction
  Core Register contain:
  - Core Register
  - Core VIC Register
  - Core SCB Register
  - Core SysTick Register
 ******************************************************************************/
/**
  \defgroup CSI_core_register Defines and Type Definitions
  \brief Type definitions and defines for CK80X processor based devices.
*/

/**
  \ingroup    CSI_core_register
  \defgroup   CSI_CORE  Status and Control Registers
  \brief      Core Register type definitions.
  @{
 */

/**
  \brief  Access Processor Status Register(PSR)struct definition.
 */
typedef union {
    struct {
        uint32_t C: 1;                       /*!< bit:      0  Conditional code/Carry flag */
        uint32_t _reserved0: 5;              /*!< bit:  2.. 5  Reserved */
        uint32_t IE: 1;                      /*!< bit:      6  Interrupt effective control bit */
        uint32_t IC: 1;                      /*!< bit:      7  Interrupt control bit */
        uint32_t EE: 1;                      /*!< bit:      8  Abnormally effective control bit */
        uint32_t MM: 1;                      /*!< bit:      9  Unsymmetrical masking bit */
        uint32_t _reserved1: 6;              /*!< bit: 10..15  Reserved */
        uint32_t VEC: 8;                     /*!< bit: 16..23  Abnormal event vector value */
        uint32_t _reserved2: 7;              /*!< bit: 24..30  Reserved */
        uint32_t S: 1;                       /*!< bit:     31  Superuser mode set bit */
    } b;                                   /*!< Structure    Access by bit */
    uint32_t w;                            /*!< Type         Access by whole register */
} PSR_Type;

/* PSR Register Definitions */
#define PSR_S_Pos                          31U                                            /*!< PSR: S Position */
#define PSR_S_Msk                          (1UL << PSR_S_Pos)                             /*!< PSR: S Mask */

#define PSR_VEC_Pos                        16U                                            /*!< PSR: VEC Position */
#define PSR_VEC_Msk                        (0x7FUL << PSR_VEC_Pos)                        /*!< PSR: VEC Mask */

#define PSR_MM_Pos                         9U                                             /*!< PSR: MM Position */
#define PSR_MM_Msk                         (1UL << PSR_MM_Pos)                            /*!< PSR: MM Mask */

#define PSR_EE_Pos                         8U                                             /*!< PSR: EE Position */
#define PSR_EE_Msk                         (1UL << PSR_EE_Pos)                            /*!< PSR: EE Mask */

#define PSR_IC_Pos                         7U                                             /*!< PSR: IC Position */
#define PSR_IC_Msk                         (1UL << PSR_IC_Pos)                            /*!< PSR: IC Mask */

#define PSR_IE_Pos                         6U                                             /*!< PSR: IE Position */
#define PSR_IE_Msk                         (1UL << PSR_IE_Pos)                            /*!< PSR: IE Mask */

#define PSR_C_Pos                          0U                                             /*!< PSR: C Position */
#define PSR_C_Msk                          (1UL << PSR_C_Pos)                             /*!< PSR: C Mask */

/**
  \brief Consortium definition for accessing Cache Configuration Registers(CCR, CR<18, 0>).
 */
typedef union {
    struct {
        uint32_t MP: 1;                      /*!< bit:      0  memory protection settings */
        uint32_t _reserved0: 6;              /*!< bit:  1.. 6  Reserved */
        uint32_t BE: 1;                      /*!< bit:      7  Endian mode */
        uint32_t SCK: 3;                     /*!< bit:  8..10  the clock ratio of the system and the processor */
        uint32_t _reserved1: 2;              /*!< bit: 11..12  Reserved */
        uint32_t BE_V2: 1;                   /*!< bit:     13  V2 Endian mode */
        uint32_t _reserved2: 18;             /*!< bit: 14..31  Reserved */
    } b;                                   /*!< Structure    Access by bit */
    uint32_t w;                            /*!< Type         Access by whole register */
} CCR_Type;

/* CCR Register Definitions */
#define CCR_BE_V2_Pos                     13U                                            /*!< CCR: BE_V2 Position */
#define CCR_BE_V2_Msk                     (0x1UL << CCR_ISR_Pos)                         /*!< CCR: BE_V2 Mask */

#define CCR_SCK_Pos                       8U                                             /*!< CCR: SCK Position */
#define CCR_SCK_Msk                       (0x3UL << CCR_SCK_Pos)                         /*!< CCR: SCK Mask */

#define CCR_BE_Pos                        7U                                             /*!< CCR: BE Position */
#define CCR_BE_Msk                        (0x1UL << CCR_BE_Pos)                          /*!< CCR: BE Mask */

#define CCR_MP_Pos                        0U                                             /*!< CCR: MP Position */
#define CCR_MP_Msk                        (0x1UL << CCR_MP_Pos)                          /*!< CCR: MP Mask */

/**
  \brief  Consortium definition for accessing high ease access permission configutation registers(CAPR, CR<19,0>)
 */
typedef union {
    struct {
        uint32_t X0: 1;                      /*!< bit:      0  Non executable attribute setting */
        uint32_t X1: 1;                      /*!< bit:      1  Non executable attribute setting */
        uint32_t X2: 1;                      /*!< bit:      2  Non executable attribute setting */
        uint32_t X3: 1;                      /*!< bit:      3  Non executable attribute setting */
        uint32_t X4: 1;                      /*!< bit:      4  Non executable attribute setting */
        uint32_t X5: 1;                      /*!< bit:      5  Non executable attribute setting */
        uint32_t X6: 1;                      /*!< bit:      6  Non executable attribute setting */
        uint32_t X7: 1;                      /*!< bit:      7  Non executable attribute setting */
        uint32_t AP0: 2;                     /*!< bit:  8.. 9  access permissions settings bit */
        uint32_t AP1: 2;                     /*!< bit: 10..11  access permissions settings bit */
        uint32_t AP2: 2;                     /*!< bit: 12..13  access permissions settings bit */
        uint32_t AP3: 2;                     /*!< bit: 14..15  access permissions settings bit */
        uint32_t AP4: 2;                     /*!< bit: 16..17  access permissions settings bit */
        uint32_t AP5: 2;                     /*!< bit: 18..19  access permissions settings bit */
        uint32_t AP6: 2;                     /*!< bit: 20..21  access permissions settings bit */
        uint32_t AP7: 2;                     /*!< bit: 22..23  access permissions settings bit */
        uint32_t S0: 1;                      /*!< bit:     24  Security property settings */
        uint32_t S1: 1;                      /*!< bit:     25  Security property settings */
        uint32_t S2: 1;                      /*!< bit:     26  Security property settings */
        uint32_t S3: 1;                      /*!< bit:     27  Security property settings */
        uint32_t S4: 1;                      /*!< bit:     28  Security property settings */
        uint32_t S5: 1;                      /*!< bit:     29  Security property settings */
        uint32_t S6: 1;                      /*!< bit:     30  Security property settings */
        uint32_t S7: 1;                      /*!< bit:     31  Security property settings */
    } b;                                   /*!< Structure    Access by bit */
    uint32_t w;                            /*!< Type         Access by whole register */
} CAPR_Type;

/* CAPR Register Definitions */
#define CAPR_S7_Pos                        31U                                            /*!< CAPR: S7 Position */
#define CAPR_S7_Msk                        (1UL << CAPR_S7_Pos)                           /*!< CAPR: S7 Mask */

#define CAPR_S6_Pos                        30U                                            /*!< CAPR: S6 Position */
#define CAPR_S6_Msk                        (1UL << CAPR_S6_Pos)                           /*!< CAPR: S6 Mask */

#define CAPR_S5_Pos                        29U                                            /*!< CAPR: S5 Position */
#define CAPR_S5_Msk                        (1UL << CAPR_S5_Pos)                           /*!< CAPR: S5 Mask */

#define CAPR_S4_Pos                        28U                                            /*!< CAPR: S4 Position */
#define CAPR_S4_Msk                        (1UL << CAPR_S4_Pos)                           /*!< CAPR: S4 Mask */

#define CAPR_S3_Pos                        27U                                            /*!< CAPR: S3 Position */
#define CAPR_S3_Msk                        (1UL << CAPR_S3_Pos)                           /*!< CAPR: S3 Mask */

#define CAPR_S2_Pos                        26U                                            /*!< CAPR: S2 Position */
#define CAPR_S2_Msk                        (1UL << CAPR_S2_Pos)                           /*!< CAPR: S2 Mask */

#define CAPR_S1_Pos                        25U                                            /*!< CAPR: S1 Position */
#define CAPR_S1_Msk                        (1UL << CAPR_S1_Pos)                           /*!< CAPR: S1 Mask */

#define CAPR_S0_Pos                        24U                                            /*!< CAPR: S0 Position */
#define CAPR_S0_Msk                        (1UL << CAPR_S0_Pos)                           /*!< CAPR: S0 Mask */

#define CAPR_AP7_Pos                       22U                                            /*!< CAPR: AP7 Position */
#define CAPR_AP7_Msk                       (0x3UL << CAPR_AP7_Pos)                        /*!< CAPR: AP7 Mask */

#define CAPR_AP6_Pos                       20U                                            /*!< CAPR: AP6 Position */
#define CAPR_AP6_Msk                       (0x3UL << CAPR_AP6_Pos)                        /*!< CAPR: AP6 Mask */

#define CAPR_AP5_Pos                       18U                                            /*!< CAPR: AP5 Position */
#define CAPR_AP5_Msk                       (0x3UL << CAPR_AP5_Pos)                        /*!< CAPR: AP5 Mask */

#define CAPR_AP4_Pos                       16U                                            /*!< CAPR: AP4 Position */
#define CAPR_AP4_Msk                       (0x3UL << CAPR_AP4_Pos)                        /*!< CAPR: AP4 Mask */

#define CAPR_AP3_Pos                       14U                                            /*!< CAPR: AP3 Position */
#define CAPR_AP3_Msk                       (0x3UL << CAPR_AP3_Pos)                        /*!< CAPR: AP3 Mask */

#define CAPR_AP2_Pos                       12U                                            /*!< CAPR: AP2 Position */
#define CAPR_AP2_Msk                       (0x3UL << CAPR_AP2_Pos)                        /*!< CAPR: AP2 Mask */

#define CAPR_AP1_Pos                       10U                                            /*!< CAPR: AP1 Position */
#define CAPR_AP1_Msk                       (0x3UL << CAPR_AP1_Pos)                        /*!< CAPR: AP1 Mask */

#define CAPR_AP0_Pos                       8U                                             /*!< CAPR: AP0 Position */
#define CAPR_AP0_Msk                       (0x3UL << CAPR_AP0_Pos)                        /*!< CAPR: AP0 Mask */

#define CAPR_X7_Pos                        7U                                             /*!< CAPR: X7 Position */
#define CAPR_X7_Msk                        (0x1UL << CAPR_X7_Pos)                         /*!< CAPR: X7 Mask */

#define CAPR_X6_Pos                        6U                                             /*!< CAPR: X6 Position */
#define CAPR_X6_Msk                        (0x1UL << CAPR_X6_Pos)                         /*!< CAPR: X6 Mask */

#define CAPR_X5_Pos                        5U                                             /*!< CAPR: X5 Position */
#define CAPR_X5_Msk                        (0x1UL << CAPR_X5_Pos)                         /*!< CAPR: X5 Mask */

#define CAPR_X4_Pos                        4U                                             /*!< CAPR: X4 Position */
#define CAPR_X4_Msk                        (0x1UL << CAPR_X4_Pos)                         /*!< CAPR: X4 Mask */

#define CAPR_X3_Pos                        3U                                             /*!< CAPR: X3 Position */
#define CAPR_X3_Msk                        (0x1UL << CAPR_X3_Pos)                         /*!< CAPR: X3 Mask */

#define CAPR_X2_Pos                        2U                                             /*!< CAPR: X2 Position */
#define CAPR_X2_Msk                        (0x1UL << CAPR_X2_Pos)                         /*!< CAPR: X2 Mask */

#define CAPR_X1_Pos                        1U                                             /*!< CAPR: X1 Position */
#define CAPR_X1_Msk                        (0x1UL << CAPR_X1_Pos)                         /*!< CAPR: X1 Mask */

#define CAPR_X0_Pos                        0U                                             /*!< CAPR: X0 Position */
#define CAPR_X0_Msk                        (0x1UL << CAPR_X0_Pos)                         /*!< CAPR: X0 Mask */

/**
  \brief  Consortium definition for accessing control register(PACR, CR<20,0>).
 */
typedef union {
    struct {
        uint32_t E: 1;                       /*!< bit:      0  Effective setting of protected area */
        uint32_t Size: 5;                    /*!< bit:  1.. 5  Size of protected area */
        uint32_t _reserved0: 1;              /*!< bit:      6  Reserved */
        uint32_t base_addr: 25;              /*!< bit: 7..31   The high position of the address of a protected area */
    } b;                                   /*!< Structure    Access by bit */
    uint32_t w;                            /*!< Type         Access by whole register */
} PACR_Type;

/* PACR Register Definitions */
#define PACR_BASE_ADDR_Pos                 7U                                              /*!< PACR: base_addr Position */
#define PACR_BASE_ADDR_Msk                 (0x1FFFFFFUL << PACR_BASE_ADDR_Pos)             /*!< PACR: base_addr Mask */

#define PACR_SIZE_Pos                      1U                                              /*!< PACR: Size Position */
#define PACR_SIZE_Msk                      (0x1FUL << PACR_SIZE_Pos)                       /*!< PACR: Size Mask */

#define PACR_E_Pos                         0U                                              /*!< PACR: E Position */
#define PACR_E_Msk                         (0x1UL << PACR_E_Pos)                           /*!< PACR: E Mask */

/**
  \brief  Consortium definition for accessing protection area selection register(PRSR,CR<21,0>).
 */
typedef union {
    struct {
        uint32_t RID: 3;                     /*!< bit:  0.. 2  Protected area index value */
        uint32_t _reserved0: 29;             /*!< bit:  3..31  Reserved */
    } b;                                   /*!< Structure    Access by bit */
    uint32_t w;                            /*!< Type         Access by whole register */
} PRSR_Type;

/* PRSR Register Definitions */
#define PRSR_RID_Pos                       0U                                            /*!< PRSR: RID Position */
#define PRSR_RID_Msk                       (0x7UL << PRSR_RID_Pos)                       /*!< PRSR: RID Mask */

/*@} end of group CSI_CORE */


/**
  \ingroup    CSI_core_register
  \defgroup   CSI_VIC Vectored Interrupt Controller (VIC)
  \brief      Type definitions for the VIC Registers
  @{
 */

/**
  \brief Access to the structure of a vector interrupt controller.
 */
typedef struct {
    __IOM uint32_t ISER[1U];               /*!< Offset: 0x000 (R/W)  Interrupt set enable register */
    uint32_t RESERVED0[15U];
    __IOM uint32_t IWER[1U];               /*!< Offset: 0x040 (R/W)  Interrupt wake-up set register */
    uint32_t RESERVED1[15U];
    __IOM uint32_t ICER[1U];               /*!< Offset: 0x080 (R/W)  Interrupt clear enable register */
    uint32_t RESERVED2[15U];
    __IOM uint32_t IWDR[1U];               /*!< Offset: 0x0c0 (R/W)  Interrupt wake-up clear register */
    uint32_t RESERVED3[15U];
    __IOM uint32_t ISPR[1U];               /*!< Offset: 0x100 (R/W)  Interrupt set pend register */
    uint32_t RESERVED4[15U];
    __IOM uint32_t ISSR[1U];               /*!< Offset: 0x140 (R/W)  Security interrupt set register */
    uint32_t RESERVED5[15U];
    __IOM uint32_t ICPR[1U];               /*!< Offset: 0x180 (R/W)  Interrupt clear pend register */
    uint32_t RESERVED6[15U];
    __IOM uint32_t ICSR[1U];               /*!< Offset: 0x1c0 (R/W)  Security interrupt clear register */
    uint32_t RESERVED7[15U];
    __IOM uint32_t IABR[1U];               /*!< Offset: 0x200 (R/W)  Interrupt answer stateregister */
    uint32_t RESERVED8[63U];
    __IOM uint32_t IPR[8U];                /*!< Offset: 0x300 (R/W)  Interrupt priority register */
    uint32_t RESERVED9[504U];
    __IM  uint32_t ISR;                    /*!< Offset: 0xB00 (R/ )  Interrupt state register */
    __IOM uint32_t IPTR;                   /*!< Offset: 0xB04 (R/W)  Interrupt priority thershold register */
} VIC_Type;

/*@} end of group CSI_VIC */

/**
  \ingroup  CSI_core_register
  \defgroup CSI_SysTick     System Tick Timer (CORET)
  \brief    Type definitions for the System Timer Registers.
  @{
 */

/**
  \brief  The data structure of the access system timer.
 */
typedef struct {
    __IOM uint32_t CTRL;                   /*!< Offset: 0x000 (R/W)  Control register */
    __IOM uint32_t LOAD;                   /*!< Offset: 0x004 (R/W)  Backfill register */
    __IOM uint32_t VAL;                    /*!< Offset: 0x008 (R/W)  Current register */
    __IM  uint32_t CALIB;                  /*!< Offset: 0x00C (R/ )  Calibration register */
} CORET_Type;

/* CORET Control / Status Register Definitions */
#define CORET_CTRL_COUNTFLAG_Pos           16U                                            /*!< CORET CTRL: COUNTFLAG Position */
#define CORET_CTRL_COUNTFLAG_Msk           (1UL << CORET_CTRL_COUNTFLAG_Pos)              /*!< CORET CTRL: COUNTFLAG Mask */

#define CORET_CTRL_CLKSOURCE_Pos            2U                                            /*!< CORET CTRL: CLKSOURCE Position */
#define CORET_CTRL_CLKSOURCE_Msk           (1UL << CORET_CTRL_CLKSOURCE_Pos)              /*!< CORET CTRL: CLKSOURCE Mask */

#define CORET_CTRL_TICKINT_Pos              1U                                            /*!< CORET CTRL: TICKINT Position */
#define CORET_CTRL_TICKINT_Msk             (1UL << CORET_CTRL_TICKINT_Pos)                /*!< CORET CTRL: TICKINT Mask */

#define CORET_CTRL_ENABLE_Pos               0U                                            /*!< CORET CTRL: ENABLE Position */
#define CORET_CTRL_ENABLE_Msk              (1UL /*<< CORET_CTRL_ENABLE_Pos*/)             /*!< CORET CTRL: ENABLE Mask */

/* CORET Reload Register Definitions */
#define CORET_LOAD_RELOAD_Pos               0U                                            /*!< CORET LOAD: RELOAD Position */
#define CORET_LOAD_RELOAD_Msk              (0xFFFFFFUL /*<< CORET_LOAD_RELOAD_Pos*/)      /*!< CORET LOAD: RELOAD Mask */

/* CORET Current Register Definitions */
#define CORET_VAL_CURRENT_Pos               0U                                            /*!< CORET VAL: CURRENT Position */
#define CORET_VAL_CURRENT_Msk              (0xFFFFFFUL /*<< CORET_VAL_CURRENT_Pos*/)      /*!< CORET VAL: CURRENT Mask */

/* CORET Calibration Register Definitions */
#define CORET_CALIB_NOREF_Pos               31U                                           /*!< CORET CALIB: NOREF Position */
#define CORET_CALIB_NOREF_Msk              (1UL << CORET_CALIB_NOREF_Pos)                 /*!< CORET CALIB: NOREF Mask */

#define CORET_CALIB_SKEW_Pos                30U                                           /*!< CORET CALIB: SKEW Position */
#define CORET_CALIB_SKEW_Msk               (1UL << CORET_CALIB_SKEW_Pos)                  /*!< CORET CALIB: SKEW Mask */

#define CORET_CALIB_TENMS_Pos               0U                                            /*!< CORET CALIB: TENMS Position */
#define CORET_CALIB_TENMS_Msk              (0xFFFFFFUL /*<< CORET_CALIB_TENMS_Pos*/)      /*!< CORET CALIB: TENMS Mask */

/*@} end of group CSI_SysTick */

/**
  \ingroup  CSI_core_register
  \defgroup CSI_DCC
  \brief    Type definitions for the DCC.
  @{
 */

/**
  \brief  Access to the data structure of DCC.
 */
typedef struct {
    uint32_t RESERVED0[13U];
    __IOM uint32_t HCR;                      /*!< Offset: 0x034 (R/W) */
    __IM uint32_t EHSR;                      /*!< Offset: 0x03C (R/ ) */
    uint32_t RESERVED1[6U];
    union {
        __IM uint32_t DERJW;                 /*!< Offset: 0x058 (R/ )  Data exchange register CPU read*/
        __OM uint32_t DERJR;                 /*!< Offset: 0x058 ( /W)  Data exchange register CPU writer*/
    };

} DCC_Type;

#define DCC_HCR_JW_Pos                   18U                                            /*!< DCC HCR: jw_int_en Position */
#define DCC_HCR_JW_Msk                   (1UL << DCC_HCR_JW_Pos)                        /*!< DCC HCR: jw_int_en Mask */

#define DCC_HCR_JR_Pos                   19U                                            /*!< DCC HCR: jr_int_en Position */
#define DCC_HCR_JR_Msk                   (1UL << DCC_HCR_JR_Pos)                        /*!< DCC HCR: jr_int_en Mask */

#define DCC_EHSR_JW_Pos                  1U                                             /*!< DCC EHSR: jw_vld Position */
#define DCC_EHSR_JW_Msk                  (1UL << DCC_EHSR_JW_Pos)                       /*!< DCC EHSR: jw_vld Mask */

#define DCC_EHSR_JR_Pos                  2U                                             /*!< DCC EHSR: jr_vld Position */
#define DCC_EHSR_JR_Msk                  (1UL << DCC_EHSR_JR_Pos)                       /*!< DCC EHSR: jr_vld Mask */

/*@} end of group CSI_DCC */

/**
  \ingroup    CSI_core_register
  \defgroup   CSI_core_bitfield     Core register bit field macros
  \brief      Macros for use with bit field definitions (xxx_Pos, xxx_Msk).
  @{
 */

/**
  \brief   Mask and shift a bit field value for use in a register bit range.
  \param[in] field  Name of the register bit field.
  \param[in] value  Value of the bit field.
  \return           Masked and shifted value.
*/
#define _VAL2FLD(field, value)    ((value << field ## _Pos) & field ## _Msk)

/**
  \brief     Mask and shift a register value to extract a bit filed value.
  \param[in] field  Name of the register bit field.
  \param[in] value  Value of register.
  \return           Masked and shifted bit field value.
*/
#define _FLD2VAL(field, value)    ((value & field ## _Msk) >> field ## _Pos)

/*@} end of group CSI_core_bitfield */

/**
  \ingroup    CSI_core_register
  \defgroup   CSI_core_base     Core Definitions
  \brief      Definitions for base addresses, unions, and structures.
  @{
 */

/* Memory mapping of CK801 Hardware */
#define TCIP_BASE           (0xE000E000UL)                            /*!< Titly Coupled IP Base Address */
#define CORET_BASE          (TCIP_BASE +  0x0010UL)                   /*!< CORET Base Address */
#define VIC_BASE            (TCIP_BASE +  0x0100UL)                   /*!< VIC Base Address */
#define DCC_BASE            (0xE0011000UL)                            /*!< DCC Base Address */

#define CORET               ((CORET_Type   *)     CORET_BASE  )       /*!< SysTick configuration struct */
#define VIC                 ((VIC_Type    *)     VIC_BASE   )         /*!< VIC configuration struct */
#define DCC                 ((DCC_Type     *)     DCC_BASE    )       /*!< DCC configuration struct */

/*@} */

/*******************************************************************************
 *                Hardware Abstraction Layer
  Core Function Interface contains:
  - Core VIC Functions
  - Core CORET Functions
  - Core Register Access Functions
 ******************************************************************************/
/**
  \defgroup CSI_Core_FunctionInterface Functions and Instructions Reference
*/

/* ##########################   VIC functions  #################################### */
/**
  \ingroup  CSI_Core_FunctionInterface
  \defgroup CSI_Core_VICFunctions VIC Functions
  \brief    Functions that manage interrupts and exceptions via the VIC.
  @{
 */

/* Interrupt Priorities are WORD accessible only under CSKYv6M                   */
/* The following MACROS handle generation of the register offset and byte masks */
#define _BIT_SHIFT(IRQn)         (  ((((uint32_t)(int32_t)(IRQn))         )      &  0x03UL) * 8UL)
#define _IP_IDX(IRQn)            (   (((uint32_t)(int32_t)(IRQn))                >>    2UL)      )

extern uint32_t __Vectors[];

/**
  \brief   Enable External Interrupt
  \details Enable a device-specific interrupt in the VIC interrupt controller.
  \param [in]      IRQn  External interrupt number. Value cannot be negative.
 */
__ALWAYS_INLINE void csi_vic_enable_irq(int32_t IRQn)
{
    VIC->ISER[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
#ifdef CONFIG_SYSTEM_SECURE
    VIC->ISSR[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
#endif
}

/**
  \brief   Disable External Interrupt
  \details Disable a device-specific interrupt in the VIC interrupt controller.
  \param [in]      IRQn  External interrupt number. Value cannot be negative.
 */
__ALWAYS_INLINE void csi_vic_disable_irq(int32_t IRQn)
{
    VIC->ICER[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}

/**
  \brief   Enable External Secure Interrupt
  \details Enable a secure device-specific interrupt in the VIC interrupt controller.
  \param [in]      IRQn  External interrupt number. Value cannot be negative.
 */
__ALWAYS_INLINE void csi_vic_enable_sirq(int32_t IRQn)
{
    VIC->ISSR[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}

/**
  \brief   Disable External Secure Interrupt
  \details Disable a secure device-specific interrupt in the VIC interrupt controller.
  \param [in]      IRQn  External interrupt number. Value cannot be negative.
 */
__ALWAYS_INLINE void csi_vic_disable_sirq(int32_t IRQn)
{
    VIC->ICSR[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}

/**
  \brief   Check Interrupt is Enabled or not
  \details Read the enabled register in the VIC and returns the pending bit for the specified interrupt.
  \param [in]      IRQn  Interrupt number.
  \return  Interrput status is enabled.
 */
__ALWAYS_INLINE uint32_t csi_vic_get_enabled_irq(int32_t IRQn)
{
    return ((uint32_t)(((VIC->ISER[0U] & (1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
}

/**
  \brief   Check Interrupt is Pending or not
  \details Read the pending register in the VIC and returns the pending bit for the specified interrupt.
  \param [in]      IRQn  Interrupt number.
  \return             0  Interrupt status is not pending.
  \return             1  Interrupt status is pending.
 */
__ALWAYS_INLINE uint32_t csi_vic_get_pending_irq(int32_t IRQn)
{
    return ((uint32_t)(((VIC->ISPR[0U] & (1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
}

/**
  \brief   Set Pending Interrupt
  \details Set the pending bit of an external interrupt.
  \param [in]      IRQn  Interrupt number. Value cannot be negative.
 */
__ALWAYS_INLINE void csi_vic_set_pending_irq(int32_t IRQn)
{
    VIC->ISPR[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}

/**
  \brief   Clear Pending Interrupt
  \details Clear the pending bit of an external interrupt.
  \param [in]      IRQn  External interrupt number. Value cannot be negative.
 */
__ALWAYS_INLINE void csi_vic_clear_pending_irq(int32_t IRQn)
{
    VIC->ICPR[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}

/**
  \brief   Check Interrupt is Wakeup or not
  \details Read the wake up register in the VIC and returns the pending bit for the specified interrupt.
  \param [in]      IRQn  Interrupt number.
  \return             0  Interrupt is not set as wake up interrupt.
  \return             1  Interrupt is set as wake up interrupt.
 */
__ALWAYS_INLINE uint32_t csi_vic_get_wakeup_irq(int32_t IRQn)
{
    return ((uint32_t)(((VIC->IWER[0U] & (1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
}

/**
  \brief   Set Wakeup Interrupt
  \details Set the wake up bit of an external interrupt.
  \param [in]      IRQn  Interrupt number. Value cannot be negative.
 */
__ALWAYS_INLINE void csi_vic_set_wakeup_irq(int32_t IRQn)
{
    VIC->IWER[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}

/**
  \brief   Clear Wake up Interrupt
  \details Clear the wake up bit of an external interrupt.
  \param [in]      IRQn  External interrupt number. Value cannot be negative.
 */
__ALWAYS_INLINE void csi_vic_clear_wakeup_irq(int32_t IRQn)
{
    VIC->IWDR[0U] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}

/**
  \brief   Get Interrupt is Active or not
  \details Read the active register in the VIC and returns the active bit for the device specific interrupt.
  \param [in]      IRQn  Device specific interrupt number.
  \return             0  Interrupt status is not active.
  \return             1  Interrupt status is active.
  \note    IRQn must not be negative.
 */
__ALWAYS_INLINE uint32_t csi_vic_get_active(int32_t IRQn)
{
    return ((uint32_t)(((VIC->IABR[0] & (1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
}

/**
  \brief   Set Threshold register
  \details set the threshold register in the VIC.
  \param [in]      VectThreshold  specific vecter threshold.
  \param [in]      PrioThreshold  specific priority threshold.
 */
__ALWAYS_INLINE void csi_vic_set_threshold(uint32_t VectThreshold, uint32_t PrioThreshold)
{
    VIC->IPTR = 0x80000000 | (((VectThreshold + 32) & 0xFF) << 8) | ((PrioThreshold & 0x3) << 6);
}

/**
  \brief   Set Interrupt Priority
  \details Set the priority of an interrupt.
  \note    The priority cannot be set for every core interrupt.
  \param [in]      IRQn  Interrupt number.
  \param [in]  priority  Priority to set.
 */
__ALWAYS_INLINE void csi_vic_set_prio(int32_t IRQn, uint32_t priority)
{
    VIC->IPR[_IP_IDX(IRQn)]  = ((uint32_t)(VIC->IPR[_IP_IDX(IRQn)]  & ~(0xFFUL << _BIT_SHIFT(IRQn))) |
                                 (((priority << (8U - __VIC_PRIO_BITS)) & (uint32_t)0xFFUL) << _BIT_SHIFT(IRQn)));
}

/**
  \brief   Get Interrupt Priority
  \details Read the priority of an interrupt.
           The interrupt number can be positive to specify an external (device specific) interrupt,
           or negative to specify an internal (core) interrupt.
  \param [in]   IRQn  Interrupt number.
  \return             Interrupt Priority.
                      Value is aligned automatically to the implemented priority bits of the microcontroller.
 */
__ALWAYS_INLINE uint32_t csi_vic_get_prio(int32_t IRQn)
{
    return ((uint32_t)(((VIC->IPR[_IP_IDX(IRQn)] >> _BIT_SHIFT(IRQn)) & (uint32_t)0xFFUL) >> (8U - __VIC_PRIO_BITS)));
}

/**
  \brief   Set interrupt handler
  \details Set the interrupt handler according to the interrupt num, the handler will be filled in __Vectors[].
  \param [in]      IRQn  Interrupt number.
  \param [in]   handler  Interrupt handler.
 */
__ALWAYS_INLINE void csi_vic_set_vector(int32_t IRQn, uint32_t handler)
{
    if (IRQn >= 0 && IRQn < 32) {
        __Vectors[32 + IRQn] = handler;
    }
}

/**
  \brief   Get interrupt handler
  \details Get the address of interrupt handler function.
  \param [in]      IRQn  Interrupt number.
 */
__ALWAYS_INLINE uint32_t csi_vic_get_vector(int32_t IRQn)
{
    if (IRQn >= 0 && IRQn < 32) {
        return (uint32_t)__Vectors[32 + IRQn];
    }

    return 0;
}

/*@} end of CSI_Core_VICFunctions */

/* ##################################    SysTick function  ############################################ */
/**
  \ingroup  CSI_Core_FunctionInterface
  \defgroup CSI_Core_SysTickFunctions SysTick Functions
  \brief    Functions that configure the System.
  @{
 */


/**
  \brief   CORE timer Configuration
  \details Initializes the System Timer and its interrupt, and starts the System Tick Timer.
           Counter is in free running mode to generate periodic interrupts.
  \param [in]  ticks  Number of ticks between two interrupts.
  \param [in]  IRQn   core timer Interrupt number.
  \return          0  Function succeeded.
  \return          1  Function failed.
  \note    When the variable <b>__Vendor_SysTickConfig</b> is set to 1, then the
           function <b>SysTick_Config</b> is not included. In this case, the file <b><i>device</i>.h</b>
           must contain a vendor-specific implementation of this function.
 */
__ALWAYS_INLINE uint32_t csi_coret_config(uint32_t ticks, int32_t IRQn)
{
    if ((ticks - 1UL) > CORET_LOAD_RELOAD_Msk) {
        return (1UL);                                                   /* Reload value impossible */
    }

    CORET->LOAD  = (uint32_t)(ticks - 1UL);                           /* set reload register */
    CORET->VAL   = 0UL;                                               /* Load the CORET Counter Value */
    CORET->CTRL  = CORET_CTRL_CLKSOURCE_Msk |
                   CORET_CTRL_TICKINT_Msk |
                   CORET_CTRL_ENABLE_Msk;                           /* Enable CORET IRQ and CORET Timer */
    return (0UL);                                                     /* Function successful */
}

/**
  \brief   get CORE timer reload value
  \return          CORE timer counter value.
 */
__ALWAYS_INLINE uint32_t csi_coret_get_load(void)
{
    return CORET->LOAD;
}

/**
  \brief   get CORE timer counter value
  \return          CORE timer counter value.
 */
__ALWAYS_INLINE uint32_t csi_coret_get_value(void)
{
    return CORET->VAL;
}

/*@} end of CSI_Core_SysTickFunctions */


/* ##################################### DCC function ########################################### */
/**
  \ingroup  CSI_Core_FunctionInterface
  \defgroup CSI_core_DebugFunctions HAD Functions
  \brief    Functions that access the HAD debug interface.
  @{
 */

/**
  \brief   HAD Send Character
  \details Transmits a character via the HAD channel 0, and
           \li Just returns when no debugger is connected that has booked the output.
           \li Is blocking when a debugger is connected, but the previous character sent has not been transmitted.
  \param [in]     ch  Character to transmit.
  \returns            Character to transmit.
 */
__ALWAYS_INLINE uint32_t csi_had_send_char(uint32_t ch)
{
    DCC->DERJR = (uint8_t)ch;

    return (ch);
}


/**
  \brief   HAD Receive Character
  \details Inputs a character via the external variable \ref HAD_RxBuffer.
  \return             Received character.
  \return         -1  No character pending.
 */
__ALWAYS_INLINE int32_t csi_had_receive_char(void)
{
    int32_t ch = -1;                           /* no character available */

    if (_FLD2VAL(DCC_EHSR_JW, DCC->EHSR)) {
        ch = DCC->DERJW;
    }

    return (ch);
}


/**
  \brief   HAD Check Character
  \details Check whether a character is pending for reading in the variable \ref HAD_RxBuffer.
  \return          0  No character available.
  \return          1  Character available.
 */
__ALWAYS_INLINE int32_t csi_had_check_char(void)
{
    return _FLD2VAL(DCC_EHSR_JW, DCC->EHSR);                              /* no character available */
}

/*@} end of CSI_core_DebugFunctions */

/* ##########################  MPU functions  #################################### */
/**
  \ingroup  CSI_Core_FunctionInterface
  \defgroup CSI_Core_MPUFunctions MPU Functions
  \brief    Functions that configure MPU.
  @{
 */

typedef enum {
    REGION_SIZE_128B     = 0x6,
    REGION_SIZE_256B     = 0x7,
    REGION_SIZE_512B     = 0x8,
    REGION_SIZE_1KB      = 0x9,
    REGION_SIZE_2KB      = 0xA,
    REGION_SIZE_4KB      = 0xB,
    REGION_SIZE_8KB      = 0xC,
    REGION_SIZE_16KB     = 0xD,
    REGION_SIZE_32KB     = 0xE,
    REGION_SIZE_64KB     = 0xF,
    REGION_SIZE_128KB    = 0x10,
    REGION_SIZE_256KB    = 0x11,
    REGION_SIZE_512KB    = 0x12,
    REGION_SIZE_1MB      = 0x13,
    REGION_SIZE_2MB      = 0x14,
    REGION_SIZE_4MB      = 0x15,
    REGION_SIZE_8MB      = 0x16,
    REGION_SIZE_16MB     = 0x17,
    REGION_SIZE_32MB     = 0x18,
    REGION_SIZE_64MB     = 0x19,
    REGION_SIZE_128MB    = 0x1A,
    REGION_SIZE_256MB    = 0x1B,
    REGION_SIZE_512MB    = 0x1C,
    REGION_SIZE_1GB      = 0x1D,
    REGION_SIZE_2GB      = 0x1E,
    REGION_SIZE_4GB      = 0x1F
} region_size_e;

typedef enum {
    AP_BOTH_INACCESSIBLE = 0,
    AP_SUPER_RW_USER_INACCESSIBLE,
    AP_SUPER_RW_USER_RDONLY,
    AP_BOTH_RW
} access_permission_e;

typedef struct {
    uint32_t nx: 1;    /* instruction fetched excution */
    access_permission_e ap: 2;    /* super user and normal user access.*/
    uint32_t s: 1;    /* security */
} mpu_region_attr_t;

/**
  \brief  enable mpu
  \details
  */
__ALWAYS_INLINE void csi_mpu_enable(void)
{
    __set_CCR(__get_CCR() | CCR_MP_Msk);
}

/**
  \brief  disable mpu
  \details
  */
__ALWAYS_INLINE void csi_mpu_disable(void)
{
    __set_CCR(__get_CCR() & (~CCR_MP_Msk));
}

/**
  \brief  configure memory protected region.
  \details
  \param [in]  idx        memory protected region (0, 1, 2, ..., 7).
  \param [in]  base_addr  base address must be aligned with page size.
  \param [in]  size       \ref region_size_e. memory protected region size.
  \param [in]  attr       \ref region_size_t. memory protected region attribute.
  \param [in]  enable     enable or disable memory protected region.
  */
__ALWAYS_INLINE void csi_mpu_config_region(uint32_t idx, uint32_t base_addr, region_size_e size,
                                           mpu_region_attr_t attr, uint32_t enable)
{
    if (idx > 7) {
        return;
    }

    CAPR_Type capr;
    PACR_Type pacr;
    PRSR_Type prsr;

    capr.w = __get_CAPR();
    pacr.w = __get_PACR();
    prsr.w = __get_PRSR();

    pacr.b.base_addr = (base_addr >> PACR_BASE_ADDR_Pos) & (0x3FFFFFF);

    prsr.b.RID = idx;
    __set_PRSR(prsr.w);

    if (size != REGION_SIZE_128B) {
        pacr.w &= ~(((1u << (size -6)) - 1) << 7);
    }

    pacr.b.Size = size;

    capr.w = (capr.w | (attr.nx << idx) | (attr.ap << (idx * 2 + 8)) | (attr.s << (idx + 24)));
    __set_CAPR(capr.w);

    pacr.b.E = enable;
    __set_PACR(pacr.w);
}

/**
  \brief  enable mpu region by idx.
  \details
  \param [in]  idx        memory protected region (0, 1, 2, ..., 7).
  */
__ALWAYS_INLINE void csi_mpu_enable_region(uint32_t idx)
{
    if (idx > 7) {
        return;
    }

    __set_PRSR((__get_PRSR() & (~PRSR_RID_Msk)) | idx);
    __set_PACR(__get_PACR() | PACR_E_Msk);
}

/**
  \brief  disable mpu region by idx.
  \details
  \param [in]  idx        memory protected region (0, 1, 2, ..., 7).
  */
__ALWAYS_INLINE void csi_mpu_disable_region(uint32_t idx)
{
    if (idx > 7) {
        return;
    }

    __set_PRSR((__get_PRSR() & (~PRSR_RID_Msk)) | idx);
    __set_PACR(__get_PACR() & (~PACR_E_Msk));
}

/*@} end of CSI_Core_MMUFunctions */

/* ##################################    IRQ Functions  ############################################ */

/**
  \brief   Save the Irq context
  \details save the psr result before disable irq.
  \param [in]      irq_num  External interrupt number. Value cannot be negative.
 */
__ALWAYS_INLINE uint32_t csi_irq_save(void)
{
    uint32_t result;
    result = __get_PSR();
    __disable_irq();
    return(result);
}

/**
  \brief   Restore the Irq context
  \details restore saved primask state.
  \param [in]      irq_state  psr irq state.
 */
__ALWAYS_INLINE void csi_irq_restore(uint32_t irq_state)
{
    __set_PSR(irq_state);
}

/*@} end of IRQ Functions */

/**
  \brief   System Reset
  \details Initiates a system reset request to reset the MCU.
 */
__ALWAYS_INLINE void csi_system_reset(void)
{
    __DSB();                                                          /* Ensure all outstanding memory accesses included
                                                                       buffered write are completed before reset */

#ifdef __RESET_CONST
    __set_SRCR(__RESET_CONST);
#else
    __set_SRCR(0xABCD1234);
#endif

    __DSB();                                                          /* Ensure completion of memory access */

    for(;;)                                                           /* wait until reset */
    {
        __NOP();
    }
}

/* ##################################    Old Interfaces  ############################################ */

/* These interfaces are deprecated */
#define NVIC_EnableIRQ(IRQn)                            csi_vic_enable_irq(IRQn)
#define NVIC_DisableIRQ(IRQn)                           csi_vic_disable_irq(IRQn)
#define NVIC_GetPendingIRQ(IRQn)                        csi_vic_get_pending_irq(IRQn)
#define NVIC_SetPendingIRQ(IRQn)                        csi_vic_set_pending_irq(IRQn)
#define NVIC_ClearPendingIRQ(IRQn)                      csi_vic_clear_pending_irq(IRQn)
#define NVIC_GetWakeupIRQ(IRQn)                         csi_vic_get_wakeup_irq(IRQn)
#define NVIC_SetWakeupIRQ(IRQn)                         csi_vic_set_wakeup_irq(IRQn)
#define NVIC_ClearWakeupIRQ(IRQn)                       csi_vic_clear_wakeup_irq(IRQn)
#define NVIC_GetActive(IRQn)                            csi_vic_get_active(IRQn)
#define NVIC_SetThreshold(VectThreshold, PrioThreshold) csi_vic_set_threshold(VectThreshold, PrioThreshold)
#define NVIC_SetPriority(IRQn, priority)                csi_vic_set_prio(IRQn, priority)
#define NVIC_GetPriority(IRQn)                          csi_vic_get_prio(IRQn)
#define NVIC_SystemReset()                              csi_system_reset()

#define SysTick_Config(ticks)                           csi_coret_config(ticks, CORET_IRQn)
#define CORET_Config(ticks)                             csi_coret_config(ticks, CORET_IRQn)

/*@} end of Old Interfaces */

#ifdef __cplusplus
}
#endif

#endif /* __CORE_CK801_H_DEPENDANT */

#endif /* __CSI_GENERIC */
