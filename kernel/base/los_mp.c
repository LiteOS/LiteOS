/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: Muti-core processing
 * Author: Huawei LiteOS Team
 * Create: 2018-07-10
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

#include "los_mp_pri.h"
#include "los_task_pri.h"
#include "los_percpu_pri.h"
#include "los_sched_pri.h"
#include "los_swtmr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_SMP

#ifdef LOSCFG_KERNEL_SMP_CALL
LITE_OS_SEC_BSS SPIN_LOCK_INIT(g_mpCallSpin);
#define MP_CALL_LOCK(state)       LOS_SpinLockSave(&g_mpCallSpin, &(state))
#define MP_CALL_UNLOCK(state)     LOS_SpinUnlockRestore(&g_mpCallSpin, (state))
#endif

VOID LOS_MpSchedule(UINT32 target)
{
    UINT32 cpuid = ArchCurrCpuid();
    target &= ~(1U << cpuid);
    HalIrqSendIpi(target, LOS_MP_IPI_SCHEDULE);
}

VOID OsMpWakeHandler(VOID)
{
    /* generic wakeup ipi, do nothing */
}

VOID OsMpScheduleHandler(VOID)
{
    /*
     * set schedule flag to differ from wake function,
     * so that the scheduler can be triggered at the end of irq.
     */
    OsPercpuGet()->schedFlag = INT_PEND_RESCH;
}

VOID OsMpHaltHandler(VOID)
{
    (VOID)LOS_IntLock();
    OsPercpuGet()->excFlag = CPU_HALT;

    while (1) {}
}

VOID OsMpCollectTasks(VOID)
{
    LosTaskCB *taskCB = NULL;
    UINT32 taskId = 0;
    UINT32 ret;

    /* recursive checking all the available task */
    for (; taskId <= g_taskMaxNum; taskId++) {
        taskCB = &g_taskCBArray[taskId];

        if ((taskCB->taskStatus & OS_TASK_STATUS_UNUSED) ||
            (taskCB->taskStatus & OS_TASK_STATUS_RUNNING)) {
            continue;
        }

        /*
         * though task status is not atomic, this check may success but not accomplish
         * the deletion; this deletion will be handled until the next run.
         */
        if (taskCB->signal & SIGNAL_KILL) {
            ret = LOS_TaskDelete(taskId);
            if (ret != LOS_OK) {
                PRINT_WARN("GC collect task failed err:0x%x\n", ret);
            }
        }
    }
}

#ifdef LOSCFG_KERNEL_SMP_CALL
VOID OsMpFuncCall(UINT32 target, SMP_FUNC_CALL func, VOID *args)
{
    UINT32 index;
    UINT32 intSave;

    if (func == NULL) {
        return;
    }

    if (!(target & OS_MP_CPU_ALL)) {
        return;
    }

    for (index = 0; index < LOSCFG_KERNEL_CORE_NUM; index++) {
        if (CPUID_TO_AFFI_MASK(index) & target) {
            MpCallFunc *mpCallFunc = (MpCallFunc *)LOS_MemAlloc(m_aucSysMem0, sizeof(MpCallFunc));
            if (mpCallFunc == NULL) {
                PRINT_ERR("smp func call malloc failed\n");
                return;
            }
            mpCallFunc->func = func;
            mpCallFunc->args = args;

            MP_CALL_LOCK(intSave);
            LOS_ListAdd(&g_percpu[index].funcLink, &(mpCallFunc->node));
            MP_CALL_UNLOCK(intSave);
        }
    }
    HalIrqSendIpi(target, LOS_MP_IPI_FUNC_CALL);
}

VOID OsMpFuncCallHandler(VOID)
{
    UINT32 intSave;
    UINT32 cpuid = ArchCurrCpuid();
    LOS_DL_LIST *list = NULL;
    MpCallFunc* mpCallFunc = NULL;

    MP_CALL_LOCK(intSave);
    while (!LOS_ListEmpty(&g_percpu[cpuid].funcLink)) {
        list = LOS_DL_LIST_FIRST(&g_percpu[cpuid].funcLink);
        LOS_ListDelete(list);
        MP_CALL_UNLOCK(intSave);

        mpCallFunc = LOS_DL_LIST_ENTRY(list, MpCallFunc, node);
        mpCallFunc->func(mpCallFunc->args);
        (VOID)LOS_MemFree(m_aucSysMem0, mpCallFunc);

        MP_CALL_LOCK(intSave);
    }
    MP_CALL_UNLOCK(intSave);
}

VOID OsMpFuncCallInit(VOID)
{
    UINT32 index;
    /* init funclink for each core */
    for (index = 0; index < LOSCFG_KERNEL_CORE_NUM; index++) {
        LOS_ListInit(&g_percpu[index].funcLink);
    }
}
#endif /* LOSCFG_KERNEL_SMP_CALL */

UINT32 OsMpInit(VOID)
{
    UINT16 swtmrId;
    UINT32 ret;

    ret = LOS_SwtmrCreate(OS_MP_GC_PERIOD, LOS_SWTMR_MODE_PERIOD,
                          (SWTMR_PROC_FUNC)OsMpCollectTasks, &swtmrId, 0);
    if (ret != LOS_OK) {
        PRINT_ERR("Swtmr Create failed err:0x%x\n", ret);
        return ret;
    }
    ret = LOS_SwtmrStart(swtmrId);
    if (ret != LOS_OK) {
        PRINT_ERR("Swtmr Start failed err:0x%x\n", ret);
        return ret;
    }
#ifdef LOSCFG_KERNEL_SMP_CALL
    OsMpFuncCallInit();
#endif
    return LOS_OK;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
