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
 *  ======== ClockPSysTick_nortos.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ti/devices/DeviceFamily.h>

#if (DeviceFamily_ID == DeviceFamily_ID_MSP432E401Y) || \
    (DeviceFamily_ID == DeviceFamily_ID_MSP432E411Y)
#include <ti/devices/msp432e4/inc/msp432.h>
#include <third_party/CMSIS/Include/core_cm4.h>

#define CPU_CLOCK_HZ            ((uint32_t) 120000000)
#define CPU_CLOCK_MHZ           ((uint32_t) 120)

#else
#include DeviceFamily_constructPath(inc/hw_nvic.h)
#include DeviceFamily_constructPath(inc/hw_types.h)

#define CPU_CLOCK_HZ            ((uint32_t) 80000000)
#define CPU_CLOCK_MHZ           ((uint32_t) 80)
#endif

#include DeviceFamily_constructPath(driverlib/rom.h)
#include DeviceFamily_constructPath(driverlib/rom_map.h)
#include DeviceFamily_constructPath(driverlib/systick.h)

#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

typedef struct _ClockP_Obj {
    ClockP_Fxn cbFxn;
    uintptr_t cbArg;
    uint32_t startTimeout;  /* timeout passed to ClockP_create() */
    uint32_t timeout;
    uint32_t period;
    struct _ClockP_Obj *next;
} ClockP_Obj;

typedef struct _ClockP_Control {
    uint64_t count;
    ClockP_Obj *list;
} ClockP_Control;

ClockP_Control ClockP_ctrl;
uint32_t ClockP_tickPeriod = 1000;

void ClockP_startup(void);

static uint64_t getTimeUsec();
static void sleepTicks(uint32_t ticks);

static bool ClockP_initialized = false;
static uint32_t sysTickPeriod;


/*
 *  ======== ClockP_sysTickHandler ========
 */
void ClockP_sysTickHandler(void)
{
    /* increment the systick counter */
    ClockP_ctrl.count++;

    /*  check if the clock instance has expired */
    while ((ClockP_ctrl.list != NULL) &&
           (ClockP_ctrl.list->timeout == ClockP_ctrl.count)) {
        ClockP_Obj *temp = ClockP_ctrl.list->next;
        /* clock instance expired */
        ClockP_ctrl.list->timeout = 0;
        /* call appropriate callback function */
        (ClockP_ctrl.list->cbFxn)(ClockP_ctrl.list->cbArg);

        ClockP_ctrl.list = temp;
    }
}

/*
 *  ======== sysTickInit ========
 */
static void sysTickInit(void)
{
    /* SysTick Enabling */
    ClockP_ctrl.count = 0;
    ClockP_ctrl.list = NULL;

    if (ClockP_tickPeriod == 0) {
        /* Set to maximum value */
        MAP_SysTickPeriodSet(0x1000000);
    }
    else {
        MAP_SysTickPeriodSet(CPU_CLOCK_HZ / ClockP_tickPeriod);
    }

    /* Reload the counter by writing to it */
#if (DeviceFamily_ID == DeviceFamily_ID_MSP432E401Y) || \
    (DeviceFamily_ID == DeviceFamily_ID_MSP432E411Y)
    SysTick->VAL = 0;
#else
    HWREG(NVIC_ST_CURRENT) = 0;
#endif

    MAP_SysTickEnable();
    MAP_SysTickIntEnable();
}

/*
 *  ======== ClockP_add ========
 */
void ClockP_add(ClockP_Struct *handle, ClockP_Fxn clockFxn,
                uint32_t timeout, uintptr_t arg)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;

    /* populate the new clock instance */
    obj->cbFxn = clockFxn;
    obj->cbArg = arg;
    obj->timeout = 0;
    obj->period = 0;
    obj->startTimeout = timeout;
    obj->next = NULL;
}

/*
 *  ======== ClockP_construct ========
 */
ClockP_Handle ClockP_construct(ClockP_Struct *handle, ClockP_Fxn clockFxn,
                               uint32_t timeout, ClockP_Params *params)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;
    ClockP_Params defaultParams;

    ClockP_startup();

    if (handle != NULL) {
        /* initialise the params, if not already */
        if (params == NULL) {
            params = &defaultParams;
            ClockP_Params_init(&defaultParams);
        }

        /* populate the new clock instance */
        obj->cbFxn = clockFxn;
        obj->cbArg = params->arg;
        obj->timeout = 0;
        obj->period = params->period;
        obj->startTimeout = timeout;
        obj->next = NULL;

        if (params->startFlag) {
            ClockP_start((ClockP_Handle)obj);
        }
    }

    return ((ClockP_Handle)obj);

}

/*
 *  ======== ClockP_create ========
 */
ClockP_Handle ClockP_create(ClockP_Fxn clockFxn, uint32_t timeout,
                            ClockP_Params *params)
{
    ClockP_Handle handle;

    handle = (ClockP_Handle)malloc(sizeof(ClockP_Obj));

    /* ClockP_construct will check handle for NULL, no need here */
    handle = ClockP_construct((ClockP_Struct *)handle, clockFxn, timeout,
                              params);

    return (handle);
}

/*
 *  ======== ClockP_delete ========
 */
void ClockP_delete(ClockP_Handle handle)
{
    ClockP_destruct((ClockP_Struct *)handle);

    free(handle);
}

/*
 *  ======== ClockP_destruct ========
 */
void ClockP_destruct(ClockP_Struct *clk)
{
    ClockP_Obj *obj = (ClockP_Obj*) clk;
    ClockP_Obj *temp;
    uintptr_t   key;

    key = HwiP_disable();

    if (ClockP_ctrl.list == NULL) {
        HwiP_restore(key);

        return;
    }

    if (ClockP_ctrl.list == obj) {
        /* Obj is first on list */
        ClockP_ctrl.list = ClockP_ctrl.list->next;
    }
    else {
        /* Search through list for obj */
        temp = ClockP_ctrl.list;
        while ((temp->next != obj) && (temp->next != NULL)) {
            temp = temp->next;
        }

        if (temp->next == obj) {
            temp->next = obj->next;
        }
    }

    HwiP_restore(key);
}

/*
 *  ======== ClockP_doTick ========
 *  Used internally by PowerCC32XX nortos.  Called with interrupts disabled.
 */
void ClockP_doTick()
{
    ClockP_sysTickHandler();
}

/*
 *  ======== ClockP_getCpuFreq ========
 */
void ClockP_getCpuFreq(ClockP_FreqHz *freq)
{
    freq->lo = (uint32_t)CPU_CLOCK_HZ;
    freq->hi = 0;
}

/*
 *  ======== ClockP_getSystemTickPeriod ========
 */
uint32_t ClockP_getSystemTickPeriod()
{
    return (ClockP_tickPeriod);
}

/*
 *  ======== ClockP_getSystemTicks ========
 */
uint32_t ClockP_getSystemTicks()
{
    ClockP_startup();

    return ((uint32_t)ClockP_ctrl.count);
}

/*
 *  ======== ClockP_getTicksUntilInterrupt ========
 */
uint32_t ClockP_getTicksUntilInterrupt()
{
    uint32_t  ticks = 0xFFFFFFFF;
    uintptr_t key;

    key = HwiP_disable();

    if (ClockP_ctrl.list != NULL) {
        if (ClockP_ctrl.list->timeout <= ClockP_ctrl.count) {
            ticks = 0;
        }
        else {
            ticks = ClockP_ctrl.list->timeout - ClockP_ctrl.count;
        }
    }

    HwiP_restore(key);

    return (ticks);
}

/*
 *  ======== ClockP_getTimeout ========
 */
uint32_t ClockP_getTimeout(ClockP_Handle handle)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;

    if (obj->timeout > 0) {
        return (obj->timeout - ClockP_ctrl.count);
    }
    else {
        return (obj->startTimeout);
    }
}

/*
 *  ======== ClockP_isActive ========
 */
bool ClockP_isActive(ClockP_Handle handle)
{
    ClockP_Obj *obj = (ClockP_Obj *)handle;

    return (obj->timeout > 0);
}

/*
 *  ======== ClockP_Params_init ========
 */
void ClockP_Params_init(ClockP_Params *params)
{
    params->arg = (uintptr_t)0;
    params->startFlag = false;
    params->period = 0;
}

/*
 *  ======== ClockP_setTicks ========
 *  Used intenally by PowerCC32XX nortos.  Called with interrupts disabled.
 */
void ClockP_setTicks(uint32_t ticks)
{
    ClockP_ctrl.count = ticks;
}

/*
 *  ======== ClockP_setTimeout ========
 */
void ClockP_setTimeout(ClockP_Handle handle, uint32_t timeout)
{
    ClockP_Obj *obj = (ClockP_Obj*)handle;

    obj->startTimeout = timeout;
}

/*
 *  ======== ClockP_sleep ========
 */
void ClockP_sleep(uint32_t sec)
{
    uint64_t ticks = (uint64_t)sec * 1000000 / (uint64_t)ClockP_tickPeriod;

    ClockP_startup();

    sleepTicks((uint32_t)ticks);
}

/*
 *  ======== ClockP_start ========
 */
void ClockP_start(ClockP_Handle handle)
{
    ClockP_Obj *obj = (ClockP_Obj*)handle;
    uintptr_t   key;

    /* protect the context by disable the interrupt */
    key = HwiP_disable();

    /* in case the timer is active, restart it with the new timeout */
    if (obj->timeout > 0) {
        ClockP_stop(handle);
    }
    obj->timeout = ClockP_ctrl.count + obj->startTimeout;

    /* check if the linked list exists */
    if (ClockP_ctrl.list != NULL) {
        ClockP_Obj *temp = ClockP_ctrl.list;

        if (temp->timeout > obj->timeout) {
            obj->next = ClockP_ctrl.list;
            ClockP_ctrl.list = obj;
        }
        else {
            while (temp->next != NULL) {
                if ((temp->next)->timeout > obj->timeout) {
                    obj->next = temp->next;
                    temp->next = obj;
                    break;
                }
                else {
                    temp = temp->next;
                }
            }

            if (temp->next == NULL) {
                temp->next = obj;
                obj->next = NULL;
            }
        }
    }
    else {
        ClockP_ctrl.list = obj;
        obj->next = NULL;
    }

    /* Enable interrupts */
    HwiP_restore(key);
}

/*
 *  ======== ClockP_startup ========
 */
void ClockP_startup(void)
{
    if (!ClockP_initialized) {
        sysTickInit();

        sysTickPeriod = CPU_CLOCK_HZ / ClockP_tickPeriod;
        ClockP_initialized = true;
    }
}

/*
 *  ======== ClockP_staticObjectSize ========
 *  Internal function for testing that ClockP_Struct is large enough
 *  to hold ClockP object.
 */
size_t ClockP_staticObjectSize(void)
{
    return (sizeof(ClockP_Obj));
}

/*
 *  ======== ClockP_stop ========
 */
void ClockP_stop(ClockP_Handle handle)
{
    ClockP_Obj *obj = (ClockP_Obj*)handle;
    ClockP_Obj *temp;
    uintptr_t   key;

    /* protect the context by disable the interrupt */
    key = HwiP_disable();

    temp = ClockP_ctrl.list;

    if (!temp) {
        /* Enable the systick interrupt */
        HwiP_restore(key);
        return;
    }

    if (ClockP_ctrl.list == obj) {
        ClockP_ctrl.list = ClockP_ctrl.list->next;
        obj->next = NULL;
        obj->timeout = 0;
        /* Enable the systick interrupt */
        HwiP_restore(key);
        return;
    }

    while ((temp->next != obj) && (temp->next != NULL)) {
        temp = temp->next;
    }

    if (temp->next == obj) {
        temp->next = obj->next;
        obj->next = NULL;
        obj->timeout = 0;
    }

    /* Enable the systick interrupt */
    HwiP_restore(key);
}

/*
 *  ======== ClockP_usleep ========
 */
void ClockP_usleep(uint32_t usec)
{
    uint64_t curTime, endTime;
    uint32_t ticksToSleep;

    ClockP_startup();

    curTime = getTimeUsec();
    endTime = curTime + usec;

    if (usec >= ClockP_tickPeriod) {
        ticksToSleep = usec / ClockP_tickPeriod;
        sleepTicks(ticksToSleep);
    }

    curTime = getTimeUsec();
    while (curTime < endTime) {
        curTime = getTimeUsec();
    }
}

/*
 *  ======== getTimeUsec() ========
 *  Get the current time in microseconds.
 */
static uint64_t getTimeUsec()
{
    uint64_t       ts;
    unsigned long  ticks;
    uint64_t       count1;
    uint64_t       count2;

    do {
        count1 = ClockP_ctrl.count;
        ticks = MAP_SysTickValueGet();
        count2 = ClockP_ctrl.count;
    } while (count1 != count2);

    /* Get the current time in microseconds */
    ts = count1 * (uint64_t)ClockP_tickPeriod +
            (uint64_t)((sysTickPeriod - ticks) / CPU_CLOCK_MHZ);

    return (ts);
}

/*
 *  ======== sleepTicks ========
 *  Sleep for a given number of ClockP ticks.
 */
static void sleepTicks(uint32_t ticks)
{
    SemaphoreP_Struct semStruct;
    SemaphoreP_Handle sem;

    if (ticks > 0) {
        /*
         *  Construct a semaphore with 0 count that will never be posted.
         *  We will timeout pending on this semaphore.
         */
        sem = SemaphoreP_construct(&semStruct, 0, NULL);
        SemaphoreP_pend(sem, ticks);
        SemaphoreP_destruct(&semStruct);
    }
}
