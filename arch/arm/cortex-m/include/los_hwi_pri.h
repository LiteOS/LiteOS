/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2018. All rights reserved.
 * Description: hwi cortex-m
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
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/**
 * @defgroup los_hwi Hardware interrupt
 * @ingroup kernel
 */
#ifndef _LOS_HWI_M_H
#define _LOS_HWI_M_H

#include "los_base.h"
#include "los_sys.h"
#include "los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if (LOSCFG_PLATFORM_HWI == YES)
/**
 * @ingroup los_hwi
 * Maximum number of used hardware interrupts.
 */
#ifndef OS_HWI_MAX_NUM
#define OS_HWI_MAX_NUM              32
#endif

/**
 * @ingroup los_hwi
 * Highest priority of a hardware interrupt.
 */
#ifndef OS_HWI_PRIO_HIGHEST
#define OS_HWI_PRIO_HIGHEST         0
#endif

/**
 * @ingroup los_hwi
 * Lowest priority of a hardware interrupt.
 */
#ifndef OS_HWI_PRIO_LOWEST
#define OS_HWI_PRIO_LOWEST          7
#endif

/**
 * @ingroup los_config
 * Configuration item for interrupt with argument
 */
#ifndef OS_HWI_WITH_ARG
#define OS_HWI_WITH_ARG             NO
#endif

/**
 * @ingroup los_hwi
 * Define the type of a hardware interrupt number.
 */
typedef UINT32 HWI_HANDLE_T;

/**
 * @ingroup los_hwi
 * Define the type of a hardware interrupt priority.
 */
typedef UINT16 HWI_PRIOR_T;

/**
 * @ingroup los_hwi
 * Define the type of hardware interrupt mode configurations.
 */
typedef UINT16 HWI_MODE_T;

/**
 * @ingroup los_hwi
 * Define the type of the parameter used for the hardware interrupt creation function.
 * The function of this parameter varies among platforms.
 */
typedef UINT32 HWI_ARG_T;

/**
 * @ingroup los_hwi
 * Count of M-Core system interrupt vector.
 */
#define OS_SYS_VECTOR_CNT           16

/**
 * @ingroup los_hwi
 * AIRCR register priority group parameter .
 */
#define OS_NVIC_AIRCR_PRIGROUP      7
#endif

/**
 * @ingroup los_hwi
 * SysTick control and status register.
 */
#define OS_SYSTICK_CONTROL_REG      0xE000E010

/**
 * @ingroup los_hw
 * SysTick reload value register.
 */
#define OS_SYSTICK_RELOAD_REG       0xE000E014

/**
 * @ingroup los_hw
 * SysTick current value register.
 */
#define OS_SYSTICK_CURRENT_REG      0xE000E018

/**
 * @ingroup los_hwi
 * Interrupt Priority-Level Registers.
 */
#define OS_NVIC_PRI_BASE            0xE000E400

/**
 * @ingroup los_hwi
 * Interrupt enable register for 0-31.
 */
#define OS_NVIC_SETENA_BASE         0xE000E100

/**
 * @ingroup los_hwi
 * interrupt pending register.
 */
#define OS_NVIC_SETPEND_BASE        0xE000E200

/**
 * @ingroup los_hwi
 * interrupt activity status register
 */
#define OS_NVIC_INT_ACT_BASE        0xE000E300

/**
 * @ingroup los_hwi
 * Interrupt disable register for 0-31.
 */
#define OS_NVIC_CLRENA_BASE         0xE000E180

/**
 * @ingroup los_hwi
 * Interrupt control and status register.
 */
#define OS_NVIC_INT_CTRL            0xE000ED04

/**
 * @ingroup los_hwi
 * Vector table offset register.
 */
#define OS_NVIC_VTOR                0xE000ED08

/**
 * @ingroup los_hwi
 * Application interrupt and reset control register
 */
#define OS_NVIC_AIRCR               0xE000ED0C

/**
 * @ingroup los_hwi
 * System exception priority register.
 */
#define OS_NVIC_EXCPRI_BASE         0xE000ED18

#if (LOSCFG_PLATFORM_HWI == YES)
/**
 * @ingroup los_hwi
 * Interrupt No. 1 :reset.
 */
#define OS_EXC_RESET           1

/**
 * @ingroup los_hwi
 * Interrupt No. 2 :Non-Maskable Interrupt.
 */
#define OS_EXC_NMI             2

/**
 * @ingroup los_hwi
 * Interrupt No. 3 :(hard)fault.
 */
#define OS_EXC_HARD_FAULT      3

/**
 * @ingroup los_hwi
 * Interrupt No. 4 :MemManage fault.
 */
#define OS_EXC_MPU_FAULT       4

/**
 * @ingroup los_hwi
 * Interrupt No. 5 :Bus fault.
 */
#define OS_EXC_BUS_FAULT       5

/**
 * @ingroup los_hwi
 * Interrupt No. 6 :Usage fault.
 */
#define OS_EXC_USAGE_FAULT     6

/**
 * @ingroup los_hwi
 * Interrupt No. 11 :SVCall.
 */
#define OS_EXC_SVC_CALL        11

/**
 * @ingroup los_hwi
 * Interrupt No. 12 :Debug monitor.
 */
#define OS_EXC_DBG_MONITOR     12

/**
 * @ingroup los_hwi
 * Interrupt No. 14 :PendSV.
 */
#define OS_EXC_PEND_SV         14

/**
 * @ingroup los_hwi
 * Interrupt No. 15 :SysTick.
 */
#define OS_EXC_SYS_TICK        15

extern HWI_HANDLE_FORM_S g_hwiForm[OS_HWI_MAX_NUM + OS_SYS_VECTOR_CNT];

/**
 * @ingroup los_hwi
 * hardware interrupt form mapping handling function array.
 */
extern HWI_PROC_FUNC g_hwiVec[OS_VECTOR_CNT];
extern VOID IrqEntryV7M(VOID);
#if (OS_HWI_WITH_ARG == YES)
/**
 * @ingroup los_hwi
 * Set interrupt vector table.
 */
#define OsSetVector(num, vector, arg) do {       \
    g_hwiVec[num + OS_SYS_VECTOR_CNT] = (HWI_PROC_FUNC)IrqEntryV7M;\
    g_hwiForm[num + OS_SYS_VECTOR_CNT].pfnHook = vector; \
    g_hwiForm[num + OS_SYS_VECTOR_CNT].uwParam = (VOID*)arg; \
} while (0)
#else
/**
 * @ingroup los_hwi
 * Set interrupt vector table.
 */
#define OsSetVector(num, vector) do {       \
    g_hwiVec[num + OS_SYS_VECTOR_CNT] = (HWI_PROC_FUNC)IrqEntryV7M;\
    g_hwiForm[num + OS_SYS_VECTOR_CNT].pfnHook = vector; \
} while (0)
#endif

extern VOID OsInterrupt(UINT32 intNum);

extern VOID OsHwiInit(VOID);
#endif

/**
 * @ingroup  los_hwi
 * @brief: Default vector handling function.
 *
 * @par Description:
 * This API is used to configure interrupt for null function.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param:None.
 *
 * @retval:None.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li
></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID OsHwiDefaultHandler(VOID);

/**
 * @ingroup  los_hwi
 * @brief: Reset the vector table.
 *
 * @par Description:
 * This API is used to reset the vector table.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param:None.
 *
 * @retval:None.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID Reset_Handler(VOID);

/**
 * @ingroup  los_hwi
 * @brief: Pended System Call.
 *
 * @par Description:
 * PendSV can be pended and is useful for an OS to pend an exception
 * so that an action can be performed after other important tasks are completed.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param:None.
 *
 * @retval:None.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID osPendSV(VOID);

extern VOID LOS_GetCpuCycle(UINT32 *puwCntHi, UINT32 *puwCntLo);

#if (LOSCFG_PLATFORM_HWI == NO)
extern BOOL OsIntActive(VOID);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* _LOS_HWI_M_H */
