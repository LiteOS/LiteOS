/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: Circular Buffer Implementation
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

#include "los_cir_buf_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

UINT32 LOS_CirBufUsedSize(CirBuf *cirbufCB)
{
    UINT32 size;
    UINT32 intSave;

    if (cirbufCB == NULL) {
        return 0;
    }

    LOS_SpinLockSave(&cirbufCB->lock, &intSave);
    size = cirbufCB->size - cirbufCB->remain;
    LOS_SpinUnlockRestore(&cirbufCB->lock, intSave);

    return size;
}

/*
 *                    startIdx
 *                    |
 *    0 0 0 0 0 0 0 0 X X X X X X X X 0 0 0 0 0 0
 *                                    |
 *                                  endIdx
 */
STATIC UINT32 OsCirBufWriteLinear(CirBuf *cirbufCB, const CHAR *buf, UINT32 size)
{
    UINT32 cpSize;
    errno_t err;

    cpSize = (cirbufCB->remain < size) ? cirbufCB->remain : size;

    if (cpSize == 0) {
        return 0;
    }

    err = memcpy_s(cirbufCB->fifo + cirbufCB->endIdx, cirbufCB->remain, buf, cpSize);
    if (err != EOK) {
        return 0;
    }

    cirbufCB->remain -= cpSize;
    cirbufCB->endIdx += cpSize;

    return cpSize;
}

STATIC UINT32 OsCirBufWriteLoop(CirBuf *cirbufCB, const CHAR *buf, UINT32 size)
{
    UINT32 right, cpSize;
    errno_t err;

    right = cirbufCB->size - cirbufCB->endIdx;
    cpSize = (right < size) ? right : size;

    err = memcpy_s(cirbufCB->fifo + cirbufCB->endIdx, right, buf, cpSize);
    if (err != EOK) {
        return 0;
    }

    cirbufCB->remain -= cpSize;
    cirbufCB->endIdx += cpSize;
    if (cirbufCB->endIdx == cirbufCB->size) {
        cirbufCB->endIdx = 0;
    }

    if (cpSize == size) {
        return size;
    } else {
        cpSize += OsCirBufWriteLinear(cirbufCB, buf + cpSize, size - cpSize);
    }

    return cpSize;
}

UINT32 LOS_CirBufWrite(CirBuf *cirbufCB, const CHAR *buf, UINT32 size)
{
    UINT32 cpSize = 0;

    if ((cirbufCB == NULL) || (buf == NULL) || (size == 0)) {
        return 0;
    }

    if ((cirbufCB->fifo == NULL) || (cirbufCB->remain == 0))  {
        return 0;
    }

    if (cirbufCB->startIdx <= cirbufCB->endIdx) {
        cpSize = OsCirBufWriteLoop(cirbufCB, buf, size);
    } else {
        cpSize = OsCirBufWriteLinear(cirbufCB, buf, size);
    }

    return cpSize;
}

STATIC UINT32 OsCirBufReadLinear(CirBuf *cirbufCB, CHAR *buf, UINT32 size)
{
    UINT32 cpSize, remain;
    errno_t err;

    remain = cirbufCB->endIdx - cirbufCB->startIdx;
    cpSize = (remain < size) ? remain : size;

    if (cpSize == 0) {
        return 0;
    }

    err = memcpy_s(buf, size, cirbufCB->fifo + cirbufCB->startIdx, cpSize);
    if (err != EOK) {
        return 0;
    }

    cirbufCB->remain += cpSize;
    cirbufCB->startIdx += cpSize;

    return cpSize;
}

STATIC UINT32 OsCirBufReadLoop(CirBuf *cirbufCB, CHAR *buf, UINT32 size)
{
    UINT32 right, cpSize;
    errno_t err;

    right = cirbufCB->size - cirbufCB->startIdx;
    cpSize = (right < size) ? right : size;

    err = memcpy_s(buf, size, cirbufCB->fifo + cirbufCB->startIdx, cpSize);
    if (err != EOK) {
        return 0;
    }

    cirbufCB->remain += cpSize;
    cirbufCB->startIdx += cpSize;
    if (cirbufCB->startIdx == cirbufCB->size) {
        cirbufCB->startIdx = 0;
    }

    if (cpSize < size) {
        cpSize += OsCirBufReadLinear(cirbufCB, buf + cpSize, size - cpSize);
    }

    return cpSize;
}

UINT32 LOS_CirBufRead(CirBuf *cirbufCB, CHAR *buf, UINT32 size)
{
    UINT32 cpSize;

    if ((cirbufCB == NULL) || (buf == NULL) || (size == 0)) {
        return 0;
    }

    if ((cirbufCB->fifo == NULL) || (cirbufCB->remain == cirbufCB->size)) {
        return 0;
    }

    if (cirbufCB->startIdx >= cirbufCB->endIdx) {
        cpSize = OsCirBufReadLoop(cirbufCB, buf, size);
    } else {
        cpSize = OsCirBufReadLinear(cirbufCB, buf, size);
    }

    return cpSize;
}

UINT32 LOS_CirBufInit(CirBuf *cirbufCB, CHAR *fifo, UINT32 size)
{
    if ((cirbufCB == NULL) || (fifo == NULL)) {
        return LOS_NOK;
    }

    (VOID)memset_s(cirbufCB, sizeof(CirBuf), 0, sizeof(CirBuf));
    LOS_SpinInit(&cirbufCB->lock);
    cirbufCB->size = size;
    cirbufCB->remain = size;
    cirbufCB->status = CBUF_USED;
    cirbufCB->fifo = fifo;

    return LOS_OK;
}

VOID LOS_CirBufDeinit(CirBuf *cirbufCB)
{
    (VOID)memset_s(cirbufCB, sizeof(CirBuf), 0, sizeof(CirBuf));
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
