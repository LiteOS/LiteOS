#include "los_event.h"
#include "los_task.h"
//#include "osTest.h"

/*任务PID*/
UINT32 g_TestTaskID01;

/*事件控制结构体*/
EVENT_CB_S  example_event;

/*等待的事件类型*/
#define event_wait 0x00000001

/*用例任务入口函数*/
VOID Example_Event()
{
    UINT32 uwRet;
    UINT32 uwEvent;

/*超时 等待方式读事件,超时时间为100 ticks
   若100 ticks 后未读取到指定事件，读事件超时，任务直接唤醒*/
    printf("Example_Event wait event 0x%x \n",event_wait);

    uwEvent = Los_EventRead(&example_event, event_wait, LOS_WAITMODE_AND, 100);
    if(uwEvent == event_wait)
    {
        printf("Example_Event,read event :0x%x\n",uwEvent);
    }
    else
        printf("Example_Event,read event timeout\n");
    return;
}

UINT32 Example_TaskEntry()
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTask1;

    /*事件初始化*/
    uwRet = Los_EventInit(&example_event);
    if(uwRet != LOS_OK)
    {
        printf("init event failed .\n");
        return -1;
    }

    /*创建任务*/
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_Event;
    stTask1.pcName       = "EventTsk1";
    stTask1.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = 5;
    uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1);
    if(uwRet != LOS_OK)
    {
        printf("task create failed .\n");
        return LOS_NOK;
    }

    /*写g_TestTaskID01 等待事件*/
    printf("Example_TaskEntry write event .\n");

    uwRet = Los_EventWrite(&example_event, event_wait);
    if(uwRet != LOS_OK)
    {
        printf("event write failed .\n");
        return LOS_NOK;
    }

	/*清标志位*/
    printf("EventMask:%d\n",example_event.uwEventID);
    LOS_EventClear(&example_event, ~example_event.uwEventID);
    printf("EventMask:%d\n",example_event.uwEventID);
	
    /*删除任务*/
    uwRet = LOS_TaskDelete(g_TestTaskID01);
    if(uwRet != LOS_OK)
    {
        printf("task delete failed .\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

