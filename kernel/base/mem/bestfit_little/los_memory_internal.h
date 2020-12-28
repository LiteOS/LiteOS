/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
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

/*
 * @defgroup los_heap Heap
 * @ingroup kernel
 */

#ifndef _LOS_MEMORY_INTERNAL_H
#define _LOS_MEMORY_INTERNAL_H

#include "los_typedef.h"
#include "los_memstat_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define ALIGNE(sz)                            (((sz) + HEAP_ALIGN - 1) & (~(HEAP_ALIGN - 1)))
#define OS_MEM_ALIGN(value, align)            (((UINT32)(UINTPTR)(value) + (UINT32)((align) - 1)) & \
                                               (~(UINT32)((align) - 1)))
#define OS_MEM_ALIGN_FLAG                     0x80000000
#define OS_MEM_SET_ALIGN_FLAG(align)          ((align) = ((align) | OS_MEM_ALIGN_FLAG))
#define OS_MEM_GET_ALIGN_FLAG(align)          ((align) & OS_MEM_ALIGN_FLAG)
#define OS_MEM_GET_ALIGN_GAPSIZE(align)       ((align) & (~OS_MEM_ALIGN_FLAG))

typedef struct tagLosHeapStatus {
    UINT32 totalUsedSize;
    UINT32 totalFreeSize;
    UINT32 maxFreeNodeSize;
    UINT32 usedNodeNum;
    UINT32 freeNodeNum;
#ifdef LOSCFG_MEM_TASK_STAT
    UINT32 usageWaterLine;
#endif
} LosHeapStatus;

struct LosHeapNode {
    struct LosHeapNode *prev;
#ifdef LOSCFG_MEM_TASK_STAT
    UINT32 taskId;
#endif
    UINT32 size     : 30;
    UINT32 used     : 1;
    UINT32 align    : 1;
    UINT8  data[0];
};

extern BOOL OsHeapInit(VOID *pool, UINT32 size);
extern VOID* OsHeapAlloc(VOID *pool, UINT32 size);
extern VOID* OsHeapAllocAlign(VOID *pool, UINT32 size, UINT32 boundary);
extern BOOL OsHeapFree(VOID *pool, VOID* ptr);
extern UINT32 OsHeapStatisticsGet(VOID *pool, LosHeapStatus *status);
extern UINT32 OsHeapIntegrityCheck(struct LosHeapManager *heap);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MEMORY_INTERNAL_H */