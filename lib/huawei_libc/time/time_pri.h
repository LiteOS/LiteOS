/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Time HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2019-12-18
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

#ifndef _TIME_PRI_H
#define _TIME_PRI_H

#include "time.h"
#include "stdint.h"
#include "errno.h"
#include "los_tick_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* internal functions */
STATIC INLINE BOOL ValidTimespec(const struct timespec *tp)
{
    /* Fail a NULL pointer */
    if (tp == NULL) {
        return FALSE;
    }

    /* Fail illegal nanosecond values */
    if ((tp->tv_nsec < 0) || (tp->tv_nsec >= OS_SYS_NS_PER_SECOND) || \
        (tp->tv_sec < 0) || (tp->tv_sec > INT32_MAX)) {
        return FALSE;
    }

    return TRUE;
}

STATIC INLINE UINT32 OsTimespec2Tick(const struct timespec *tp)
{
    UINT64 tickCnt, tmp, tickNs;

    /* Fail a NULL pointer */
    if (tp == NULL) {
        return 0;
    }

    tickNs = ((UINT64)tp->tv_nsec * LOSCFG_BASE_CORE_TICK_PER_SECOND) / OS_SYS_NS_PER_SECOND;
    tmp = (UINT64)tp->tv_nsec - (((UINT64)tickNs * OS_SYS_NS_PER_SECOND) / LOSCFG_BASE_CORE_TICK_PER_SECOND);
    if (tmp > 0) {
        tickNs++;
    }

    tickCnt = (UINT64)tp->tv_sec * LOSCFG_BASE_CORE_TICK_PER_SECOND + tickNs;
    if (tickCnt > LOS_WAIT_FOREVER) {
        tickCnt = LOS_WAIT_FOREVER;
    }
    return (UINT32)tickCnt;
}

STATIC INLINE VOID OsTick2TimeSpec(struct timespec *tp, UINT32 tick)
{
    tp->tv_sec = (time_t)(tick / LOSCFG_BASE_CORE_TICK_PER_SECOND);
    tick = tick % LOSCFG_BASE_CORE_TICK_PER_SECOND;
    tp->tv_nsec = (long)((UINT64)tick * OS_SYS_NS_PER_SECOND) / LOSCFG_BASE_CORE_TICK_PER_SECOND;
}

extern VOID OsAdjTime(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* _TIME_PRI_H */
