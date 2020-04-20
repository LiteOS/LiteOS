/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: System Config Implementation
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

#include "los_config.h"
#include "string.h"
#include "stdio.h"
#ifdef LOSCFG_COMPAT_LINUX
#include "linux/workqueue.h"
#include "linux/module.h"
#endif
#include "los_sys.h"
#include "los_tick.h"
#include "los_task_pri.h"
#include "los_printf.h"
#include "los_swtmr_pri.h"
#include "los_timeslice_pri.h"
#include "los_memory_pri.h"
#include "los_sem_pri.h"
#include "los_mux_pri.h"
#include "los_queue_pri.h"
#include "los_memstat_pri.h"
#include "los_hwi_pri.h"
#include "los_spinlock.h"
#include "los_atomic.h"
#include "los_exc_pri.h"
#include "gic_common.h"

#ifdef LOSCFG_FS_VFS
#include "fs/fs.h"
#endif

#if (LOSCFG_KERNEL_TRACE == YES)
#include "los_trace.h"
#endif

#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif

#ifdef LOSCFG_COMPAT_POSIX
#include "pprivate.h"
#endif

#ifdef LOSCFG_DRIVERS_UART
#include "console.h"
#endif
#ifdef LOSCFG_KERNEL_TICKLESS
#include "los_tickless.h"
#endif
#ifdef LOSCFG_ARCH_CORTEX_M7
#include "los_exc_pri.h"
#endif
#ifdef LOSCFG_MEM_RECORDINFO
#include "los_memrecord_pri.h"
#endif
#include "los_hw_tick_pri.h"
#include "los_hwi_pri.h"
#ifdef LOSCFG_KERNEL_DYNLOAD
#include "los_ld_initlib_pri.h"
#endif

#ifdef LOSCFG_KERNEL_RUNSTOP
#include "los_runstop_pri.h"
#endif

#if defined(LOSCFG_HW_RANDOM_ENABLE) || defined (LOSCFG_DRIVERS_RANDOM)
#include "randomdev.h"
#include "yarrow.h"
#endif
#ifdef LOSCFG_SHELL_DMESG
#include "dmesg_pri.h"
#endif
#ifdef LOSCFG_SHELL_LK
#include "shell_pri.h"
#endif

#ifndef LOSCFG_PLATFORM_OSAPPINIT
#include "los_test_pri.h"
#endif
#ifdef LOSCFG_DRIVERS_BASE
#include "driver_base_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_PLATFORM_OSAPPINIT
extern UINT32 OsAppInit(VOID);
extern VOID app_init(VOID);
#endif

#if (LOSCFG_LIB_CONFIGURABLE == YES)
LITE_OS_SEC_BSS UINT32 g_taskLimit;
LITE_OS_SEC_BSS UINT32 g_semLimit;
LITE_OS_SEC_BSS UINT32 g_swtmrLimit;
LITE_OS_SEC_BSS UINT32 g_muxLimit;
LITE_OS_SEC_BSS UINT32 g_queLimit;

#endif


LITE_OS_SEC_TEXT_INIT VOID OsRegister(VOID)
{
#if (LOSCFG_LIB_CONFIGURABLE == YES)
    g_taskLimit = LOSCFG_BASE_CORE_TSK_CONFIG;
    g_semLimit = LOSCFG_BASE_IPC_SEM_CONFIG;
    g_swtmrLimit = LOSCFG_BASE_CORE_SWTMR_CONFIG;
    g_muxLimit = LOSCFG_BASE_IPC_MUX_CONFIG;
    g_queLimit = LOSCFG_BASE_IPC_QUEUE_CONFIG;
#endif

    /* LOSCFG_BASE_CORE_TSK_LIMIT include IDLE task */
    g_taskMaxNum = LOSCFG_BASE_CORE_TSK_LIMIT;
    g_sysClock = OS_SYS_CLOCK;
    g_tickPerSecond =  LOSCFG_BASE_CORE_TICK_PER_SECOND;

    return;
}

LITE_OS_SEC_TEXT_INIT VOID OsStart(VOID)
{
    LosTaskCB *taskCB = NULL;
    UINT32 cpuid = ArchCurrCpuid();

    OsTickStart();

    LOS_SpinLock(&g_taskSpin);
    taskCB = OsGetTopTask();

    OS_SCHEDULER_SET(cpuid);
    OsCurrTaskSet(taskCB);
    PRINTK("cpu %d entering scheduler\n", cpuid);
    OsStartToRun(taskCB);
}

LITE_OS_SEC_TEXT_INIT STATIC UINT32 OsIpcInit(VOID)
{
    UINT32 ret;
#if (LOSCFG_BASE_IPC_SEM == YES)
    ret = OsSemInit();
    if (ret != LOS_OK) {
        return ret;
    }
#endif

#if (LOSCFG_BASE_IPC_MUX == YES)
    ret = OsMuxInit();
    if (ret != LOS_OK) {
        return ret;
    }
#endif

#if (LOSCFG_BASE_IPC_QUEUE == YES)
    ret = OsQueueInit();
    if (ret != LOS_OK) {
        return ret;
    }
#endif
    return LOS_OK;
}

#if (LOSCFG_DRIVERS_BASE == YES)
LITE_OS_SEC_TEXT_INIT STATIC VOID OsDriverBaseInit(VOID)
{
    (VOID)BusInit();
    (VOID)PlatformBusInit();
    (VOID)do_initCalls(LEVEL_ARCH);
}
#endif

LITE_OS_SEC_TEXT_INIT UINT32 OsMain(void)
{
    UINT32 ret;

    ret = OsMemSystemInit((UINTPTR)&OS_SYS_MEM_START + OS_EXC_INTERACTMEM_SIZE);
    if (ret != LOS_OK) {
        return ret;
    }

    OsRegister();

#ifdef LOSCFG_SHELL_LK
    OsLkLoggerInit(NULL);
#endif

#ifdef LOSCFG_EXC_INTERACTION
#ifdef LOSCFG_ARCH_CORTEX_M7
    /* 4096: 4K space for Stack */
    ret = OsMemExcInteractionInit((UINT32)&__bss_end + 4096);
#else
    ret = OsMemExcInteractionInit((UINTPTR)&__bss_end);
#endif
    if (ret != LOS_OK) {
        return ret;
    }
#endif

#ifdef LOSCFG_SHELL_DMESG
    ret = OsDmesgInit();
    if (ret != LOS_OK) {
        return ret;
    }
#endif

#if (LOSCFG_PLATFORM_HWI == YES)
    OsHwiInit();
#endif

    OsExcInit();

    ret = OsTickInit(g_sysClock, LOSCFG_BASE_CORE_TICK_PER_SECOND);
    if (ret != LOS_OK) {
        return ret;
    }

#ifdef LOSCFG_PLATFORM_UART_WITHOUT_VFS
    uart_init();
#ifdef LOSCFG_SHELL
    uart_hwiCreate();
#endif //LOSCFG_SHELL
#endif //LOSCFG_PLATFORM_UART_WITHOUT_VFS
    ret = OsTaskInit();
    if (ret != LOS_OK) {
        PRINT_ERR("OsTaskInit error\n");
        return ret;
    }

#if (LOSCFG_BASE_CORE_TSK_MONITOR == YES)
    OsTaskMonInit();
#endif

#if ((LOSCFG_BASE_IPC_QUEUE == YES) || (LOSCFG_BASE_IPC_MUX == YES) || (LOSCFG_BASE_IPC_SEM == YES))
    ret = OsIpcInit();
    if (ret != LOS_OK) {
        return ret;
    }
#endif

    /*
     * CPUP should be inited before first task creation. Don't change this init sequence
     * if not neccessary. The sequence should be like this:
     * 1. OsIpcInit
     * 2. OsCpupInit -> has first task creation
     * 3. other inits have task creation
     */
#ifdef LOSCFG_KERNEL_CPUP
    ret = OsCpupInit();
    if (ret != LOS_OK) {
        PRINT_ERR("OsCpupInit error\n");
        return ret;
    }
#endif

#if (LOSCFG_BASE_CORE_SWTMR == YES)
    ret = OsSwtmrInit();
    if (ret != LOS_OK) {
        return ret;
    }
#endif

#ifdef LOSCFG_KERNEL_DYNLOAD
    ret = OsDynloadInit();
    if (ret != LOS_OK) {
        return ret;
    }
#endif

#if defined(LOSCFG_HW_RANDOM_ENABLE) || defined (LOSCFG_DRIVERS_RANDOM)
    random_alg_context.ra_init_alg(NULL);
    run_harvester_iterate(NULL);
#endif

    ret = OsIdleTaskCreate();
    if (ret != LOS_OK) {
        return ret;
    }

#ifdef LOSCFG_KERNEL_RUNSTOP
    ret = OsWowWriteFlashTaskCreate();
    if (ret != LOS_OK) {
        return ret;
    }
#endif

#if (LOSCFG_DRIVERS_BASE == YES)
    OsDriverBaseInit();
#endif

#ifdef LOSCFG_PLATFORM_OSAPPINIT
    ret = OsAppInit();
#else /* LOSCFG_TEST */
    ret = OsTestInit();
#endif
    if (ret != LOS_OK) {
        return ret;
    }
#if (LOSCFG_KERNEL_TRACE == YES)
    LOS_TraceInit();
#endif

    return LOS_OK;
}

#ifdef LOSCFG_PLATFORM_OSAPPINIT
STATIC UINT32 OsAppTaskCreate(VOID)
{
    UINT32 taskID;
    TSK_INIT_PARAM_S appTask;

    (VOID)memset_s(&appTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    appTask.pfnTaskEntry = (TSK_ENTRY_FUNC)app_init;
    appTask.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    appTask.pcName = "app_Task";
    appTask.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    appTask.uwResved = LOS_TASK_STATUS_DETACHED;
    return LOS_TaskCreate(&taskID, &appTask);
}

#ifdef LOSCFG_MEM_RECORDINFO
STATIC UINT32 OsMemShowTaskCreate(VOID)
{
    UINT32 taskID;
    TSK_INIT_PARAM_S appTask;

    (VOID)memset_s(&appTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    appTask.pfnTaskEntry = (TSK_ENTRY_FUNC)OsMemRecordShowTask;
    appTask.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    appTask.pcName = "memshow_Task";
    appTask.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    appTask.uwResved = LOS_TASK_STATUS_DETACHED;
    return LOS_TaskCreate(&taskID, &appTask);
}
#endif
#endif

UINT32 OsAppInit(VOID)
{
#ifdef LOSCFG_PLATFORM_OSAPPINIT
    UINT32 ret;
#ifdef LOSCFG_FS_VFS
    los_vfs_init();
#endif
#ifdef LOSCFG_COMPAT_LINUX
    ret = HrtimersInit();
    if (ret != LOS_OK) {
        PRINT_ERR("HrtimersInit error\n");
        return ret;
    }
    g_pstSystemWq = create_workqueue("system_wq");
#endif
    ret = OsAppTaskCreate();
    PRINTK("OsAppInit\n");
    if (ret != LOS_OK) {
        return ret;
    }
#ifdef LOSCFG_MEM_RECORDINFO
    ret = OsMemShowTaskCreate();
    if (ret != LOS_OK) {
        PRINTK("create memshow_Task error %u\n", ret);
        return ret;
    }
    PRINTK("create memshow_Task ok\n");
#endif
#ifdef LOSCFG_KERNEL_TICKLESS
    LOS_TicklessEnable();
#endif

#endif /* LOSCFG_PLATFORM_OSAPPINIT */

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
