#ifndef _LOS_HWI_H
#define _LOS_HWI_H
#include "los_base.h"
#include "los_sys.h"
#include <signal.h>
typedef UINT32 HWI_HANDLE_T;
typedef UINT16 HWI_PRIOR_T;
typedef UINT16 HWI_MODE_T;
typedef UINT32 HWI_ARG_T;

#if (OS_HWI_WITH_ARG == YES)

typedef VOID(* HWI_PROC_FUNC)(VOID *pParm);

typedef struct {
    HWI_PROC_FUNC pfnHandler;
    VOID* pParm;
} HWI_SLAVE_FUNC;

#else
typedef VOID(* HWI_PROC_FUNC)(void);
#endif

/**
 * @ingroup los_hwi
 * Count of interrupts.
 */
extern UINT32 g_vuwIntCount;

/**
 * @ingroup los_hwi
 * An interrupt is active.
 */
#define OS_INT_ACTIVE               (g_vuwIntCount > 0)

/**
 * @ingroup los_hwi
 * An interrupt is inactive.
 */
#define OS_INT_INACTIVE             (!(OS_INT_ACTIVE))
extern UINT32 LOS_HwiCreate(HWI_HANDLE_T uwHwiNum,
        HWI_PRIOR_T usHwiPrio,
        HWI_MODE_T usMode,
        HWI_PROC_FUNC pfnHandler,
        HWI_ARG_T uwArg
        );
extern UINTPTR LOS_IntUnLock(VOID);
extern UINTPTR LOS_IntLock(VOID);
extern VOID LOS_IntRestore(UINTPTR uvIntSave);
extern VOID osInterrupt(int sig);
#if (LOSCFG_KERNEL_TICKLESS == YES)
extern VOID LOS_SysTickReload(UINT32 uwCyclesPerTick);
#endif
extern UINT32 LOS_SysTickCurrCycleGet(VOID);
extern UINT32 LOS_HwiDelete(HWI_HANDLE_T uwHwiNum);
#endif
