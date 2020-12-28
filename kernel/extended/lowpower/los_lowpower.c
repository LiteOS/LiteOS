/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Low-power Framework.
 * Author: Huawei LiteOS Team
 * Create: 2020-09-19
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

#include "los_lowpower_pri.h"
#ifdef LOSCFG_KERNEL_TICKLESS
#include "los_tickless_pri.h"
#endif

#define CALL_PMOPS_FUNC_VOID(func, ...) CALL_FUNC_NO_RETURN_FROM_PTR(g_pmOps, func, __VA_ARGS__)
#define CALL_PMOPS_FUNC_RET(func, ret, ...) CALL_FUNC_WITH_RETURN_FROM_PTR(g_pmOps, ret, func, __VA_ARGS__)

__attribute__((section(".data"))) STATIC const PowerMgrOps *g_pmOps = NULL;

VOID LOS_LowpowerInit(const PowerMgrOps *pmOps)
{
    if (pmOps == NULL) {
        PRINT_ERR("\r\n [PM] PowerMgrOps must be non-null.\n");
        return;
    } else if (pmOps->process == NULL) {
        PRINT_ERR("\r\n [PM] %s must be non-null.\n", __FUNCTION__);
        return;
    }

#ifdef LOSCFG_KERNEL_POWER_MGR
    if (g_pmOps != NULL) {
        PRINT_ERR("\r\n [PM] Reassignment of PowerMgrOps is forbidden.\n");
        return;
    }
#endif
    g_pmOps = pmOps;

    LOS_LowpowerHookReg(OsPowerMgrProcess);

    LOS_IntWakeupHookReg(OsPowerMgrWakeUpFromInterrupt);
}

VOID OsPowerMgrProcess(VOID)
{
#ifdef LOSCFG_KERNEL_POWER_MGR
    CALL_PMOPS_FUNC_VOID(process);
#else
    if (g_pmOps == NULL) {
#ifdef LOSCFG_KERNEL_TICKLESS
        OsTicklessOpen();
        wfi();
#endif
    } else {
        CALL_PMOPS_FUNC_VOID(process);
    }
#endif
}

VOID OsPowerMgrWakeUpFromInterrupt(UINT32 intNum)
{
#ifdef LOSCFG_KERNEL_POWER_MGR
    CALL_PMOPS_FUNC_VOID(resumeFromInterrupt, intNum);
#else
    if (g_pmOps == NULL) {
#ifdef LOSCFG_KERNEL_TICKLESS
        OsTicklessUpdate(intNum);
#endif
    } else {
        CALL_PMOPS_FUNC_VOID(resumeFromInterrupt, intNum);
    }
#endif
}

VOID OsPowerMgrWakeupFromReset(VOID)
{
    CALL_PMOPS_FUNC_VOID(wakeupFromReset);
}

VOID LOS_PowerMgrChangeFreq(LosFreqMode freq)
{
    CALL_PMOPS_FUNC_VOID(changeFreq, freq);
}

VOID LOS_PowerMgrDeepSleepVoteBegin(VOID)
{
    CALL_PMOPS_FUNC_VOID(deepSleepVoteBegin);
}

VOID LOS_PowerMgrDeepSleepVoteEnd(VOID)
{
    CALL_PMOPS_FUNC_VOID(deepSleepVoteEnd);
}

VOID LOS_PowerMgrSleepDelay(UINT32 tick)
{
    CALL_PMOPS_FUNC_VOID(deepSleepVoteDelay, tick);
}

VOID LOS_PowerMgrRegisterExtVoter(UINT32 (*callback)(VOID))
{
    CALL_PMOPS_FUNC_VOID(registerExternalVoter, callback);
}

UINT32 LOS_PowerMgrGetSleepMode(VOID)
{
    UINT32 ret = 0;
    CALL_PMOPS_FUNC_RET(getSleepMode, ret);
    return ret;
}

UINT32 LOS_PowerMgrGetDeepSleepVoteCount(VOID)
{
    UINT32 ret = 0;
    CALL_PMOPS_FUNC_RET(getDeepSleepVoteCount, ret);
    return ret;
}
