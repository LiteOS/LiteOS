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

#ifndef __CHUNK_H__
#define __CHUNK_H__

#include <stddef.h>
#include <stdint.h>

#include <list.h>

#ifndef ALLOC_ALIGN_SIZE
#define ALLOC_ALIGN_SIZE        (4)
#endif

#define ALLOC_ALIGN_MASK        (ALLOC_ALIGN_SIZE - 1)

#define ALLOC_MIN_SIZE          (sizeof (chunk_t) - sizeof (ach_t))

typedef struct chunk
{
    struct chunk * prev;
    uintptr_t      size;        /* bit[0] means is_allocated */
    dlist_t        node;        /* used for free chunks only */
} fch_t, chunk_t;

typedef struct ach
{
    struct chunk * prev;
    uintptr_t      size;        /* bit[0] means is_allocated */
} ach_t;

/* inlines */

/**
 * __is_free - check if a chunk is free
 * @chunk: the given chunk
 *
 * return: true if the chunk is free, false if not
 */

static inline int __is_free (chunk_t * chunk)
{
    return (chunk->size & 1) == 0;
}

/**
 * __set_chunk_allocated - set a chunk as allocated
 * @chunk: the given chunk
 */

static inline void __set_chunk_allocated (chunk_t * chunk)
{
    chunk->size |= 1;
}

/**
 * __set_chunk_free - set a chunk as free
 * @chunk: the given chunk
 */

static inline void __set_chunk_free (chunk_t * chunk)
{
    chunk->size &= ~1;
}

/**
 * __get_next_chunk - get the next chunk
 * @chunk: the current chunk
 *
 * return: next chunk
 */

static inline chunk_t * __get_next_chunk (chunk_t * chunk)
{
    return (chunk_t *) (((char *) chunk) + (chunk->size & ~1));
}

/**
 * __get_prev_chunk - get the previous chunk
 * @chunk: the current chunk
 *
 * return: previous chunk
 */

static inline chunk_t * __get_prev_chunk (chunk_t * chunk)
{
    return chunk->prev;
}

/**
 * __get_mem_block - get the memory block in a chunk
 * @chunk: the chunk
 *
 * return: the memory block in the chunk
 */

static inline char * __get_mem_block (chunk_t * chunk)
{
    return (char *) chunk + sizeof (ach_t);
}

/**
 * __get_fch_from_mem - create a new chunk for a memory block
 * @mem: the memory block
 *
 * return: the new chunk containing the memory block
 */

static inline chunk_t * __get_fch_from_mem (char * mem)
{
    return &((chunk_t *) mem) [-1];
}

/**
 * __get_ach_from_mem - get the chunk of a allocated memory block
 * @mem: the memory block
 *
 * return: the chunk containing the memory block
 */

static inline ach_t * __get_ach_from_mem (char * mem)
{
    uintptr_t szorptr;

    /*
     * there are two possible cases here:
     *
     * 1) chunk->head is just behind the 'mem':
     *
     *    +-------------+--------------+
     *    | prev | size | memory block |
     *    +-------------+--------------+
     *
     * 2) after an aligned alloc, there may extra memory in the front but the
     *    size is less than sizeof (chunk_t), there will be a pointer in the
     *    'extra mem':
     *
     *    +-------------+-----------+----------------------+
     *    | prev | size | extra mem | aligned memory block |
     *    +-------------+-----------+----------------------+
     *
     *    please refer to the routine of <__carve_head> for this case
     *
     * for both cases, this routine just works fine
     */

    szorptr = ((uintptr_t *) mem) [-1];

    if (szorptr & 1)
    {

        /* case 1, chunk->size */

        return &((ach_t *) mem) [-1];
    }

    /* case 2, a pointer to the chunk */

    return (ach_t *) szorptr;
}

/**
 * __set_ach_to_mem - set chunk in the front of memory block
 * @mem: the memory block
 *
 * return: NA
 */

static inline void __set_ach_to_mem (char * mem, chunk_t * chunk)
{
    ((chunk_t **) mem) [-1] = chunk;
}

/**
 * __get_avail_size - get available memory size in a allocated memory block
 * @mem:   the given memory block, must be begining of the memory block of a chunk
 *
 * return: the size of the available memory size in the block
 */

static inline size_t __get_avail_size (char * mem)
{
    return (char *) __get_next_chunk ((chunk_t *) __get_ach_from_mem (mem)) - mem;
}

#endif  /* __CHUNK_H__ */

