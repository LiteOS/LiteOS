/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Runstop
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

#include "los_runstop_pri.h"
#include "los_task_pri.h"
#include "los_event_pri.h"
#include "los_tick_pri.h"
#ifndef LOSCFG_PLATFORM_NO_UART
#include "uart.h"
#endif
#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif

#include "arch/cpu.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define WAKEUP_FROM_SUSPEND 0x01
#define FLASH_IMG_SUCCESS   0x02

/* If core is ready for imaging */
LITE_OS_SEC_DATA_MINOR STATIC UINT32 g_sysDoneFlag[LOSCFG_KERNEL_CORE_NUM] = {
    [0 ... (LOSCFG_KERNEL_CORE_NUM - 1)] = OS_NO_STORE_SYSTEM
};

/* Start position of flash to write image */
LITE_OS_SEC_DATA_MINOR STATIC UINTPTR g_flashImgAddr;

/* Start position of heap memory after carry the image from flash to memory */
LITE_OS_SEC_DATA_MINOR STATIC const VOID *g_heapMemStart = NULL;

/* Size of heap memory in image */
LITE_OS_SEC_DATA_MINOR STATIC size_t g_heapMemSize = 0;
#ifdef LOSCFG_EXC_INTERACTION
/* Start position of exc interaction heap memory after carry the image from flash to memory */
LITE_OS_SEC_DATA_MINOR STATIC const VOID *g_excInteractionMemStart = NULL;

/* Size of exc interaction heap memory in image */
LITE_OS_SEC_DATA_MINOR STATIC size_t g_excInteractionMemSize = 0;
#endif
/* Size of wow image */
LITE_OS_SEC_DATA_MINOR STATIC size_t g_wowImgSize = 0;

LITE_OS_SEC_DATA_MINOR STATIC RUNSTOP_PARAM_S g_runstopParam;
LITE_OS_SEC_DATA_MINOR STATIC EVENT_CB_S g_suspendResumeEvent;
LITE_OS_SEC_DATA_MINOR STATIC EVENT_CB_S g_writeFlashEvent;

typedef struct {
    UINTPTR memStart;
    UINTPTR flashStart;
    size_t memSize;
} FlashWriteParam;

BOOL IsImageResume(VOID)
{
    return (g_resumeFromImg != LOS_COLD_RESET);
}

LITE_OS_SEC_TEXT_MINOR STATIC VOID OsDoWriteWow2Flash(FLASH_WRITE_FUNC flashWriteFunc,
                                                      const FlashWriteParam *wowSection,
                                                      const FlashWriteParam *excHeapSection,
                                                      const FlashWriteParam *heapMemSection)
{
    if ((wowSection->memSize != 0) &&
        (flashWriteFunc((VOID *)wowSection->memStart, wowSection->flashStart, wowSection->memSize) != 0)) {
        PRINT_ERR("%s, %d\n", __FUNCTION__, __LINE__);
        return;
    }

    if ((excHeapSection->memSize != 0) &&
        (flashWriteFunc((VOID *)excHeapSection->memStart, excHeapSection->flashStart, excHeapSection->memSize)) != 0) {
        PRINT_ERR("%s, %d\n", __FUNCTION__, __LINE__);
        return;
    }

    if ((heapMemSection->memSize != 0) &&
        (flashWriteFunc((VOID *)heapMemSection->memStart, heapMemSection->flashStart, heapMemSection->memSize) != 0)) {
        PRINT_ERR("%s, %d\n", __FUNCTION__, __LINE__);
        return;
    }
}

LITE_OS_SEC_TEXT_MINOR VOID OsWriteWow2Flash(VOID)
{
    FlashWriteParam wowSection;
    FlashWriteParam excHeapSection = {0};
    FlashWriteParam heapMemSection;
    size_t eraseAlignSize;
    size_t writeAlignSize;
    FLASH_WRITE_FUNC flashWriteFunc = g_runstopParam.pfFlashWriteFunc;

    eraseAlignSize = g_runstopParam.uwFlashEraseAlignSize;
    writeAlignSize = g_runstopParam.uwFlashWriteAlignSize;
    writeAlignSize = (writeAlignSize >= eraseAlignSize) ? writeAlignSize : eraseAlignSize;
    if (flashWriteFunc == NULL) {
        PRINT_ERR("%s, %d\n", __FUNCTION__, __LINE__);
        return;
    }

    wowSection.memStart = (UINTPTR)&__ram_vectors_vma;
    wowSection.flashStart = g_flashImgAddr;
    wowSection.memSize = ((UINTPTR)&__wow_end) - ((UINTPTR)&__ram_vectors_vma);
    wowSection.memSize = (wowSection.memSize + writeAlignSize - 1) & ~(writeAlignSize - 1);

#ifdef LOSCFG_EXC_INTERACTION
    excHeapSection.memStart = (UINTPTR)m_aucSysMem0;
    excHeapSection.flashStart = g_flashImgAddr + wowSection.memSize;
    excHeapSection.memSize = LOS_MemLastUsedGet(m_aucSysMem0) - (UINTPTR)m_aucSysMem0;
    excHeapSection.memSize = (excHeapSection.memSize + writeAlignSize - 1) & ~(writeAlignSize - 1);

    g_excInteractionMemStart = (VOID *)(((UINTPTR)&__ram_vectors_vma) + wowSection.memSize);
    g_excInteractionMemSize = excHeapSection.memSize;
#endif

    heapMemSection.memStart = (UINTPTR)m_aucSysMem1;
    heapMemSection.flashStart = g_flashImgAddr + wowSection.memSize + excHeapSection.memSize;
    heapMemSection.memSize = LOS_MemLastUsedGet(m_aucSysMem1) - (UINTPTR)m_aucSysMem1;
    heapMemSection.memSize = (heapMemSection.memSize + writeAlignSize - 1) & ~(writeAlignSize - 1);

    g_heapMemSize = heapMemSection.memSize;
    g_heapMemStart = (VOID *)(((UINTPTR)&__ram_vectors_vma) + wowSection.memSize + excHeapSection.memSize);
    g_wowImgSize = wowSection.memSize + heapMemSection.memSize + excHeapSection.memSize;

    OsDoWriteWow2Flash(flashWriteFunc, &wowSection, &excHeapSection, &heapMemSection);
}

LITE_OS_SEC_TEXT_MINOR VOID OsSystemSuspend(VOID)
{
    UINT32 cpuid;

    (VOID)LOS_IntLock();
    LOS_TaskLock();
    cpuid = ArchCurrCpuid();

    g_sysDoneFlag[cpuid] = OS_NO_STORE_SYSTEM;
    g_saveTsk[cpuid] = OsCurrTaskGet();

    OsSRSaveRegister();
    /* If 1 core, only to save registers */
    if (cpuid != 0) {
        if (g_otherCoreResume != 0) {
            HalIrqInitPercpu();
            OsTickStart();
            LOS_TaskUnlock();
            (VOID)LOS_IntUnLock();
            return;
        }
        g_sysDoneFlag[cpuid - 1] = OS_STORE_SYSTEM;
        while (1) {}
    }

    if (g_resumeFromImg) {
        OsWriteWow2Flash();
        LOS_TaskUnlock();
        (VOID)LOS_IntUnLock();
        (VOID)LOS_EventWrite(&g_suspendResumeEvent, FLASH_IMG_SUCCESS);
    } else {
        OsTickStart();
        LOS_TaskUnlock();
        (VOID)LOS_IntUnLock();

        if (g_runstopParam.pfIdleWakeupCallback != NULL) {
            g_runstopParam.pfIdleWakeupCallback();
        }
        (VOID)LOS_EventWrite(&g_suspendResumeEvent, WAKEUP_FROM_SUSPEND);
    }
}

LITE_OS_SEC_TEXT VOID OsWriteToFlashTask(VOID)
{
    (VOID)LOS_EventInit(&g_writeFlashEvent);

    while (1) {
        (VOID)LOS_EventRead(&g_writeFlashEvent, 0x01, LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);
        OsSystemSuspend();
    }
}

LITE_OS_SEC_TEXT VOID OsStoreSystemInfoBeforeSuspend(VOID)
{
    UINT32 cpuid = ArchCurrCpuid();
    if (cpuid != 0) {
        OsSystemSuspend();
    } else {
        (VOID)LOS_EventWrite(&g_writeFlashEvent, 0x01);
    }
    return;
}

LITE_OS_SEC_TEXT_MINOR VOID OsSystemWakeup(VOID)
{
    UINT32 cpuid;
    errno_t err;

    if (!g_resumeFromImg) {
        return;
    }

#ifdef LOSCFG_EXC_INTERACTION
    err = memmove_s(m_aucSysMem0, g_excInteractMemSize, g_excInteractionMemStart, g_excInteractionMemSize);
    if (err != EOK) {
        PRINT_RELEASE("%s, %d failed, errno = %d\n", __FUNCTION__, __LINE__, err);
        return;
    }
#endif
    err = memmove_s(m_aucSysMem1, OS_SYS_MEM_SIZE, g_heapMemStart, g_heapMemSize);
    if (err != EOK) {
        PRINT_RELEASE("%s, %d failed, errno = %d\n", __FUNCTION__, __LINE__, err);
        return;
    }
    dsb();
    /* Set the task of the current CPU */
    cpuid = ArchCurrCpuid();
    OsCurrTaskSet(g_saveTsk[cpuid]);

    /* Set system counter freq */
    HalClockFreqWrite(OS_SYS_CLOCK);
    dsb();
#ifdef LOSCFG_EXC_INTERACTION
    if (LOS_MemPoolSizeGet(m_aucSysMem0) != g_excInteractionMemSize) {
        OsMemResetEndNode(m_aucSysMem0, (UINTPTR)m_aucSysMem0 + g_excInteractionMemSize);
    }
#endif
    if (LOS_MemPoolSizeGet(m_aucSysMem1) != g_heapMemSize) {
        OsMemResetEndNode(m_aucSysMem1, (UINTPTR)m_aucSysMem1 + g_heapMemSize);
    }

    (VOID)memset_s((VOID *)&__bss_start, (((UINTPTR)&__bss_end) - ((UINTPTR)&__bss_start)), 0,
        (((UINTPTR)&__bss_end) - ((UINTPTR)&__bss_start)));

    HalIrqInit();

#ifndef LOSCFG_PLATFORM_NO_UART
    uart_early_init();
#endif

    g_resumeFromImg = LOS_COLD_RESET;
    g_otherCoreResume = 1;

#ifdef LOSCFG_KERNEL_CPUP
    OsSetCpuCycle(0);
#endif

#if (LOSCFG_KERNEL_SMP == YES)
    release_secondary_cores();
#endif
    OsSRRestoreRegister();
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsWaitImagingDone(UINTPTR wowFlashAddr, size_t *wowImgSize)
{
    UINT32 ret;

    g_flashImgAddr = wowFlashAddr;
    (VOID)LOS_EventInit(&g_suspendResumeEvent);

    // This flag will be stored into flash, and will affect the wakeup procedure when cpu is rebooting.
    g_resumeFromImg = LOS_RUN_STOP_RESET;
    // This flag affects the suspending procedure later,
    // and will be reset depending on 'g_resumeFromImg' when cpu is rebooting.
    g_otherCoreResume = 0;
    g_sysDoneFlag[LOSCFG_KERNEL_CORE_NUM - 1] = OS_STORE_SYSTEM;

    ret = LOS_EventRead(&g_suspendResumeEvent, 0xFF, LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);
    if (wowImgSize != NULL) {
        *wowImgSize = g_wowImgSize;
    }

    return ret;
}

LITE_OS_SEC_TEXT_MINOR VOID OsCarryLeftScatter(VOID)
{
    size_t size;
    UINTPTR memAddr;
    size_t wowSize;
    size_t readAlignSize;
    size_t eraseAlignSize;
    size_t writeAlignSize;
    UINTPTR imageFlashAddr;
    FLASH_READ_FUNC flashReadFunc = g_runstopParam.pfFlashReadFunc;

    imageFlashAddr = g_runstopParam.uwImageFlashAddr;
    readAlignSize = g_runstopParam.uwFlashReadAlignSize;
    eraseAlignSize = g_runstopParam.uwFlashEraseAlignSize;
    writeAlignSize = g_runstopParam.uwFlashWriteAlignSize;
    writeAlignSize = (writeAlignSize >= eraseAlignSize) ? writeAlignSize : eraseAlignSize;

    wowSize = ((UINTPTR)&__wow_end) - ((UINTPTR)&__ram_vectors_vma);
    wowSize = (wowSize + writeAlignSize - 1) & ~(writeAlignSize - 1);
    imageFlashAddr += wowSize;

    memAddr = ((UINTPTR)&__ram_vectors_vma) + wowSize;
    if (memAddr >= ((UINTPTR)&__bss_start)) {
        return;
    }
    size = ((UINTPTR)&__int_stack_start) - memAddr;
    size = (size + readAlignSize - 1) & ~(readAlignSize - 1);

    if ((flashReadFunc != NULL) && (flashReadFunc((VOID *)memAddr, imageFlashAddr, size) != 0)) {
        PRINT_ERR("%s, %d\n", __FUNCTION__, __LINE__);
    }
    dsb();
    flush_icache();
    flush_dcache(memAddr, memAddr + size);
}

LITE_OS_SEC_TEXT_MINOR VOID OsRunstopParamInit(const RUNSTOP_PARAM_S *runstopParam)
{
    g_runstopParam.pfIdleWakeupCallback = runstopParam->pfIdleWakeupCallback;
    g_runstopParam.pfWakeupCallback = runstopParam->pfWakeupCallback;
    g_runstopParam.pfImageDoneCallback = runstopParam->pfImageDoneCallback;
    g_runstopParam.pfFlashReadFunc = runstopParam->pfFlashReadFunc;
    g_runstopParam.pfFlashWriteFunc = runstopParam->pfFlashWriteFunc;
    g_runstopParam.uwFlashReadAlignSize = runstopParam->uwFlashReadAlignSize;
    g_runstopParam.uwFlashEraseAlignSize = runstopParam->uwFlashEraseAlignSize;
    g_runstopParam.uwFlashWriteAlignSize = runstopParam->uwFlashWriteAlignSize;
    g_runstopParam.uwWowFlashAddr = runstopParam->uwWowFlashAddr;
    g_runstopParam.uwImageFlashAddr = runstopParam->uwImageFlashAddr;
}

LITE_OS_SEC_TEXT_MINOR VOID LOS_MakeImage(RUNSTOP_PARAM_S *runstopParam)
{
    UINT32 ret;
    size_t imgSize;

    if (runstopParam == NULL) {
        return;
    }
    OsRunstopParamInit(runstopParam);

    ret = OsWaitImagingDone(g_runstopParam.uwWowFlashAddr, &imgSize);
    if (ret == WAKEUP_FROM_SUSPEND) {
        if (g_runstopParam.pfWakeupCallback != NULL) {
            g_runstopParam.pfWakeupCallback();
        }
        OsCarryLeftScatter();
        PRINT_INFO("Resume ok!\n");
    } else if (ret == FLASH_IMG_SUCCESS) {
        if (g_runstopParam.pfImageDoneCallback != NULL) {
            g_runstopParam.pfImageDoneCallback();
        }

        PRINT_INFO("Flash ok! Image length 0x%x\n", imgSize);
    }
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsWowWriteFlashTaskCreate(VOID)
{
    UINT32 ret;
    UINT32 writeFlashTaskId;
    TSK_INIT_PARAM_S taskInitParam;

    (VOID)memset_s((VOID *)(&taskInitParam), sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)OsWriteToFlashTask;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.pcName = "WowWriteFlashTask";
    taskInitParam.usTaskPrio = OS_TASK_PRIORITY_LOWEST - 1;
#ifdef LOSCFG_KERNEL_SMP
    taskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(0);
#endif
    ret = LOS_TaskCreate(&writeFlashTaskId, &taskInitParam);
    return ret;
}

LITE_OS_SEC_TEXT_MINOR size_t OsWowImageSizeGet(VOID)
{
    return g_wowImgSize;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsWowSysDoneFlagGet(VOID)
{
    UINT32 cpuid = ArchCurrCpuid();
    return g_sysDoneFlag[cpuid];
}

LITE_OS_SEC_TEXT_MINOR VOID OsWowOtherCoreResume(UINT32 cpuid)
{
    if (g_otherCoreResume == 1) {
        OsCurrTaskSet(g_saveTsk[cpuid]);
        OsSRRestoreRegister();
    }
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
