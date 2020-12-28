/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Timer Header File
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#ifndef _LOS_TIMER_H
#define _LOS_TIMER_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern UINT64 hi_sched_clock(VOID);
extern UINT32 HalClockFreqRead(VOID);
extern VOID HalClockFreqWrite(UINT32 freq);
extern VOID HalClockStart(VOID);
extern VOID HalClockIrqInit(VOID);
extern VOID HalClockIrqClear(VOID);
extern VOID HalClockInitStart(VOID);
extern VOID HalClockEnable(VOID);
extern VOID HalClockInit(VOID);
extern VOID PlatformHalClockInitialize(UINT32 period);

extern UINT64 HalClockGetCycles(VOID);
extern UINT64 HalClockGetCurrCycles(VOID);
extern VOID HalDelayUs(UINT32 usecs);
extern UINT32 HalClockGetTickTimerCycles(VOID);
extern VOID HalClockTickTimerReload(UINT32 cycles);

extern UINT32 HrtimersInit(VOID);
extern VOID HrtimerClockIrqClear(VOID);
extern VOID HrtimerClockStart(UINT32 period);
extern VOID HrtimerClockStop(VOID);
extern UINT32 HrtimerClockValueGet(VOID);
extern VOID HrtimerClockInit(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_TIMER_H */
