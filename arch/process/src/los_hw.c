#define _GNU_SOURCE
#include <ucontext.h>
#include "los_hw.h"
#include "los_task.ph"
#include "los_priqueue.ph"

void LOS_Schedule()
{
    UINTPTR IntSave;
    IntSave = LOS_IntLock();
    g_stLosTask.pstNewTask = LOS_DL_LIST_ENTRY(osPriqueueTop(), LOS_TASK_CB, stPendList);
    if (g_stLosTask.pstRunTask != g_stLosTask.pstNewTask) {
        if (!g_usLosTaskLock) {
            LOS_IntRestore(IntSave);
            osTaskSchedule();
            return;
        }
    }
    LOS_IntRestore(IntSave);
}

static void taskStub(int TaskID)
{
    LOS_TASK_CB *tcb = OS_TCB_FROM_TID(TaskID);
    tcb->pfnTaskEntry(tcb->uwArg);
}

void *osTskStackInit(UINT32 uwTaskID, UINT32 uwStackSize, VOID *pTopStack)
{
    ucontext_t *context;
    *((UINT32 *) pTopStack) = OS_TASK_MAGIC_WORD;

    context = (ucontext_t *) (pTopStack + uwStackSize - sizeof (ucontext_t));
    getcontext(context);
    context->uc_stack.ss_sp = pTopStack;
    //context->uc_stack.ss_flags = 0;
    context->uc_stack.ss_size = uwStackSize - sizeof (ucontext_t);
    sigemptyset(&context->uc_sigmask);

    /*compiler would warning passing incompatibale function pointer here.
     * see  https://en.wikipedia.org/wiki/Setcontext
     */
    makecontext(context, taskStub, 1, uwTaskID);
    return context;
}

void osSchedule()
{
}

void osEnterSleep()
{
}


