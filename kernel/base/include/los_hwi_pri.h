/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Aarch32 Hwi Inner HeadFile
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#ifndef _LOS_HWI_PRI_H
#define _LOS_HWI_PRI_H

#include "los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_HWI_FORM_EXC_NUM 0
#if OS_HWI_FORM_EXC_NUM != 0
#error "OS_HWI_FORM_EXC_NUM must be zero"
#endif

typedef struct tagHwiHandleForm {
    HWI_PROC_FUNC pfnHook; /* User registered callback function */
    HWI_ARG_T uwParam; /* Variable parameters: In the interrupt sharing mode, the head node means the shared flag bit,
                          and the non-head node is the user input address */
#ifndef LOSCFG_NO_SHARED_IRQ
    struct tagHwiHandleForm *pstNext;
#endif
    CHAR *name;   /* Interrupt name */
    UINT32 count; /* Interrupt response counts */
} HWI_HANDLE_FORM_S;

typedef struct {
    VOID (*triggerIrq)(HWI_HANDLE_T hwiNum);
    VOID (*clearIrq)(HWI_HANDLE_T hwiNum);
    VOID (*enableIrq)(HWI_HANDLE_T hwiNum);
    VOID (*disableIrq)(HWI_HANDLE_T hwiNum);
    UINT32 (*setIrqPriority)(HWI_HANDLE_T hwiNum, UINT8 priority);
    UINT32 (*getCurIrqNum)(VOID);
    CHAR *(*getIrqVersion)(VOID);
    HWI_HANDLE_FORM_S *(*getHandleForm)(HWI_HANDLE_T hwiNum);
    VOID (*handleIrq)(VOID);
#ifdef LOSCFG_KERNEL_SMP
    VOID (*setIrqCpuAffinity)(HWI_HANDLE_T hwiNum, UINT32 cpuMask);
    VOID (*sendIpi)(UINT32 target, UINT32 ipi);
#endif
} HwiControllerOps;

extern VOID OsHwiInit(VOID);
extern VOID OsIncHwiFormCnt(UINT32 index);
extern UINT32 OsGetHwiFormCnt(UINT32 index);
extern CHAR *OsGetHwiFormName(UINT32 index);
extern VOID OsIntHandle(UINT32 hwiNum, HWI_HANDLE_FORM_S *handleForm);
extern UINT32 OsIntNumGet(VOID);
extern CHAR *OsIntVersionGet(VOID);
extern BOOL OsIntIsRegisted(UINT32 num);
extern HWI_ARG_T OsIntGetPara(UINT32 num);

/* OS Interrupt entry, called by assembly code */
extern VOID OsIntEntry(VOID);

/* When interrupting the controller driver initialization, controller-related operations need to be implemented and
 * registered;Because the function pointer is not judged in the abstraction layer, all function members defined in
 * HwiControllerOps need to be registered. If this function is not supported, you can call the LOS_Panic interface in
 * the implementation of the stub function to report an error in time. */
extern VOID OsHwiControllerReg(const HwiControllerOps *ops);

#define HWI_IS_REGISTED(num) OsIntIsRegisted(num)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_HWI_PRI_H */
