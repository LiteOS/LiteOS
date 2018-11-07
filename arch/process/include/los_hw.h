#ifndef _LOS_HW_H
#define _LOS_HW_H

#include "los_base.h"
#define LOS_CHECK_SCHEDULE (!g_usLosTaskLock)

extern VOID *osTskStackInit(UINT32 uwTaskID, UINT32 uwStackSize, VOID *pTopStack);

extern VOID osSchedule(VOID);
extern VOID LOS_Schedule(VOID);
extern VOID osEnterSleep(VOID);
#endif
