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

#ifndef _LOS_EXC_PRI_H
#define _LOS_EXC_PRI_H

#include "los_exc.h"
#include "los_sys_pri.h"
#ifdef LOSCFG_LIB_LIBC
#include "string.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

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
    ExcInfoType             uwType;
    UINT32                  uwValid;
    EXC_INFO_SAVE_CALLBACK  pFnExcInfoCb;
    VOID                    *pArg;
} ExcInfoArray;

#define MAX_SCENE_INFO_SIZE     (8 + sizeof(EXC_INFO_S) - 4 + sizeof(EXC_CONTEXT_S))
#define MAX_TSK_INFO_SIZE       (8 + sizeof(TSK_INFO_S)* (LOSCFG_BASE_CORE_TSK_LIMIT + 1))
#define MAX_INT_INFO_SIZE       (8 + 0x164)

#if (LOSCFG_BASE_IPC_QUEUE == YES)
#define MAX_QUEUE_INFO_SIZE     (8 + sizeof(QUEUE_INFO_S)* LOSCFG_BASE_IPC_QUEUE_LIMIT)
#else
#define MAX_QUEUE_INFO_SIZE     (0)
#endif

#if (LOSCFG_BASE_CORE_EXC_TSK_SWITCH == YES)
#define MAX_SWITCH_INFO_SIZE    (8 + (sizeof(UINT32) + sizeof(CHAR) * LOS_TASK_NAMELEN)* OS_TASK_SWITCH_INFO_COUNT)
#else
#define MAX_SWITCH_INFO_SIZE    (0)
#endif

#if (LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK == YES)
#define MAX_MEM_INFO_SIZE       (8 + sizeof(MEM_INFO_S) * OS_SYS_MEM_NUM)
#else
#define MAX_MEM_INFO_SIZE       (0)
#endif

#define MAX_EXC_MEM_SIZE (4 + MAX_SCENE_INFO_SIZE + MAX_TSK_INFO_SIZE + MAX_QUEUE_INFO_SIZE + \
                            MAX_INT_INFO_SIZE + MAX_SWITCH_INFO_SIZE + MAX_MEM_INFO_SIZE + 4)

VOID OsExcRegister(ExcInfoType type, EXC_INFO_SAVE_CALLBACK func, VOID* arg);


#define OS_EXC_IN_INIT              0
#define OS_EXC_IN_TASK              1
#define OS_EXC_IN_HWI               2

#define OS_EXC_FLAG_FAULTADDR_VALID     0x01

#define OS_EXC_FLAG_IN_HWI              0x02

#define OS_EXC_IMPRECISE_ACCESS_ADDR    0xABABABAB

#define OS_EXC_FLAG_NO_FLOAT            0x10000000


#define OS_EXC_MAX_BUF_LEN          25
#define OS_EXC_MAX_NEST_DEPTH       1

#define OS_NVIC_SHCSR               0xE000ED24
#define OS_NVIC_CCR                 0xE000ED14

#define OS_NVIC_INT_ENABLE_SIZE     0x20
#define OS_NVIC_INT_PRI_SIZE        0xF0

#define OS_NVIC_INT_PEND_SIZE       OS_NVIC_INT_ACT_SIZE
#define OS_NVIC_INT_ACT_SIZE        OS_NVIC_INT_ENABLE_SIZE

#define OS_EXC_BF_STKERR           1

#define OS_EXC_BF_UNSTKERR         2

#define OS_EXC_BF_IMPRECISERR      3

#define OS_EXC_BF_PRECISERR        4

#define OS_EXC_BF_IBUSERR          5

#define OS_EXC_MF_MSTKERR          6

#define OS_EXC_MF_MUNSTKERR        7

#define OS_EXC_MF_DACCVIOL         8

#define OS_EXC_MF_IACCVIOL         9

#define OS_EXC_UF_DIVBYZERO        10

#define OS_EXC_UF_UNALIGNED        11

#define OS_EXC_UF_NOCP             12

#define OS_EXC_UF_INVPC            13

#define OS_EXC_UF_INVSTATE         14

#define OS_EXC_UF_UNDEFINSTR       15

#define OS_EXC_CAUSE_NMI           16

#define OS_EXC_CAUSE_HARDFAULT     17

#define OS_EXC_CAUSE_TASK_EXIT     18

#define OS_EXC_CAUSE_FATAL_ERR     19

#define OS_EXC_CAUSE_DEBUGEVT      20

#define OS_EXC_CAUSE_VECTBL        21

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
