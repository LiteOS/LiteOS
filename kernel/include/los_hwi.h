/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Hwi HeadFile
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

/**
 * @defgroup los_hwi Hardware interrupt
 * @ingroup kernel
 */
#ifndef _LOS_HWI_H
#define _LOS_HWI_H

#include "los_base.h"
#include "hal_hwi.h"
#include "arch/cpu.h"
#include "arch/interrupt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_hwi
 * Count of interrupts.
 */
extern size_t g_intCount[];

/**
 * An interrupt is active.
 */
extern size_t IntActive(VOID);

/**
 * @ingroup los_hwi
 * It is used to check whether there are active interrupts or not.
 *
 * @see OS_INT_INACTIVE
 */
#define OS_INT_ACTIVE IntActive()

/**
 * @ingroup los_hwi
 * Check whether there are active interrupts or not.
 * The API returns a boolean value. True means no active interrupts on the current CPU.
 * False means that there are active interrupts on the current CPU.
 *
 * @see OS_INT_ACTIVE
 */
#define OS_INT_INACTIVE (!(OS_INT_ACTIVE))

/**
 * @ingroup los_hwi
 * Highest priority of a hardware interrupt.This is an external parameter.
 * The priority range is [OS_HWI_PRIO_HIGHEST, OS_HWI_PRIO_HIGHEST + LOSCFG_HWI_PRIO_LIMIT - 1].
 */
#define OS_HWI_PRIO_HIGHEST 0

/**
 * @ingroup los_hwi
 * This represents the interrupt priority range, the larger number, the lower priority.
 * If the actual chip is the larger number, the higher priority,
 * needs to be modified at the controller implementation layer.
 */
#define OS_HWI_PRIO_LOWEST (LOSCFG_HWI_PRIO_LIMIT - 1)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: Invalid interrupt number.
 *
 * Value: 0x02000900.
 *
 * Solution: Ensure that the interrupt number is valid.
 */
#define OS_ERRNO_HWI_NUM_INVALID                LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x00)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: Null hardware interrupt handling function.
 *
 * Value: 0x02000901.
 *
 * Solution: Pass in a valid non-null hardware interrupt handling function.
 */
#define OS_ERRNO_HWI_PROC_FUNC_NULL             LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x01)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: Insufficient interrupt resources for hardware interrupt creation.
 *
 * Value: 0x02000902.
 *
 * Solution: This error code is not in use temporarily.
 */
#define OS_ERRNO_HWI_CB_UNAVAILABLE             LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x02)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: Insufficient memory for hardware interrupt initialization.
 *
 * Value: 0x02000903.
 *
 * Solution: Expand the configured memory.
 */
#define OS_ERRNO_HWI_NO_MEMORY                  LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x03)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: The interrupt has already been created.
 *
 * Value: 0x02000904.
 *
 * Solution: Check whether the interrupt specified by the passed-in interrupt number has
 * already been created.
 */
#define OS_ERRNO_HWI_ALREADY_CREATED            LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x04)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: Invalid interrupt priority.
 *
 * Value: 0x02000905.
 *
 * Solution: Ensure that the interrupt priority is valid.
 */
#define OS_ERRNO_HWI_PRIO_INVALID               LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x05)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: Incorrect interrupt creation mode.
 *
 * Value: 0x02000906.
 *
 * Solution: This error code is not in use temporarily.
 */
#define OS_ERRNO_HWI_MODE_INVALID               LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x06)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: The interrupt has already been created as a fast interrupt.
 *
 * Value: 0x02000907.
 *
 * Solution: This error code is not in use temporarily.
 */
#define OS_ERRNO_HWI_FASTMODE_ALREADY_CREATED LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x07)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: The API is called during an interrupt, which is not allowed.
 *
 * Value: 0x02000908.
 *
 * Solution: This error code is not in use temporarily.
 */
#define OS_ERRNO_HWI_INTERR                     LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x08)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: the hardware interrupt supports SHARED error.
 *
 * Value: 0x02000909.
 *
 * Solution: Check the input params hwiMode and irqParam of LOS_HwiCreate or
 * LOS_HwiDelete whether adapt the current hardware interrupt.
 */
#define OS_ERRNO_HWI_SHARED_ERROR               LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x09)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: Invalid interrupt Arg when interrupt mode is IRQF_SHARED.
 *
 * Value: 0x0200090a.
 *
 * Solution: This error code is not in use temporarily.
 */
#define OS_ERRNO_HWI_ARG_INVALID                LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x0a)

/**
 * @ingroup los_hwi
 * Hardware interrupt error code: The interrupt corresponded to the hardware interrupt number
 * or devid has not been created.
 *
 * Value: 0x0200090b.
 *
 * Solution: Check the irqParam->pDevId of LOS_HwiDelete, make sure the devid need to delete.
 */
#define OS_ERRNO_HWI_HWINUM_UNCREATE            LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x0b)

/**
 * @ingroup los_hwi
 * Define the type HWI_HANDLE_T for a hardware interrupt number, the type is an unsigned int.
 */
typedef UINT32 HWI_HANDLE_T;

/**
 * @ingroup los_hwi
 * Define the type HWI_PRIOR_T for a hardware interrupt priority, the type is an unsigned short.
 */
typedef UINT16 HWI_PRIOR_T;

/**
 * @ingroup los_hwi
 * Define the type HWI_MODE_T for hardware interrupt mode configurations, the type is an unsigned short.
 */
typedef UINT16 HWI_MODE_T;

/**
 * @ingroup los_hwi
 * Define the type HWI_ARG_T for the parameter used for the hardware interrupt creation function.
 * The function of this parameter varies among platforms.
 */
typedef UINTPTR HWI_ARG_T;

/**
 * @ingroup  los_hwi
 * @brief Define the type of a hardware interrupt handling function.
 *
 * @par Description:
 * This definition is used to declare the type of a hardware interrupt handling function.
 * It will be used when calling LOS_HwiCreate.
 * @attention
 * None.
 *
 * @param None.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_HwiCreate
 * @since Huawei LiteOS V100R001C00
 */
typedef VOID (*HWI_PROC_FUNC)(VOID);

/**
 * @ingroup  los_hwi
 * The flag means the IRQ is allowed to share among several devices.
 *
 * The flag only used by the kernel as part of the IRQ handling routines.
 */
#define IRQF_SHARED 0x8000U

/**
 * @ingroup  los_hwi
 * The hardware interrupt parameter for #LOS_HwiDelete and interrupt handler in #LOS_HwiCreate.
 */
typedef struct tagIrqParam {
    int swIrq;          /**< The interrupt number */
    VOID *pDevId;       /**< The pointer to the device ID that launches the interrupt */
    const CHAR *pName;  /**< The interrupt name */
} HWI_IRQ_PARAM_S;

/**
 * @ingroup los_hwi
 * @brief Disable all interrupts.
 *
 * @par Description:
 * This API is used to disable all IRQ and FIQ interrupts in the CPSR.
 * @attention
 * None.
 *
 * @param None.
 *
 * @retval #UINT32 CPSR value before all interrupts are disabled.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_IntRestore
 * @since Huawei LiteOS V100R001C00
 */
STATIC INLINE UINT32 LOS_IntLock(VOID)
{
    return ArchIntLock();
}

/**
 * @ingroup los_hwi
 * @brief Enable all interrupts.
 *
 * @par Description:
 * This API is used to enable all IRQ and FIQ interrupts in the CPSR.
 * @attention
 * None.
 *
 * @param None.
 *
 * @retval #UINT32 CPSR value after all interrupts are enabled.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_IntLock
 */
STATIC INLINE UINT32 LOS_IntUnLock(VOID)
{
    return ArchIntUnlock();
}

/**
 * @ingroup los_hwi
 * @brief Restore interrupts.
 *
 * @par Description:
 * This API is used to restore the CPSR value obtained before all interrupts are disabled by #LOS_IntLock.
 * @attention
 * This API can be called only after all interrupts are disabled, and the input parameter value should be
 * the value returned by #LOS_IntLock.
 *
 * @param intSave [IN] Type #UINT32. CPSR value before all interrupts are disabled.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_IntLock
 * @since Huawei LiteOS V100R001C00
 */
STATIC INLINE VOID LOS_IntRestore(UINT32 intSave)
{
    ArchIntRestore(intSave);
}

/**
 * @ingroup  los_hwi
 * @brief Create a hardware interrupt.
 *
 * @par Description:
 * This API is used to configure a hardware interrupt and register a hardware interrupt handling function.
 *
 * @attention
 * <ul>
 * <li>The hardware interrupt module is usable only when the configuration item for
 * hardware interrupt tailoring is enabled.</li>
 * <li>Hardware interrupt number value range: [OS_USER_HWI_MIN, OS_USER_HWI_MAX]. </li>
 * <li>OS_HWI_MAX_NUM specifies the maximum number of interrupts that can be created.</li>
 * <li>Before executing an interrupt on a platform, refer to the chip manual of the platform.</li>
 * <li>The parameter handler of this interface is a interrupt handler, it should be correct, otherwise,
 * the system may be abnormal.</li>
 * <li>The input irqParam could be NULL, if not, it should be address which point to a struct HWI_IRQ_PARAM_S,
 * the parameter pDenId and pName should be constant.</li>
 * </ul>
 *
 * @param  hwiNum     [IN] Type #HWI_HANDLE_T. The hardware interrupt number.
 *                                             for an ARM926 platform is [0,31].
 * @param  hwiPrio    [IN] Type #HWI_PRIOR_T. The hardware interrupt priority. The value range is
 *                                            [0, GIC_MAX_INTERRUPT_PREEMPTION_LEVEL - 1] << PRIORITY_SHIFT.
 * @param  hwiMode    [IN] Type #HWI_MODE_T. The hardware interrupt mode. Ignore this parameter temporarily.
 * @param  hwiHandler [IN] Type #HWI_PROC_FUNC. The interrupt handler used when a hardware interrupt is triggered.
 * @param  irqParam   [IN] Type #HWI_IRQ_PARAM_S. The input parameter of the interrupt handler used when
 *                                                a hardware interrupt is triggered.
 *
 * @retval #OS_ERRNO_HWI_PROC_FUNC_NULL              Null hardware interrupt handling function.
 * @retval #OS_ERRNO_HWI_NUM_INVALID                 Invalid interrupt number.
 * @retval #OS_ERRNO_HWI_NO_MEMORY                   Insufficient memory for hardware interrupt creation.
 * @retval #OS_ERRNO_HWI_ALREADY_CREATED             The interrupt handler being created has already been created.
 * @retval #OS_ERRNO_HWI_SHARED_ERROR                The interrupt can not be shared. The interrupt number has been
 *                                                   registered as a non-shared interrupt, or a shared interrupt is
 *                                                   specified to be created, but the device ID is empty.
 * @retval #LOS_OK                                   The interrupt is successfully created.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_HwiDelete
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_HwiCreate(HWI_HANDLE_T hwiNum,
                            HWI_PRIOR_T hwiPrio,
                            HWI_MODE_T hwiMode,
                            HWI_PROC_FUNC hwiHandler,
                            HWI_IRQ_PARAM_S *irqParam);

/**
 * @ingroup  los_hwi
 * @brief delete a hardware interrupt.
 *
 * @par Description:
 * This API is used to delete a hardware interrupt.
 *
 * @attention
 * <ul>
 * <li>The hardware interrupt module is usable only when the configuration item for
 * hardware interrupt tailoring is enabled.</li>
 * <li>Hardware interrupt number value range: [OS_USER_HWI_MIN, OS_USER_HWI_MAX].</li>
 * <li>OS_HWI_MAX_NUM specifies the maximum number of interrupts that can be created.</li>
 * <li>Before executing an interrupt on a platform, refer to the chip manual of the platform.</li>
 * </ul>
 *
 * @param  hwiNum   [IN] Type #HWI_HANDLE_T. The hardware interrupt number.
 * @param  irqParam [IN] Type #HWI_IRQ_PARAM_S *. ID of hardware interrupt which will base on
 *                                                when delete the hardware interrupt.
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID         Invalid interrupt number.
 * @retval #OS_ERRNO_HWI_SHARED_ERROR        The interrupt number is a shared interrupt, but the device ID of the
 *                                           shared interrupt to be deleted is not specified.
 * @retval #OS_ERRNO_HWI_HWINUM_UNCREATE     The interrupt corresponded to the hwiNum(
 *                                           the hardware interrupt number) or
 *                                           irqParam->pDevId(the interrupt device id)
 *                                           has not been created.
 * @retval #LOS_OK                           The interrupt is successfully deleted.

 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_HwiCreate
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_HwiDelete(HWI_HANDLE_T hwiNum, HWI_IRQ_PARAM_S *irqParam);

/**
 * @ingroup los_hwi
 * @brief Trigger interrupts.
 *
 * @par Description:
 * The generation of external hardware interrupts is simulated by writing
 * the relevant registers of the interrupt controller.
 * @attention
 * This function depends on the hardware implementation of the interrupt
 * controller.
 *
 * @param hwiNum   [IN] Type #HWI_HANDLE_T. The hardware interrupt number.
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID         Invalid interrupt number.
 * @retval #LOS_OK                           The interrupt is successfully
 * deleted.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API
 * declaration.</li></ul>
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_HwiTrigger(HWI_HANDLE_T hwiNum);

/**
 * @ingroup los_hwi
 * @brief clear interrupts.
 *
 * @par Description:
 * Clear the status bit of the interrupt number corresponding to the
 * interrupt controller.
 * @attention
 * This function depends on the hardware implementation of the interrupt
 * controller.
 *
 * @param hwiNum   [IN] Type #HWI_HANDLE_T. The hardware interrupt number.
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID         Invalid interrupt number.
 * @retval #LOS_OK                           The interrupt is successfully
 * deleted.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API
 * declaration.</li></ul>
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_HwiClear(HWI_HANDLE_T hwiNum);

/**
 * @ingroup los_hwi
 * @brief Enable interrupts.
 *
 * @par Description:
 * Enable the corresponding interrupt mask of the interrupt controller, so
 * that the interrupt source can be sent to the CPU.
 * @attention
 * This function depends on the hardware implementation of the interrupt
 * controller.
 *
 * @param hwiNum   [IN] Type #HWI_HANDLE_T. The hardware interrupt number.
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID         Invalid interrupt number.
 * @retval #LOS_OK                           The interrupt is successfully
 * deleted.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_HwiDisable
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_HwiEnable(HWI_HANDLE_T hwiNum);

/**
 * @ingroup los_hwi
 * @brief Disable interrupts.
 *
 * @par Description:
 * Disable the corresponding interrupt mask of the interrupt controller, so
 * that the interrupt source can be sent to the CPU.
 * @attention
 * This function depends on the hardware implementation of the interrupt
 * controller.
 *
 * @param hwiNum   [IN] Type #HWI_HANDLE_T. The hardware interrupt number.
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID         Invalid interrupt number.
 * @retval #LOS_OK                           The interrupt is successfully
 * deleted.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_HwiEnable
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_HwiDisable(HWI_HANDLE_T hwiNum);

/**
 * @ingroup los_hwi
 * @brief Send inter-core interrupts to designated cores.
 *
 * @par Description:
 * Send inter-core interrupts to designated cores.
 * @attention
 * This function depends on the hardware implementation of the interrupt
 * controller and CPU architecture, Only used in SMP architecture.
 *
 * @param hwiNum   [IN] Type #HWI_HANDLE_T: hardware interrupt number.
 * @param cpuMask   [IN] Type #UINT32: CPU number.
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID         Invalid interrupt number.
 * @retval #LOS_OK                           The interrupt is successfully
 * deleted.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API
 * declaration.</li></ul>
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_HwiSendIpi(UINT32 hwiNum, UINT32 cpuMask);

/**
 * @ingroup los_hwi
 * @brief Interrupt response specified CPU processing.
 *
 * @par Description:
 * Interrupt response specified CPU processing.
 * @attention
 * This function depends on the hardware implementation of the interrupt
 * controller and CPU architecture, Only used in SMP architecture.
 *
 * @param hwiNum   [IN] Type #HWI_HANDLE_T. The hardware interrupt number.
 * @param cpuMask  [IN] Type #UINT32. The CPU number.
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID         Invalid interrupt number.
 * @retval #LOS_OK                           The interrupt is successfully
 * deleted.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API
 * declaration.</li></ul>
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_HwiSetAffinity(UINT32 hwiNum, UINT32 cpuMask);

/**
 * @ingroup los_hwi
 * @brief Set interrupts priority.
 *
 * @par Description:
 * Set interrupts priority.
 * @attention
 * This function depends on the hardware implementation of the interrupt
 * controller and CPU architecture.
 *
 * @param hwiNum     [IN] Type #HWI_HANDLE_T: hardware interrupt number.
 * @param priority   [IN] Type #UINT32: interrupt priority to be set.
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID         Invalid interrupt number.
 * @retval #OS_ERRNO_HWI_PRIO_INVALID        Invalid interrupt priority.
 * @retval #OS_ERRNO_HWI_PROC_FUNC_NULL      Not supported by the interrupt
 *                                           controller.
 * @retval #LOS_OK                           The interrupt is successfully
 *                                           deleted.
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API
 * declaration.</li></ul>
 * @see None
 * @since Huawei LiteOS V200R005C00
 */
extern UINT32 LOS_HwiSetPriority(HWI_HANDLE_T hwiNum, UINT32 priority);

#ifdef LOSCFG_KERNEL_LOWPOWER
typedef VOID (*IntWakeupHookFn)(HWI_HANDLE_T hwiNum);
extern VOID LOS_IntWakeupHookReg(IntWakeupHookFn hook);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_HWI_H */
