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

/* Define to prevent recursive inclusion ------------------------------------*/

#ifndef __JFFS2_OS_H__
#define __JFFS2_OS_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdint.h>
#include "los_typedef.h"
#include "los_event.h"


struct completion
{
    int reserve;
};

struct task_struct
{
    int reserve;
};

/*=============================================================================
                        Sched
=============================================================================*/

#define cond_resched() do { } while(0)
#define signal_pending(x) (0)

/*=============================================================================
                        Semaphore mutex
=============================================================================*/

typedef struct jffs2_mutex
{
    uint32_t mux;
}jffs2_mutex_t;

#define semaphore jffs2_mutex

#define DECLARE_MUTEX(x)            jffs2_mutex_t x = { 0xffffffff };
#define DECLARE_MUTEX_LOCKED(x)     jffs2_mutex_t x = { 0xffffffff };

#define INIT_MUTEX(x)               init_MUTEX(x)
#define DESTROY_MUTEX(x)            destroy_MUTEX(x)

#define init_MUTEX(sem)             jffs2_mutex_init((jffs2_mutex_t *)sem)
#define init_MUTEX_LOCKED(sem)      do { \
                                        if(0 == jffs2_mutex_init((jffs2_mutex_t *)sem)) \
                                            jffs2_mutex_lock((jffs2_mutex_t *)sem); \
                                    } while(0)
#define destroy_MUTEX(sem)          jffs2_mutex_release((jffs2_mutex_t *)sem)
#define down(sem)                   jffs2_mutex_lock((jffs2_mutex_t *)sem)
#define down_interruptible(sem)     (jffs2_mutex_lock((jffs2_mutex_t *)sem), 0)
#define down_trylock(sem)           jffs2_mutex_trylock((jffs2_mutex_t *)sem)
#define up(sem)                     jffs2_mutex_unlock((jffs2_mutex_t *)sem)

struct rw_semaphore
{
    uint32_t sem;
};

#define down_read(sem)
#define down_read_trylock(sem)
#define down_write(sem)
#define down_write_trylock(sem)
#define up_read(sem)
#define up_write(sem)
#define downgrade_write(sem)

int jffs2_mutex_init( jffs2_mutex_t *mutex );
int jffs2_mutex_lock( jffs2_mutex_t *mutex );
int jffs2_mutex_trylock( jffs2_mutex_t *mutex );
int jffs2_mutex_unlock( jffs2_mutex_t *mutex );
int jffs2_mutex_release( jffs2_mutex_t *mutex );

/*=============================================================================
                        Event
=============================================================================*/
#define jffs2_flag_t    EVENT_CB_S

int jffs2_event_init(jffs2_flag_t *event);
int jffs2_event_destroy(jffs2_flag_t *event);
/*=============================================================================
                        Thread
=============================================================================*/

typedef struct jffs2_thread
{
    uint32_t taskid;
}jffs2_thread_t;

typedef void (*jffs2_thread_fn)(void *arg);


int jffs2_thread_new(jffs2_thread_t *thread, char *name, jffs2_thread_fn function,
                        void *arg, int stacksize, int prio);
int jffs2_thread_delete(jffs2_thread_t *thread);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __JFFS2_OS_H__ */
