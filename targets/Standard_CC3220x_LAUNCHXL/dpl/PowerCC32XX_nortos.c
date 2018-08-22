/*
 * Copyright (c) 2016-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== PowerCC32XX_nortos.c ========
 */

#include <stdint.h>
#include <stdbool.h>

/* driverlib header files */
#include <ti/devices/cc32xx/inc/hw_nvic.h>
#include <ti/devices/cc32xx/inc/hw_types.h>
#include <ti/devices/cc32xx/driverlib/prcm.h>
#include <ti/devices/cc32xx/driverlib/cpu.h>
#include <ti/devices/cc32xx/driverlib/rom.h>
#include <ti/devices/cc32xx/driverlib/rom_map.h>
#include <ti/devices/cc32xx/driverlib/systick.h>
#include <ti/devices/cc32xx/driverlib/utils.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC32XX.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SwiP.h>

/* bitmask of constraints that disallow LPDS */
#define LPDS_DISALLOWED (1 << PowerCC32XX_DISALLOW_LPDS)

/*
 * macro to resolve RTC count when reading from 40MHz domain (need to read 3
 * times and compare values to ensure proper sync with RTC running at 32786Hz)
 */
#define COUNT_WITHIN_TRESHOLD(a, b, c, th) \
        ((((b) - (a)) <= (th)) ? (b) : (c))

/* macros for enabling RTC */
#define HWREG(x) (*((volatile unsigned long *)(x)))
#define HIB3P3_BASE                        0x4402F800
#define HIB3P3_O_MEM_HIB_RTC_TIMER_ENABLE  0x00000004


extern void ClockP_doTick(void);
extern void ClockP_setTicks(uint32_t ticks);

extern PowerCC32XX_ModuleState PowerCC32XX_module;

static uint64_t getCountsRTC();

static volatile uint32_t idleTime = 0;


/*
 *  ======== PowerCC32XX_initPolicy ========
 */
void PowerCC32XX_initPolicy()
{
}

/*
 *  ======== PowerCC32XX_sleepPolicy ========
 */
void PowerCC32XX_sleepPolicy()
{
    ClockP_FreqHz   cpuFreq;
    uint32_t        constraintMask;
    uint32_t        deltaTicksPerRTC;
    uint32_t        beforeTick;
    uint64_t        beforeRTC;
    uint64_t        wakeRTC;
    uint64_t        deltaTime;
    uint64_t        remain;
    uint64_t        temp64;
    uint32_t        deltaTick;
    uint32_t        workTick;
    uint32_t        newTick;
    uint32_t        newDelta;
    uintptr_t       key;
    uint32_t        swiKey;
    uint32_t        i;
    bool            slept = false;

    /* For resetting the SysTick period after wakeup */
    ClockP_getCpuFreq(&cpuFreq);

    key = HwiP_disable();

    /*
     * Check if the Power policy has been disabled since we last checked.
     * Since we're in this policy function already, the policy must have
     * been enabled (with a valid policyFxn) when we were called, but
     * could have been disbled to short-circuit this function.
     * SemaphoreP_post() does this purposely (see comments in there).
     */
    if (!PowerCC32XX_module.enablePolicy) {
        HwiP_restore(key);

        return;
    }

    swiKey = SwiP_disable();

    /* query the declared constraints */
    constraintMask = Power_getConstraintMask();

    /* check if there is a constraint to disallow LPDS */
    if ((constraintMask & LPDS_DISALLOWED) == 0) {

        /* query Clock for the next tick that has a scheduled timeout */
        deltaTick = ClockP_getTicksUntilInterrupt();

        /* convert next tick to units of microseconds in the future */
        deltaTime = (uint64_t)deltaTick * (uint64_t)ClockP_tickPeriod;

        /* check if there is enough time to transition to/from LPDS */
        if (deltaTime > PowerCC32XX_TOTALTIMELPDS) {

            /* decision is now made, going to transition to LPDS ... */

            /* get current Clock tick count */
            beforeTick = ClockP_getSystemTicks();

            /* sample the RTC count before sleep */
            beforeRTC = getCountsRTC();

            /* set tick count for next scheduled work */
            workTick = beforeTick + deltaTick;

            /* Stop the SysTick clock and disable the interrupt */
            HWREG(NVIC_ST_CTRL) = 0;
            MAP_SysTickIntDisable();

            /* compute the time interval for the LPDS wake timer */
            deltaTime -= PowerCC32XX_TOTALTIMELPDS;

            /* convert the interval in usec to units of RTC counts */
            remain = (deltaTime * 32768) / 1000000;

            /* if necessary clip the interval to a max 32-bit value */
            if (remain > 0xFFFFFFFF) {
                remain = 0xFFFFFFFF;
            }

            /* set the interval for the LPDS wake timer */
            MAP_PRCMLPDSIntervalSet(remain);

            /* enable the RTC as an LPDS wake source */
            MAP_PRCMLPDSWakeupSourceEnable(PRCM_LPDS_TIMER);

            /* now go to LPDS ... */
            Power_sleep(PowerCC32XX_LPDS);

            /* get the RTC count after wakeup */
            wakeRTC = getCountsRTC();

            /* compute delta Clock ticks spent in LPDS */
            temp64 = (((wakeRTC - beforeRTC) * 1000000) /
                (32768 * ClockP_tickPeriod));
            deltaTicksPerRTC = (uint32_t) temp64;

            /* compute resulting 'new' tick count */
            newTick = beforeTick + deltaTicksPerRTC;

            /*
             * now update Clock's internal tick count to reflect elapsed
             * time per the RTC, accomodating eventual rollover of the
             * 32-bit Clock tick count
             *
             * 'workDelta' is 'deltaTick', the number of ticks from before
             *  go to LPDS to when work is scheduled with Clock
             *
             * 'newDelta' is the number of ticks from before go to LPDS to
             * the newly computed newTick
             */
            newDelta = newTick - beforeTick;

            /* first, handle normal case of early wake (before workTick) ... */
            if (newDelta < deltaTick) {
                /* just set count to newTick */
                ClockP_setTicks(newTick);
            }
            /* else, if woke on workTick or later */
            else {
                /*
                 * update tick count, trigger Clock Swi to run ASAP
                 *  1. set tick count to workTick - 1
                 *  2. call Clock_doTick() as many times as necessary to
                 *     account for the number of missed ticks [each call will
                 *     increment tick count and post the Clock Swi, which will
                 *     run Clock_workFunc(), which uses the Swi post count to
                 *     catch up for ticks that weren't serviced on time]
                 */
                ClockP_setTicks(workTick - 1);
                for (i = 0; i < (newDelta - deltaTick + 1); i++) {
                    ClockP_doTick();
                }
            }

            /*
             *  Reconfigure and restart the SYSTICK timer
             */
            MAP_SysTickPeriodSet(cpuFreq.lo / ClockP_tickPeriod);
            HWREG(NVIC_ST_CURRENT) = 0; /* Reload counter by writing to it */
            MAP_SysTickEnable();
            MAP_SysTickIntEnable();

            /* set a flag to indicate LPDS was invoked */
            slept = true;
        }
    }

    /* re-enable interrupts */
    HwiP_restore(key);
    SwiP_restore(swiKey);

    /* sleep, but only if did not invoke a sleep state above */
    if (!(slept)) {
        MAP_PRCMSleepEnter();
    }
}

/*
 *  ======== getCountsRTC ========
 */
static uint64_t getCountsRTC()
{
    uint64_t count[3];
    uint64_t curr;
    uint32_t i;

    /*
     *  get the current RTC count, using the fast interface; to use the
     *  fast interface the count must be read three times, and then
     *  the value that matches on at least two of the reads is chosen
     */
    for (i = 0; i < 3; i++) {
        count[i] = MAP_PRCMSlowClkCtrFastGet();
    }
    curr = COUNT_WITHIN_TRESHOLD(count[0], count[1], count[2], 1);

    return (curr);
}
