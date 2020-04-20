#include "los_mux.h"
#include "los_task.h"



/*互斥锁句柄id*/
MUX_HANDLE_T  g_Testmux01;
/*任务PID*/
UINT32 g_TestTaskID01;
UINT32 g_TestTaskID02;

VOID Example_MutexTask1()
{
    UINT32 uwRet;

    printf("task1 try to get  mutex, wait 10 ticks.\n");
    /*申请互斥锁*/
    uwRet=LOS_MuxPend(g_Testmux01, 10);

    if(uwRet == LOS_OK)
    {
        printf("task1 get mutex g_Testmux01.\n");
        /*释放互斥锁*/
        LOS_MuxPost(g_Testmux01);
        return;
    }
    else if(uwRet == LOS_ERRNO_MUX_TIMEOUT )
    {
            printf("task1 timeout and try to get  mutex, wait forever.\n");
            /*申请互斥锁*/
            uwRet=LOS_MuxPend(g_Testmux01, LOS_WAIT_FOREVER);
            if(uwRet == LOS_OK)
            {
                printf("task1 wait forever,get mutex g_Testmux01.\n");
                /*释放互斥锁*/
                LOS_MuxPost(g_Testmux01);
                return;
            }
    }
    return;
}

VOID Example_MutexTask2()
{
    UINT32 uwRet;

    printf("task2 try to get  mutex, wait forever.\n");
    /*申请互斥锁*/
    uwRet=LOS_MuxPend(g_Testmux01, LOS_WAIT_FOREVER);

    printf("task2 get mutex g_Testmux01 and suspend 100 ticks.\n");

    /*任务休眠100 ticks*/
    LOS_TaskDelay(100);

    printf("task2 resumed and post the g_Testmux01\n");
    /*释放互斥锁*/
    LOS_MuxPost(g_Testmux01);
    return;

}

UINT32 Example_TaskEntry()
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTask1;
    TSK_INIT_PARAM_S stTask2;

    /*创建互斥锁*/
    LOS_MuxCreate(&g_Testmux01);

    /*锁任务调度*/
    LOS_TaskLock();

    /*创建任务1*/
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask1;
    stTask1.pcName       = "MutexTsk1";
    stTask1.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = 5;
    uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1);
    if(uwRet != LOS_OK)
    {
        printf("task1 create failed .\n");
        return LOS_NOK;
    }

    /*创建任务2*/
    memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S));
    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask2;
    stTask2.pcName       = "MutexTsk2";
    stTask2.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    stTask2.usTaskPrio   = 4;
    uwRet = LOS_TaskCreate(&g_TestTaskID02, &stTask2);
    if(uwRet != LOS_OK)
    {
        printf("task2 create failed .\n");
        return LOS_NOK;
    }

    /*解锁任务调度*/
    LOS_TaskUnlock();
    /*任务休眠300 ticks*/
    LOS_TaskDelay(300);

    /*删除互斥锁*/
    LOS_MuxDelete(g_Testmux01);

    /*删除任务1*/
    uwRet = LOS_TaskDelete(g_TestTaskID01);
    if(uwRet != LOS_OK)
    {
        printf("task1 delete failed .\n");
        return LOS_NOK;
    }
    /*删除任务2*/
    uwRet = LOS_TaskDelete(g_TestTaskID02);
    if(uwRet != LOS_OK)
    {
        printf("task2 delete failed .\n");
        return LOS_NOK;
    }

    return LOS_OK;
}


