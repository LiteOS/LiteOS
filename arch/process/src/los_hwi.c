#define _GNU_SOURCE
#include "los_hwi.h"
#include "los_memory.h"
#include <signal.h>
#include <stdlib.h>
#include <signal.h>
//used in los_init.c
UINT32 g_vuwIntCount = 0;
sigset_t registerred_signals;
void switcher(int sig, siginfo_t *info, void *context);

void osHwiInit()
{
    sigemptyset(&registerred_signals);
    sigaddset(&registerred_signals, SIGALRM);
    sigaddset(&registerred_signals, SIGUSR1);

    stack_t stk;
    stk.ss_sp = (VOID *) LOS_MemAllocAlign(OS_TASK_STACK_ADDR, 1 << 13, LOSCFG_STACK_POINT_ALIGN_SIZE);
    stk.ss_flags = 0;
    stk.ss_size = 1 << 13;
    if (sigaltstack(&stk, NULL) != 0) {
        perror("sigaltstack failed");
        exit(-1);
    }
    struct sigaction switch_act;
    memset(&switch_act, 0, sizeof (switch_act));
    switch_act.sa_flags = SA_SIGINFO;
    switch_act.sa_sigaction = switcher;
    sigemptyset(&switch_act.sa_mask);

    if (sigaction(SIGUSR1, &switch_act, NULL) != 0) {
        perror("add switch interrupt failed");
        exit(-1);
    }
    

}

UINT32 LOS_HwiDelete(HWI_HANDLE_T uwHwiNum)
{
    return 0;
}

UINT32 LOS_HwiCreate(HWI_HANDLE_T uwHwiNum,
        HWI_PRIOR_T usHwiPrio,
        HWI_MODE_T usMode,
        HWI_PROC_FUNC pfnHandler,
        HWI_ARG_T uwArg
        )
{
    return 0;
}

void registerSignal(int sig)
{

}

VOID osInterrupt(int sig)
{

}

