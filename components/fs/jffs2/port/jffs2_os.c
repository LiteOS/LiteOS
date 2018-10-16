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
#include "jffs2_os.h"
#include "jffs2_debug.h"
#include "los_mux.h"
#include "los_task.h"

#ifndef JFFS2_INVALID_VALUE
#define JFFS2_INVALID_VALUE     0xffff
#endif

#ifndef JFFS2_P_ASSERT
#define JFFS2_P_ASSERT(m)   \
    if(m == NULL)               \
    {                           \
        JFFS2_ERROR("Invalid param!\n");\
        return -1; \
    }
#endif

int jffs2_mutex_init( jffs2_mutex_t *mutex )
{
    UINT32 uwRet;

    JFFS2_P_ASSERT(mutex);
    uwRet = LOS_MuxCreate((UINT32 *)&mutex->mux);
    if(uwRet != LOS_OK)
    {
        JFFS2_ERROR("LOS_MuxCreate failed! ret = %x\n", uwRet);
        return -1;
    }
    return 0;
}

int jffs2_mutex_lock( jffs2_mutex_t *mutex )
{
    UINT32 uwRet;

    JFFS2_P_ASSERT(mutex);
    uwRet = LOS_MuxPend(mutex->mux, LOS_WAIT_FOREVER);
    if(uwRet != LOS_OK)
    {
        JFFS2_ERROR("LOS_MuxPend failed! ret = %x\n", uwRet);
        return -1;
    }
    return 0;
}

int jffs2_mutex_trylock( jffs2_mutex_t *mutex )
{
    UINT32 uwRet;

    JFFS2_P_ASSERT(mutex);
    uwRet = LOS_MuxPend(mutex->mux, LOS_NO_WAIT);
    if(uwRet != LOS_OK)
    {
        return 0;
    }
    return 1;
}

int jffs2_mutex_unlock( jffs2_mutex_t *mutex )
{
    UINT32 uwRet;

    JFFS2_P_ASSERT(mutex);
    uwRet = LOS_MuxPost(mutex->mux);
    if(uwRet != LOS_OK)
    {
        JFFS2_ERROR("LOS_MuxPost failed! ret = %x\n", uwRet);
        return -1;
    }
    return 0;
}

int jffs2_mutex_release( jffs2_mutex_t *mutex )
{
    UINT32 uwRet;

    JFFS2_P_ASSERT(mutex);
    uwRet = LOS_MuxDelete(mutex->mux);
    if(uwRet != LOS_OK)
    {
        JFFS2_ERROR("LOS_MuxDelete failed!, ret = %x\n", uwRet);
        return -1;
    }
    mutex->mux = JFFS2_INVALID_VALUE;
    return 0;
}


int jffs2_event_init(jffs2_flag_t *event)
{
    UINT32 uwRet;

    JFFS2_P_ASSERT(event);
    uwRet = LOS_EventInit(event);
    if(uwRet != LOS_OK)
    {
        JFFS2_ERROR("LOS_EventInit failed!, ret = %x\n", uwRet);
        return -1;
    }
    return 0;
}

int jffs2_event_destroy(jffs2_flag_t *event)
{
    UINT32 uwRet;

    JFFS2_P_ASSERT(event);
    uwRet = LOS_EventDestory(event);
    if(uwRet != LOS_OK)
    {
        JFFS2_ERROR("LOS_EventDestory failed!, ret = %x\n", uwRet);
        return -1;
    }
    return 0;
}

int jffs2_thread_new(jffs2_thread_t *thread, char *name, jffs2_thread_fn function,
                        void *arg, int stacksize, int prio)
{
    TSK_INIT_PARAM_S task;
    UINT32 ret;

    if(thread == NULL || function == NULL)
        return -1;

    memset(&task, 0, sizeof(task));

    /* Create host Task */
    task.pfnTaskEntry = (TSK_ENTRY_FUNC)function;
    task.uwStackSize  = stacksize;
    task.pcName = (char *)name;
    task.usTaskPrio = prio;
    task.uwArg = (UINT32)arg;
    ret = LOS_TaskCreate((UINT32 *)&thread->taskid, &task);

    if (LOS_OK != ret )
    {
        JFFS2_ERROR("LOS_TaskCreate error %u\n", (unsigned int)ret);
        return  -1;
    }

    return 0;
}

int jffs2_thread_delete(jffs2_thread_t *thread)
{
    UINT32 ret;

    JFFS2_P_ASSERT(thread);
    ret = LOS_TaskDelete(thread->taskid);
    if (LOS_OK != ret )
    {
        JFFS2_ERROR("LOS_TaskDelete error %u\n", ret);
        return  -1;
    }
    thread->taskid = JFFS2_INVALID_VALUE;
    return 0;
}

