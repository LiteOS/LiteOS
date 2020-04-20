/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Mutex Debug Private HeadFile
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

#ifndef _LOS_MUX_DEBUG_PRI_H
#define _LOS_MUX_DEBUG_PRI_H

#include "los_mux_pri.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Deadlock detection initialization interface */
extern UINT32 OsMuxDlockCheckInit(VOID);
STATIC INLINE UINT32 OsMuxDlockCheckInitHook(VOID)
{
#ifdef LOSCFG_DEBUG_DEADLOCK
    return OsMuxDlockCheckInit();
#else
    return LOS_OK;
#endif
}

/* Add holding mutex lock node information */
extern VOID OsMuxDlockNodeInsert(UINT32 taskID, VOID *muxCB);
STATIC INLINE VOID OsMuxDlockNodeInsertHook(UINT32 taskID, VOID *muxCB)
{
#ifdef LOSCFG_DEBUG_DEADLOCK
    OsMuxDlockNodeInsert(taskID, muxCB);
#endif
}
/* Delete holding mutex lock node information */
extern VOID OsMuxDlockNodeDelete(UINT32 taskID, const VOID *muxCB);
STATIC INLINE VOID OsMuxDlockNodeDeleteHook(UINT32 taskID, const VOID *muxCB)
{
#ifdef LOSCFG_DEBUG_DEADLOCK
    OsMuxDlockNodeDelete(taskID, muxCB);
#endif
}
/* Update the last time the task was executed */
extern VOID OsTaskTimeUpdate(UINT32 taskID, UINT64 tickCount);
STATIC INLINE VOID OsTaskTimeUpdateHook(UINT32 taskID, UINT64 tickCount)
{
#ifdef LOSCFG_DEBUG_DEADLOCK
    OsTaskTimeUpdate(taskID, tickCount);
#endif
}

/* mutex debug initialization interface */
extern UINT32 OsMuxDbgInit(VOID);
STATIC INLINE UINT32 OsMuxDbgInitHook(VOID)
{
#ifdef LOSCFG_DEBUG_MUTEX
    return OsMuxDbgInit();
#else
    return LOS_OK;
#endif
}
/* Update the last time the mutex was executed */
extern VOID OsMuxDbgTimeUpdate(UINT32 muxID);
STATIC INLINE VOID OsMuxDbgTimeUpdateHook(UINT32 muxID)
{
#ifdef LOSCFG_DEBUG_MUTEX
    OsMuxDbgTimeUpdate(muxID);
#endif
}
/* Update the MUX_DEBUG_CB of the mutex when created or deleted */
extern VOID OsMuxDbgUpdate(UINT32 muxID, TSK_ENTRY_FUNC creater);
STATIC INLINE VOID OsMuxDbgUpdateHook(UINT32 muxID, TSK_ENTRY_FUNC creater)
{
#ifdef LOSCFG_DEBUG_MUTEX
    OsMuxDbgUpdate(muxID, creater);
#endif
}
/* check the leak of mutex */
extern VOID OsMutexCheck(VOID);
STATIC INLINE VOID OsMutexCheckHook(VOID)
{
#ifdef LOSCFG_DEBUG_MUTEX
    OsMutexCheck();
#endif
}
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MUX_DEBUG_PRI_H */
