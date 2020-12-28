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

/* *
 * @defgroup los_lowpower lowpower_framework
 * @ingroup kernel
 */

#ifndef _LOS_LOWPOWER_H
#define _LOS_LOWPOWER_H

#include "los_base.h"
#include "los_sys.h"
#include "los_err.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup  los_lowpower
 *
 * Intermit modes
 */
typedef enum LOS_INTERMIT_MODE {
    LOS_INTERMIT_NONE = 0,
    LOS_INTERMIT_LIGHT_SLEEP, /**< Light sleep mode */
    LOS_INTERMIT_DEEP_SLEEP,  /**< Deep sleep mode */
    LOS_INTERMIT_STANDBY,     /**< Standby mode */
    LOS_INTERMIT_SHUTDOWN,    /**< Shutdown mode */
    LOS_INTERMIT_MAX,
} LosIntermitMode;

/**
 * @ingroup  los_lowpower
 *
 * System main frequency modes
 */
typedef enum LOS_FREQ_MODE {
    LOS_SYS_FREQ_SUPER = 0, /**< Super high freq */
    LOS_SYS_FREQ_HIGH,      /**< High freq */
    LOS_SYS_FREQ_NORMAL,    /**< Normal freq */
    LOS_SYS_FREQ_LOW,       /**< Low freq */
    LOS_SYS_FREQ_MAX,
} LosFreqMode;

typedef UINT32 (*LowpowerExternalVoterHandle)(VOID);

STATIC INLINE BOOL FreqHigher(LosFreqMode freq1, LosFreqMode freq2)
{
    return freq1 < freq2;
}

/**
 * @ingroup los_lowpower
 *
 * Define the structure of the power manager operations.
 */
typedef struct {
    VOID (*process)(VOID);                   /**< Power manager framework entry interface */
    VOID (*wakeupFromReset)(VOID);           /**< Recovery interface used to wakeup from image */
    VOID (*resumeFromInterrupt)(UINT32);     /**< Recovery interface used to wakeup from interrupt */
    VOID (*changeFreq)(LosFreqMode);         /**< System frequency tuning interface, the param is LosFreqMode */
    VOID (*deepSleepVoteBegin)(VOID);        /**< Deep sleep vote mark interface */
    VOID (*deepSleepVoteEnd)(VOID);          /**< Deep sleep vote erase interface */
    VOID (*deepSleepVoteDelay)(UINT32 tick); /**< Deep sleep vote delay interface, the param is the delayed ticks */
    VOID (*registerExternalVoter)(UINT32 (*handler)(VOID));  /**< External voter registration interface */
    UINT32 (*getDeepSleepVoteCount)(VOID);   /**< Get deep sleep vote count interface */
    UINT32 (*getSleepMode)(VOID);            /**< Get sleep mode interface, the retval type is LosIntermitMode */
    VOID (*setSleepMode)(UINT32 mode);       /**< Set sleep mode interface, the param type is LosIntermitMode */
} PowerMgrOps;

/**
 * @ingroup  los_lowpower
 * @brief System main frequency tuning.
 *
 * @par Description:
 * This API is used to tune the system main frequency.
 *
 * @attention None.
 *
 * @param  freq [IN] The system frequency, corresponding to LosFreqMode.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern VOID LOS_PowerMgrChangeFreq(LosFreqMode freq);

/**
 * @ingroup  los_lowpower
 * @brief Vote to enter deep sleep.
 *
 * @par Description:
 * This API is used to mark the deep sleep vote. Called when the current state is idle.
 *
 * @attention None.
 *
 * @param  None.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern VOID LOS_PowerMgrDeepSleepVoteBegin(VOID);

/**
 * @ingroup  los_lowpower
 * @brief Erase the deep sleep vote.
 *
 * @par Description:
 * This API is used to erase the deep sleep vote. Called when the current state is busy.
 *
 * @attention None.
 *
 * @param  None.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern VOID LOS_PowerMgrDeepSleepVoteEnd(VOID);

/**
 * @ingroup  los_lowpower
 * @brief Sleep delay vote.
 *
 * @par Description:
 * This API is used to delay sleep vote. Called when the current state busy, but can enter sleep later.
 *
 * @attention None.
 *
 * @param  tick [IN] The sleeptime.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern VOID LOS_PowerMgrSleepDelay(UINT32 tick);

/**
 * @ingroup  los_lowpower
 * @brief Register the external voter.
 *
 * @par Description:
 * This API is used to register the external voter, provided for developers with special needs.
 *
 * @attention None.
 *
 * @param  UINT32 (*)(VOID) [IN] The external voter.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern VOID LOS_PowerMgrRegisterExtVoter(UINT32 (*)(VOID));

/**
 * @ingroup  los_lowpower
 * @brief Get the sleep mode.
 *
 * @par Description:
 * This API is used to get sleep mode. Developers can set different corresponding modes.
 * according to the actual scene, then perform follow-up operations.
 *
 * @attention None.
 *
 * @param  None.
 *
 * @retval #UINT32 Sleep mode, corresponding to LosIntermitMode.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern UINT32 LOS_PowerMgrGetSleepMode(VOID);

/**
 * @ingroup  los_lowpower
 * @brief Get the deep sleep vote count.
 *
 * @par Description:
 * This API is used to get the deep sleep vote count.
 *
 * @attention None.
 *
 * @param  None.
 *
 * @retval #UINT32 Deep sleep vote count.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern UINT32 LOS_PowerMgrGetDeepSleepVoteCount(VOID);

/**
 * @ingroup  los_lowpower
 * @brief Register power manager operations.
 *
 *
 * @par Description:
 * This API is used to register power manager operations or customized power manager by developers.
 *
 * @attention None.
 *
 * @param  pmOps [IN] The power manager operations.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern VOID LOS_LowpowerInit(const PowerMgrOps *pmOps);

/**
 * @ingroup los_lowpower
 * @brief Define the lowpower framework process function type.
 *
 * @par Description:
 * This API is used to define the lowpower framework entry function type.
 *
 * @attention None.
 *
 * @param None.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
typedef VOID (*LowPowerHookFn)(VOID);

/**
 * @ingroup  los_lowpower
 * @brief Register a hook to enter lowpower framework process.
 *
 * @par Description:
 * This API is used to register lowpower framework entry function.
 *
 * @attention None.
 *
 * @param  hook [IN] The lowpower framework hook.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern VOID LOS_LowpowerHookReg(LowPowerHookFn hook);

/**
 * @ingroup los_lowpower
 * @brief Define the lowpower framework wakup function type.
 *
 * @par Description:
 * This API is used to define the lowpower framework wakup function type.
 *
 * @attention None.
 *
 * @param  hwiNum [IN] The interrupt number.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
typedef VOID (*IntWakeupHookFn)(HWI_HANDLE_T hwiNum);

/**
 * @ingroup  los_lowpower
 * @brief Register a hook to wakeup from interrupt.
 *
 * @par Description:
 * This API is used to register a recovery function after wakeup from interrupt
 *
 * @attention None.
 *
 * @param  hook [IN] The lowpower wakeup hook.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_lowpower.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R005C10
 */
extern VOID LOS_IntWakeupHookReg(IntWakeupHookFn hook);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* _LOS_LOWPOWER_H */
