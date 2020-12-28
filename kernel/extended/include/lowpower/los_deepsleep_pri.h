/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Low-power Framework.
 * Author: Huawei LiteOS Team
 * Create: 2020-10-27
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

#ifndef _LOS_DEEPSLEEP_H
#define _LOS_DEEPSLEEP_H

#include "los_task_pri.h"

typedef enum {
    LOS_COLD_RESET = 0,
    LOS_RUN_STOP_RESET,
    LOS_DEEP_SLEEP_RESET,
    LOS_INTERMIT_STANDBY_RESET,
} LosResetMode;

#if defined(LOSCFG_KERNEL_RUNSTOP) || defined(LOSCFG_KERNEL_DEEPSLEEP)
/* Is system is up from the memory image, then this flag should be 1; else 0 */
extern INT32 g_resumeFromImg;
extern INTPTR g_otherCoreResume;

extern VOID OsSRSaveRegister(VOID);
extern VOID OsSRRestoreRegister(VOID);

#if (LOSCFG_KERNEL_SMP == YES)
extern VOID release_secondary_cores(VOID);
#endif

#ifdef LOSCFG_AARCH64
/*
 * 34: The number of task context registers(X0~X30, SP, DAIF, NZCV)
 * 3: The number of available universal registers(X0, X1, X2) temporarily saved
 */
#define OS_TASK_REG_NUM 34
#define OS_UNI_REG_NUM 3
extern UINT64 g_saveSRContext[OS_TASK_REG_NUM];
extern UINT64 g_saveAR[OS_UNI_REG_NUM];
#else
/*
 * 18: The number of task context registers(R0~R15, SPSR, CPSR)
 * 2: The number of available universal registers(R0, R1) temporarily saved
 */
#define OS_TASK_REG_NUM 18
#define OS_UNI_REG_NUM 2
extern UINT32 g_saveSRContext[LOSCFG_KERNEL_CORE_NUM][OS_TASK_REG_NUM];
extern UINT32 g_saveAR[OS_UNI_REG_NUM];
extern LosTaskCB *g_saveTsk[LOSCFG_KERNEL_CORE_NUM];
#endif
#endif
#endif // _LOS_DEEPSLEEP_H