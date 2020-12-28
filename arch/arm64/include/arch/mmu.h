/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Arm64 Mmu Inner HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2019-01-01
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#ifndef _ARCH_MMU_H
#define _ARCH_MMU_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define BM(base, count, val) (((UINT64)(val) & (((UINT64)1UL << (count)) - 1)) << (base))

#define MMU_SH_NON_SHAREABLE   0
#define MMU_SH_OUTER_SHAREABLE 2
#define MMU_SH_INNER_SHAREABLE 3

#define MMU_RGN_NON_CACHEABLE             0
#define MMU_RGN_WRITE_BACK_ALLOCATE       1
#define MMU_RGN_WRITE_THROUGH_NO_ALLOCATE 2
#define MMU_RGN_WRITE_BACK_NO_ALLOCATE    3

#define MMU_MAIR_ATTR(index, attr) BM((index) * 8, 8, (attr))

/* L0/L1/L2/L3 descriptor types */
#define MMU_PTE_DESCRIPTOR_INVALID BM(0, 2, 0)

/* L0/L1/L2 descriptor types */
#define MMU_PTE_L012_DESCRIPTOR_BLOCK BM(0, 2, 1)
#define MMU_PTE_L012_DESCRIPTOR_TABLE BM(0, 2, 3)

/* L3 descriptor types */
#define MMU_PTE_L3_DESCRIPTOR_PAGE BM(0, 2, 3)

/* Block/Page attrs */
#define MMU_PTE_ATTR_RES_SOFTWARE BM(55, 4, 0xf)
#define MMU_PTE_ATTR_UXN          BM(54, 1, 1)
#define MMU_PTE_ATTR_PXN          BM(53, 1, 1)
#define MMU_PTE_ATTR_CONTIGUOUS   BM(52, 1, 1)

#define MMU_PTE_ATTR_NON_GLOBAL BM(11, 1, 1)
#define MMU_PTE_ATTR_AF         BM(10, 1, 1)

#define MMU_PTE_ATTR_SH_NON_SHAREABLE   BM(8, 2, 0)
#define MMU_PTE_ATTR_SH_OUTER_SHAREABLE BM(8, 2, 2)
#define MMU_PTE_ATTR_SH_INNER_SHAREABLE BM(8, 2, 3)

#define MMU_PTE_ATTR_AP_P_RW_U_NA BM(6, 2, 0)
#define MMU_PTE_ATTR_AP_P_RW_U_RW BM(6, 2, 1)
#define MMU_PTE_ATTR_AP_P_RO_U_NA BM(6, 2, 2)
#define MMU_PTE_ATTR_AP_P_RO_U_RO BM(6, 2, 3)
#define MMU_PTE_ATTR_AP_MASK      BM(6, 2, 3)

#define MMU_PTE_ATTR_NON_SECURE BM(5, 1, 1)

#define MMU_PTE_ATTR_ATTR_INDEX(attrindex) BM(2, 3, (attrindex))
#define MMU_PTE_ATTR_ATTR_INDEX_MASK       MMU_PTE_ATTR_ATTR_INDEX(7)

/* Device-nGnRnE memory */
#define MMU_PTE_ATTR_STRONGLY_ORDERED MMU_PTE_ATTR_ATTR_INDEX(0)

/* Device-nGnRE memory */
#define MMU_PTE_ATTR_DEVICE MMU_PTE_ATTR_ATTR_INDEX(1)

/* Normal Memory, Outer Write-back, Inner Write-back */
#define MMU_PTE_ATTR_NORMAL_CACHE MMU_PTE_ATTR_ATTR_INDEX(2)

/* Normal Memory, Outer Non-cacheable,Inner Non-cacheable */
#define MMU_PTE_ATTR_NORMAL_NONCACHE MMU_PTE_ATTR_ATTR_INDEX(3)

#define MMU_PTE_CACHE_RO_FLAGS                                                                                      \
    (MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE | MMU_PTE_ATTR_NORMAL_CACHE | MMU_PTE_ATTR_AP_P_RO_U_NA)

#define MMU_PTE_CACHE_RW_FLAGS                                                                                      \
    (MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE | MMU_PTE_ATTR_NORMAL_CACHE | MMU_PTE_ATTR_AP_P_RW_U_NA)

#define MMU_PTE_CACHE_RW_XN_FLAGS                                                                                   \
    (MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE |                      \
     MMU_PTE_ATTR_NORMAL_CACHE | MMU_PTE_ATTR_AP_P_RW_U_NA)

#define MMU_PTE_NONCACHE_RO_FLAGS                                                                                   \
    (MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE | MMU_PTE_ATTR_NORMAL_NONCACHE | MMU_PTE_ATTR_AP_P_RO_U_NA)

#define MMU_PTE_NONCACHE_RW_FLAGS                                                                                   \
    (MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE | MMU_PTE_ATTR_NORMAL_NONCACHE | MMU_PTE_ATTR_AP_P_RW_U_NA)

#define MMU_PTE_NONCACHE_RW_XN_FLAGS                                                                                \
    (MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_SH_INNER_SHAREABLE |                      \
     MMU_PTE_ATTR_NORMAL_NONCACHE | MMU_PTE_ATTR_AP_P_RW_U_NA)

#define MMU_INITIAL_MAP_STRONGLY_ORDERED                                                                            \
    (MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_STRONGLY_ORDERED |                        \
     MMU_PTE_ATTR_AP_P_RW_U_NA)

#define MMU_INITIAL_MAP_DEVICE                                                                                      \
    (MMU_PTE_ATTR_UXN | MMU_PTE_ATTR_PXN | MMU_PTE_ATTR_AF | MMU_PTE_ATTR_DEVICE | MMU_PTE_ATTR_AP_P_RW_U_NA)

#ifndef PAGE_SIZE
#define PAGE_SIZE MMU_4K
#endif

/**
 * @ingroup mmu
 * mmu second page information structure.
 *
 */
typedef struct {
    UINTPTR page_addr;            /**< The second page start addr */
    UINTPTR page_length;          /**< The second page length */
    UINTPTR page_descriptor_addr; /**< The second page page table storage addr,
                                       diff second page table can't be coincided */
    UINTPTR page_type;            /**< The second page type, it can be set small page ID(4K):
                                       MMU_SECOND_LEVEL_SMALL_PAGE_TABLE_ID
                                       or big page ID(64K):MMU_SECOND_LEVEL_BIG_PAGE_TABLE_ID */
} SENCOND_PAGE;

/**
 * @ingroup mmu
 * The first section(2M/item).
 */
#define PMD_AREA 0xdaea

/**
 * @ingroup mmu
 * The second page(4K/item).
 */
#define PTE_AREA 0

/**
 * @ingroup mmu
 * mmu param setting information structure.
 *
 */
typedef struct {
    UINTPTR startAddr; /**< Starting address of a section.     */
    UINTPTR endAddr;   /**< Ending address of a section.     */
    UINTPTR uwFlag;    /* <   mode set.
                                                MMU_PTE_CACHE_RO_FLAGS,
                                                MMU_PTE_CACHE_RW_FLAGS,
                                                MMU_PTE_NONCACHE_RO_FLAGS,
                                                MMU_PTE_NONCACHE_RW_FLAGS,
                                                MMU_INITIAL_MAP_STRONGLY_ORDERED,
                                                MMU_INITIAL_MAP_DEVICE
                                                and so on */
    UINTPTR uwArea;
    SENCOND_PAGE
    *stPage; /* <  the goal object of second page, if uwFlag bit3 is FIRST_SECTION,
                   stPage will be ignored, and you can set this member as NULL */
} MMU_PARAM;


/**
 * @ingroup mmu
 * @brief Memory Management Unit Second page memory map.
 *
 * @par Description:
 * This API is used to set the second page memory map of a section that is specified by
 * a starting address and ending address.
 * @attention
 * <ul>
 * <li>The passed-in starting address and ending address must be aligned on a boundary of 1M.
 * The access permission mode can be only set to ACCESS_PERM_RO_RO and ACCESS_PERM_RW_RW.
 * </li>
 * </ul>
 *
 * @param SENCOND_PAGE  [IN] param for second page enable, the struct contains below members:
 * page_addr:               starting address of the section.
 * page_length:             length of the section.
 * page_descriptor_addr:    second page descriptor address for the section.
 * page_type:               the second page type, MMU_SECOND_LEVEL_SMALL_PAGE_TABLE_ID
 *                              or MMU_SECOND_LEVEL_BIG_PAGE_TABLE_ID.
 * @param flag          [IN] param for second page enable, the mode of the section.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>mmu.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V200R005C20
 */
VOID ArchSecPageEnable(SENCOND_PAGE *page, UINT64 flag);

/**
 * @ingroup mmu
 * @brief Memory Management Unit Cache/Buffer/Access Permission Setting.
 *
 * @par Description:
 * This API is used to set the Cache/Buffer/access permission mode of a section
 * that is specified by a starting address and ending address
 * @attention
 * <ul>
 * <li>The passed-in starting address and ending address must be aligned on a boundary of 4K.
 * The access permission mode can be only set to ACCESS_PERM_RO_RO and ACCESS_PERM_RW_RW.</li>
 * </ul>
 *
 * @param MMU_PARAM  [IN] param for mmu setting, the struct contains below members
 * startAddr:             starting address of a section.
 * endAddr:               ending address of a section.
 * uwFlag:                mode set.
 * stPage:                the goal object of second page
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>mmu.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V200R005C20
 */
void ArchMMUParamSet(MMU_PARAM *mPara);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_MMU_H */
