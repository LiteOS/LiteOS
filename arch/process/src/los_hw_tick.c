#define _GNU_SOURCE
#include "los_base.h"
#include "los_hwi.h"
#include "los_tick.h"
#include "los_tick.ph"
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>

//used in los_tickless.c

UINT32 LOS_SysTickCurrCycleGet(VOID)
{
    return 0;
}

//used in los_tickless.c and los_tick.c

VOID LOS_SysTickReload(UINT32 uwCyclesPerTick)
{
}

void alarm_handler(int sig, siginfo_t *info, void *context)
{
    osTickHandler();
}

//used in los_init.c declared in log_config.h

UINT32 osTickStart()
{
    struct itimerval timerval;
    struct sigaction action;
    if ((0 == OS_SYS_CLOCK)
            || (0 == LOSCFG_BASE_CORE_TICK_PER_SECOND)
            || (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK)) {
        return LOS_ERRNO_TICK_CFG_INVALID;
    }
    memset(&action, 0, sizeof (action));
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = alarm_handler;
    sigemptyset(&action.sa_mask);
    //SIGUSR1 should be handled after tick handler returns
    //actually, SIGUSR1 is the context switch interrupt handler that has the lowest priority
    sigaddset(&action.sa_mask, SIGUSR1);

    if (sigaction(SIGVTALRM, &action, NULL) != 0) {
        puts(strerror(errno));
        return LOS_ERRNO_TICK_CFG_INVALID;
    }
    timerval.it_interval.tv_sec = 0;
    timerval.it_interval.tv_usec = (suseconds_t) (1000 / LOSCFG_BASE_CORE_TICK_PER_SECOND); // 4 microseconds
    memcpy(&timerval.it_value, &timerval.it_interval, sizeof ( struct timeval));
    if (setitimer(ITIMER_VIRTUAL, &timerval, NULL) != 0)
        return LOS_ERRNO_TICK_CFG_INVALID;

    return 0;
}
