/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: MMU Config Implementation
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */
#include "los_config.h"
#include "hisoc/mmu_config.h"
#include "los_hwi.h"
#include "asm/dma.h"
#ifdef LOSCFG_KERNEL_RUNSTOP
#include "los_runstop_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* This is opration for page table */
MMU_SET_PAGE_TABLE_PLACE

SENCOND_PAGE g_mmuOsPage = {0};
SENCOND_PAGE g_mmuAppPage = {0};

#ifdef LOSCFG_NULL_ADDRESS_PROTECT
__attribute__((aligned(MMU_1K))) UINT32 g_excSecondPageTable[MMU_1K];
SENCOND_PAGE g_excPage = {0};
#define EXC_VECTOR_ADDR 0xFFFF0000
/* EXC_VECTOR_ALIGN equal the value that EXC_VECTOR_ADDR aligned by 1M */
#define EXC_VECTOR_ALIGN 0xFFF00000
#endif

#define ITEM_PRINT_LEN 20
#define BYTES_PER_ITEM 4
#define ITEM_TYPE_MASK 0x3

STATIC VOID OsSetMemPage(MMU_PARAM *para)
{
    UINT32 pageBase;
    UINT32 pageStartIndex, pageEndIndex;
    UINT32 length, bitsCache, bitsBuf, bitsAP;
#if defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A17) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
    UINT32 bitsXn;
#endif
    UINT32 endAddr = para->endAddr;
    UINT32 pageSize = (para->stPage->page_type == MMU_SECOND_LEVEL_BIG_PAGE_TABLE_ID) ? MMU_64K : MMU_4K;
    UINT32 sttBase = para->stPage->page_descriptor_addr;
    UINT32 rshiftBits = (pageSize == MMU_64K) ? SHIFT_64K : SHIFT_4K;

    if ((para->startAddr & (pageSize - 1)) != 0) {
        return;
    }

    if ((endAddr & (pageSize - 1)) != 0) {
        endAddr = ALIGN(endAddr, pageSize);
    }

    pageStartIndex = (para->startAddr - para->stPage->page_addr) >> rshiftBits;
    pageEndIndex = (endAddr - para->stPage->page_addr) >> rshiftBits;
    length = pageEndIndex - pageStartIndex;
#ifdef LOSCFG_NULL_ADDRESS_PROTECT
    if (para->startAddr == EXC_VECTOR_ALIGN) {
        para->startAddr -= EXC_VECTOR_ADDR - SYS_MEM_BASE;
    }
#endif
    bitsAP = MMU_AP_STATE(para->uwFlag);
    bitsCache = MMU_CACHE_STATE(para->uwFlag);
    bitsBuf = MMU_BUFFER_STATE(para->uwFlag);

#ifdef LOSCFG_ARCH_ARM926
    if (pageSize == MMU_64K) {
        pageBase = para->startAddr >> SHIFT_64K;
        X_MMU_TWO_LEVEL_PAGE64K(pageBase, pageStartIndex, length, bitsCache, bitsBuf, bitsAP);
    } else {
        pageBase = para->startAddr >> SHIFT_4K;
        X_MMU_TWO_LEVEL_PAGE(pageBase, pageStartIndex, length, bitsCache, bitsBuf, bitsAP);
    }

#elif defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A17) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
    bitsXn = MMU_EXECUTE_STATE(para->uwFlag);
    if (pageSize == MMU_64K) {
        pageBase = para->startAddr >> SHIFT_64K;
        X_MMU_TWO_LEVEL_PAGE64K(pageBase, pageStartIndex, length, bitsCache, bitsBuf, bitsAP, bitsXn);
    } else {
        pageBase = para->startAddr >> SHIFT_4K;
        X_MMU_TWO_LEVEL_PAGE(pageBase, pageStartIndex, length, bitsCache, bitsBuf, bitsAP, bitsXn);
    }
#endif
}

STATIC UINT32 OsSetMmuFirstSection(const MMU_PARAM *para, UINT32 itemStart, UINT32 itemEnd)
{
    UINT32 intSave;
    UINT32 itemTemp = itemStart;
    UINT32 bitsCache, bitsBuf, bitsAP;
#if defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A17) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
    UINT32 bitsXn = MMU_EXECUTE_STATE(para->uwFlag);
#endif

    bitsAP = MMU_AP_STATE(para->uwFlag);
    bitsCache = MMU_CACHE_STATE(para->uwFlag);
    bitsBuf = MMU_BUFFER_STATE(para->uwFlag);

    while (itemTemp <= itemEnd) {
        if (((*(UINTPTR *)itemTemp) & ITEM_TYPE_MASK) != MMU_FIRST_LEVEL_SECTION_ID) {
            PRINT_ERR("not all mem belongs to first section(1M every item), mmu table ID:%u\n",
                      ((*(UINTPTR *)itemTemp) & ITEM_TYPE_MASK));
            return LOS_NOK;
        }
        itemTemp += sizeof(UINTPTR);
    }

    itemTemp = itemStart;
    intSave = LOS_IntLock();
    DisableAPCheck();
    while (itemTemp <= itemEnd) {
#ifdef LOSCFG_ARCH_ARM926
        SECTION_CHANGE(itemTemp, bitsCache, bitsBuf, bitsAP);
#elif defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A17) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
        SECTION_CHANGE(itemTemp, bitsCache, bitsBuf, bitsAP, bitsXn);
#endif
        itemTemp += sizeof(UINTPTR);
    }
    EnableAPCheck();
    LOS_IntRestore(intSave);
    dma_cache_clean(itemStart, itemEnd);
    return LOS_OK;
}

STATIC UINT32 OsSetMmuSecondPage(MMU_PARAM *para, UINT32 itemStart, UINT32 itemEnd)
{
    UINT32 intSave;
    UINT32 itemTemp = itemStart;
    UINT32 pageStart, pageEnd;

    if ((para->startAddr < para->stPage->page_addr) ||
        (para->endAddr > (para->stPage->page_length + para->stPage->page_addr))) {
        PRINT_ERR("addr input not belongs to this second page \n"
                  "para->startAddr:0x%x, para->stPage->page_addr:0x%x\n",
                  para->startAddr, para->stPage->page_addr);
        PRINT_ERR("para->endAddr:0x%x, (para->stPage->page_length + para->stPage->page_addr):0x%x\n",
                  para->endAddr, para->stPage->page_length + para->stPage->page_addr);
        return LOS_NOK;
    }
    while (itemTemp <= itemEnd) {
        if (((*(UINTPTR *)itemTemp) & ITEM_TYPE_MASK) != MMU_FIRST_LEVEL_PAGE_TABLE_ID) {
            PRINT_ERR("not all mem belongs to second page(4K or 64K every item), mmu table ID:%u \n",
                      ((*(UINTPTR *)itemTemp) & ITEM_TYPE_MASK));
            return LOS_NOK;
        }
        itemTemp += sizeof(UINTPTR);
    }

    intSave = LOS_IntLock();
    DisableAPCheck();
    OsSetMemPage(para);
    EnableAPCheck();
    LOS_IntRestore(intSave);
    pageStart = MMU_GET_SECOND_TABLE_ADDR(para->startAddr);
    pageEnd = MMU_GET_SECOND_TABLE_ADDR(para->endAddr - 1);
    dma_cache_clean(pageStart, pageEnd);
    return LOS_OK;
}

VOID LOS_SecPageEnable(SENCOND_PAGE *page, UINT32 flag)
{
    UINT32 pageStart, pageEnd;
    UINT32 secStart, secEnd;
    UINT32 ttbBase = FIRST_PAGE_DESCRIPTOR_ADDR;
    MMU_PARAM para;

    if (page == NULL) {
        PRINT_ERR("second page table(stPage) can't be NULL\n");
        return;
    }
    para.startAddr = page->page_addr;
    para.endAddr = page->page_addr + page->page_length;
    para.uwFlag = flag;
    para.stPage = page;

    pageStart = page->page_descriptor_addr;
    /* page size = 2 ^ 12, 4K */
    pageEnd = page->page_descriptor_addr + ((page->page_length >> SHIFT_4K) * BYTES_PER_ITEM);
    DisableAPCheck();
    OsSetMemPage(&para);
    dma_cache_clean(pageStart, pageEnd);

    X_MMU_ONE_LEVEL_PAGE(pageStart >> SHIFT_1K, page->page_addr >> SHIFT_1M,
                         page->page_length >> SHIFT_1M, D_CLIENT);

    secStart = ttbBase + ((para.startAddr >> SHIFT_1M) * BYTES_PER_ITEM);
    secEnd = ttbBase + ((para.endAddr >> SHIFT_1M) * BYTES_PER_ITEM);
    dma_cache_clean(secStart, secEnd);
    CleanTLB();
    EnableAPCheck();
}

VOID LOS_MMUParamSet(MMU_PARAM *para)
{
    UINT32 ret;
    UINT32 itemStart, itemEnd;
    UINT32 tableType;

    if (para == NULL) {
        PRINT_ERR("input is null\n");
        return;
    }
    itemStart = MMU_GET_FIRST_TABLE_ADDR(para->startAddr);
    itemEnd = MMU_GET_FIRST_TABLE_ADDR(para->endAddr - 1);
    if (itemStart > itemEnd) {
        PRINT_ERR("wrong addr input, itemStart:0x%x, itemEnd:0x%x\n", itemStart, itemEnd);
        return;
    }

    tableType = MMU_GET_AREA(para->uwFlag);
    if (tableType == SECOND_PAGE) {
        ret = OsSetMmuSecondPage(para, itemStart, itemEnd);
        if (ret == LOS_NOK) {
            return;
        }
    } else if (tableType == FIRST_SECTION) {
        ret = OsSetMmuFirstSection(para, itemStart, itemEnd);
        if (ret == LOS_NOK) {
            return;
        }
    }

    CleanTLB();
}

VOID OsSecPageInit(VOID)
{
    X_MMU_SECOND_TABLE_OS_PAGE_SET();
    X_MMU_SECOND_TABLE_APP_PAGE_SET();
#ifdef LOSCFG_NULL_ADDRESS_PROTECT
    X_MMU_SECOND_TABLE_EXC_PAGE_SET();
#endif
}
VOID OsRemapCached(UINTPTR physAddr, size_t size)
{
    MMU_PARAM para;

    para.startAddr = physAddr;
    para.endAddr = physAddr + size;
#ifdef LOSCFG_ARCH_ARM926
    para.uwFlag = BUFFER_ENABLE | CACHE_ENABLE | ACCESS_PERM_RW_RW;
#elif defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A17) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
    para.uwFlag = BUFFER_ENABLE | CACHE_ENABLE | EXEC_DISABLE | ACCESS_PERM_RW_RW;
#endif
    para.stPage = (SENCOND_PAGE *)&g_mmuAppPage;
    LOS_MMUParamSet(&para);
}

VOID OsRemapNoCached(UINTPTR physAddr, size_t size)
{
    MMU_PARAM para;

    para.startAddr = physAddr;
    para.endAddr = physAddr + size;
#ifdef LOSCFG_ARCH_ARM926
    para.uwFlag = BUFFER_DISABLE | CACHE_DISABLE | ACCESS_PERM_RW_RW;
#elif defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A17) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
    para.uwFlag = BUFFER_DISABLE | CACHE_DISABLE | EXEC_DISABLE | ACCESS_PERM_RW_RW;
#endif
    para.stPage = (SENCOND_PAGE *)&g_mmuAppPage;
    LOS_MMUParamSet(&para);
}

VOID OsCodeProtect(VOID)
{
    CODE_PROTECT;
}

INT32 OsMemNoAccessSet(UINTPTR startaddr, size_t length)
{
    UINTPTR ttbBase = FIRST_PAGE_DESCRIPTOR_ADDR;
    UINTPTR endAddr = startaddr + length;
    UINT32 base;

    if (startaddr >= endAddr) {
        PRINT_ERR("The Input param invalid ,length equal 0 or the configuration scope overflow."
                  "startaddr:0x%x, length:0x%x\n",
                  startaddr, length);
        return -1;
    }

    if ((startaddr >= SYS_MEM_BASE) && (startaddr <= g_sys_mem_addr_end)) {
        PRINT_ERR("The no access permission area sholud not contain os system mem,startaddr:0x%x\n", startaddr);
        return -1;
    }
    if ((endAddr >= SYS_MEM_BASE) && (endAddr <= g_sys_mem_addr_end)) {
        PRINT_ERR("The no access permission area sholud not contain os system mem,endAddr:0x%x\n", endAddr);
        return -1;
    }
    if (((startaddr & (MMU_1M - 1)) != 0) || ((length & (MMU_1M - 1)) != 0)) {
        PRINT_ERR("The start address or the length is not aligned as 1M, startaddr:0x%x, length:0x%x\n", startaddr,
                  length);
        return -1;
    }

    DisableAPCheck();
    base = startaddr >> SHIFT_1M;
#ifdef LOSCFG_ARCH_ARM926
    X_MMU_SECTION(base, base, length >> SHIFT_1M, 0, 0, 0, D_NA);
#elif defined(LOSCFG_ARCH_CORTEX_A7) || defined(LOSCFG_ARCH_CORTEX_A17) || defined(LOSCFG_ARCH_CORTEX_A53_AARCH32)
    X_MMU_SECTION(base, base, length >> SHIFT_1M, 0, 0, 0, 0, D_NA);
#endif
    dma_cache_clean(ttbBase + ((startaddr >> SHIFT_1M) * BYTES_PER_ITEM),
                    ttbBase + ((endAddr >> SHIFT_1M) * BYTES_PER_ITEM));

    CleanTLB();
    EnableAPCheck();
    return 0;
}

VOID OsPrintPageItem(const MMU_PARAM *para)
{
    UINT32 tmp;
    UINT32 startAddr;
    UINT32 pageLen;

    if (para == NULL) {
        return;
    }

    if (MMU_GET_AREA(para->uwFlag) == SECOND_PAGE) {
        startAddr = para->stPage->page_descriptor_addr +
                    (((para->startAddr - para->stPage->page_addr) >> SHIFT_4K) * BYTES_PER_ITEM);
        pageLen = ((para->endAddr - para->startAddr) >> SHIFT_4K) * BYTES_PER_ITEM;
        if ((para->endAddr & (MMU_4K - 1)) != 0) {
            pageLen += sizeof(UINT32);
        }
        PRINTK("SECOND_PAGE:\n");
    } else if (MMU_GET_AREA(para->uwFlag) == FIRST_SECTION) {
        startAddr = FIRST_PAGE_DESCRIPTOR_ADDR + ((para->startAddr >> SHIFT_1M) * BYTES_PER_ITEM);
        pageLen = ((para->endAddr - para->startAddr) >> SHIFT_1M) * BYTES_PER_ITEM;
        if ((para->endAddr & (MMU_1M - 1)) != 0) {
            pageLen += sizeof(UINT32);
        }
        PRINTK("FIRST_SECTION:\n");
    } else {
        return;
    }

    PRINTK("para->endAddr = 0x%x para->startAddr = 0x%x page_len = %u * 4\n",
           para->endAddr, para->startAddr, pageLen / BYTES_PER_ITEM);

    for (tmp = 0; tmp < pageLen; tmp += sizeof(UINT32)) {
        if (tmp % ITEM_PRINT_LEN == 0) {
            PRINTK("\n");
        }
        PRINTK ("0x%0+8x  ", *(UINTPTR *)(startAddr + tmp));
    }
    PRINTK("\n");
}

VOID OsMmuInit(VOID)
{
    UINT32 ttbBase = FIRST_PAGE_DESCRIPTOR_ADDR + 0x0;
    UINT32 regDACR;

    /* Set the TTB register */
    __asm volatile ("mcr p15, 0, %0, c2, c0, 0" : : "r"(ttbBase));
    /* Set the Domain Access Control Register */
    regDACR = ACCESS_TYPE_MANAGER(DOMAIN0) | ACCESS_TYPE_CLIENT(DOMAIN1); /* D0:manager; D1:client */
    __asm volatile ("mcr p15, 0, %0, c3, c0, 0" : : "r"(regDACR));

#ifdef LOSCFG_KERNEL_RUNSTOP
    if (IsImageResume()) return;
#endif

    /* First clear all TT entries - ie Set them to Faulting */
    (VOID)memset_s((UINTPTR *)ttbBase, MMU_16K, 0, MMU_16K);

    /*
     * Set domain of mmu descriptor of (0~1M) D_NA, check the illegal access to NULL pointer in code.
     * Access to NULL pointer and mem (0 ~ 1M) will trigger exception immediately
     */
    X_MMU_SECTION(0, 0, (MMU_1M >> SHIFT_1M), UNCACHEABLE, UNBUFFERABLE,
                  ACCESS_NA, NON_EXECUTABLE, D_NA);

    /* Set all mem 4G except (0~1M) as uncacheable & rw first */
    X_MMU_SECTION((MMU_1M >> SHIFT_1M), (MMU_1M >> SHIFT_1M), ((MMU_4G - MMU_1M) >> SHIFT_1M),
                  UNCACHEABLE, UNBUFFERABLE, ACCESS_RW, NON_EXECUTABLE, D_CLIENT);

    /*
     * set table as your config
     * 1: LITEOS_CACHE_ADDR ~ LITEOS_CACHE_ADDR + LITEOS_CACHE_LENGTH ---- set as section(1M) and cacheable & rw
     */
    X_MMU_SECTION((LITEOS_CACHE_ADDR >> SHIFT_1M), (SYS_MEM_BASE >> SHIFT_1M), (LITEOS_CACHE_LENGTH >> SHIFT_1M),
                  CACHEABLE, BUFFERABLE, ACCESS_RW, NON_EXECUTABLE, 0);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
