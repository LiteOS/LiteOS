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

/**
 * @defgroup los_slab Slab
 * @ingroup kernel
 */

#ifndef _LOS_SLAB_PRI_H
#define _LOS_SLAB_PRI_H

#include "los_slab.h"
#include "los_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_MEM_SLAB_EXTENTION

/* number of slab class */
#define SLAB_MEM_COUNT              4

/* step size of each class */
#define SLAB_MEM_CALSS_STEP_SIZE    0x10U

/* max slab block size */
#define SLAB_MEM_MAX_SIZE           (SLAB_MEM_CALSS_STEP_SIZE << (SLAB_MEM_COUNT - 1))

typedef struct tagLosSlabStatus {
    UINT32 totalSize;
    UINT32 usedSize;
    UINT32 freeSize;
    UINT32 allocCount;
    UINT32 freeCount;
} LosSlabStatus;

typedef struct tagOsSlabBlockNode {
    UINT16 magic;
    UINT8  blkSz;
    UINT8  recordId;
} OsSlabBlockNode;

struct AtomicBitset {
    UINT32 numBits;
    UINT32 words[0];
};

typedef struct tagOsSlabAllocator {
    UINT32 itemSz;
    UINT8 *dataChunks;
    struct AtomicBitset *bitset;
} OsSlabAllocator;

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
typedef struct tagOsSlabMemAllocator {
    struct tagOsSlabMemAllocator *next;
    OsSlabAllocator *slabAlloc;
} OsSlabMemAllocator;
#endif

typedef struct tagOsSlabMem {
    UINT32 blkSz;
    UINT32 blkCnt;
    UINT32 blkUsedCnt;
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    UINT32 allocatorCnt;
    OsSlabMemAllocator *bucket;
#else
    OsSlabAllocator *alloc;
#endif
} OsSlabMem;

struct LosSlabControlHeader {
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    OsSlabAllocator *allocatorBucket;
#endif
    OsSlabMem slabClass[SLAB_MEM_COUNT];
};

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
#define SLAB_MEM_DFEAULT_BUCKET_CNT                1
#endif

#define LOW_BITS_MASK                              31U
#define OS_SLAB_MAGIC                              0xdede
#define OS_SLAB_BLOCK_HEAD_GET(ptr)                ((OsSlabBlockNode *)(VOID *)((UINT8 *)(ptr) - \
                                                      sizeof(OsSlabBlockNode)))
#define OS_SLAB_BLOCK_MAGIC_SET(slabNode)          (((OsSlabBlockNode *)(slabNode))->magic = (UINT16)OS_SLAB_MAGIC)
#define OS_SLAB_BLOCK_MAGIC_GET(slabNode)          (((OsSlabBlockNode *)(slabNode))->magic)
#define OS_SLAB_BLOCK_SIZE_SET(slabNode, size)     (((OsSlabBlockNode *)(slabNode))->blkSz = (UINT8)(size))
#define OS_SLAB_BLOCK_SIZE_GET(slabNode)           (((OsSlabBlockNode *)(slabNode))->blkSz)
#define OS_SLAB_BLOCK_ID_SET(slabNode, id)         (((OsSlabBlockNode *)(slabNode))->recordId = (id))
#define OS_SLAB_BLOCK_ID_GET(slabNode)             (((OsSlabBlockNode *)(slabNode))->recordId)
#define OS_ALLOC_FROM_SLAB_CHECK(slabNode)         (((OsSlabBlockNode *)(slabNode))->magic == (UINT16)OS_SLAB_MAGIC)
#define ATOMIC_BITSET_SZ(numbits)                  (sizeof(struct AtomicBitset) + \
                                                      ((numbits) + LOW_BITS_MASK) / 8) /* 8, byte alignment */
#define OS_SLAB_LOG2(value)                        ((UINT32)(32 - CLZ(value) - 1)) /* get highest bit one position */
#define OS_SLAB_CLASS_LEVEL_GET(size) \
        (OS_SLAB_LOG2((size - 1) >> (OS_SLAB_LOG2(SLAB_MEM_CALSS_STEP_SIZE - 1))))

extern OsSlabAllocator *OsSlabAllocatorNew(VOID *pool, UINT32 itemSz, UINT32 itemAlign, UINT32 numItems);
extern VOID OsSlabAllocatorDestroy(VOID *pool, OsSlabAllocator *allocator);
extern VOID *OsSlabAllocatorAlloc(OsSlabAllocator *allocator);
extern BOOL OsSlabAllocatorFree(OsSlabAllocator *allocator, VOID* ptr);
extern BOOL OsSlabAllocatorEmpty(const OsSlabAllocator *allocator);
extern VOID OsSlabAllocatorGetSlabInfo(const OsSlabAllocator *allocator, UINT32 *itemSize,
    UINT32 *itemCnt, UINT32 *curUsage);
extern BOOL OsSlabAllocatorCheck(const OsSlabAllocator *allocator, VOID *ptr);
extern VOID OsSlabMemInit(VOID *pool, UINT32 size);
extern VOID OsSlabMemDeinit(VOID *pool);
extern VOID *OsSlabMemAlloc(VOID *pool, UINT32 sz);
extern BOOL OsSlabMemFree(VOID *pool, VOID *ptr);
extern UINT32 OsSlabMemCheck(const VOID *pool, VOID *ptr);
extern UINT32 OsSlabStatisticsGet(const VOID *pool, LosSlabStatus *status);
extern UINT32 OsSlabGetMaxFreeBlkSize(const VOID *pool);
extern VOID *OsSlabCtrlHdrGet(const VOID *pool);

#else /* !LOSCFG_KERNEL_MEM_SLAB_EXTENTION */

STATIC INLINE VOID OsSlabMemInit(VOID *pool, UINT32 size)
{
}

STATIC INLINE VOID OsSlabMemDeinit(VOID *pool)
{
}

STATIC INLINE VOID *OsSlabMemAlloc(VOID *pool, UINT32 size)
{
    return NULL;
}

STATIC INLINE BOOL OsSlabMemFree(VOID *pool, VOID *ptr)
{
    return FALSE;
}

STATIC INLINE UINT32 OsSlabMemCheck(const VOID *pool, VOID *ptr)
{
    return (UINT32)-1;
}

#endif /* LOSCFG_KERNEL_MEM_SLAB_EXTENTION */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_SLAB_PRI_H */
