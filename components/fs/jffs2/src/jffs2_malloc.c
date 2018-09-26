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
 * Copyright (C) 2001-2003 Free Software Foundation, Inc.
 *
 * Created by David Woodhouse <dwmw2@cambridge.redhat.com>
 *
 * For licensing information, see the file 'LICENCE' in this directory.
 *
 */

#include <jffs2_kernel.h>
#include "jffs2_os.h"
#include "jffs2_nodelist.h"
#include "jffs2_slab.h"

#if !defined(JFFS2_RAW_NODE_REF_CACHE_POOL_SIZE)
# define JFFS2_RAW_NODE_REF_CACHE_POOL_SIZE 0
#endif

struct jffs2_full_dirent *jffs2_alloc_full_dirent(int namesize)
{
    return (struct jffs2_full_dirent *)vmalloc(sizeof(struct jffs2_full_dirent) + namesize);
}

void jffs2_free_full_dirent(struct jffs2_full_dirent *x)
{
    vfree(x);
}

struct jffs2_full_dnode *jffs2_alloc_full_dnode(void)
{
    return (struct jffs2_full_dnode *)vmalloc(sizeof(struct jffs2_full_dnode));
}

void jffs2_free_full_dnode(struct jffs2_full_dnode *x)
{
    vfree(x);
}

struct jffs2_raw_dirent *jffs2_alloc_raw_dirent(void)
{
    return (struct jffs2_raw_dirent *)vmalloc(sizeof(struct jffs2_raw_dirent));
}

void jffs2_free_raw_dirent(struct jffs2_raw_dirent *x)
{
    vfree(x);
}

struct jffs2_raw_inode *jffs2_alloc_raw_inode(void)
{
    return (struct jffs2_raw_inode *)vmalloc(sizeof(struct jffs2_raw_inode));
}

void jffs2_free_raw_inode(struct jffs2_raw_inode *x)
{
    vfree(x);
}

struct jffs2_tmp_dnode_info *jffs2_alloc_tmp_dnode_info(void)
{
    return (struct jffs2_tmp_dnode_info *)vmalloc(sizeof(struct jffs2_tmp_dnode_info));
}

void jffs2_free_tmp_dnode_info(struct jffs2_tmp_dnode_info *x)
{
    vfree(x);
}

struct jffs2_node_frag *jffs2_alloc_node_frag(void)
{
    return (struct jffs2_node_frag *)vmalloc(sizeof(struct jffs2_node_frag));
}

void jffs2_free_node_frag(struct jffs2_node_frag *x)
{
    vfree(x);
}

#if JFFS2_RAW_NODE_REF_CACHE_POOL_SIZE == 0

int jffs2_create_slab_caches(void)
{
    return 0;
}

void jffs2_destroy_slab_caches(void)
{
}

struct jffs2_raw_node_ref *jffs2_alloc_raw_node_ref(void)
{
    return (struct jffs2_raw_node_ref *)vmalloc(sizeof(struct jffs2_raw_node_ref));
}

void jffs2_free_raw_node_ref(struct jffs2_raw_node_ref *x)
{
    vfree(x);
}

#else // JFFS2_RAW_NODE_REF_CACHE_POOL_SIZE == 0

static struct jffs2_raw_node_ref
rnr_pool[JFFS2_RAW_NODE_REF_CACHE_POOL_SIZE] __attribute__ ((aligned (4))),
         * first = NULL;
static jffs2_mutex_t mutex;

int jffs2_create_slab_caches(void)
{
    struct jffs2_raw_node_ref *p;

    jffs2_mutex_init(&mutex);
    for (p = rnr_pool; p < rnr_pool + JFFS2_RAW_NODE_REF_CACHE_POOL_SIZE - 1; p++)
    {
        p->next_phys = p + 1;
    }
    rnr_pool[JFFS2_RAW_NODE_REF_CACHE_POOL_SIZE - 1].next_phys = NULL;
    first = &rnr_pool[0];
    return 0;
}

void jffs2_destroy_slab_caches(void)
{
}

struct jffs2_raw_node_ref *jffs2_alloc_raw_node_ref(void)
{
    struct jffs2_raw_node_ref *p;

    jffs2_mutex_lock(&mutex);
    p = first;
    if (p != NULL)
        first = p->next_phys;
    jffs2_mutex_unlock(&mutex);
    return p;
}

void jffs2_free_raw_node_ref(struct jffs2_raw_node_ref *x)
{
    jffs2_mutex_lock(&mutex);
    x->next_phys = first;
    first = x;
    jffs2_mutex_unlock(&mutex);
}

#endif // JFFS2_RAW_NODE_REF_CACHE_POOL_SIZE == 0

struct jffs2_inode_cache *jffs2_alloc_inode_cache(void)
{
    struct jffs2_inode_cache *ret = (struct jffs2_inode_cache *)vmalloc(sizeof(struct jffs2_inode_cache));
    D1(printk(KERN_DEBUG "Allocated inocache at %p\n", ret));
    return ret;
}

void jffs2_free_inode_cache(struct jffs2_inode_cache *x)
{
    D1(printk(KERN_DEBUG "Freeing inocache at %p\n", x));
    vfree(x);
}

