/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Memory Module Private HeadFile
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

#ifndef _LOS_MEMORY_PRI_H
#define _LOS_MEMORY_PRI_H

#include "los_memory.h"
#include "los_spinlock.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern UINTPTR g_sys_mem_addr_end;

extern UINT32 OsMemSystemInit(UINTPTR memStart);

#ifdef LOSCFG_EXC_INTERACTION
extern UINT32 OsMemExcInteractionInit(UINTPTR memStart);
#endif
#define IS_ALIGNED(value, alignSize) ((((UINTPTR)(value)) & ((UINTPTR)((alignSize) - 1))) == 0)
extern VOID OsDumpMemByte(UINT32 length, UINTPTR addr);

#ifdef LOSCFG_MEM_LEAKCHECK
extern VOID OsMemUsedNodeShow(VOID *pool);
#endif

extern VOID OsMemResetEndNode(VOID *pool, UINTPTR preAddr);

extern UINT32 OsShellCmdMemCheck(INT32 argc, const CHAR *argv[]);

/* spinlock for mem module */
extern SPIN_LOCK_S g_memSpin;
#define MEM_LOCK(state)       LOS_SpinLockSave(&g_memSpin, &(state))
#define MEM_UNLOCK(state)     LOS_SpinUnlockRestore(&g_memSpin, (state))

#if (LOSCFG_KERNEL_MEM_STATISTICS == YES)
#define TASK_BLOCK_NUM                          (LOSCFG_BASE_CORE_TSK_CONFIG + 2)
typedef struct {
    UINT32 memUsed;
    UINT32 memPeak;
} TaskMemUsedInfo;
#endif

#if (LOSCFG_KERNEL_MEM_BESTFIT == YES)
/* Memory pool information structure */
typedef struct {
    VOID *pool;      /* Starting address of a memory pool */
    UINT32 poolSize; /* Memory pool size */
#if defined(OS_MEM_WATERLINE) && (OS_MEM_WATERLINE == YES)
    UINT32 poolWaterLine;   /* Maximum usage size in a memory pool */
    UINT32 poolCurUsedSize; /* Current usage size in a memory pool */
#endif
#ifdef LOSCFG_MEM_MUL_POOL
    VOID *nextPool;
#endif
#if (LOSCFG_KERNEL_MEM_STATISTICS == YES)
    TaskMemUsedInfo memStats[TASK_BLOCK_NUM];
#endif
} LosMemPoolInfo;

#elif (LOSCFG_KERNEL_MEM_BESTFIT_LITTLE == YES)
typedef struct {
    struct LosHeapNode *head;
    struct LosHeapNode *tail;
    UINT32 size;
#if (LOSCFG_MEM_MUL_POOL == YES)
    VOID *nextPool;
#endif
#if (LOSCFG_KERNEL_MEM_STATISTICS == YES)
    TaskMemUsedInfo memStats[TASK_BLOCK_NUM];
#endif
} LosMemPoolInfo;

#endif

#if (LOSCFG_MEM_MUL_POOL == YES)
extern VOID *g_poolHead;
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MEMORY_PRI_H */
