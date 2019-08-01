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

#ifndef MQTT_OSDEP_H
#define MQTT_OSDEP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include  <osal.h>
#include  <mqtt_al.h>

#define MQTT_TASK 1


typedef osal_loop_timer_t  Timer;

#define TimerInit(timer)               osal_loop_timer_init(timer)
#define TimerIsExpired(timer)          osal_loop_timer_expired(timer)
#define TimerCountdownMS(timer,value)  osal_loop_timer_count_downms(timer,value)
#define TimerLeftMS(timer)             osal_loop_timer_left(timer)
#define TimerCountdown(timer,value)    osal_loop_timer_count_down(timer,value)

typedef osal_mutex_t  Mutex;
#define MutexInit(mutex)     ((true==osal_mutex_create(mutex))?0:-1)
#define MutexLock(mutex)     ((true==osal_mutex_lock(*mutex))?0:-1)
#define MutexUnlock(mutex)   ((true==osal_mutex_unlock(*mutex))?0:-1)
#define MutexDestory(mutex)  osal_mutex_del(*mutex)


typedef struct
{
	void * no_used;
} Thread;
int ThreadStart(Thread *thread, void (*fn)(void *), void *arg);


typedef struct Network
{
    void *ctx;                      ///< if it is tls, then it is tls context, else it is socket fd
    mqtt_al_security_para_t arg;
    int (*mqttread) (struct Network*, unsigned char*, int, int);
    int (*mqttwrite) (struct Network*, unsigned char*, int, int);
} Network;

#endif
