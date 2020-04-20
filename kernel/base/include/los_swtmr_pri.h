/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Software Timer Manager Private HeadFile
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

#ifndef _LOS_SWTMR_PRI_H
#define _LOS_SWTMR_PRI_H

#include "los_swtmr.h"
#include "los_spinlock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_swtmr_pri
 * Software timer state
 */
enum SwtmrState {
    OS_SWTMR_STATUS_UNUSED,     /**< The software timer is not used.    */
    OS_SWTMR_STATUS_CREATED,    /**< The software timer is created.     */
    OS_SWTMR_STATUS_TICKING     /**< The software timer is timing.      */
};

/**
 * @ingroup los_swtmr_pri
 * Structure of the callback function that handles software timer timeout
 */
typedef struct {
    SWTMR_PROC_FUNC handler;    /**< Callback function that handles software timer timeout  */
    UINTPTR arg;                /**< Parameter passed in when the callback function
                                     that handles software timer timeout is called */
} SwtmrHandlerItem;

/**
 * @ingroup los_swtmr_pri
 * Type of the pointer to the structure of the callback function that handles software timer timeout
 */
typedef SwtmrHandlerItem *SwtmrHandlerItemPtr;

/**
 * @ingroup los_swtmr
 * Configuration item for software timer to use dynamic memory
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
#define LOSCFG_BASE_CORE_SWTMR_DYN_MEM       YES
#else
#define LOSCFG_BASE_CORE_SWTMR_DYN_MEM       NO
#endif

#if (LOSCFG_BASE_CORE_SWTMR_DYN_MEM == YES)
extern SWTMR_CTRL_S *g_swtmrCBArray;
#else
extern LITE_OS_SEC_BSS SWTMR_CTRL_S    g_swtmrCBArray[LOSCFG_BASE_CORE_SWTMR_LIMIT];
#endif
extern SortLinkAttribute g_swtmrSortLink; /* The software timer count list */

#define OS_SWT_FROM_SID(swtmrID) ((SWTMR_CTRL_S *)g_swtmrCBArray + ((swtmrID) % LOSCFG_BASE_CORE_SWTMR_LIMIT))

/**
 * @ingroup los_swtmr_pri
 * @brief Scan a software timer.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to scan a software timer when a Tick interrupt occurs and determine whether
 * the software timer expires.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param  None.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_swtmr_pri.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_SwtmrStop
 * @since Huawei LiteOS V100R001C00
 */
extern VOID OsSwtmrScan(VOID);
extern UINT32 OsSwtmrInit(VOID);
extern VOID OsSwtmrTask(VOID);
extern SPIN_LOCK_S g_swtmrSpin;
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_SWTMR_PRI_H */
