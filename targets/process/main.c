#include "los_base.h"
#include "los_typedef.h"
#include "los_task.h"

UINT32 mytaskA(void *arg)
{
    while (1) {
    };
    return 0;
}

UINT32 mytaskB(void *arg)
{
    while (1) {
    };
    return 0;
}

int main()
{
    UINT32 uwRet = LOS_OK;
    //HardWare_init();
    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
        return LOS_NOK;

    //shell_install();
    UINT32 id;
    TSK_INIT_PARAM_S param;
    memset(&param, 0, sizeof (param));
    param.uwArg = NULL;
    param.usTaskPrio = 10;
    param.pcName = "myTaskA";
    param.pfnTaskEntry = mytaskA;
    //param.uwStackSize = 0x800;
    LOS_TaskCreate(&id, &param);

    memset(&param, 0, sizeof (param));
    param.pcName = "myTaskB";
    param.usTaskPrio = 10;
    param.pfnTaskEntry = mytaskB;
    //param.uwStackSize = 0x800;
    LOS_TaskCreate(&id, &param);
    LOS_Start();
    return 0;
}
