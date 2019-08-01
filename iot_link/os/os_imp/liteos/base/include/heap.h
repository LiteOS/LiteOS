/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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

#ifndef __HEAP_H__
#define __HEAP_H__

#include <los_config.h>

#include <stddef.h>
#include <stdbool.h>

#include <los_mux.h>

#include <los_slab.ph>

#include "chunk.h"

#if !defined (LOSCFG_CONFIG_CM_TLSF) && !defined (LOSCFG_CONFIG_CM_BESTFIT)
#define LOSCFG_CONFIG_CM_BESTFIT
#endif

#if   defined (LOSCFG_CONFIG_CM_TLSF)
#include "cm-tlsf.h"
#elif defined (LOSCFG_CONFIG_CM_BESTFIT)
#include "cm-bestfit.h"
#endif

#define MIN_BLOCK_SIZE          (sizeof (block_t) + sizeof (ach_t) * 2 + \
                                 sizeof (fch_t))

#ifndef ALLOC_ALIGN
#define ALLOC_ALIGN             8
#endif

typedef struct block
{
    struct block * next;        /* link into next block      */
    size_t         size;
} block_t;

#if (LOSCFG_MEM_STATISTICS == YES)
typedef struct mem_stat
{
    unsigned int       free_chunks;
    size_t             free_size;
    unsigned int       busy_chunks;
    size_t             busy_size;
    size_t             max_busy_size;
    size_t             max_free_size;
    unsigned long long cum_allocated;
    unsigned long long cum_size_allocated;
    unsigned long long cum_freed;
    unsigned long long cum_size_freed;
} mem_stat_t;
#endif

typedef struct heap
{
    chunk_mgr_t        cm;
    block_t          * blocks;
    UINT32             mux;
#if (LOSCFG_MEM_STATISTICS == YES)
    struct mem_stat    stat;
#endif
#if (LOSCFG_KERNEL_MEM_SLAB == YES)
    struct LOS_SLAB_CONTROL_HEADER stSlabCtrlHdr;
#endif
} heap_t;

/* inlines */

#if (LOSCFG_MEM_STATISTICS == YES)
static inline void __stat_chunk_add (mem_stat_t * stat, chunk_t * chunk)
{
    stat->free_chunks++;
    stat->free_size += (chunk->size - sizeof (ach_t));
}

static inline void __stat_chunk_del (mem_stat_t * stat, chunk_t * chunk)
{
    stat->free_chunks--;
    stat->free_size -= (chunk->size - sizeof (ach_t));
}

static inline void __stat_chunk_alloc (mem_stat_t * stat, chunk_t * chunk)
{
    stat->busy_chunks++;
    stat->busy_size += chunk->size;
    stat->cum_allocated++;
    stat->cum_size_allocated += chunk->size;

    if (stat->busy_size > stat->max_busy_size)
    {
        stat->max_busy_size = stat->busy_size;
    }
}

static inline void __stat_chunk_free (mem_stat_t * stat, chunk_t * chunk)
{
    stat->busy_chunks--;
    stat->busy_size -= chunk->size;
    stat->cum_freed++;
    stat->cum_size_freed += chunk->size;
}
#endif

/* externs */

extern char * heap_alloc_align (heap_t * heap, size_t align, size_t bytes);
extern char * heap_alloc       (heap_t * heap, size_t bytes);
extern int    heap_free        (heap_t * heap, char * mem);
extern char * heap_realloc     (heap_t * heap, char * ptr, size_t size);
extern int    heap_init        (heap_t * heap);
extern int    heap_add         (heap_t * heap, char * buff, size_t size);
extern void __dump_heap        (heap_t * heap, bool show_chunk);
#if (LOSCFG_MEM_STATISTICS == YES)
extern int    heap_stat_get    (heap_t * heap, mem_stat_t * stat);
#endif

#endif  /* __HEAP_H__ */

