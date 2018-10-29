#include "los_hwi.h"
//used in los_init.c
UINT32 g_vuwIntCount = 0;
void osHwiInit();
UINT32 LOS_HwiDelete(HWI_HANDLE_T uwHwiNum) {
	return 0;
}

UINT32 LOS_HwiCreate(HWI_HANDLE_T  uwHwiNum,
                           HWI_PRIOR_T   usHwiPrio,
                           HWI_MODE_T    usMode,
                           HWI_PROC_FUNC pfnHandler,
                           HWI_ARG_T     uwArg
                           ) {
	return 0;
}



