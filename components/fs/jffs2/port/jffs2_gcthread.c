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
/*
 * JFFS2 -- Journalling Flash File System, Version 2.
 *
 * Copyright (C) 2001-2003 Red Hat, Inc.
 *
 * Created by David Woodhouse <dwmw2@redhat.com>
 *
 * For licensing information, see the file 'LICENCE' in this directory.
 *
 */

#include <jffs2_kernel.h>
#include "jffs2_nodelist.h"
#include "jffs2_os.h"

#if defined(CONFIG_JFFS2_GC_THREAD)

#define GC_THREAD_FLAG_TRIG         0x01
#define GC_THREAD_FLAG_STOP         0x02
#define GC_THREAD_FLAG_HAS_EXIT     0x04

uint32_t jffs2_current_time(void)
{
    return 0;
}

static void jffs2_garbage_collect_thread(void *data);

void jffs2_garbage_collect_trigger(struct jffs2_sb_info *c)
{
    struct super_block *sb = OFNI_BS_2SFFJ(c);

    /* Wake up the thread */
    D1(printk("jffs2_garbage_collect_trigger\n"));

    LOS_EventWrite(&sb->s_gc_thread_flags, GC_THREAD_FLAG_TRIG);
}

void jffs2_start_garbage_collect_thread(struct jffs2_sb_info *c)
{
    struct super_block *sb = NULL;
    int result;

    if(c == NULL) return;
    sb = OFNI_BS_2SFFJ(c);

    jffs2_event_init(&sb->s_gc_thread_flags);
    jffs2_mutex_init(&sb->s_lock);

    D1(printk("jffs2_start_garbage_collect_thread\n"));
    /* Start the thread. Doesn't matter if it fails -- it's only an
     * optimisation anyway */
    result =  jffs2_thread_new(&sb->s_gc_thread,
                             "jffs2_gc_thread",
                             (jffs2_thread_fn)jffs2_garbage_collect_thread,
                             (void *)c,
                             CONFIG_JFFS2_GC_THREAD_STACK_SIZE,
                             CONFIG_JFFS2_GC_THREAD_PRIORITY
                            );
    if (result != 0)
    {
        /* how to deal with the following filed? */
        jffs2_mutex_release(&sb->s_lock);
        jffs2_event_destroy(&sb->s_gc_thread_flags);
    }
}

void jffs2_stop_garbage_collect_thread(struct jffs2_sb_info *c)
{
    struct super_block *sb = OFNI_BS_2SFFJ(c);

    D1(printk("jffs2_stop_garbage_collect_thread\n"));
    /* Stop the thread and wait for it if necessary */

    LOS_EventWrite(&sb->s_gc_thread_flags, GC_THREAD_FLAG_STOP);

    D1(printk("jffs2_stop_garbage_collect_thread wait\n"));

    LOS_EventRead(&sb->s_gc_thread_flags,
                  GC_THREAD_FLAG_HAS_EXIT,
                  LOS_WAITMODE_OR | LOS_WAITMODE_CLR,
                  LOS_WAIT_FOREVER);

    // Kill and free the resources ...  this is safe due to the flag
    // from the thread.
    jffs2_thread_delete(&sb->s_gc_thread);
    jffs2_mutex_release(&sb->s_lock);
    jffs2_event_destroy(&sb->s_gc_thread_flags);
}


static void jffs2_garbage_collect_thread(void *data)
{
    struct jffs2_sb_info *c = (struct jffs2_sb_info *)data;
    struct super_block *sb;
    UINT32 flag = 0;

    if(c == NULL) return;
    sb = OFNI_BS_2SFFJ(c);

    D1(printk("jffs2_garbage_collect_thread START\n"));

    while(1)
    {
        flag = LOS_EventRead(&sb->s_gc_thread_flags,
                      GC_THREAD_FLAG_TRIG | GC_THREAD_FLAG_STOP,
                      LOS_WAITMODE_OR | LOS_WAITMODE_CLR,
                      jffs2_current_time() + CONFIG_JFFS2_GS_THREAD_TICKS);

        if (flag & GC_THREAD_FLAG_STOP)
            break;

        D1(printk("jffs2: GC THREAD GC BEGIN\n"));

        // syscmode todo start
        if (jffs2_garbage_collect_pass(c) == -ENOSPC)
        {
            printf("No space for garbage collection. "
                   "Aborting JFFS2 GC thread\n");
            break;
        }
        // syscmode todo end
        D1(printk("jffs2: GC THREAD GC END\n"));
    }

    D1(printk("jffs2_garbage_collect_thread EXIT\n"));

    LOS_EventWrite(&sb->s_gc_thread_flags, GC_THREAD_FLAG_HAS_EXIT);
}

#endif

