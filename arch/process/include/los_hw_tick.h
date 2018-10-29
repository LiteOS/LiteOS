#ifndef _LOS_HW_TICK_H
#define _LOS_HW_TICK_H

#include "los_base.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if (LOSCFG_KERNEL_TICKLESS == YES)
/**
 * @ingroup los_hwi
 * Check whether the counting direction of system tick is decreasing, it will be used to
 * readjust the value of the system tick, if not decreasing, please set this macro to NO.
 */
#define LOSCFG_SYSTICK_CNT_DIR_DECREASE      YES

/**
 * @ingroup los_hwi
 * Max reload value of system tick.
 */
#define LOSCFG_SYSTICK_LOAD_RELOAD_MAX      SysTick_LOAD_RELOAD_Msk

/*****************************************************************************
Function   : LOS_SysTickStop
Description: stop systick
Input   : none
output  : none
return  : none
*****************************************************************************/
LITE_OS_SEC_TEXT_MINOR static inline VOID LOS_SysTickStop(VOID)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/*****************************************************************************
Function   : LOS_SysTickStart
Description: start systick
Input   : none
output  : none
return  : none
*****************************************************************************/
LITE_OS_SEC_TEXT_MINOR static inline VOID LOS_SysTickStart(VOID)
{
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/*****************************************************************************
Function   : LOS_SysTickGetIntStatus
Description: get systick interrupt status
Input   : none
output  : none
return  : systick interrupt status
*****************************************************************************/
LITE_OS_SEC_TEXT_MINOR static inline UINT32 LOS_SysTickGetIntStatus(VOID)
{
    return SCB->ICSR & SCB_ICSR_PENDSTSET_Msk;
}

extern VOID LOS_SysTickReload(UINT32 uwCyclesPerTick);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _LOS_HW_H */

