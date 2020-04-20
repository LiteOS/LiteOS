#include "los_sem.h"


/*任务PID*/
static UINT32 g_TestTaskID01,g_TestTaskID02;
/*测试任务优先级*/
#define TASK_PRIO_TEST  5
/*信号量结构体id*/
static SEM_HANDLE_T g_usSemID;


VOID Example_SemTask1(void)
{
    UINT32 uwRet;

    printf("Example_SemTask1 try get sem g_usSemID ,timeout 10 ticks.\n");
    /*定时阻塞模式申请信号量，定时时间为10ticks*/
    uwRet = LOS_SemPend(g_usSemID, 10);

    /*申请到信号量*/
    if(LOS_OK == uwRet)
    {
         LOS_SemPost(g_usSemID);
         return;
    }
    /*定时时间到，未申请到信号量*/
    if(LOS_ERRNO_SEM_TIMEOUT == uwRet)
    {
        printf("Example_SemTask1 timeout and try get sem g_usSemID wait forever.\n");
        /*永久阻塞模式申请信号量*/
        uwRet = LOS_SemPend(g_usSemID, LOS_WAIT_FOREVER);
        printf("Example_SemTask1 wait_forever and get sem g_usSemID .\n");
        if(LOS_OK == uwRet)
        {
            LOS_SemPost(g_usSemID);
            return;
        }
    }
    return;

}
 VOID   Example_SemTask2(void)
{
    UINT32 uwRet;
    printf("Example_SemTask2 try get sem g_usSemID wait forever.\n");
    /*永久阻塞模式申请信号量*/
    uwRet = LOS_SemPend(g_usSemID, LOS_WAIT_FOREVER);

    if(LOS_OK == uwRet)
    printf("Example_SemTask2 get sem g_usSemID and then delay 20ticks .\n");

    /*任务休眠20 ticks*/
    LOS_TaskDelay(20);

    printf("Example_SemTask2 post sem g_usSemID .\n");
    /*释放信号量*/
    LOS_SemPost(g_usSemID);

    return;

}
UINT32 Example_TaskEntry()
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTask1;
    TSK_INIT_PARAM_S stTask2;

   /*创建信号量*/
    LOS_SemCreate(0,&g_usSemID);

    /*锁任务调度*/
    LOS_TaskLock();

    /*创建任务1*/
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask1;
    stTask1.pcName       = "MutexTsk1";
    stTask1.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = TASK_PRIO_TEST;
    uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1);
    if(uwRet != LOS_OK)
    {
        printf("task1 create failed .\n");
        return LOS_NOK;
    }

    /*创建任务2*/
    memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S));
    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask2;
    stTask2.pcName       = "MutexTsk2";
    stTask2.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    stTask2.usTaskPrio   = (TASK_PRIO_TEST - 1);
    uwRet = LOS_TaskCreate(&g_TestTaskID02, &stTask2);
    if(uwRet != LOS_OK)
    {
        printf("task2 create failed .\n");
        return LOS_NOK;
    }

    /*解锁任务调度*/
    LOS_TaskUnlock();

    uwRet = LOS_SemPost(g_usSemID);

    /*任务休眠40 ticks*/
    LOS_TaskDelay(40);

    /*删除信号量*/
    LOS_SemDelete(g_usSemID);

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


