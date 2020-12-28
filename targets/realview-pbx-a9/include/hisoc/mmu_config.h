/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Memory Management Unit HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2020-12-10
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

/**
 * @defgroup mmu Memory management unit
 *  @ingroup mmu
*/

#ifndef _MMU_CONFIG_H
#define _MMU_CONFIG_H

#include "board.h"
#include "los_config.h"
#include "los_memory.h"
#include "arch/mmu.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MMU_SET_PAGE_TABLE_PLACE  \
__attribute__((aligned(MMU_16K))) __attribute__((section(".bss.prebss.translation_table"))) \
UINT8 first_page_table[MMU_16K]; \
__attribute__((aligned(MMU_1K))) UINT8 second_page_table_os[MMU_16K]; \
__attribute__((aligned(MMU_1K))) UINT8 second_page_table_app[MMZ_MEM_LEN / MMU_1K];

/**
 * @ingroup mmu
 * The liteos cache addr & length
 */
#define LITEOS_CACHE_ADDR             SYS_MEM_BASE
#define LITEOS_CACHE_LENGTH           (g_sys_mem_addr_end - LITEOS_CACHE_ADDR)

/**
 * @ingroup mmu
 * The page table storage addr
 * notice: must ensure it has enough free mem for storage page table
 */
extern UINT8 first_page_table[];
extern UINT8 second_page_table_os[];
extern UINT8 second_page_table_app[];

#define FIRST_PAGE_DESCRIPTOR_ADDR    ((UINTPTR)first_page_table)
#define stAppPage                     g_mmuAppPage

#define X_MMU_SECOND_TABLE_OS_PAGE_SET() do {                                                               \
    g_mmuOsPage.page_addr = SYS_MEM_BASE;                                                                   \
    g_mmuOsPage.page_length = ((((UINTPTR)&__ram_data_start - SYS_MEM_BASE) + MMU_1M - 1) & ~(MMU_1M - 1)); \
    g_mmuOsPage.page_descriptor_addr = (UINTPTR)second_page_table_os;                                       \
    g_mmuOsPage.page_type = MMU_SECOND_LEVEL_SMALL_PAGE_TABLE_ID;                                           \
    if (g_mmuOsPage.page_length > (sizeof(second_page_table_os) << 10)) {                                   \
        PRINT_ERR("%s,%d\n", __FUNCTION__,__LINE__);                                                        \
        PRINT_ERR("the mapping size of os second page is 0x%x, should be not bigger than 0x%x\n",           \
                  g_mmuOsPage.page_length,(sizeof(second_page_table_os) << 10));                            \
        return;                                                                                             \
    }                                                                                                       \
    ArchSecPageEnable(&g_mmuOsPage, BUFFER_ENABLE | CACHE_ENABLE | ACCESS_PERM_RW_RW);                      \
} while (0)

#define X_MMU_SECOND_TABLE_APP_PAGE_SET() do {                                          \
    stAppPage.page_addr = MMZ_MEM_BASE;                                                 \
    stAppPage.page_length = MMZ_MEM_LEN;                                                \
    stAppPage.page_descriptor_addr = (UINTPTR)second_page_table_app;                    \
    stAppPage.page_type = MMU_SECOND_LEVEL_SMALL_PAGE_TABLE_ID;                         \
    ArchSecPageEnable(&stAppPage, BUFFER_DISABLE | CACHE_DISABLE | ACCESS_PERM_RW_RW);  \
} while (0)

/* table start position + offset = 'addr' table item position */
#define MMU_GET_FIRST_TABLE_ADDR(addr)    (((addr) / MMU_1M) * 4 + FIRST_PAGE_DESCRIPTOR_ADDR)
/* get item content which storaged by table */
#define MMU_GET_FIRST_TABLE_ITEM(addr)    (*(UINT32 *)MMU_GET_FIRST_TABLE_ADDR(addr))
/* if the first item ID is MMU_FIRST_LEVEL_PAGE_TABLE_ID, get second table item addr by hi 22bits */
#define MMU_GET_SECOND_TABLE_BASE(addr)   ((MMU_GET_FIRST_TABLE_ITEM(addr)) & 0xfffffc00)
/* second table item offset */
#define MMU_GET_SECOND_TABLE_OFFSET(addr) (((addr) % MMU_1M) / MMU_4K * 4)
/* second table item address */
#define MMU_GET_SECOND_TABLE_ADDR(addr)   (MMU_GET_SECOND_TABLE_BASE(addr) + MMU_GET_SECOND_TABLE_OFFSET(addr))

#define CODE_PROTECT do {                                            \
    MMU_PARAM mPara;                                                 \
    /* note: must confirm that every addr be aglined as 4K(64K) */   \
    mPara.startAddr = (UINTPTR)&__text_start;                        \
    mPara.endAddr = (UINTPTR)&__ram_data_start;                      \
    mPara.uwFlag = BUFFER_ENABLE | CACHE_ENABLE | ACCESS_PERM_RO_RO; \
    mPara.stPage = (SENCOND_PAGE *)&g_mmuOsPage;                     \
    ArchMMUParamSet(&mPara);                                         \
} while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
