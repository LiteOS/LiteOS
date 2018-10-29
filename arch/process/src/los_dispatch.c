#include "los_base.h"

UINT32 __LOS_HEAP_ADDR_START__;
UINT32 __LOS_HEAP_ADDR_END__;
//los_config.h
void LOS_StartToRun(){
}


//following declared in los_hwi.h
UINTPTR LOS_IntLock() {
	return NULL;
}

void LOS_IntRestore(UINTPTR uvIntSave) {
}

UINTPTR LOS_IntUnLock() {
	return NULL;
}

