#include "los_base.h"
#include "los_typedef.h"
int main() {
	UINT32 uwRet = LOS_OK;
	
	//HardWare_init();
	uwRet = LOS_KernelInit();
	if (uwRet != LOS_OK)
		return LOS_NOK;
	
	//shell_install();
	LOS_Start();
	return 0;
}
