/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: SIGNAL ERROR
 * Author: Huawei LiteOS Team
 * Create: 2020-01-15
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

#include "errno.h"
#include "los_task.h"

#ifndef LOSCFG_LIB_CONFIGURABLE
int g_errnoArray[LOSCFG_BASE_CORE_TSK_LIMIT + 1];
#else
__attribute__((section(".libc.errno"))) int g_errnoArray[LOSCFG_BASE_CORE_TSK_LIMIT_CONFIG + 1];
#endif

/* the specific errno get or set in interrupt service routine */
static int g_errnoIsr;

void set_errno(int errCode)
{
    /* errno can not be set to 0 as posix standard */
    if (errCode == 0) {
        return;
    }

    if (OS_INT_INACTIVE) {
        g_errnoArray[LOS_CurTaskIDGet()] = errCode;
    } else {
        g_errnoIsr = errCode;
    }
}

int get_errno(void)
{
    if (OS_INT_INACTIVE) {
        return g_errnoArray[LOS_CurTaskIDGet()];
    } else {
        return g_errnoIsr;
    }
}

int *__errno_location(void)
{
    if (OS_INT_INACTIVE) {
        return &g_errnoArray[LOS_CurTaskIDGet()];
    } else {
        return &g_errnoIsr;
    }
}

volatile int *__errno(void)
{
    if (OS_INT_INACTIVE) {
        return (volatile int *)(&g_errnoArray[LOS_CurTaskIDGet()]);
    } else {
        return (volatile int *)(&g_errnoIsr);
    }
}
