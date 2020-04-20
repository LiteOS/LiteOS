UINT32 g_uwTskLoID;
UINT32 g_uwTskHiID;
#define TSK_PRIOR_HI 4
#define TSK_PRIOR_LO 5

UINT32 Example_TaskHi(VOID)
{
    UINT32 uwRet;
    UINT32 uwCurrentID;
    TSK_INFO_S stTaskInfo;

    printf("[cpu%d] Enter TaskHi Handler.\r\n", osCurrCpuid());

    /*延时2个Tick，延时后该任务会挂起，执行剩余任务中最高优先级的任务(g_uwTskLoID任务)*/
    uwRet = LOS_TaskDelay(2);
    if (uwRet != LOS_OK)
    {
        printf("Delay Task Failed.\r\n");
        return LOS_NOK;
    }

    /*2个Tick时间到了后，该任务恢复，继续执行*/
    printf("TaskHi LOS_TaskDelay Done.\r\n");

    /*挂起自身任务*/
    uwRet = LOS_TaskSuspend(g_uwTskHiID);
    if (uwRet != LOS_OK)
    {
        printf("Suspend TaskHi Failed.\r\n");
        return LOS_NOK;
    }
    printf("TaskHi LOS_TaskResume Success.\r\n");
}

/*低优先级任务入口函数*/
UINT32 Example_TaskLo(VOID)
{
    UINT32 uwRet;
    UINT32 uwCurrentID;
    TSK_INFO_S stTaskInfo;

    printf("[cpu%d] Enter TaskLo Handler.\r\n", osCurrCpuid());

    /*延时2个Tick，延时后该任务会挂起，执行剩余任务中就高优先级的任务(背景任务)*/
    uwRet = LOS_TaskDelay(2);
    if (uwRet != LOS_OK)
    {
        printf("Delay TaskLo Failed.\r\n");
        return LOS_NOK;
    }

    printf("TaskHi LOS_TaskDelete Success.\r\n");
}

/*任务测试入口函数，在里面创建优先级不一样的两个任务*/
UINT32 Example_TskCaseEntry(VOID)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stInitParam = {0};

    /*锁任务调度*/
    LOS_TaskLock();

    printf("LOS_TaskLock() Success on cpu%d!\r\n", osCurrCpuid());

    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskHi;
    stInitParam.usTaskPrio = TSK_PRIOR_HI;
    stInitParam.pcName = "HIGH_NAME";
    stInitParam.uwStackSize = 0x700;
    stInitParam.uwResved   = LOS_TASK_STATUS_DETACHED;
    /*绑定高优先级任务到CPU1运行*/
    stInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(osCurrCpuid());
    /*创建高优先级任务，由于CPU1的调度器被锁，任务创建成功后不会马上执行*/
    uwRet = LOS_TaskCreate(&g_uwTskHiID, &stInitParam);
    if (uwRet != LOS_OK)
    {
        LOS_TaskUnlock();

        printf("Example_TaskHi create Failed!\r\n");
        return LOS_NOK;
    }

    printf("Example_TaskHi create Success!\r\n");

    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskLo;
    stInitParam.usTaskPrio = TSK_PRIOR_LO;
    stInitParam.pcName = "LOW_NAME";
    stInitParam.uwStackSize = 0x700;
    stInitParam.uwResved   = LOS_TASK_STATUS_DETACHED;
    /*低优先级任务不设置CPU亲和性*/
    stInitParam.usCpuAffiMask = 0;
    /*创建低优先级任务，尽管锁任务调度，但是由于该任务没有绑定该处理器，任务创建成功后可以马上在其他CPU执行*/
    uwRet = LOS_TaskCreate(&g_uwTskLoID, &stInitParam);
    if (uwRet != LOS_OK)
    {
        LOS_TaskUnlock();

        printf("Example_TaskLo create Failed!\r\n");
        return LOS_NOK;
    }

    printf("Example_TaskLo create Success!\r\n");

    /*解锁任务调度，此时会发生任务调度，执行就绪列表中最高优先级任务*/
    LOS_TaskUnlock();

    while(1){};

    return LOS_OK;
}
