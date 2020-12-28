/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 * Description: LiteOS memory Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-05-12
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

#include "los_slab_pri.h"
#include "los_memory_pri.h"

#include "string.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define NUM_BITS_IN_ONE_BYTE              32
#define NUMBITS_TO_NUMBYTES(numBits)      (((numBits) + LOW_BITS_MASK) / 8)

VOID OsAtomicBitsetInit(struct AtomicBitset *set, UINT32 numBits)
{
    set->numBits = numBits;

    (VOID)memset_s(set->words, NUMBITS_TO_NUMBYTES(numBits), 0, NUMBITS_TO_NUMBYTES(numBits));
    /* mark all high bits so that OsAtomicBitsetFindClearAndSet() is simpler */
    if (numBits & LOW_BITS_MASK) {
        set->words[numBits / NUM_BITS_IN_ONE_BYTE] =
            ((UINT32)((INT32) - 1LL)) << (numBits & LOW_BITS_MASK);
    }
}

UINT32 OsAtomicBitsetGetNumBits(const struct AtomicBitset *set)
{
    return set->numBits;
}

BOOL OsAtomicBitsetGetBit(const struct AtomicBitset *set, UINT32 num)
{
    /* any value is as good as the next */
    if (num >= set->numBits) {
        return FALSE;
    }
    return !!((set->words[num / NUM_BITS_IN_ONE_BYTE]) & (1U << (num & LOW_BITS_MASK)));
}

VOID OsAtomicBitsetClearBit(struct AtomicBitset *set, UINT32 num)
{
    UINT32 *wordPtr = set->words + (num / NUM_BITS_IN_ONE_BYTE);

    if (num >= set->numBits) {
        return;
    }
    (*wordPtr) &= ~(1U << (num & LOW_BITS_MASK));
}

/* find from the high bit to high bit return the address of the first available bit */
INT32 OsAtomicBitsetFindClearAndSet(struct AtomicBitset *set)
{
    UINT32 idx;
    UINT32 numWords = (set->numBits + LOW_BITS_MASK) / NUM_BITS_IN_ONE_BYTE;
    UINT32 *wordPtr = set->words;
    UINT32 tmpWord;
    UINT32 count = 0;

    for (idx = 0; idx < numWords; idx++, wordPtr++) {
        if (*wordPtr == 0xFFFFFFFF) {
            continue;
        }

        tmpWord = ~(*wordPtr);

        while (tmpWord) {
            tmpWord = tmpWord >> 1U;
            count++;
        }

        *wordPtr |= (1U << (count - 1));

        return (INT32)(idx * NUM_BITS_IN_ONE_BYTE + count - 1);
    }

    return -1;
}

BOOL OsAtomicBitsetEmpty(const struct AtomicBitset *bitset)
{
    UINT32 idx;
    for (idx = 0; idx < (bitset->numBits / NUM_BITS_IN_ONE_BYTE); idx++) {
        if (bitset->words[idx] != 0) {
            return FALSE;
        }
    }
    if (bitset->numBits & LOW_BITS_MASK) {
        if (bitset->words[idx] & ~((UINT32)0xFFFFFFFF << (bitset->numBits & LOW_BITS_MASK))) {
            return FALSE;
        }
    }
    return TRUE;
}

OsSlabAllocator *OsSlabAllocatorNew(VOID *pool, UINT32 itemSz, UINT32 itemAlign, UINT32 numItems)
{
    OsSlabAllocator *allocator = NULL;
    UINT32 bitSetSz;
    UINT32 dataSz;
    UINT32 itemSize;

    /* calculate size */
    bitSetSz = ATOMIC_BITSET_SZ(numItems);

    bitSetSz = (bitSetSz + itemAlign - 1) & (~(itemAlign - 1));
    itemSize = (itemSz + itemAlign - 1) & (~(itemAlign - 1));
    dataSz = itemSize * numItems;

    allocator = (OsSlabAllocator*)OsMemAlloc(pool, sizeof(OsSlabAllocator) + bitSetSz + dataSz);
    if (allocator != NULL) {
        allocator->itemSz = itemSize;

        allocator->bitset = (struct AtomicBitset *)(VOID *)((UINT8*)allocator + sizeof(OsSlabAllocator));
        allocator->dataChunks = ((UINT8*)allocator->bitset) + bitSetSz;
        OsAtomicBitsetInit(allocator->bitset, numItems);
    }

    return allocator;
}

VOID OsSlabAllocatorDestroy(VOID *pool, OsSlabAllocator *allocator)
{
    (VOID)OsMemFree(pool, allocator);
}

VOID *OsSlabAllocatorAlloc(OsSlabAllocator *allocator)
{
    INT32 itemIdx = OsAtomicBitsetFindClearAndSet(allocator->bitset);
    if (itemIdx < 0) {
        return NULL;
    }

    return allocator->dataChunks + allocator->itemSz * (UINT32)itemIdx;
}

BOOL OsSlabAllocatorFree(OsSlabAllocator *allocator, VOID* ptr)
{
    UINT8 *ptrTmp = (UINT8*)ptr;
    UINT32 itemOffset = (UINT32)(ptrTmp - allocator->dataChunks);
    UINT32 itemIdx = itemOffset / allocator->itemSz;

    /* check for invalid inputs */
    if ((itemOffset % allocator->itemSz) || (itemIdx >= OsAtomicBitsetGetNumBits(allocator->bitset)) ||
         !(OsAtomicBitsetGetBit(allocator->bitset, itemIdx))) {
        return FALSE;
    }

    OsAtomicBitsetClearBit(allocator->bitset, itemIdx);
    return TRUE;
}

VOID *OsSlabAllocatorGetNth(const OsSlabAllocator *allocator, UINT32 idx)
{
    if (!OsAtomicBitsetGetBit(allocator->bitset, idx)) {
        return NULL;
    }

    return allocator->dataChunks + allocator->itemSz * idx;
}

VOID *OsSlabAllocatorGetIdxP(const OsSlabAllocator *allocator, UINT32 idx)
{
    return allocator->dataChunks + allocator->itemSz * idx;
}

UINT32 OsSlabAllocatorGetIndex(const OsSlabAllocator *allocator, VOID* ptr)
{
    UINT8 *ptrTmp = (UINT8*)ptr;
    UINT32 itemOffset = (UINT32)(ptrTmp - allocator->dataChunks);
    UINT32 itemIdx = itemOffset / allocator->itemSz;

    if ((itemOffset % allocator->itemSz) ||
        (itemIdx >= OsAtomicBitsetGetNumBits(allocator->bitset)) ||
        !(OsAtomicBitsetGetBit(allocator->bitset, itemIdx))) {
        return (UINT32)(-1);
    }

    return itemIdx;
}

UINT32 OsSlabAllocatorGetNumItems(const OsSlabAllocator *allocator)
{
    return OsAtomicBitsetGetNumBits(allocator->bitset);
}

BOOL OsSlabAllocatorEmpty(const OsSlabAllocator *allocator)
{
    return OsAtomicBitsetEmpty(allocator->bitset);
}

UINT32 OsSlabAllocatorGetUsedItemCnt(const OsSlabAllocator *allocator)
{
    UINT32 used;
    UINT32 idx;
    struct AtomicBitset *bitset = allocator->bitset;
    for (used = 0, idx = 0; idx < bitset->numBits; idx++) {
        if (OsAtomicBitsetGetBit(bitset, idx)) {
            used++;
        }
    }
    return used;
}

VOID OsSlabAllocatorGetSlabInfo(const OsSlabAllocator *allocator, UINT32 *pitemSz, UINT32 *itemCnt, UINT32 *curUsage)
{
    *pitemSz = allocator->itemSz;
    *itemCnt = OsAtomicBitsetGetNumBits(allocator->bitset);
    *curUsage = OsSlabAllocatorGetUsedItemCnt(allocator);
}

BOOL OsSlabAllocatorCheck(const OsSlabAllocator *allocator, VOID* ptr)
{
    UINT8 *ptrTmp = (UINT8*)ptr;
    UINT32 itemOffset = (UINT32)(ptrTmp - allocator->dataChunks);
    UINT32 itemIdx = itemOffset / allocator->itemSz;

    /* check for invalid inputs */
    if ((itemOffset % allocator->itemSz) || (itemIdx >= OsAtomicBitsetGetNumBits(allocator->bitset)) ||
        !(OsAtomicBitsetGetBit(allocator->bitset, itemIdx))) {
        return FALSE;
    }

    return TRUE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
