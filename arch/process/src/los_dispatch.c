#define _GNU_SOURCE
#include "los_base.h"
#include "los_task.ph"
#include <stdlib.h>
#include <signal.h>
#include <ucontext.h>

#define GET_UCONTEXT(tcb)  (ucontext_t *)((tcb)->uwTopOfStack + (tcb)->uwStackSize - sizeof(ucontext_t))
void populateHeap() __attribute__ ((constructor));
extern sigset_t registerred_signals;
UINT32 __LOS_HEAP_ADDR_START__;
UINT32 __LOS_HEAP_ADDR_END__;
extern TSKSWITCHHOOK g_pfnTskSwitchHook;
static sigset_t saveInt;
//los_config.h
BOOL g_bInterruptState = 0;

void LOS_StartToRun()
{
    ucontext_t *context;
    extern BOOL g_bTaskScheduled;
    g_bTaskScheduled = 1;
    g_stLosTask.pstRunTask = g_stLosTask.pstNewTask;
    g_stLosTask.pstRunTask->usTaskStatus |= OS_TASK_STATUS_RUNNING;
    context = (ucontext_t *) g_stLosTask.pstRunTask->pStackPointer;
    LOS_IntUnLock();
    setcontext(context);
}

void osTaskSchedule()
{
    raise(SIGUSR1);
}

void switcher(int sig, siginfo_t *info, void *context)
{
    ucontext_t *oldcontext = GET_UCONTEXT(g_stLosTask.pstRunTask);
    ucontext_t *newcontext = GET_UCONTEXT(g_stLosTask.pstNewTask);

    LOS_IntLock();
    if (g_pfnTskSwitchHook != NULL)
        (*g_pfnTskSwitchHook)();
    //XXX not sure if it is working, need to dive in.....
    g_stLosTask.pstRunTask->pStackPointer = (void *)((ucontext_t *) context)->uc_mcontext.gregs[REG_ESP];
    g_stLosTask.pstRunTask->usTaskStatus &= ~(OS_TASK_STATUS_RUNNING);
    g_stLosTask.pstRunTask = g_stLosTask.pstNewTask;
    g_stLosTask.pstRunTask->usTaskStatus |= OS_TASK_STATUS_RUNNING;
    LOS_IntUnLock();
    swapcontext(oldcontext, newcontext);
}
//following declared in los_hwi.h

UINTPTR LOS_IntLock()
{
    g_bInterruptState = 0;
    sigprocmask(SIG_BLOCK, &registerred_signals, &saveInt);
    return (UINTPTR)&saveInt;
}

void LOS_IntRestore(UINTPTR uvIntSave)
{
    g_bInterruptState = 1;
    sigprocmask(SIG_UNBLOCK, &saveInt, NULL);
}

UINTPTR LOS_IntUnLock()
{
    g_bInterruptState = 0;
    sigprocmask(SIG_UNBLOCK, &registerred_signals, &saveInt);
    return (UINTPTR)&saveInt;
}

void populateHeap()
{
    puts("constructor called!\n");
    __LOS_HEAP_ADDR_START__ = (UINT32) malloc(1 << 24);
    __LOS_HEAP_ADDR_END__ = __LOS_HEAP_ADDR_START__ + (1 << 20);
    main();
}
