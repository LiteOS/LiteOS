/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
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

#include <los_config.h>
#include <los_printf.h>

#if (LOSCFG_HEAP_IMPROVED == YES)

#include <string.h>

#include <heap.h>
#include <mem.h>

static inline int __put_chunk (heap_t * heap, chunk_t * chunk)
{
    if (__cm_put_chunk (&heap->cm, chunk) != 0)
    {
        return -1;
    }

#if (LOSCFG_MEM_STATISTICS == YES)
    __stat_chunk_add (&heap->stat, chunk);
#endif

    return 0;
}

static inline void __del_chunk (heap_t * heap, chunk_t * chunk)
{
    __cm_del_chunk (&heap->cm, chunk);

#if (LOSCFG_MEM_STATISTICS == YES)
    __stat_chunk_del (&heap->stat, chunk);
#endif
}

static inline chunk_t * __get_chunk (heap_t * heap, size_t size)
{
    chunk_t * chunk = __cm_get_chunk (&heap->cm, size);

    if (chunk != NULL)
    {
        __del_chunk (heap, chunk);
    }

    return chunk;
}

/**
 * heap_init - initialize a heap struct
 * @heap: the given heap
 *
 * return: 0 on success, negtive value on error
 */

int heap_init (heap_t * heap)
{
    UINT32 ret;

    if (heap == NULL)
    {
        return -1;
    }

    memset (heap, 0, sizeof (heap_t));

    ret = LOS_MuxCreate (&heap->mux);

    if (ret != LOS_OK)
    {
        return ret;
    }

    __cm_init (&heap->cm);

    return LOS_OK;
}

/**
 * heap_add - add a block of memory to a heap
 * @heap: the heap to add in
 * @buff: the memory address to add
 * @size: the memory block size
 *
 * return: 0 on success, negtive value on error
 */

int heap_add (heap_t * heap, char * buff, size_t size)
{
    int       ret;
    block_t * cb;
    chunk_t * chunk, * last;

    if ((heap == NULL) || (buff == NULL))
    {
        return -1;
    }

    size -= (uintptr_t) buff & ALLOC_ALIGN_MASK;
    size &= ~ALLOC_ALIGN_MASK;
    buff  = (char *) ((uintptr_t) buff & ~ALLOC_ALIGN_MASK);

    if (buff > (buff + size))
    {
        return -1;
    }

    if (size < MIN_BLOCK_SIZE)
    {
        return -1;
    }

    ret = LOS_MuxPend (heap->mux, LOS_WAIT_FOREVER);

    if (ret != LOS_OK)
    {
        return ret;
    }

    /*
     * this block of memory will be initialized as:
     *
     * +----+-----+-----+-----+
     * | CB | ACH | FCH | ACH |
     * +----+-----+-----+-----+
     *
     * CB  - chunk block
     * ACH - allocated chunk header
     * FCH - free chunk header
     *
     * These two ACH at the beginning and ending are guards.
     */

    cb           = (block_t *) (buff);
    cb->size     = size;

    cb->next     = heap->blocks;
    heap->blocks = cb;

    chunk        = (chunk_t *) (cb + 1);

    chunk->prev  = NULL;
    chunk->size  = sizeof (ach_t);
    __set_chunk_allocated (chunk);

    chunk        = __get_next_chunk (chunk);
    chunk->prev  = (chunk_t *) (cb + 1);
    chunk->size  = (size - sizeof (block_t) - 2 * sizeof (ach_t));

    if (__put_chunk (heap, chunk) != 0)
    {
        return -1;
    }

    last         = __get_next_chunk (chunk);

    last->prev   = chunk;
    last->size   = sizeof (ach_t);
    __set_chunk_allocated (last);

    (void) LOS_MuxPost (heap->mux);

    return 0;
}

/**
 * __carve_extra - round up a block of memory and carve the unused memory at
 *                 the beginning and ending
 * @heap:  the heap used
 * @chunk: the chunk which hold the memory block to round up and carve
 * @align: the alignment to round up
 *
 * return: the new aligned memory block
 */

static inline char * __carve_extra (heap_t * heap, chunk_t * chunk,
                                    size_t align, size_t size)
{
    char    * mem         = __get_mem_block (chunk);
    char    * aligned_mem = (char *) round_up (mem, align);
    ach_t   * ach         = (ach_t *) chunk;
    chunk_t * next;
    size_t    usable_size;

    if (aligned_mem != mem)
    {

        /*
         * carve at the begining (carve head)
         *
         *   .-chunk(FCH)-.
         *  /              \
         * +---+---+---+---+~~~+---+---+~~
         * | p | s |   l   | ~ | p | s |
         * | r | i |   i   | ~ | r | i |
         * | e | z |   s   | ~ | e | z |
         * | v | e |   t   | ~ | v | e |
         * +---+---+---+---+~~~+---+---+~~
         *         \           \       /\
         *          `-- mem     `-ach-' `-- aligned_mem
         *
         * now "mem != aligned_mem", so there must be "chunk < ach" so there are
         * memory space in front of ach (chunk ~ ach), if "(ach - chunk) <= chunk",
         * needless create new chunk, it is not worse to add logic save such memory
         * block, because for any method to save such memory block we must allocate
         * a chunk head for it, the chunk head itself waste more memory
         */

        if ((aligned_mem - (char *) chunk) <= (sizeof (chunk_t) + sizeof (ach_t)))
        {
            __set_ach_to_mem (aligned_mem, chunk);
        }
        else
        {
            ach = (ach_t *) (aligned_mem - sizeof (ach_t));

            ach->prev = chunk;
            ach->size = chunk->size - ((char *) ach - (char *) chunk);

            __get_next_chunk ((chunk_t *) ach)->prev = (chunk_t *) ach;

            chunk->size = (char *) ach - (char *) chunk;

            __put_chunk (heap, chunk);
        }
    }

    /*
     * carve at the ending (carve tail)
     *
     * there may be small memory gap (less than a chunk head size) betwen
     * __get_mem_block (chunk) and mem, see above "carve head" for details
     *
     *                     .-- total size is chunk->size
     *                    /
     *  ..................+....................
     * /                                       \
     * +-----+-----+----------------------------+------------+ ~ ...
     * | ACH | GAP |        usable memory       | next chunk | ~ ...
     * +-----+-----+----------------------------+------------+ ~ ...
     * \            \
     *  \            `-------- mem
     *   `-------------------- chunk
     *
     * so, the usable memory size is (chunk->size - (addr (mem) - (addr) chunk))
     *
     * we are going to carve extra memory block at the end of the usable memory
     */

    usable_size = ach->size - (aligned_mem - (char *) ach);

    /*
     * if the extra memory at the end is smaller than a chunk head size, do not
     * create new chunk
     */

    if ((usable_size - size) >= sizeof (chunk_t))
    {

        /*
         * there is extra memory at the end of usable memory greater than chunk size:
         *
         *                              .----- usable memory
         *                             /
         *                   .........+........
         *                  /                  \
         *                 /      .- size       \
         *                /      /               \
         *               .......+......           \
         *              /              \           \
         * +-----+-----+----------------+-----------+------------+ ~ ...
         * | ACH | GAP | wanted memory  | extra mem | next chunk | ~ ...
         * +-----+-----+----------------+-----------+------------+ ~ ...
         *  \           \                 \
         *   \           \                 `-- extra memory at the end
         *    \           `------------------- mem
         *     `------------------------------ ach
         *
         * ==>
         *
         * +-----+-----+----------------+-----+-----+------------+ ~ ...
         * | ACH | GAP | wanted memory  | FCH | mem | next chunk | ~ ...
         * +-----+-----+----------------+-----+-----+------------+ ~ ...
         *  \           \                \
         *   \           \                `--- new_chunk, will return to heap
         *    \           `------------------- mem, will return to user
         *     `------------------------------ ach
         */

        ach->size  -= (usable_size - size);

        chunk       = (chunk_t *) (aligned_mem + size);
        chunk->prev = (chunk_t *) ach;
        chunk->size = (usable_size - size);

        next        = __get_next_chunk (chunk);

        next->prev  = chunk;

        __put_chunk (heap, chunk);
    }

#if (LOSCFG_MEM_STATISTICS == YES)
    __stat_chunk_alloc (&heap->stat, (chunk_t *) ach);
#endif

    __set_chunk_allocated ((chunk_t *) ach);

    return aligned_mem;
}

/**
 * heap_alloc_align - allocate a block of memory from a heap with alignment
 * @heap:  the heap to allocate from
 * @bytes: size of memory in bytes to allocate
 * @align: the expected alignment value
 *
 * return: the allocated memory block or NULL if fail
 */

char * heap_alloc_align (heap_t * heap, size_t align, size_t bytes)
{
    chunk_t * chunk;
    char    * mem = NULL;

    if (heap == NULL)
    {
        return NULL;
    }

    /* align must be power of 2 */

    if (align & (align - 1))
    {
        return NULL;
    }

    bytes = bytes == 0 ? 1 : bytes;

    align = align < ALLOC_ALIGN_SIZE ? ALLOC_ALIGN_SIZE : align;

    if (bytes < ALLOC_MIN_SIZE)
    {
        bytes = ALLOC_MIN_SIZE;
    }

    bytes = round_up (bytes, ALLOC_ALIGN_SIZE);

    if (LOS_MuxPend (heap->mux, LOS_WAIT_FOREVER) != LOS_OK)
    {
        return NULL;
    }

    /* find a big enough memory chunk */

    chunk = __get_chunk (heap, bytes + sizeof (ach_t) + align - ALLOC_ALIGN_SIZE);

    if (chunk != NULL)
    {
        mem = __carve_extra (heap, chunk, align, bytes);
    }

    (void) LOS_MuxPost (heap->mux);

    return mem;
}

/**
 * heap_alloc - allocate a block of memory from a heap
 * @heap:  the heap to allocate from
 * @bytes: size of memory in bytes to allocate
 *
 * return: the allocated memory block or NULL if fail
 */

char * heap_alloc (heap_t * heap, size_t bytes)
{
    return heap_alloc_align (heap, ALLOC_ALIGN_SIZE, bytes);
}

/**
 * heap_free - free a block of memory
 * @heap:  the heap to allocate from
 *
 * return: 0 on sucess
 */

int heap_free (heap_t * heap, char * mem)
{
    chunk_t * chunk;
    chunk_t * prev_chunk;
    chunk_t * next_chunk;

    if (heap == NULL)
    {
        return -1;
    }

    /* ANSI C - free of NULL is OK */

    if (mem == NULL)
    {
        return 0;
    }

    chunk = (chunk_t *) __get_ach_from_mem (mem);

    if (LOS_MuxPend (heap->mux, LOS_WAIT_FOREVER) != LOS_OK)
    {
        return -1;
    }

    __set_chunk_free (chunk);

#if (LOSCFG_MEM_STATISTICS == YES)
    __stat_chunk_free (&heap->stat, chunk);
#endif

    prev_chunk = __get_prev_chunk (chunk);
    next_chunk = __get_next_chunk (chunk);

    if (__is_free (prev_chunk))
    {
        __del_chunk (heap, prev_chunk);
        prev_chunk->size += chunk->size;
        chunk = prev_chunk;
    }

    if (__is_free (next_chunk))
    {
        __del_chunk (heap, next_chunk);
        chunk->size += next_chunk->size;

        next_chunk = __get_next_chunk (chunk);
    }

    next_chunk->prev = chunk;

    __put_chunk (heap, chunk);

    (void) LOS_MuxPost (heap->mux);

    return 0;
}

/**
 * heap_realloc - realloc memory from a heap
 * @heap: the heap to allocate from
 * @ptr:  the original memory
 * @size: the new size
 *
 * return: the allocated memory block or NULL if fail
 */

char * heap_realloc (heap_t * heap, char * ptr, size_t size)
{
    char    * mem;
    size_t    usable_size;
    size_t    left_size;
    chunk_t * chunk;
    chunk_t * new;
    chunk_t * next;

    if (ptr == NULL)
    {
        return heap_alloc (heap, size);
    }

    if (size == 0)
    {
        heap_free (heap, ptr);
        return NULL;
    }

    if (size < ALLOC_MIN_SIZE)
    {
        size = ALLOC_MIN_SIZE;
    }
    else
    {
        size  = round_up (size, ALLOC_ALIGN);
    }

    chunk = (chunk_t *) __get_ach_from_mem (ptr);
    next  = __get_next_chunk (chunk);

    usable_size = ((char *) next) - ptr;

    if (usable_size < size)
    {
        mem = heap_alloc (heap, size);

        if (mem != NULL)
        {
            memcpy (mem, ptr, usable_size);
            heap_free (heap, ptr);
        }

        return mem;
    }

    /* memory in this chunk is enough, needless alloc new one */

    left_size = usable_size - size;

    if (left_size < sizeof (chunk_t))
    {
        return ptr;
    }

    /* extra memory in the end and can be released */

    new = (chunk_t *) ((char *) next - left_size);

    new->prev = chunk;

    if (LOS_MuxPend (heap->mux, LOS_WAIT_FOREVER) != LOS_OK)
    {
        return ptr;
    }

    chunk->size -= left_size;

    if (__is_free (next))
    {
        new->size = left_size + next->size;
        __del_chunk (heap, next);
        next = __get_next_chunk (new);
    }
    else
    {
        new->size = left_size;
    }

    next->prev = new;

    __put_chunk (heap, new);

    (void) LOS_MuxPost (heap->mux);

    return ptr;
}

#if (LOSCFG_MEM_STATISTICS == YES)
int heap_stat_get (heap_t * heap, mem_stat_t * stat)
    {
    if ((heap == NULL) || (stat == NULL))
        {
        return LOS_NOK;
        }

    if (LOS_MuxPend (heap->mux, LOS_WAIT_FOREVER) != LOS_OK)
        {
        return LOS_NOK;
        }

    *stat = heap->stat;

    stat->max_free_size = __get_max_free (&heap->cm);

    (void) LOS_MuxPost (heap->mux);

    return LOS_OK;
    }
#endif

static inline void __dump_block (block_t * block)
    {
    chunk_t * chunk = (chunk_t *) (block + 1);

    PRINTK ("\taddress    size       status\n\r");
    PRINTK ("\t---------- ---------- ---------\n\r");

    do
        {
        PRINTK ("\t%p 0x%08x %s", chunk, chunk->size & ~1,
                 (chunk->size & 1) == 0 ? "free\n\r" : "allocated\n\r");

        if ((chunk->prev != NULL) && (chunk->size == (sizeof (ach_t) | 1)))
            {
            break;
            }

        chunk = __get_next_chunk (chunk);
        } while (1);
    }

void __dump_heap (heap_t * heap, bool show_chunk)
    {
    block_t * block = heap->blocks;

    if (show_chunk)
        {
        PRINTK ("\r\n\tchunk information:\n\r\n\r");
        }

    if (LOS_MuxPend (heap->mux, LOS_WAIT_FOREVER) != LOS_OK)
        {
        return;
        }

    if (show_chunk)
        {
        while (block)
            {
            __dump_block (block);
            block = block->next;
            }
        }

#if (LOSCFG_MEM_STATISTICS == YES)
    PRINTK ("\n\r\tSUMMARY:\n\r\n\r");
    PRINTK ("\tstatus  bytes              blocks\n\r");
    PRINTK ("\t------  ------------------ ------\n\r");
    PRINTK ("\tcurrent:\n\r");
    PRINTK ("\tfree\t0x%-16x %d\n\r", heap->stat.free_size, heap->stat.free_chunks);
    PRINTK ("\talloc\t0x%-16x %d\n\r", heap->stat.busy_size, heap->stat.busy_chunks);
    PRINTK ("\tcumulative:\n\r");
    PRINTK ("\tfree\t0x%-16llx %lld\n\r", heap->stat.cum_size_freed, heap->stat.cum_freed);
    PRINTK ("\talloc\t0x%-16llx %lld\n\r", heap->stat.cum_size_allocated, heap->stat.cum_allocated);

    PRINTK ("\n\r\tmaximum allocated size ever: 0x%08x\n\r", heap->stat.max_busy_size);
    PRINTK ("\n\r\tmaximum free size: 0x%08x\n\r", __get_max_free (&heap->cm));
#endif

    LOS_MuxPost (heap->mux);
    }

#endif
