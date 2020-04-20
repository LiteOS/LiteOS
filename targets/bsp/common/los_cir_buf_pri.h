/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: Circular Buffer Inner HeadFile
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

#ifndef _LOS_CIRBUF_PRI_H
#define _LOS_CIRBUF_PRI_H

#include "los_typedef.h"
#include "los_spinlock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef enum {
    CBUF_UNUSED,
    CBUF_USED
} CirBufStatus;

typedef struct {
    UINT32 startIdx;
    UINT32 endIdx;
    UINT32 size;
    UINT32 remain;
    SPIN_LOCK_S lock;
    CirBufStatus status;
    CHAR *fifo;
} CirBuf;

STATIC INLINE VOID LOS_CirBufLock(CirBuf *cirbufCB, UINT32 *intSave)
{
    if (cirbufCB == NULL) {
        return;
    }
    LOS_SpinLockSave(&cirbufCB->lock, intSave);
}

STATIC INLINE VOID LOS_CirBufUnlock(CirBuf *cirbufCB, UINT32 intSave)
{
    if (cirbufCB == NULL) {
        return;
    }
    LOS_SpinUnlockRestore(&cirbufCB->lock, intSave);
}

extern UINT32 LOS_CirBufInit(CirBuf *cirbufCB, CHAR *fifo, UINT32 size);
extern VOID LOS_CirBufDeinit(CirBuf *cirbufCB);
extern UINT32 LOS_CirBufWrite(CirBuf *cirbufCB, const CHAR *buf, UINT32 size);
extern UINT32 LOS_CirBufRead(CirBuf *cirbufCB, CHAR *buf, UINT32 size);
extern UINT32 LOS_CirBufUsedSize(CirBuf *cirbufCB);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_CIRBUF_PRI_H */
