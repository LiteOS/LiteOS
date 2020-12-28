/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Low-power Framework.
 * Author: Huawei LiteOS Team
 * Create: 2020-09-19
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

#ifndef _LOS_LOWPOWER_IMPL_H
#define _LOS_LOWPOWER_IMPL_H

#include "los_lowpower.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup  los_lowpower
 *
 * Power manager run options
 */
typedef struct {
    VOID (*changeFreq)(UINT8 freq);           /**< Tune system frequency */
    VOID (*enterLightSleep)(VOID);            /**< Enter light sleep */
    VOID (*enterDeepSleep)(VOID);             /**< Enter deep sleep */
    VOID (*setWakeUpTimer)(UINT32 timeout);   /**< Set wakeup timer */
    UINT32 (*withdrawWakeUpTimer)(VOID);      /**< Withdraw wakeup timer */
    UINT32 (*getSleepTime)(VOID);             /**< Get sleep time */
    UINT32 (*selectSleepMode)(UINT32);        /**< Select sleep mode, developers can set their own mode selecetion
                                                   strategy */
    UINT32 (*preConfig)(VOID);                /**< Preconfig, provided for special needs before entering sleep */
    VOID (*postConfig)(VOID);                 /**< Postconfig, provided for special needs after wakeup */
    VOID (*contextSave)(VOID);                /**< Context save */
    VOID (*contextRestore)(VOID);             /**< Context restore */
    UINT32 (*getDeepSleepVoteCount)(VOID);    /**< Get deep sleep vote count */
    UINT32 (*getSleepMode)(VOID);             /**< Get sleep mode */
    VOID (*setSleepMode)(UINT32 mode);        /**< Set sleep mode */
} PowerMgrRunOps;

/**
 * @ingroup  los_lowpower
 *
 * Power manager config, corresponding to mode selection strategies.
 */
typedef struct {
    UINT32 minLightSleepTicks; /**< Min light sleep ticks */
    UINT32 minDeepSleepTicks;  /**< Min deep sleep ticks */
    UINT32 maxDeepSleepTicks;  /**< Max deep sleep ticks */
} PowerMgrConfig;

/**
 * @ingroup  los_lowpower
 *
 * Power manager deep sleep options
 */
typedef struct {
    BOOL (*couldDeepSleep)(VOID);    /**< Check whether could enter deep sleep */
    VOID (*systemWakeup)(VOID);      /**< System wakup */
    BOOL (*suspendPreConfig)(VOID);  /**< Suspend preconfig, provided for special needs before entering deep sleep */
    VOID (*suspendDevice)(VOID);     /**< Supend device before entering deep sleep */
    VOID (*rollback)(VOID);          /**< Rollback if failed */
    VOID (*resumeDevice)(VOID);      /**< Resume device after wakeup from deep sleep */
    VOID (*resumePostConfig)(VOID);  /**< Resume postconfig, provided for special needs after wakeup from deep sleep */
    VOID (*resumeCallBack)(VOID);    /**< Resume callback */
    VOID (*otherCoreResume)(VOID);   /**< Other core Resume for multi-core scenes */
    VOID (*resumeFromReset)(VOID);   /**< Resume from image */
} PowerMgrDeepSleepOps;

/**
 * @ingroup  los_lowpower
 *
 * Power manager parameter
 */
typedef struct {
    PowerMgrRunOps runOps;               /**< power manager framework running operations */
    PowerMgrDeepSleepOps deepSleepOps;   /**< power manager deep sleep operations */
    PowerMgrConfig config;               /**< power manager config */
} PowerMgrParameter;

/**
 * @ingroup  los_lowpower
 * @brief Init the power manager framework.
 *
 * @par Description:
 * This API is used to init the power manager framework.
 *
 * @attention None.
 *
 * @param  para [IN] The power manager parameter.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lopower_impl.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern VOID LOS_PowerMgrInit(const PowerMgrParameter *para);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif  // _LOS_LOWPOWER_IMPL_H
