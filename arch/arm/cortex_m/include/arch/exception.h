/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Exception Operations HeadFile
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
 * ---------------------------------------------------------------------------- */

#ifndef _ARCH_EXCEPTION_H
#define _ARCH_EXCEPTION_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Initial bit32 stack value. */
#define OS_STACK_INIT       0xCACACACA
/* Bit32 stack top magic number. */
#define OS_STACK_MAGIC_WORD 0xCCCCCCCC

extern CHAR _estack;

typedef enum {
    OS_EXC_TYPE_CONTEXT     = 0,
    OS_EXC_TYPE_TSK         = 1,
    OS_EXC_TYPE_QUE         = 2,
    OS_EXC_TYPE_NVIC        = 3,
    OS_EXC_TYPE_TSK_SWITCH  = 4,
    OS_EXC_TYPE_MEM         = 5,
    OS_EXC_TYPE_MAX         = 6
} ExcInfoType;

typedef UINT32 (*EXC_INFO_SAVE_CALLBACK)(UINT32, VOID*);

typedef struct tagExcInfoCallBackArray {
    ExcInfoType             type;
    UINT32                  valid;
    EXC_INFO_SAVE_CALLBACK  fnExcInfoCb;
    VOID                    *pArg;
} ExcInfoArray;

#define OS_EXC_IN_INIT                  0
#define OS_EXC_IN_TASK                  1
#define OS_EXC_IN_HWI                   2

#define OS_EXC_MAX_BUF_LEN              25
#define OS_EXC_MAX_NEST_DEPTH           1

#define OS_NVIC_SHCSR                   0xE000ED24
#define OS_NVIC_CCR                     0xE000ED14
#define OS_NVIC_INT_ENABLE_SIZE         0x20
#define OS_NVIC_INT_PRI_SIZE            0xF0
#define OS_NVIC_EXCPRI_SIZE             0xC
#define OS_NVIC_INT_PEND_SIZE           OS_NVIC_INT_ACT_SIZE
#define OS_NVIC_INT_ACT_SIZE            OS_NVIC_INT_ENABLE_SIZE

#define OS_EXC_FLAG_NO_FLOAT            0x10000000
#define OS_EXC_FLAG_FAULTADDR_VALID     0x01
#define OS_EXC_FLAG_IN_HWI              0x02
#define OS_EXC_IMPRECISE_ACCESS_ADDR    0xABABABAB

#define FAULT_STATUS_REG_BIT            32
/**
 * @ingroup los_exc
 * the struct of register files
 *
 * description: the register files that saved when exception triggered
 *
 */
typedef struct tagExcContext {
    // handler save
#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED) && (__FPU_USED == 1U)))
    UINT32 S16;
    UINT32 S17;
    UINT32 S18;
    UINT32 S19;
    UINT32 S20;
    UINT32 S21;
    UINT32 S22;
    UINT32 S23;
    UINT32 S24;
    UINT32 S25;
    UINT32 S26;
    UINT32 S27;
    UINT32 S28;
    UINT32 S29;
    UINT32 S30;
    UINT32 S31;
#endif
    UINT32 R4;
    UINT32 R5;
    UINT32 R6;
    UINT32 R7;
    UINT32 R8;
    UINT32 R9;
    UINT32 R10;
    UINT32 R11;
    UINT32 PriMask;
    // auto save
    UINT32 SP;
    UINT32 R0;
    UINT32 R1;
    UINT32 R2;
    UINT32 R3;
    UINT32 R12;
    UINT32 LR;
    UINT32 PC;
    UINT32 xPSR;
#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED) && (__FPU_USED == 1U)))
    UINT32 S0;
    UINT32 S1;
    UINT32 S2;
    UINT32 S3;
    UINT32 S4;
    UINT32 S5;
    UINT32 S6;
    UINT32 S7;
    UINT32 S8;
    UINT32 S9;
    UINT32 S10;
    UINT32 S11;
    UINT32 S12;
    UINT32 S13;
    UINT32 S14;
    UINT32 S15;
    UINT32 FPSCR;
    UINT32 NO_NAME;
#endif
} ExcContext;

typedef VOID (* EXC_PROC_FUNC)(UINT32, ExcContext *);
VOID OsExcHandleEntry(UINT32 excType, UINT32 faultAddr, UINT32 pid, const ExcContext *excBufAddr);

/**
 * @ingroup  los_hwi
 * @brief: Exception initialization.
 *
 * @par Description:
 * This API is used to configure the exception function vector table.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param None.
 *
 * @retval: None
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
VOID OsExcInit(VOID);

extern VOID OsExcNMI(VOID);
extern VOID OsExcHardFault(VOID);
extern VOID OsExcMemFault(VOID);
extern VOID OsExcBusFault(VOID);
extern VOID OsExcUsageFault(VOID);
extern VOID OsExcSvcCall(VOID);
extern VOID OsBackTrace(VOID);

/**
 * @ingroup los_exc
 * bus fault err in stack
 */
#define OS_EXC_BF_STKERR           1

/**
 * @ingroup los_exc
 * bus fault err out stack
 */
#define OS_EXC_BF_UNSTKERR         2

/**
 * @ingroup los_exc
 * bus fault err imprecise access
 */
#define OS_EXC_BF_IMPRECISERR      3

/**
 * @ingroup los_exc
 * bus fault err precise access
 */
#define OS_EXC_BF_PRECISERR        4

/**
 * @ingroup los_exc
 * bus fault err ibus
 */
#define OS_EXC_BF_IBUSERR          5

/**
 * @ingroup los_exc
 * mem fault err for manager resiter in stack
 */
#define OS_EXC_MF_MSTKERR          6

/**
 * @ingroup los_exc
 * mem fault err for manager resiter out stack
 */
#define OS_EXC_MF_MUNSTKERR        7

/**
 * @ingroup los_exc
 * mem fault err for data access invalid
 */
#define OS_EXC_MF_DACCVIOL         8

/**
 * @ingroup los_exc
 * mem fault err for instruction access invalid
 */
#define OS_EXC_MF_IACCVIOL         9

/**
 * @ingroup los_exc
 * divide zero err
 */
#define OS_EXC_UF_DIVBYZERO        10

/**
 * @ingroup los_exc
 * data unaligned err
 */
#define OS_EXC_UF_UNALIGNED        11

/**
 * @ingroup los_exc
 * no co-processor err
 */
#define OS_EXC_UF_NOCP             12

/**
 * @ingroup los_exc
 * invalid PC err
 */
#define OS_EXC_UF_INVPC            13

/**
 * @ingroup los_exc
 * invalid state err
 */
#define OS_EXC_UF_INVSTATE         14

/**
 * @ingroup los_exc
 * undefined instruction err
 */
#define OS_EXC_UF_UNDEFINSTR       15

/**
 * @ingroup los_exc
 * nu-masked interrupt
 */
#define OS_EXC_CAUSE_NMI           16

/**
 * @ingroup los_exc
 * hardware fault
 */
#define OS_EXC_CAUSE_HARDFAULT     17

/**
 * @ingroup los_exc
 * task exit err
 */
#define OS_EXC_CAUSE_TASK_EXIT     18

/**
 * @ingroup los_exc
 * fatal err
 */
#define OS_EXC_CAUSE_FATAL_ERR     19

/**
 * @ingroup los_exc
 * debug event trigger hardware interrupt err
 */
#define OS_EXC_CAUSE_DEBUGEVT      20

/**
 * @ingroup los_exc
 * access vector trigger hardware interrupt err
 */
#define OS_EXC_CAUSE_VECTBL        21

typedef struct tagExcInfo {
    UINT16 phase;              /* Phase in which an exception occurs:
                                * 0 means that the exception occurred during initialization,
                                * 1 means that the exception occurred in the task,
                                * 2 means that the exception occurred in the interrupt.
                                */
    UINT16 type;               /* Exception type */
    UINT32 faultAddr;          /* The wrong access address when the exception occurred */
    UINT32 intNumOrTaskId;     /* An exception occurred during the interrupt indicating the interrupt number,
                                * An exception occurs in the task, indicating the task id,
                                * If it occurs in the initialization, it is 0xffffffff
                                */
    UINT16 nestCnt;            /* Count of nested exception */
    UINT16 reserved;           /* Reserved for alignment */
    ExcContext *context;       /* Hardware context when an exception occurs */
} ExcInfo;

extern UINT32 g_curNestCount;

VOID OsExcInfoDisplay(const ExcInfo *exc, ExcContext *excBufAddr);

/**
 * @ingroup los_exc
 * @brief Kernel task backtrace function.
 *
 * @par Description:
 * Backtrace function that prints task call stack information traced from the input task.
 * @attention
 * <ul>
 * <li>The input taskID should be valid.</li>
 * </ul>
 *
 * @param  taskID [IN] Type #UINT32 Task ID.
 *
 * @retval #None.
 *
 * @par Dependency:
 * los_exc.h: the header file that contains the API declaration.
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID OsTaskBackTrace(UINT32 taskId);
VOID ArchExcInit(VOID);

STATIC INLINE VOID ArchHaltCpu(VOID)
{
    __asm__ __volatile__("swi 0");
}

STATIC INLINE UINTPTR ArchGetSp(VOID)
{
    UINTPTR regSp;

    __asm__ __volatile__("mov %0, sp\n" : "=r"(regSp));

    return regSp;
}

STATIC INLINE UINTPTR ArchGetPsp(VOID)
{
    UINTPTR regPsp;

    __asm__ __volatile__("MRS %0, psp\n" : "=r"(regPsp));

    return regPsp;
}

STATIC INLINE UINTPTR ArchGetMsp(VOID)
{
    UINTPTR regMsp;

    __asm__ __volatile__("MRS %0, msp\n" : "=r"(regMsp));

    return regMsp;
}

VOID ArchBackTrace(VOID);
VOID ArchBackTraceWithSp(const VOID *stackPointer);

#ifdef __cplusplus
#if __cplusplus
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_EXCEPTION_H */