/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: MMU Config Implementation
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

#include "mmu_pri.h"
#include "mmu.h"
#include "board.h"
#include "asm/dma.h"
#include "los_base.h"
#include "los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define ALIGNED_4K __attribute__((aligned(MMU_4K)))
ALIGNED_4K __attribute__((section(".bss.prebss.translation_table"))) UINT8 g_firstPageTable[FIRST_SECTION_TABLE_LEN];
static ALIGNED_4K UINT8 g_secondPageTableOs[SECOND_PAGE_TABLE_OS_LEN];
static ALIGNED_4K UINT8 g_secondPageTableApp[SECOND_PAGE_TABLE_APP_LEN];

static SENCOND_PAGE g_mmuOsPage = {0};
static SENCOND_PAGE g_mmuAppPage = {0};

#define PGD_ADDR  (UINT64)g_firstPageTable
#define PMD_ADDR0 (PGD_ADDR + PAGE_SIZE)
#define PMD_ADDR1 (PMD_ADDR0 + PAGE_SIZE)
#define PMD_ADDR2 (PMD_ADDR1 + PAGE_SIZE)

#define ITEM_MASK ((1U << (SHIFT_4K - 3)) - 1)
#define PMD_PAGE_GET(addr)   (((addr) >> SHIFT_1G) * PAGE_SIZE + PMD_ADDR0)
#define PMD_OFFSET_GET(addr) ((((addr) & (MMU_1G - 1)) >> SHIFT_2M) * 8)
#define PMD_ADDR_GET(addr)   (PMD_PAGE_GET(addr) + PMD_OFFSET_GET(addr))
#define PTE_TABLE_GET(addr)  (*(UINT64 *)(PMD_ADDR_GET(addr)) >> SHIFT_4K << SHIFT_4K)

#define BYTES_PER_ITEM 8
#define ITEM_TYPE_MASK 0x3

/* Difference of the output-address bit width between each level, 1G / 2M = 2^9, 2M / 4K = 2^9 */
#define ADDR_WIDTH_DIFF 9

typedef struct {
    UINT64 tbl;
    UINT64 flags;
    UINT64 phys;
    UINT64 start;
    UINT64 end;
    UINT64 blockShift;
} BlockCB;

STATIC INLINE VOID CreatPgdItem(UINT64 tbl, UINT64 pmdTableAddr)
{
    *(UINT64 *)(tbl) = pmdTableAddr | MMU_PTE_L012_DESCRIPTOR_TABLE;
}

STATIC VOID CreatPmdTable(UINT64 pteTableAddr, UINT64 startAddr, UINT64 endAddr)
{
    UINT64 tbl = PMD_PAGE_GET(startAddr);
    UINT64 indexStart = startAddr >> SHIFT_2M;
    UINT64 indexEnd = endAddr >> SHIFT_2M;
    UINT64 index;

    indexStart &= ITEM_MASK;
    indexEnd &= ITEM_MASK;
    indexEnd += ((endAddr - startAddr) >> SHIFT_1G) << ADDR_WIDTH_DIFF;

    for (index = indexStart; index <= indexEnd; ++index) {
        *(UINT64 *)(tbl + (index * BYTES_PER_ITEM)) = pteTableAddr | MMU_PTE_L012_DESCRIPTOR_TABLE;
        pteTableAddr += PAGE_SIZE;
    }
    v8_dma_clean_range(tbl + (indexStart * BYTES_PER_ITEM), tbl + (indexEnd * BYTES_PER_ITEM));
    __asm__ __volatile__("tlbi    vmalle1");
}

STATIC VOID CreatPteTable(const MMU_PARAM *para)
{
    UINT64 indexMax;
    UINT64 indexTemp;
    UINT64 phy = (para->startAddr >> SHIFT_4K) << SHIFT_4K;
    UINT64 offset = (para->startAddr - para->stPage->page_addr) >> SHIFT_4K;
    UINT64 tbl = para->stPage->page_descriptor_addr;

    if (para->endAddr <= para->startAddr) {
        return;
    }
    indexMax = (para->endAddr - 1 - para->startAddr) >> SHIFT_4K;
    phy |= para->uwFlag;

    for (indexTemp = 0; indexTemp <= indexMax; ++indexTemp, ++offset) {
        *(UINT64 *)(tbl + (offset * BYTES_PER_ITEM)) = phy | MMU_PTE_L3_DESCRIPTOR_PAGE;
        phy += PAGE_SIZE;
    }
    v8_dma_clean_range(tbl + ((offset - indexTemp) * BYTES_PER_ITEM), tbl + (offset * BYTES_PER_ITEM));
    __asm__ __volatile__("tlbi    vmalle1");
}

STATIC VOID CreatBlockMap(const BlockCB *blockCB)
{
    UINT64 tbl = blockCB->tbl;
    UINT64 flags = blockCB->flags;
    UINT64 phys = blockCB->phys;
    UINT64 start = blockCB->start;
    UINT64 end = blockCB->end;
    UINT64 blockShift = blockCB->blockShift;
    UINT64 offsetGB = (end - start) >> SHIFT_1G;

    phys = phys >> blockShift;
    start = start >> blockShift;
    start &= ITEM_MASK;
    phys = flags | (phys << blockShift);
    end = end >> blockShift;
    end &= ITEM_MASK;
    end += offsetGB << ADDR_WIDTH_DIFF;
    while (start <= end) {
        *(UINT64 *)(tbl + (start * BYTES_PER_ITEM)) = phys;
        start++;
        phys += ((UINT64)1UL << blockShift);
    }
}

VOID OsBlockMapsInit(UINT64 flags, UINT64 start, UINT64 end)
{
    UINT64 startOffset;
    UINT64 endOffset;
    UINT64 pageSize;
    BlockCB blockCB;

    if ((start & (MMU_1G - 1)) != 0) {
        PRINT_ERR("%s, %d, the start of mapping addr is 0x%llx, should be aligned as 1G \n",
                  __FUNCTION__, __LINE__, start);
        return;
    }
    end = ((end + (MMU_1G - 1)) & ~(MMU_1G - 1));
    pageSize = (sizeof(g_firstPageTable) >> SHIFT_4K) - 1;
    startOffset = start >> SHIFT_1G;
    endOffset = end >> SHIFT_1G;
    if (endOffset > pageSize) {
        PRINT_ERR("%s, %d, the end of mapping addr is 0x%llx, should not be bigger than 0x%llx \n",
                  __FUNCTION__, __LINE__, end, pageSize << SHIFT_1G);
        return;
    }
    for (; startOffset < endOffset; ++startOffset) {
        CreatPgdItem(PGD_ADDR + (startOffset * BYTES_PER_ITEM), PMD_ADDR0 + (startOffset * PAGE_SIZE));
        blockCB.tbl = PMD_ADDR0 + (startOffset * PAGE_SIZE);
        blockCB.flags = flags;
        blockCB.phys = startOffset << SHIFT_1G;
        blockCB.start = startOffset << SHIFT_1G;
        blockCB.end = ((startOffset + 1) << SHIFT_1G) - 1;
        blockCB.blockShift = SHIFT_2M;
        CreatBlockMap(&blockCB);
    }
}

VOID OsBlockMapsSet(UINT64 flags, UINT64 start, UINT64 end)
{
    UINT64 startOffset;
    BlockCB blockCB;
    UINT32 intSave;

    if ((start & ((1u << SHIFT_2M) - 1)) != 0) {
        PRINT_ERR("%s, %d, the start of mapping addr is 0x%llx, should be aligned as 2M \n",
                  __FUNCTION__, __LINE__, start);
        return;
    }

    if (start >= end) {
        PRINT_ERR("%s, %d, input parameters are error: start: 0x%llx, end: 0x%llx\n",
                  __FUNCTION__, __LINE__, start, end);
        return;
    }

    startOffset = start >> SHIFT_1G;
    intSave = LOS_IntLock();
    blockCB.tbl = PMD_ADDR0 + (startOffset * PAGE_SIZE);
    blockCB.flags = flags;
    blockCB.phys = start;
    blockCB.start = start;
    blockCB.end = end - 1;
    blockCB.blockShift = SHIFT_2M;
    CreatBlockMap(&blockCB);
    LOS_IntRestore(intSave);
}

STATIC INLINE INT32 OsMMUFlagCheck(UINT64 flag)
{
    switch (flag) {
        case MMU_PTE_CACHE_RO_FLAGS:
        case MMU_PTE_CACHE_RW_FLAGS:
        case MMU_PTE_CACHE_RW_XN_FLAGS:
        case MMU_PTE_NONCACHE_RO_FLAGS:
        case MMU_PTE_NONCACHE_RW_FLAGS:
        case MMU_PTE_NONCACHE_RW_XN_FLAGS:
        case MMU_INITIAL_MAP_STRONGLY_ORDERED:
        case MMU_INITIAL_MAP_DEVICE:
            break;
        default:
            PRINT_ERR("illegal mmu flag 0x%llx\n", flag);
            return LOS_NOK;
    }
    return LOS_OK;
}

VOID ArchMMUParamSet(MMU_PARAM *para)
{
    UINT64 pmdStart, pmdEnd, pmdTmp;
    UINT32 intSave;

    if ((para == NULL) || (OsMMUFlagCheck(para->uwFlag) == LOS_NOK)) {
        PRINT_ERR("para is invalid\n");
        return;
    }

    if (para->startAddr >= para->endAddr) {
        PRINT_ERR("wrong addr input para->startAddr[0x%llx] para->endAddr[0x%llx]\n", para->startAddr, para->endAddr);
        return;
    }

    if (para->uwArea == PMD_AREA) {
        pmdStart = PMD_ADDR_GET(para->startAddr);
        pmdEnd = PMD_ADDR_GET(para->endAddr);
        for (pmdTmp = pmdStart; pmdTmp < pmdEnd; pmdTmp += BYTES_PER_ITEM) {
            if (((*(UINTPTR *)pmdTmp) & ITEM_TYPE_MASK) != MMU_PTE_L012_DESCRIPTOR_BLOCK) {
                PRINT_ERR("not all mem belongs to pmd section(2M every item), descriptor types:0x%llx\n",
                          ((*(UINTPTR *)pmdTmp) & ITEM_TYPE_MASK));
                return;
            } else {
                PRINT_DEBUG("pmdTmp = 0x%llx : 0x%llx\n", pmdTmp, *(UINTPTR *)pmdTmp);
            }
        }
        OsBlockMapsSet(para->uwFlag | MMU_PTE_L012_DESCRIPTOR_BLOCK, para->startAddr, para->endAddr);
        PRINT_DEBUG("pmdStart = 0x%llx : 0x%llx\n", pmdStart, *(UINTPTR *)pmdStart);
        v8_dma_clean_range(pmdStart, pmdEnd);
        __asm__ __volatile__("tlbi    vmalle1");
        return;
    }

    if ((para->startAddr & (MMU_4K - 1)) != 0) {
        PRINT_ERR("para->startAddr[0x%llx] not aligned as 4K\n", para->startAddr);
        return;
    }

    if ((para->endAddr & (MMU_4K - 1)) != 0) {
        para->endAddr = ALIGN(para->endAddr, MMU_4K);
    }

    if ((para->startAddr < para->stPage->page_addr) ||
        (para->endAddr > (para->stPage->page_length + para->stPage->page_addr))) {
        PRINT_ERR("addr input not belongs to this second page \n"
                  "para->startAddr:0x%llx, para->stPage->page_addr:0x%llx\n",
                  para->startAddr, para->stPage->page_addr);
        PRINT_ERR("para->endAddr:0x%llx, (para->stPage->page_length + para->stPage->page_addr):0x%llx\n",
                  para->endAddr, para->stPage->page_length + para->stPage->page_addr);
        return;
    }
    intSave = LOS_IntLock();
    CreatPteTable(para);
    LOS_IntRestore(intSave);
}

#define SECOND_PAGE_TABLE_MAPPING_LEN(table) ((sizeof(table) >> 3) << 12)
VOID OsKernelSecPteInit(UINTPTR startAddr, UINTPTR len, UINT64 flag)
{
    g_mmuOsPage.page_addr = startAddr;
    g_mmuOsPage.page_length = len;
    g_mmuOsPage.page_descriptor_addr = (UINTPTR)g_secondPageTableOs;
    if (g_mmuOsPage.page_length > SECOND_PAGE_TABLE_MAPPING_LEN(g_secondPageTableOs)) {
        PRINT_ERR("the mapping size of os second page is 0x%llx, sholud be not bigger than 0x%llx\n",
                  g_mmuOsPage.page_length, (UINT64)SECOND_PAGE_TABLE_MAPPING_LEN(g_secondPageTableOs));
        return;
    }
    ArchSecPageEnable(&g_mmuOsPage, flag);
}

VOID OsAppSecPteInit(UINTPTR startAddr, UINTPTR len, UINT64 flag)
{
    g_mmuAppPage.page_addr = startAddr;
    g_mmuAppPage.page_length = len;
    g_mmuAppPage.page_descriptor_addr = (UINTPTR)g_secondPageTableApp;
    if (g_mmuAppPage.page_length > SECOND_PAGE_TABLE_MAPPING_LEN(g_secondPageTableApp)) {
        PRINT_ERR("the mapping size of app second page is 0x%llx, sholud be not bigger than 0x%llx\n",
                  g_mmuAppPage.page_length, (UINT64)SECOND_PAGE_TABLE_MAPPING_LEN(g_secondPageTableApp));
        return;
    }
    ArchSecPageEnable(&g_mmuAppPage, flag);
}

VOID ArchSecPageEnable(SENCOND_PAGE *page, UINT64 flag)
{
    UINT32 intSave;
    MMU_PARAM para;

    if (page == NULL) {
        PRINT_ERR("second page table(stPage) can't be NULL\n");
        return;
    }

    if (OsMMUFlagCheck(flag) == LOS_NOK) {
        return;
    }

    para.startAddr = page->page_addr;
    para.endAddr = page->page_addr + page->page_length;
    para.uwFlag = flag;
    para.stPage = page;

    intSave = LOS_IntLock();
    CreatPteTable(&para);
    CreatPmdTable(page->page_descriptor_addr, page->page_addr, (page->page_addr + page->page_length) - 1);
    LOS_IntRestore(intSave);
}

VOID RemapCachedMemAttr(UINTPTR physAddr, size_t size, UINTPTR flag)
{
    MMU_PARAM para;

    if (physAddr < SYS_MEM_BASE)
        return;

    para.startAddr = physAddr;
    para.endAddr = physAddr + size;
    para.uwFlag = flag;
    para.stPage = (SENCOND_PAGE *)&g_mmuAppPage;
    para.uwArea = PTE_AREA;
    ArchMMUParamSet(&para);
}

VOID OsCachedRemap(UINTPTR physAddr, size_t size)
{
    RemapCachedMemAttr(physAddr, size, MMU_PTE_CACHE_RW_XN_FLAGS);
}

VOID OsNoCachedRemap(UINTPTR physAddr, size_t size)
{
    RemapCachedMemAttr(physAddr, size, MMU_PTE_NONCACHE_RW_XN_FLAGS);
}

VOID ArchCodeProtect(VOID)
{
    MMU_PARAM para;
    para.startAddr = (unsigned long)&__text_start;
    para.endAddr = (unsigned long)&__ram_data_start;
    para.uwFlag = MMU_PTE_CACHE_RO_FLAGS;
    para.stPage = (SENCOND_PAGE *)&g_mmuOsPage;
    para.uwArea = PTE_AREA;
    PRINTK("para.startAddr = 0x%llx para.endAddr = 0x%llx\n", para.startAddr, para.endAddr);
    ArchMMUParamSet(&para);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
