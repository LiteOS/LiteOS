/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Multi-Core Private HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2018-07-11
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

#ifndef _LOS_MP_PRI_H
#define _LOS_MP_PRI_H

#include "los_typedef.h"
#include "los_toolchain.h"
#include "los_config.h"
#include "los_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_MP_CPU_ALL       LOSCFG_KERNEL_CPU_MASK

#define OS_MP_GC_PERIOD     100 /* ticks */

typedef enum {
    LOS_MP_IPI_WAKEUP,
    LOS_MP_IPI_SCHEDULE,
    LOS_MP_IPI_HALT,
#ifdef LOSCFG_KERNEL_SMP_CALL
    LOS_MP_IPI_FUNC_CALL,
#endif
} MP_IPI_TYPE;

typedef VOID (*SMP_FUNC_CALL)(VOID *args);

#ifdef LOSCFG_KERNEL_SMP
extern VOID LOS_MpSchedule(UINT32 target);
extern VOID OsMpWakeHandler(VOID);
extern VOID OsMpScheduleHandler(VOID);
extern VOID OsMpHaltHandler(VOID);
extern UINT32 OsMpInit(VOID);
#else
STATIC INLINE VOID LOS_MpSchedule(UINT32 target)
{
    (VOID)target;
}
#endif

#ifdef LOSCFG_KERNEL_SMP_CALL

typedef struct {
    LOS_DL_LIST node;
    SMP_FUNC_CALL func;
    VOID *args;
} MpCallFunc;

/**
 * It is used to call function on target cpus by send ipi, and the first param is target cpu mask value.
 */
extern VOID OsMpFuncCall(UINT32 target, SMP_FUNC_CALL func, VOID *args);
extern VOID OsMpFuncCallHandler(VOID);
#else
STATIC INLINE VOID OsMpFuncCall(UINT32 target, SMP_FUNC_CALL func, VOID *args)
{
    (VOID)target;
    if (func != NULL) {
        func(args);
    }
}
#endif /* LOSCFG_KERNEL_SMP_CALL */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MP_PRI_H */
