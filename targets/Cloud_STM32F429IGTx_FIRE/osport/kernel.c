#include "los_task.h"

#include "osport.h"
void task_sleepms(s32_t ms)
{
    LOS_TaskDelay(ms);//which tick is ms
    return;
}
s32_t task_create(const char *name,fnTaskEntry fnTask,s32_t stackisize,void *stack,void *args,s32_t prior)
{
    int ret =-1;
    UINT32 uwRet = LOS_OK;
    UINT32  handle;
    TSK_INIT_PARAM_S task_init_param;
	
    task_init_param.uwArg = (unsigned int)args;
    task_init_param.usTaskPrio = prior;
    task_init_param.pcName =(char *) name;
    task_init_param.pfnTaskEntry = fnTask;
    task_init_param.uwStackSize = stackisize;
    uwRet = LOS_TaskCreate(&handle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return ret;
    }
    ret = handle;
    return ret;
}
