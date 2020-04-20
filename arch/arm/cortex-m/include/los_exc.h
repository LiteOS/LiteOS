/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2018. All rights reserved.
 * Description: Exception Handler
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

#ifndef _LOS_EXC_H
#define _LOS_EXC_H

#include "los_hwi_pri.h"
#include "los_task_pri.h"
#include "los_queue.h"
#include "los_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define OS_EXC_IN_INIT                      0
#define OS_EXC_IN_TASK                      1
#define OS_EXC_IN_HWI                       2

#define OS_EXC_MAX_BUF_LEN                  25
#define OS_EXC_MAX_NEST_DEPTH               1

#define OS_NVIC_SHCSR                       0xE000ED24
#define OS_NVIC_CCR                         0xE000ED14

#define OS_NVIC_INT_ENABLE_SIZE             0x20
#define OS_NVIC_INT_PRI_SIZE                0xF0
#define OS_NVIC_EXCPRI_SIZE                 0xC

#define OS_NVIC_INT_PEND_SIZE               OS_NVIC_INT_ACT_SIZE
#define OS_NVIC_INT_ACT_SIZE                OS_NVIC_INT_ENABLE_SIZE

#define OS_EXC_FLAG_NO_FLOAT                0x10000000
#define OS_EXC_FLAG_FAULTADDR_VALID         0x01
#define OS_EXC_FLAG_IN_HWI                  0x02

#define OS_EXC_IMPRECISE_ACCESS_ADDR        0xABABABAB

#define FAULT_STATUS_REG_BIT            32
/**
 *@ingroup los_exc
 * the struct of register files
 *
 * description: the register files that saved when exception triggered
 *
 * notes:the following register with prefix 'uw'  correspond to the registers in the cpu  data sheet.
 */
typedef struct tagExcContext {
    //handler save
#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED   ) && (__FPU_USED    == 1U))     )
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
    UINT32 uwR4;
    UINT32 uwR5;
    UINT32 uwR6;
    UINT32 uwR7;
    UINT32 uwR8;
    UINT32 uwR9;
    UINT32 uwR10;
    UINT32 uwR11;
    UINT32 uwPriMask;
    //auto save
    UINT32 uwSP;
    UINT32 uwR0;
    UINT32 uwR1;
    UINT32 uwR2;
    UINT32 uwR3;
    UINT32 uwR12;
    UINT32 uwLR;
    UINT32 uwPC;
    UINT32 uwxPSR;
#if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
     (defined (__FPU_USED   ) && (__FPU_USED    == 1U))     )
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
} EXC_CONTEXT_S;

typedef VOID (* EXC_PROC_FUNC)(UINT32, EXC_CONTEXT_S *);
VOID OsExcHandleEntry(UINT32 uwExcType, UINT32 uwFaultAddr, UINT32 uwPid, const EXC_CONTEXT_S *  pstExcBufAddr);

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
extern UINT8 m_aucTaskArray[];

/**
 *@ingroup los_exc
 *@brief Kernel panic function.
 *
 *@par Description:
 *Stack function that prints kernel panics.
 *@attention After this function is called and stack information is printed, the system will fail to respond.
 *@attention The input parameter can be NULL.
 *@param fmt [IN] Type #char* : variadic argument.
 *
 *@retval #None.
 *
 *@par Dependency:
 *los_exc.h: the header file that contains the API declaration.
 *@see None.
 *@since Huawei LiteOS V100R001C00
*/
void LOS_Panic(const char *fmt, ...);

/**
 * @ingroup los_exc
 * Cortex-M3:bus fault err in stack
 */
#define OS_EXC_BF_STKERR           1

/**
 * @ingroup los_exc
 * Cortex-M3:bus fault err out stack
 */
#define OS_EXC_BF_UNSTKERR         2

/**
 * @ingroup los_exc
 * Cortex-M3:bus fault err imprecise access
 */
#define OS_EXC_BF_IMPRECISERR      3

/**
 * @ingroup los_exc
 * Cortex-M3:bus fault err precise access
 */
#define OS_EXC_BF_PRECISERR        4

/**
 * @ingroup los_exc
 * Cortex-M3:bus fault err ibus
 */
#define OS_EXC_BF_IBUSERR          5

/**
 * @ingroup los_exc
 * Cortex-M3:mem fault err for manager resiter in stack
 */
#define OS_EXC_MF_MSTKERR          6

/**
 * @ingroup los_exc
 * Cortex-M3:mem fault err for manager resiter out stack
 */
#define OS_EXC_MF_MUNSTKERR        7

/**
 * @ingroup los_exc
 * Cortex-M3:mem fault err for data access invalid
 */
#define OS_EXC_MF_DACCVIOL         8

/**
 * @ingroup los_exc
 * Cortex-M3:mem fault err for instruction access invalid
 */
#define OS_EXC_MF_IACCVIOL         9


/**
 * @ingroup los_exc
 * Cortex-M3: divide zero err
 */
#define OS_EXC_UF_DIVBYZERO        10

/**
 * @ingroup los_exc
 * Cortex-M3: data unaligned err
 */
#define OS_EXC_UF_UNALIGNED        11

/**
 * @ingroup los_exc
 * Cortex-M3: no co-processor err
 */
#define OS_EXC_UF_NOCP             12

/**
 * @ingroup los_exc
 * Cortex-M3: invalid PC err
 */
#define OS_EXC_UF_INVPC            13

/**
 * @ingroup los_exc
 * Cortex-M3:invalid state err
 */
#define OS_EXC_UF_INVSTATE         14

/**
 * @ingroup los_exc
 * Cortex-M3:undefined instruction err
 */
#define OS_EXC_UF_UNDEFINSTR       15

/**
 * @ingroup los_exc
 * Cortex-M3: nu-masked interrupt
 */
#define OS_EXC_CAUSE_NMI           16

/**
 * @ingroup los_exc
 * Cortex-M3: hardware fault
 */
#define OS_EXC_CAUSE_HARDFAULT     17

/**
 * @ingroup los_exc
 * Cortex-M3:task exit err
 */
#define OS_EXC_CAUSE_TASK_EXIT     18

/**
 * @ingroup los_exc
 * Cortex-M3:fatal err
 */
#define OS_EXC_CAUSE_FATAL_ERR     19

/**
 * @ingroup los_exc
 * Cortex-M3: debug event trigger hardware interrupt err
 */
#define OS_EXC_CAUSE_DEBUGEVT      20

/**
 * @ingroup los_exc
 * Cortex-M3:access vector trigger hardware interrupt err
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
    UINT32 thrdPid;            /* An exception occurred during the interrupt indicating the interrupt number,
                                * An exception occurs in the task, indicating the task id,
                                * If it occurs in the initialization, it is 0xffffffff
                                */
    UINT16 nestCnt;            /* Count of nested exception */
    UINT16 reserved;           /* Reserved for alignment */
    EXC_CONTEXT_S *context;    /* Hardware context when an exception occurs */
} EXC_INFO_S;

extern UINT32 g_curNestCount;
extern UINT32 g_intCount[];
extern EXC_INFO_S m_stExcInfo;

VOID OsExcInfoDisplay(const EXC_INFO_S *exc);

extern UINT8 g_uwExcTbl[32];

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
extern VOID OsTaskBackTrace(UINT32 taskID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _LOS_EXC_H */

