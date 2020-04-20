#include "los_sys.h"
#include "los_task.h"

VOID Example_TransformTime(VOID)
{
    UINT32 uwMs;
    UINT32 uwTick;
    uwTick = LOS_MS2Tick(10000);// 10000 ms数转换为tick数
        dprintf("uwTick = %d \n",uwTick);
    uwMs = LOS_Tick2MS(100);// 100 tick数转换为ms数
        dprintf("uwMs = %d \n",uwMs);
}


VOID Example_GetTime(VOID)
{
    UINT32 uwcyclePerTick;
    UINT64 uwTickCount;

    uwcyclePerTick  = LOS_CyclePerTickGet();
    if(0 != uwcyclePerTick)
    {
        dprintf("LOS_CyclePerTickGet = %d \n", uwcyclePerTick);
    }

    uwTickCount = LOS_TickCountGet();
    if(0 != uwTickCount)
    {
        dprintf("LOS_TickCountGet = %d \n", (UINT32)uwTickCount);
    }
	LOS_TaskDelay(200);
	uwTickCount = LOS_TickCountGet();
    if(0 != uwTickCount)
    {
        dprintf("LOS_TickCountGet after delay = %d \n", (UINT32)uwTickCount);
    }
	
}
