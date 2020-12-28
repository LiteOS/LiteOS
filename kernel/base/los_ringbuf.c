/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: Ring Buffer Implementation
 * Author: Huawei LiteOS Team
 * Create: 2019-10-10
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

#include "los_ringbuf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

UINT32 LOS_RingbufUsedSize(Ringbuf *ringbuf)
{
    UINT32 size;
    UINT32 intSave;
    if ((ringbuf == NULL) || (ringbuf->status != RBUF_INITED)) {
        return 0;
    }

    LOS_SpinLockSave(&ringbuf->lock, &intSave);
    size = ringbuf->size - ringbuf->remain;
    LOS_SpinUnlockRestore(&ringbuf->lock, intSave);

    return size;
}

/*
 *                    startIdx
 *                    |
 *    0 0 0 0 0 0 0 0 X X X X X X X X 0 0 0 0 0 0
 *                                    |
 *                                  endIdx
 */
STATIC UINT32 OsRingbufWriteLinear(Ringbuf *ringbuf, const CHAR *buf, UINT32 size)
{
    UINT32 cpSize;
    errno_t err;

    cpSize = (ringbuf->remain < size) ? ringbuf->remain : size;

    if (cpSize == 0) {
        return 0;
    }

    err = memcpy_s(ringbuf->fifo + ringbuf->endIdx, ringbuf->remain, buf, cpSize);
    if (err != EOK) {
        return 0;
    }

    ringbuf->remain -= cpSize;
    ringbuf->endIdx += cpSize;

    return cpSize;
}

STATIC UINT32 OsRingbufWriteLoop(Ringbuf *ringbuf, const CHAR *buf, UINT32 size)
{
    UINT32 right, cpSize;
    errno_t err;

    right = ringbuf->size - ringbuf->endIdx;
    cpSize = (right < size) ? right : size;

    err = memcpy_s(ringbuf->fifo + ringbuf->endIdx, right, buf, cpSize);
    if (err != EOK) {
        return 0;
    }

    ringbuf->remain -= cpSize;
    ringbuf->endIdx += cpSize;
    if (ringbuf->endIdx == ringbuf->size) {
        ringbuf->endIdx = 0;
    }

    if (cpSize == size) {
        return size;
    } else {
        cpSize += OsRingbufWriteLinear(ringbuf, buf + cpSize, size - cpSize);
    }

    return cpSize;
}

UINT32 LOS_RingbufWrite(Ringbuf *ringbuf, const CHAR *buf, UINT32 size)
{
    UINT32 cpSize = 0;
    UINT32 intSave;
    if ((ringbuf == NULL) || (buf == NULL) || (size == 0) || (ringbuf->status != RBUF_INITED)) {
        return 0;
    }

    LOS_SpinLockSave(&ringbuf->lock, &intSave);

    if ((ringbuf->fifo == NULL) || (ringbuf->remain == 0))  {
        cpSize = 0;
        goto EXIT;
    }

    if (ringbuf->startIdx <= ringbuf->endIdx) {
        cpSize = OsRingbufWriteLoop(ringbuf, buf, size);
    } else {
        cpSize = OsRingbufWriteLinear(ringbuf, buf, size);
    }
EXIT:
    LOS_SpinUnlockRestore(&ringbuf->lock, intSave);
    return cpSize;
}

STATIC UINT32 OsRingbufReadLinear(Ringbuf *ringbuf, CHAR *buf, UINT32 size)
{
    UINT32 cpSize, remain;
    errno_t err;

    remain = ringbuf->endIdx - ringbuf->startIdx;
    cpSize = (remain < size) ? remain : size;

    if (cpSize == 0) {
        return 0;
    }

    err = memcpy_s(buf, size, ringbuf->fifo + ringbuf->startIdx, cpSize);
    if (err != EOK) {
        return 0;
    }

    ringbuf->remain += cpSize;
    ringbuf->startIdx += cpSize;

    return cpSize;
}

STATIC UINT32 OsRingbufReadLoop(Ringbuf *ringbuf, CHAR *buf, UINT32 size)
{
    UINT32 right, cpSize;
    errno_t err;

    right = ringbuf->size - ringbuf->startIdx;
    cpSize = (right < size) ? right : size;

    err = memcpy_s(buf, size, ringbuf->fifo + ringbuf->startIdx, cpSize);
    if (err != EOK) {
        return 0;
    }

    ringbuf->remain += cpSize;
    ringbuf->startIdx += cpSize;
    if (ringbuf->startIdx == ringbuf->size) {
        ringbuf->startIdx = 0;
    }

    if (cpSize < size) {
        cpSize += OsRingbufReadLinear(ringbuf, buf + cpSize, size - cpSize);
    }

    return cpSize;
}

UINT32 LOS_RingbufRead(Ringbuf *ringbuf, CHAR *buf, UINT32 size)
{
    UINT32 cpSize;
    UINT32 intSave;

    if ((ringbuf == NULL) || (buf == NULL) || (size == 0) || (ringbuf->status != RBUF_INITED)) {
        return 0;
    }

    LOS_SpinLockSave(&ringbuf->lock, &intSave);

    if ((ringbuf->fifo == NULL) || (ringbuf->remain == ringbuf->size)) {
        cpSize = 0;
        goto EXIT;
    }

    if (ringbuf->startIdx >= ringbuf->endIdx) {
        cpSize = OsRingbufReadLoop(ringbuf, buf, size);
    } else {
        cpSize = OsRingbufReadLinear(ringbuf, buf, size);
    }
EXIT:
    LOS_SpinUnlockRestore(&ringbuf->lock, intSave);
    return cpSize;
}

UINT32 LOS_RingbufInit(Ringbuf *ringbuf, CHAR *fifo, UINT32 size)
{
    if ((ringbuf == NULL) || (fifo == NULL) ||
        (ringbuf->status == RBUF_INITED) || (size == 0)) {
        return LOS_NOK;
    }

    (VOID)memset_s(ringbuf, sizeof(Ringbuf), 0, sizeof(Ringbuf));
    LOS_SpinInit(&ringbuf->lock);
    ringbuf->size = size;
    ringbuf->remain = size;
    ringbuf->fifo = fifo;
    ringbuf->status = RBUF_INITED;
    return LOS_OK;
}

VOID LOS_RingbufReset(Ringbuf *ringbuf)
{
    UINT32 intSave;

    if ((ringbuf == NULL) || (ringbuf->status != RBUF_INITED)) {
        return;
    }

    LOS_SpinLockSave(&ringbuf->lock, &intSave);
    ringbuf->startIdx = 0;
    ringbuf->endIdx = 0;
    ringbuf->remain = ringbuf->size;
    (VOID)memset_s(ringbuf->fifo, ringbuf->size, 0, ringbuf->size);
    LOS_SpinUnlockRestore(&ringbuf->lock, intSave);
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
