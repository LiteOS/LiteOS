/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Queue Debug Pri HeadFile
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

#ifndef _LOS_QUEUE_DEBUG_PRI_H
#define _LOS_QUEUE_DEBUG_PRI_H

#include "los_config.h"
#include "los_task.h"
#include "los_queue_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* queue debug initialization interface */
extern UINT32 OsQueueDbgInit(VOID);
STATIC INLINE UINT32 OsQueueDbgInitHook(VOID)
{
#ifdef LOSCFG_DEBUG_QUEUE
    return OsQueueDbgInit();
#else
    return LOS_OK;
#endif
}
/* Update the last time the queue was executed */
extern VOID OsQueueDbgTimeUpdate(UINT32 queueID);
STATIC INLINE VOID OsQueueDbgTimeUpdateHook(UINT32 queueID)
{
#ifdef LOSCFG_DEBUG_QUEUE
    OsQueueDbgTimeUpdate(queueID);
#endif
}
/* Update the task  entry of  the queue debug info when created or deleted */
extern VOID OsQueueDbgUpdate(UINT32 queueID, TSK_ENTRY_FUNC entry);
STATIC INLINE VOID OsQueueDbgUpdateHook(UINT32 queueID, TSK_ENTRY_FUNC entry)
{
#ifdef LOSCFG_DEBUG_QUEUE
    OsQueueDbgUpdate(queueID, entry);
#endif
}
/* check the leak of queue */
extern VOID OsQueueCheck(VOID);
STATIC INLINE VOID OsQueueCheckHook(VOID)
{
#ifdef LOSCFG_DEBUG_QUEUE
    OsQueueCheck();
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_QUEUE_DEBUG_PRI_H */
