#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include "los_task.h"

#include "uart.h"

/*****************************************************************************
    global var
 *****************************************************************************/
static UINT32 g_uwDemoTaskID;

/*****************************************************************************
 Function    : LOS_Printf_TskDeal
 Description : Printf Task Deal
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static VOID LOS_Printf_TskDeal(VOID)
{
    while(1) {
        uart2_printf("\r\nLos Printf start.\r\n");
        LOS_TaskDelay(1000);
    }
}

/*****************************************************************************
 Function    : LOS_Printf_Entry
 Description : Create Printf task
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
UINT32 LOS_Printf_Entry(VOID)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((VOID *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_Printf_TskDeal;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "PrintfTsk";
    stTaskInitParam.usTaskPrio = 9;
    uwRet = LOS_TaskCreate(&g_uwDemoTaskID, &stTaskInitParam);
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
