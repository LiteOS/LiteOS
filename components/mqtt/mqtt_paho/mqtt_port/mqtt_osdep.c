/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "mqtt_osdep.h"
#include "osdepends/atiny_osdep.h"
#include "log/atiny_log.h"

#define get_time_ms atiny_gettime_ms

void TimerInit(Timer *timer)
{
    timer->end_time = get_time_ms();
}

char TimerIsExpired(Timer *timer)
{
    unsigned long long now = get_time_ms();
    return now >= timer->end_time;
}

void TimerCountdownMS(Timer *timer, unsigned int timeout)
{
    unsigned long long now = get_time_ms();
    timer->end_time = now + timeout;
}

void TimerCountdown(Timer *timer, unsigned int timeout)
{
    unsigned long long now = get_time_ms();
    timer->end_time = now + timeout * 1000;
}

int TimerLeftMS(Timer *timer)
{
    UINT64 now = get_time_ms();
    return timer->end_time <= now ? 0 : timer->end_time - now;
}

int MutexInit(Mutex* mutex)
{
    int ret = atiny_task_mutex_create(mutex);
    if (ret != LOS_OK)
    {
        ATINY_LOG(LOG_ERR, "create mutex fail, ret %d.", ret);
    }
    return ret;
}
int MutexLock(Mutex* mutex)
{
    int ret = atiny_task_mutex_lock(mutex);
    if (ret != LOS_OK)
    {
        ATINY_LOG(LOG_ERR, "lock mutex fail,mutex %d,ret %d.", mutex->mutex, ret);
    }
    return ret;
}

int MutexUnlock(Mutex* mutex)
{
    int ret = atiny_task_mutex_unlock(mutex);
    if (ret != LOS_OK)
    {
        ATINY_LOG(LOG_ERR, "unlock mutex fail,mutex %d,ret %d.", mutex->mutex, ret);
    }
    return ret;
}

void MutexDestory(Mutex* mutex)
{
    int ret = atiny_task_mutex_delete(mutex);
    if (ret != LOS_OK)
    {
        ATINY_LOG(LOG_ERR, "delete mutex fail,mutex %d,ret %d.", mutex->mutex, ret);
    }
}

int ThreadStart(Thread *thread, void (*fn)(void *), void *arg)
{
    (void)thread;
    (void)fn;
    (void)arg;
    return -1;
}



