/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Stack Info Implementation
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

#include "los_stackinfo_pri.h"
#include "los_printf_pri.h"
#include "los_base.h"
#include "los_hw_cpu.h"

const StackInfo *g_stackInfo = NULL;
UINT32 g_stackNum;

UINT32 OsStackWaterLineGet(const UINTPTR *stackBottom, const UINTPTR *stackTop, UINT32 *peakUsed)
{
    UINT32 size;
    const UINTPTR *tmp = NULL;
    if (*stackTop == OS_STACK_MAGIC_WORD) {
        tmp = stackTop + 1;
        while ((tmp < stackBottom) && (*tmp == OS_STACK_INIT)) {
            tmp++;
        }
        size = (UINTPTR)stackBottom - (UINTPTR)tmp;
        *peakUsed = (size == 0) ? size : (size + sizeof(CHAR *));
        return LOS_OK;
    } else {
        *peakUsed = OS_INVALID_WATERLINE;
        return LOS_NOK;
    }
}

VOID OsExcStackCheck(VOID)
{
    UINT32 index;
    UINT32 cpuid;
    UINTPTR *stackTop = NULL;

    if (g_stackInfo == NULL) {
        return;
    }
    for (index = 0; index < g_stackNum; index++) {
        for (cpuid = 0; cpuid < LOSCFG_KERNEL_CORE_NUM; cpuid++) {
            stackTop = (UINTPTR *)((UINTPTR)g_stackInfo[index].stackTop + cpuid * g_stackInfo[index].stackSize);
            if (*stackTop != OS_STACK_MAGIC_WORD) {
                PRINT_ERR("cpu:%u %s overflow , magic word changed to 0x%x\n",
                          LOSCFG_KERNEL_CORE_NUM - 1 - cpuid, g_stackInfo[index].stackName, *stackTop);
            }
        }
    }
}

VOID OsExcStackInfo(VOID)
{
    UINT32 index;
    UINT32 cpuid;
    UINT32 size;
    UINTPTR *stackTop = NULL;
    UINTPTR *stack = NULL;

    if (g_stackInfo == NULL) {
        return;
    }

    PrintExcInfo("\n stack name    cpu id     stack addr     total size   used size\n"
                 " ----------    ------     ---------      --------     --------\n");

    for (index = 0; index < g_stackNum; index++) {
        for (cpuid = 0; cpuid < LOSCFG_KERNEL_CORE_NUM; cpuid++) {
            stackTop = (UINTPTR *)((UINTPTR)g_stackInfo[index].stackTop + cpuid * g_stackInfo[index].stackSize);
            stack = (UINTPTR *)((UINTPTR)stackTop + g_stackInfo[index].stackSize);
            (VOID)OsStackWaterLineGet(stack, stackTop, &size);

            PrintExcInfo("%11s      %-5d    %-10p     0x%-8x   0x%-4x\n", g_stackInfo[index].stackName,
                         LOSCFG_KERNEL_CORE_NUM - 1 - cpuid, stackTop, g_stackInfo[index].stackSize, size);
        }
    }

    OsExcStackCheck();
}

VOID OsExcStackInfoReg(const StackInfo *stackInfo, UINT32 stackNum)
{
    g_stackInfo = stackInfo;
    g_stackNum = stackNum;
}

VOID OsStackInit(VOID *stacktop, UINT32 stacksize)
{
    /* initialize the task stack, write magic num to stack top */
    (VOID)memset_s(stacktop, stacksize, (INT32)OS_STACK_INIT, stacksize);
    *((UINTPTR *)stacktop) = OS_STACK_MAGIC_WORD;
}
