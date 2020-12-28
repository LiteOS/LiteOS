/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Aarch32 Cortex-M Hw Task Implementation
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

#include "los_task_pri.h"
#include "arch/task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


VOID *g_runTask = NULL;
VOID *g_oldTask = NULL;

#ifdef LOSCFG_GDB
STATIC VOID OsTaskEntrySetupLoopFrame(UINT32) __attribute__((noinline, naked));
VOID OsTaskEntrySetupLoopFrame(UINT32 arg0)
{
    asm volatile("\tsub fp, sp, #0x4\n"
                 "\tpush {fp, lr}\n"
                 "\tadd fp, sp, #0x4\n"
                 "\tpush {fp, lr}\n"

                 "\tadd fp, sp, #0x4\n"
                 "\tbl OsTaskEntry\n"

                 "\tpop {fp, lr}\n"
                 "\tpop {fp, pc}\n");
}
#endif

LITE_OS_SEC_TEXT_MINOR VOID OsTaskExit(VOID)
{
    __disable_irq();
    while (1) { }
}

LITE_OS_SEC_TEXT_INIT VOID *OsTaskStackInit(UINT32 taskId, UINT32 stackSize, VOID *topStack)
{
    TaskContext *taskContext = NULL;

    OsStackInit(topStack, stackSize);
    taskContext = (TaskContext *)(((UINTPTR)topStack + stackSize) - sizeof(TaskContext));

#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED) && (__FPU_USED == 1U)))
    taskContext->S16 = 0xAA000010;
    taskContext->S17 = 0xAA000011;
    taskContext->S18 = 0xAA000012;
    taskContext->S19 = 0xAA000013;
    taskContext->S20 = 0xAA000014;
    taskContext->S21 = 0xAA000015;
    taskContext->S22 = 0xAA000016;
    taskContext->S23 = 0xAA000017;
    taskContext->S24 = 0xAA000018;
    taskContext->S25 = 0xAA000019;
    taskContext->S26 = 0xAA00001A;
    taskContext->S27 = 0xAA00001B;
    taskContext->S28 = 0xAA00001C;
    taskContext->S29 = 0xAA00001D;
    taskContext->S30 = 0xAA00001E;
    taskContext->S31 = 0xAA00001F;
    taskContext->S0  = 0xAA000000;
    taskContext->S1  = 0xAA000001;
    taskContext->S2  = 0xAA000002;
    taskContext->S3  = 0xAA000003;
    taskContext->S4  = 0xAA000004;
    taskContext->S5  = 0xAA000005;
    taskContext->S6  = 0xAA000006;
    taskContext->S7  = 0xAA000007;
    taskContext->S8  = 0xAA000008;
    taskContext->S9  = 0xAA000009;
    taskContext->S10 = 0xAA00000A;
    taskContext->S11 = 0xAA00000B;
    taskContext->S12 = 0xAA00000C;
    taskContext->S13 = 0xAA00000D;
    taskContext->S14 = 0xAA00000E;
    taskContext->S15 = 0xAA00000F;
    taskContext->FPSCR = 0x00000000;
    taskContext->NO_NAME = 0xAA000011;
#endif

    taskContext->R4  = 0x04040404L;
    taskContext->R5  = 0x05050505L;
    taskContext->R6  = 0x06060606L;
    taskContext->R7  = 0x07070707L;
    taskContext->R8  = 0x08080808L;
    taskContext->R9  = 0x09090909L;
    taskContext->R10 = 0x10101010L;
    taskContext->R11 = 0x11111111L;
    taskContext->PriMask = 0;
    taskContext->R0  = taskId;
    taskContext->R1  = 0x01010101L;
    taskContext->R2  = 0x02020202L;
    taskContext->R3  = 0x03030303L;
    taskContext->R12 = 0x12121212L;
    taskContext->LR  = (UINT32)OsTaskExit;
    taskContext->PC  = (UINT32)OsTaskEntry;
    taskContext->xPSR = 0x01000000L;

    return (VOID *)taskContext;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
