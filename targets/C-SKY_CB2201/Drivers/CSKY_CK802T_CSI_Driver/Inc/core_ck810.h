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
 * @file     core_ck810.h
 * @brief    CSI CK810 Core Peripheral Access Layer Header File
 * @version  V1.0
 * @date     26. Jan 2018
 ******************************************************************************/

#ifndef __CORE_CK810_H_GENERIC
#define __CORE_CK810_H_GENERIC

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *                 CSI definitions
 ******************************************************************************/
/**
  \ingroup CK810
  @{
 */

/*  CSI CK810 definitions */
#define __CK810_CSI_VERSION_MAIN  (0x04U)                                      /*!< [31:16] CSI HAL main version */
#define __CK810_CSI_VERSION_SUB   (0x1EU)                                      /*!< [15:0]  CSI HAL sub version */
#define __CK810_CSI_VERSION       ((__CK810_CSI_VERSION_MAIN << 16U) | \
                                   __CK810_CSI_VERSION_SUB           )         /*!< CSI HAL version number */

#ifndef __CK810
#define __CK810                (0x0aU)                                         /*!< CK810 Core */
#endif

/** __FPU_USED indicates whether an FPU is used or not.
*/
#define __FPU_USED      1U

#if defined ( __GNUC__ )
#if defined (__VFP_FP__) && !defined(__SOFTFP__)
#error "Compiler generates FPU instructions for a device without an FPU (check __FPU_PRESENT)"
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CORE_CK810_H_GENERIC */

#ifndef __CSI_GENERIC

#ifndef __CORE_CK810_H_DEPENDANT
#define __CORE_CK810_H_DEPENDANT

#ifdef __cplusplus
extern "C" {
#endif

/* check device defines and use defaults */
#ifndef __CK810_REV
#define __CK810_REV               0x0000U
#endif

#ifndef __GSR_GCR_PRESENT
#define __GSR_GCR_PRESENT         0U
#endif

#ifndef __ICACHE_PRESENT
#define __ICACHE_PRESENT          1U
#endif

#ifndef __DCACHE_PRESENT
#define __DCACHE_PRESENT          1U
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

/*@} end of group CK810 */

/*******************************************************************************
 *                 Register Abstraction
  Core Register contain:
  - Core Register
 ******************************************************************************/
/**
  \defgroup CSI_core_register Defines and Type Definitions
  \brief Type definitions and defines for CK810 processor based devices.
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
        uint32_t MP: 2;                      /*!< bit:  0.. 1  memory protection settings */
        uint32_t IE: 1;                      /*!< bit:      2  Instruction cache enable */
        uint32_t DE: 1;                      /*!< bit:      3  Data cache enable */
        uint32_t WB: 1;                      /*!< bit:      4  Cache write back */
        uint32_t RS: 1;                      /*!< bit:      5  Address return stack settings */
        uint32_t Z: 1;                       /*!< bit:      6  Allow predictive jump bit */
        uint32_t BE: 1;                      /*!< bit:      7  Endian mode */
        uint32_t SCK: 3;                     /*!< bit:  8..10  the clock ratio of the system and the processor */
        uint32_t _reserved0: 1;              /*!< bit:     11  Reserved */
        uint32_t WA: 1;                      /*!< bit:     12  Write allocate enable */
        uint32_t E_V2: 1;                    /*!< bit:     13  V2 Endian mode */
        uint32_t BSTE: 1;                    /*!< bit:     14  Burst transmit enable */
        uint32_t IPE: 1;                     /*!< bit:     15  Indirect predict enable */
        uint32_t _reserved1: 16;             /*!< bit: 16..31  Reserved */
    } b;                                   /*!< Structure    Access by bit */
    uint32_t w;                            /*!< Type         Access by whole register */
} CCR_Type;

/* CCR Register Definitions */
#define CCR_IPE_Pos                       15u                                            /*!< CCR: IPE Position */
#define CCR_IPE_Msk                       (0x1UL << CCR_IPE_Pos)                         /*!< CCR: IPE Mask */

#define CCR_BSTE_Pos                      14u                                            /*!< CCR: BSTE Position */
#define CCR_BSTE_Msk                      (0x1UL << CCR_BSTE_Pos)                        /*!< CCR: BSTE Mask */

#define CCR_E_V2_Pos                      13U                                            /*!< CCR: E_V2 Position */
#define CCR_E_V2_Msk                      (0x1UL << CCR_E_V2_Pos)                        /*!< CCR: E_V2 Mask */

#define CCR_WA_Pos                        12u                                            /*!< CCR: WA Position */
#define CCR_WA_Msk                        (0x1UL << CCR_WA_Pos)                          /*!< CCR: WA Mask */

#define CCR_SCK_Pos                       8U                                             /*!< CCR: SCK Position */
#define CCR_SCK_Msk                       (0x3UL << CCR_SCK_Pos)                         /*!< CCR: SCK Mask */

#define CCR_BE_Pos                        7U                                             /*!< CCR: BE Position */
#define CCR_BE_Msk                        (0x1UL << CCR_BE_Pos)                          /*!< CCR: BE Mask */

#define CCR_Z_Pos                         6U                                             /*!< CCR: Z Position */
#define CCR_Z_Msk                         (0x1UL << CCR_BE_Pos)                          /*!< CCR: Z Mask */

#define CCR_RS_Pos                        5U                                             /*!< CCR: RS Position */
#define CCR_RS_Msk                        (0x1UL << CCR_BE_Pos)                          /*!< CCR: RS Mask */

#define CCR_WB_Pos                        4U                                             /*!< CCR: WB Position */
#define CCR_WB_Msk                        (0x1UL << CCR_BE_Pos)                          /*!< CCR: WB Mask */

#define CCR_DE_Pos                        3U                                             /*!< CCR: DE Position */
#define CCR_DE_Msk                        (0x1UL << CCR_BE_Pos)                          /*!< CCR: DE Mask */

#define CCR_IE_Pos                        2U                                             /*!< CCR: IE Position */
#define CCR_IE_Msk                        (0x1UL << CCR_BE_Pos)                          /*!< CCR: IE Mask */

#define CCR_MP_Pos                        0U                                             /*!< CCR: MP Position */
#define CCR_MP_Msk                        (0x3UL << CCR_MP_Pos)                          /*!< CCR: MP Mask */

/**
  \brief  Consortium definition for accessing mmu index register(MIR,CR<0,15>).
 */
typedef union {
    struct {
        uint32_t Index: 10;                  /*!< bit:   0.. 9   TLB index */
        uint32_t _reserved: 20;              /*!< bit:  10.. 29  Reserved */
        uint32_t TF: 1;                      /*!< bit:       30  TLB fatal error */
        uint32_t P: 1;                       /*!< bit:       31  TLBP instruction */
    } b;
    uint32_t w;
} MIR_Type;

/* MIR Register Definitions */
#define MIR_P_Pos                          31                                            /*!< PRSR: P(TLBP instruction) Position */
#define MIR_P_Msk                          (0x1UL << MIR_P_Pos)                          /*!< PRSR: P(TLBP instruction) Mask */

#define MIR_TF_Pos                         30                                            /*!< PRSR: Tfatal Position */
#define MIR_TF_Msk                         (0x1UL << MIR_TF_Pos)                         /*!< PRSR: Tfatal Mask */

#define MIR_Index_Pos                      0                                             /*!< PRSR: Index Position */
#define MIR_Index_Msk                      (0x3ffUL << MIR_Index_Pos)                    /*!< PRSR: Index Mask */


/**
  \brief  Consortium definition for accessing mmu entry of high physical address register(MEL, CR<2,15> and CR<3,15>).
 */
typedef union {
    struct {
        uint32_t G: 1;                        /*!< bit:       0   Global enbale bit */
        uint32_t V: 1;                        /*!< bit:       1   TLB mapping valid bit */
        uint32_t D: 1;                        /*!< bit:       2   TLB Page dirty bit */
        uint32_t C: 1;                        /*!< bit:       3   TLB Page cacheable bit */
        uint32_t SEC: 1;                      /*!< bit:       4   TLB Page security bit */
        uint32_t SO: 1;                       /*!< bit:       2   Strong order enable bit */
        uint32_t B: 1;                        /*!< bit:       2   TLB Page bufferable bit */
        uint32_t _reserved: 5;                /*!< bit:   7.. 11  Reserved */
        uint32_t PFN: 20;                     /*!< bit:  12.. 31  Physical frame number */
    } b;
    uint32_t w;
} MEL_Type;

/* MEL Register Definitions */
#define MEL_PFN_Pos                        12                                            /*!< MEL: PFN Position */
#define MEL_PFN_Msk                        (0xFFFFFUL << MEL_PFN_Pos)                    /*!< MEL: PFN Mask */

#define MEL_B_Pos                          6                                             /*!< MEL: B Position */
#define MEL_B_Msk                          (0x1UL << MEL_B_Pos)                          /*!< MEL: B Mask */

#define MEL_SO_Pos                         5                                             /*!< MEL: SO Position */
#define MEL_SO_Msk                         (0x1UL << MEL_SO_Pos)                         /*!< MEL: SO Mask */

#define MEL_SEC_Pos                        4                                             /*!< MEL: SEC Position */
#define MEL_SEC_Msk                        (0x1UL << MEL_SEC_Pos)                        /*!< MEL: SEC Mask */

#define MEL_C_Pos                          3                                             /*!< MEL: C Position */
#define MEL_C_Msk                          (0x1UL << MEL_C_Pos)                          /*!< MEL: C Mask */

#define MEL_D_Pos                          2                                             /*!< MEL: D Position */
#define MEL_D_Msk                          (0x1UL << MIR_D_Pos)                          /*!< MEL: D Mask */

#define MEL_V_Pos                          1                                             /*!< MEL: V Position */
#define MEL_V_Msk                          (0x1UL << MIR_V_Pos)                          /*!< MEL: V Mask */

#define MEL_G_Pos                          0                                             /*!< MEL: G Position */
#define MEL_G_Msk                          (0x1UL << MIR_G_Pos)                          /*!< MEL: G Mask */


/**
  \brief  Consortium definition for accessing mmu entry of high physical address register(MEH, CR<4,15>).
 */
typedef union {
    struct {
        uint32_t ASID :8;                     /*!< bit:   0.. 7   ASID */
        uint32_t _reserved :4;                /*!< bit:   7.. 10  Reserved */
        uint32_t VPN :20;                     /*!< bit:  11.. 31  Virtual page number */
    } b;
    uint32_t w;
} MEH_Type;

/* MEH Register Definitions */
#define MEH_VPN_Pos                        12                                             /*!< MEH: VPN Position */
#define MEH_VPN_Msk                        (0xFFFFFUL << MEH_VPN_Pos)                     /*!< MEH: VPN Mask */

#define MEH_ASID_Pos                       0                                              /*!< MEH: ASID Position */
#define MEH_ASID_Msk                       (0xFFUL << MEH_ASID_Pos)                       /*!< MEH: ASID Mask */


/**
  \brief  Consortium definition for accessing mmu entry of high physical address register(MPR, CR<6,15>).
 */
typedef union {
    struct {
        uint32_t _reserved0: 13;              /*!< bit:   0.. 12   Reserved */
        uint32_t page_mask: 12;               /*!< bit:  13.. 24   Page mask */
        uint32_t _reserved1: 7;               /*!< bit:  25.. 31   Reserved */
    } b;
    uint32_t w;
} MPR_Type;

/* MPR Register Definitions */
#define MPR_PAGE_MASK_Pos                        13                                             /*!< MPR: PAGE_MASK Position */
#define MPR_PAGE_MASK_Msk                        (0xFFFUL << MPR_PAGE_MASK_Pos)                 /*!< MPR: PAGE_MASK Mask */


/**
  \brief  Consortium definition for accessing mmu entry of high physical address register(CR<8,15>).
 */
typedef union {
    struct {
        uint32_t ASID: 8;                     /*!< bit:   0.. 7   ASID */
        uint32_t _reserved: 17;               /*!< bit:   8.. 24  Reserved */
        uint32_t TLBINV_INDEX: 1;             /*!< bit:       25  TLBINV_INDEX */
        uint32_t TLBINV_ALL: 1;               /*!< bit:       26  TLBINV_ALL */
        uint32_t TLBINV: 1;                   /*!< bit:       27  TLBINV */
        uint32_t TLBWR: 1;                    /*!< bit:       28  TLBWR */
        uint32_t TLBWI: 1;                    /*!< bit:       29  TLBWI */
        uint32_t TLBR: 1;                     /*!< bit:       30  TLBR */
        uint32_t TLBP: 1;                     /*!< bit:       31  TLBP */
    } b;
    uint32_t w;
} MCIR_Type;

/* MCIR Register Definitions */
#define MCIR_TLBP_Pos                        31                                               /*!< MCIR: TLBP Position */
#define MCIR_TLBP_Msk                        (0x1UL << MCIR_TLBP_Pos)                         /*!< MCIR: TLBP Mask */

#define MCIR_TLBR_Pos                        30                                               /*!< MCIR: TLBR Position */
#define MCIR_TLBR_Msk                        (0x1UL << MCIR_TLBR_Pos)                         /*!< MCIR: TLBR Mask */

#define MCIR_TLBWI_Pos                       29                                               /*!< MCIR: TLBWI Position */
#define MCIR_TLBWI_Msk                       (0x1UL << MCIR_TLBWI_Pos)                        /*!< MCIR: TLBWI Mask */

#define MCIR_TLBWR_Pos                       28                                               /*!< MCIR: TLBWR Position */
#define MCIR_TLBWR_Msk                       (0x1UL << MCIR_TLBWR_Pos)                        /*!< MCIR: TLBWR Mask */

#define MCIR_TLBINV_Pos                      27                                               /*!< MCIR: TLBINV Position */
#define MCIR_TLBINV_Msk                      (0x1UL << MCIR_TLBINV_Pos)                       /*!< MCIR: TLBINV Mask */

#define MCIR_TLBINV_ALL_Pos                  26                                               /*!< MCIR: TLBINV_ALL Position */
#define MCIR_TLBINV_ALL_Msk                  (0x1UL << MCIR_TLBINV_ALL_Pos)                   /*!< MCIR: TLBINV_ALL Mask */

#define MCIR_TLBINV_INDEX_Pos                25                                               /*!< MCIR: TLBINV_INDEX Position */
#define MCIR_TLBINV_INDEX_Msk                (0x1UL << MCIR_TLBINV_INDEX_Pos)                 /*!< MCIR: TLBINV_INDEX Mask */

#define MCIR_ASID_Pos                        0                                                /*!< MCIR: ASID Position */
#define MCIR_ASID_Msk                        (0xFFUL << MCIR_ASID_Pos)                        /*!< MCIR: ASID Mask */


/*@} end of group CSI_CORE */


/**
  \ingroup    CSI_core_register
  \defgroup   CSI_CACHE
  \brief      Type definitions for the cache Registers
  @{
 */

/**
  \brief  Consortium definition for accessing protection area selection register(CFR,CR<17,0>).
 */
typedef union {
    struct {
        uint32_t CACHE_SEL: 2;                      /*!< bit:  0..1  Instruction and data cache selection */
        uint32_t _reserved0: 2;                     /*!< bit:  2..3  Reserved */
        uint32_t INV: 1;                            /*!< bit:     4  Invalid data in cache */
        uint32_t CLR: 1;                            /*!< bit:     5  Clear the dirty tlb table */
        uint32_t OMS: 1;                            /*!< bit:     6  Cache invalid and clear operation mode (one line or all line)*/
        uint32_t ITS: 1;                            /*!< bit:     7  Cache invalid and clear operation mode (CIR used as virtual index or SET/WAY/LEVE index)*/
        uint32_t UNLOCK: 1;                         /*!< bit:     8  Unclock data cache line. */
        uint32_t _reserved1: 7;                     /*!< bit: 9..15  Reserved */
        uint32_t BHT_INV: 1;                        /*!< bit:    16  Invalid data in branch history table */
        uint32_t BTB_INV: 1;                        /*!< bit:    17  Invalid data in branch table buffer */
        uint32_t _reserved2: 13;                    /*!< bit: 18..30 Reserved */
        uint32_t LICF: 1;                           /*!< bit:     31 Failure of clearing or invalid cache line */
    } b;                                            /*!< Structure    Access by bit */
    uint32_t w;                                     /*!< Type         Access by whole register */
} CFR_Type;

#define CFR_LICF_Pos                     31U                                             /*!< CFR: LICF Position */
#define CFR_LICF_Msk                     (0x1UL << CFR_LICF_Pos)                         /*!< CFR: LICF Mask */

#define CFR_BTB_INV_Pos                  17U                                             /*!< CFR: BTB Position */
#define CFR_BTB_INV_Msk                  (0x1UL << CFR_BTB_INV_Pos)                      /*!< CFR: BTB Mask */

#define CFR_BHT_INV_Pos                  16U                                             /*!< CFR: BHT Position */
#define CFR_BHT_INV_Msk                  (0x1UL << CFR_BHT_INV_Pos)                      /*!< CFR: BHT Mask */

#define CFR_UNLOCK_Pos                   8U                                              /*!< CFR: UNLOCK Position */
#define CFR_UNLOCK_Msk                   (0x1UL << CFR_UNLOCK_Pos)                       /*!< CFR: UNLOCK Mask */

#define CFR_ITS_Pos                      7U                                              /*!< CFR: ITS Position */
#define CFR_ITS_Msk                      (0x1UL << CFR_ITS_Pos)                          /*!< CFR: ITS Mask */

#define CFR_OMS_Pos                      6U                                              /*!< CFR: OMS Position */
#define CFR_OMS_Msk                      (0x1UL << CFR_OMS_Pos)                          /*!< CFR: OMS Mask */

#define CFR_CLR_Pos                      5U                                              /*!< CFR: CLR Position */
#define CFR_CLR_Msk                      (0x1UL << CFR_CLR_Pos)                          /*!< CFR: CLR Mask */

#define CFR_INV_Pos                      4U                                              /*!< CFR: INV Position */
#define CFR_INV_Msk                      (0x1UL << CFR_INV_Pos)                          /*!< CFR: INV Mask */

#define CFR_CACHE_SEL_Pos                0                                               /*!< CFR: CACHE_SEL Position */    
#define CFR_CACHE_SEL_Msk                (0x3UL << CFR_CACHE_SEL_Pos)                    /*!< CFR: CACHE_SEL Masok */

/* CFR Register Definitions */
/*@} end of group CSI_CACHE */


/**
  \ingroup    CSI_core_register
  \defgroup   CSI_CACHE
  \brief      Type definitions for the cache Registers
  @{
 */
#define SSEG0_BASE_ADDR         0x80000000
#define CACHE_RANGE_MAX_SIZE    0x80000

#define INS_CACHE               (1 << 0)
#define DATA_CACHE              (1 << 1)
#define CACHE_INV               (1 << 4)
#define CACHE_CLR               (1 << 5)
#define CACHE_OMS               (1 << 6)
#define CACHE_ITS               (1 << 7)
#define CACHE_LICF              (1 << 31)

#define    L1_CACHE_SHIFT       4      /* 16 Bytes */
#define    L1_CACHE_BYTES       (1 << L1_CACHE_SHIFT)

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


/*******************************************************************************
 *                Hardware Abstraction Layer
  Core Function Interface contains:
  - Core VIC Functions
  - Core CORET Functions
  - Core Register Access Functions
 ******************************************************************************/

/* The following MACROS handle generation of the register offset and byte masks */
#define _BIT_SHIFT(IRQn)         (  ((((uint32_t)(int32_t)(IRQn))         )      &  0x03UL) * 8UL)
#define _IP_IDX(IRQn)            (   (((uint32_t)(int32_t)(IRQn))                >>    2UL)      )


/* ##########################  Cache functions  #################################### */
/**
  \ingroup  CSI_Core_FunctionInterface
  \defgroup CSI_Core_CacheFunctions Cache Functions
  \brief    Functions that configure Instruction and Data cache.
  @{
 */

/**
  \brief   Enable I-Cache
  \details Turns on I-Cache
  */
__ALWAYS_INLINE void csi_icache_enable (void)
{
   __set_CCR(__get_CCR() | 0x00000004);
}


/**
  \brief   Disable I-Cache
  \details Turns off I-Cache
  */
__ALWAYS_INLINE void csi_icache_disable (void)
{
   __set_CCR(__get_CCR() & 0xFFFFFFFB);
}


/**
  \brief   Invalidate I-Cache
  \details Invalidates I-Cache
  */
__ALWAYS_INLINE void csi_icache_invalid (void)
{
    __set_CFR(0x11);
    __set_CFR(INS_CACHE | CACHE_INV);
}


/**
  \brief   Enable D-Cache
  \details Turns on D-Cache
  \note    I-Cache also turns on.
  */
__ALWAYS_INLINE void csi_dcache_enable (void)
{
   __set_CCR(__get_CCR() | 0x00000008);
}


/**
  \brief   Disable D-Cache
  \details Turns off D-Cache
  \note    I-Cache also turns off.
  */
__ALWAYS_INLINE void csi_dcache_disable (void)
{
   __set_CCR(__get_CCR() & 0xFFFFFFF7);
}


/**
  \brief   Invalidate D-Cache
  \details Invalidates D-Cache
  \note    I-Cache also invalid
  */
__ALWAYS_INLINE void csi_dcache_invalid (void)
{
    __set_CFR(DATA_CACHE | CACHE_INV);
}


/**
  \brief   Clean D-Cache
  \details Cleans D-Cache
  \note    I-Cache also cleans
  */
__ALWAYS_INLINE void csi_dcache_clean (void)
{
    __set_CFR(DATA_CACHE | CACHE_CLR);
}

/**
  \brief   Clean & Invalidate D-Cache
  \details Cleans and Invalidates D-Cache
  \note    I-Cache also flush.
  */
__ALWAYS_INLINE void csi_dcache_clean_invalid (void)
{
    __set_CFR(DATA_CACHE | CACHE_CLR | CACHE_INV);
}

__ALWAYS_INLINE  void set_cache_range (uint32_t start, uint32_t end, uint32_t value)
{
    if (!(start & SSEG0_BASE_ADDR) || (end - start) &~(CACHE_RANGE_MAX_SIZE - 1)) {
        __set_CFR(value);
    }

    if (value & INS_CACHE) {
        csi_icache_disable();
    }
    uint32_t i;
    for (i = start; i < end; i += L1_CACHE_BYTES) {
        __set_CIR(i);
        __set_CFR(CACHE_OMS | value);
    }

    if (end & (L1_CACHE_BYTES-1)) {
        __set_CIR(end);
        __set_CFR(CACHE_OMS | value);
    }

    if (value & INS_CACHE) {
        csi_icache_enable();
    }

}

/**
  \brief   D-Cache Invalidate by address
  \details Invalidates D-Cache for the given address
  \param[in]   addr    address (aligned to 16-byte boundary)
  \param[in]   dsize   size of memory block (in number of bytes)
*/
__ALWAYS_INLINE void csi_dcache_invalid_range (uint32_t *addr, int32_t dsize)
{
    set_cache_range((uint32_t)addr, (uint32_t)addr + dsize, (DATA_CACHE | CACHE_INV));
}


/**
  \brief   D-Cache Clean by address
  \details Cleans D-Cache for the given address
  \param[in]   addr    address (aligned to 16-byte boundary)
  \param[in]   dsize   size of memory block (in number of bytes)
*/
__ALWAYS_INLINE void csi_dcache_clean_range (uint32_t *addr, int32_t dsize)
{
    set_cache_range((uint32_t)addr, (uint32_t)addr + dsize, (DATA_CACHE | CACHE_CLR));
}


/**
  \brief   D-Cache Clean and Invalidate by address
  \details Cleans and invalidates D_Cache for the given address
  \param[in]   addr    address (aligned to 16-byte boundary)
  \param[in]   dsize   size of memory block (in number of bytes)
*/
__ALWAYS_INLINE void csi_dcache_clean_invalid_range (uint32_t *addr, int32_t dsize)
{
    set_cache_range((uint32_t)addr, (uint32_t)addr + dsize, (DATA_CACHE | CACHE_CLR | CACHE_INV));
}


/*@} end of CSI_Core_CacheFunctions */

/* ##########################  MMU functions  #################################### */
/**
  \ingroup  CSI_Core_FunctionInterface
  \defgroup CSI_Core_MMUFunctions MMU Functions
  \brief    Functions that configure MMU.
  @{
 */

typedef struct {
    uint32_t global: 1;         /* tlb page global access. */
    uint32_t valid: 1;          /* tlb page valid */
    uint32_t writeable: 1;      /* tlb page writeable */
    uint32_t cacheable: 1;      /* tlb page cacheable*/
    uint32_t is_secure: 1;      /* tlb page security access */
    uint32_t strong_order: 1;   /* the sequence of accessing data on tlb page is corresponding to the program flow? */
    uint32_t bufferable: 1;     /* tlb page bufferable */
} page_attr_t;

typedef enum {
    PAGE_SIZE_4KB   = 0x000,
    PAGE_SIZE_16KB  = 0x003,
    PAGE_SIZE_64KB  = 0x00F,
    PAGE_SIZE_256KB = 0x03F,
    PAGE_SIZE_1MB   = 0x0FF,
    PAGE_SIZE_4MB   = 0x3FF,
    PAGE_SIZE_16MB  = 0xFFF
} page_size_e;


/**
  \brief  enable mmu
  \details
  */
__ALWAYS_INLINE void csi_mmu_enable(void)
{
    __set_CCR(__get_CCR() | (1u << CCR_MP_Pos));
}

/**
  \brief  disable mmu
  \details
  */
__ALWAYS_INLINE void csi_mmu_disable(void)
{
    __set_CCR(__get_CCR() & (~(1u << CCR_MP_Pos)));
}

/**
  \brief  create page with feature.
  \details
  \param [in]  vaddr     virtual address.
  \param [in]  paddr     physical address.
  \param [in]  asid      address sapce id (default: 0).
  \param [in]  attr      \ref page_attr_t. tlb page attribute.
  */
__ALWAYS_INLINE void csi_mmu_set_tlb(uint32_t vaddr, uint32_t paddr, uint32_t asid, page_attr_t attr)
{
     MPR_Type pgmask;
     MEH_Type meh;
     MEL_Type mel;
     uint32_t vaddr_bit;
     uint32_t page_feature = 0;

     page_feature |= attr.global << MEL_G_Pos | attr.valid << MEL_V_Pos |
                     attr.writeable << MEL_D_Pos | attr.cacheable << MEL_C_Pos |
                     attr.is_secure << MEL_SEC_Pos | attr.strong_order << MEL_SO_Pos |
                     attr.bufferable << MEL_B_Pos;

     pgmask.w = __get_MPR();
     vaddr_bit = 44 - __FF0(~((uint32_t)pgmask.b.page_mask));

     meh.b.ASID = (uint8_t)asid;
     meh.b.VPN  = (vaddr & ((~pgmask.w | 0xFE000000) & 0xFFFFE000)) >> MEH_VPN_Pos;
     __set_MEH(meh.w);

     __set_MCIR(1u << MCIR_TLBP_Pos);

     mel.w = ((paddr & ~(pgmask.b.page_mask << 12)) | page_feature);
     if (vaddr & (1 << vaddr_bit)) {
         __set_MEL1(mel.w);
     }
     else {
         __set_MEL0(mel.w);
     }

     if (__get_MIR() & (1 << MIR_P_Pos)) {
        __set_MCIR(1u << MCIR_TLBWR_Pos);
     } else {
        __set_MCIR(1u << MCIR_TLBWI_Pos);
     }
}


/**
  \brief  enble mmu
  \details
  \param [in]  size  tlb page size.
  */
__ALWAYS_INLINE void csi_mmu_set_pagesize(page_size_e size)
{
    MPR_Type pgmask;
    pgmask.b.page_mask = size;
    __set_MPR(pgmask.w);
}


/**
  \brief  read MEH, MEL0, MEL1 by tlb index.
  \details
  \param [in]    index  tlb index(0, 1, 2, ...)
  \param [out]   meh    pointer to variable for retrieving MEH.
  \param [out]   mel0   pointer to variable for retrieving MEL0.
  \param [out]   mel1   pointer to variable for retrieving MEL1.
  */
__ALWAYS_INLINE void csi_mmu_read_by_index(uint32_t index, uint32_t *meh, uint32_t *mel0, uint32_t *mel1)
{
    MIR_Type mir;

    if (meh == NULL || mel0 == NULL || mel1 == NULL) {
        return;
    }

    mir.b.Index = index;
    __set_MIR(mir.w);
    __set_MCIR(1u << MCIR_TLBR_Pos);

    *meh = __get_MEH();
    *mel0 = __get_MEL0();
    *mel1 = __get_MEL1();
}


/**
  \brief  flush all mmu tlb.
  \details
  */
__ALWAYS_INLINE void csi_mmu_invalid_tlb_all(void)
{
    __set_MCIR(1u << MCIR_TLBINV_ALL_Pos);
}


/**
  \brief  flush mmu tlb by index.
  \details
  */
__ALWAYS_INLINE void csi_mmu_invalid_tlb_by_index(uint32_t index)
{
    MIR_Type mir;

    mir.b.Index = index;
    __set_MIR(mir.w);
    __set_MCIR(1u << MCIR_TLBINV_INDEX_Pos);
}


/**
  \brief  flush mmu tlb by virtual address.
  \details
  */
__ALWAYS_INLINE void csi_mmu_invalid_tlb_by_vaddr(uint32_t vaddr, uint32_t asid)
{
    __set_MEH(vaddr | (asid & MEH_ASID_Msk));
    __set_MCIR(1u << MCIR_TLBP_Pos);

    if (__get_MIR() & (1 << MIR_P_Pos)) {
        return;
    } else {
        __set_MCIR(1u << MCIR_TLBINV_INDEX_Pos);
    }
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

#ifdef __cplusplus
}
#endif

#endif /* __CORE_CK810_H_DEPENDANT */

#endif /* __CSI_GENERIC */
