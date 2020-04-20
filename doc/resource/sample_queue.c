#include "los_base.h"
#include "los_task.h"
#include "los_swtmr.h"
#include "los_hwi.h"
#include "los_queue.h"
#include "los_event.h"
#include "los_typedef.h"

static UINT32 g_uwQueue;
CHAR abuf[] = "test is message x";

void *send_Entry(void *arg)
{
    UINT32 i = 0,uwRet = 0;
    UINT32 uwlen = sizeof(abuf);

    while (i <5)
    {
        abuf[uwlen -2] = '0' + i;
        i++;

        uwRet = LOS_QueueWrite(g_uwQueue, abuf, uwlen, 0);
        if(uwRet != LOS_OK)
        {
            dprintf("send message failure,error:%x\n",uwRet);
        }

        LOS_TaskDelay(5);
    }
}

void *recv_Entry(void *arg)
{
    UINT32 uwReadbuf;
    UINT32 uwRet = 0;

    while (1)
    {


        uwRet = LOS_QueueRead(g_uwQueue, &uwReadbuf, 50, 0);
        if(uwRet != LOS_OK)
        {
            dprintf("recv message failure,error:%x\n",uwRet);
            break;
        }

        dprintf("recv message:%s\n", (char *)uwReadbuf);
        LOS_TaskDelay(5);
    }

    while (LOS_OK != LOS_QueueDelete(g_uwQueue))
    {
        LOS_TaskDelay(1);
    }

    dprintf("delete the queue success!\n");
}

int Example_create_task(void)
{
    UINT32 uwRet = 0;
    UINT32 uwTask1, uwTask2;
    TSK_INIT_PARAM_S stInitParam1;

    stInitParam1.pfnTaskEntry = send_Entry;
    stInitParam1.usTaskPrio = 9;
    stInitParam1.uwStackSize = 0x400;
    stInitParam1.pcName = "sendQueue";
    stInitParam1.uwResved = LOS_TASK_STATUS_DETACHED ;
    LOS_TaskLock();
    uwRet = LOS_TaskCreate(&uwTask1, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        dprintf("create task1 failed!,error:%x\n",uwRet);
        return uwRet;
    }

    stInitParam1.pfnTaskEntry = recv_Entry;
    uwRet = LOS_TaskCreate(&uwTask2, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        dprintf("create task2 failed!,error:%x\n",uwRet);
        return uwRet;
    }

    uwRet = LOS_QueueCreate("queue", 5, &g_uwQueue, 0, 50);

    if(uwRet != LOS_OK)
    {
        dprintf("create queue failure!,error:%x\n",uwRet);
    }

    dprintf("create the queue success!\n");
    LOS_TaskUnlock();
}
