#include "los_hwi.h"
//used in los_tickless.c
UINT32 LOS_SysTickCurrCycleGet(VOID) {
	return 0;
}

//used in los_tickless.c and los_tick.c
VOID LOS_SysTickReload(UINT32 uwCyclesPerTick){
}

//used in los_init.c declared in log_config.h
UINT32 osTickStart() {
	return 0;
}


