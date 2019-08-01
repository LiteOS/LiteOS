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

#ifndef __LIST_H__
#define __LIST_H__

/* double list */

typedef struct dlist
    {
    struct dlist * prev, * next;
    } dlist_t;

/* macros */

/**
 * DLIST_INIT - create a empty list head body
 */

#define DLIST_INIT(h)           { &(h), &(h) }

/**
 * DLIST_DEF - define a empty list head
 */

#define DLIST_DEF(name)         dlist_t name = DLIST_INIT (name)

/* inlines */

/* list operations */

static inline void dlist_init (dlist_t * head)
    {
    head->prev = head->next = head;
    }

/**
 * __dlist_add_at - insert a new entry between two known consecutive entries
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */

static inline void __dlist_add_at (dlist_t * prev, dlist_t * next, dlist_t * new)
    {
    new->prev  = prev;
    new->next  = next;
    prev->next = new;
    next->prev = new;
    }

/**
 * dlist_add - add a new entry at the front of the list
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head
 * This is good for implementing stacks
 */

static inline void dlist_add (dlist_t * head, dlist_t * new)
    {
    __dlist_add_at (head, head->next, new);
    }

/**
 * dlist_add_tail - add a new entry at the tail of the list
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head
 * This is useful for implementing queues
 */

static inline void dlist_add_tail (dlist_t * head, dlist_t * new)
    {
    __dlist_add_at (head->prev, head, new);
    }

/**
 * dlist_del - deletes entry from list.
 * @entry: the element to delete from the list
 * Note: dlist_empty() on entry does not return true after this, the entry is
 * in an undefined state
 */

static inline void dlist_del (dlist_t * node)
    {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    }

/**
 * dlist_empty - check if a list is empty
 * @head: the list head to check
 */

static inline int dlist_empty (dlist_t * head)
    {
    return head->prev == head;
    }

/**
 * __dlist_foreach - iterate over a list
 * @pos:    the &dlist_t to use as a loop cursor
 * @head:   the head for your list
 * @dir:    iterate direction, next for forward, prev for backward
 */

#define __dlist_foreach(pos, head, dir)                     \
    for (pos = (head)->dir; pos != (head); pos = pos->dir)

/**
 * dlist_foreach - iterate over a list
 * @pos:    the &dlist_t to use as a loop cursor
 * @head:   the head for your list
 */

#define dlist_foreach(pos, head)                            \
    __dlist_foreach (pos, head, next)

/**
 * dlist_foreach_backward - iterate over a list backwards
 * @pos:    the &dlist_t to use as a loop cursor
 * @head:   the head for your list
 */

#define dlist_foreach_backward(pos, head)                   \
    __dlist_foreach (pos, head, prev)

/**
 * __list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:        the &struct list_head to use as a loop cursor
 * @n:          another &struct list_head to use as temporary storage
 * @head:       the head for your list
 * @dir:    iterate direction, next for forward, prev for backward
 */

#define __dlist_foreach_safe(pos, n, head, dir)             \
    for (pos = (head)->dir, n = pos->dir; pos != (head);    \
         pos = n, n = n->dir)

/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:        the &struct list_head to use as a loop cursor
 * @n:          another &struct list_head to use as temporary storage
 * @head:       the head for your list
 */

#define dlist_foreach_safe(pos, n, head)                    \
    __dlist_foreach_safe (pos, n, head, next)

/**
 * list_for_each_safe_backward - iterate over a list safe against removal
 *                               of list entry backwards
 * @pos:        the &struct list_head to use as a loop cursor
 * @n:          another &struct list_head to use as temporary storage
 * @head:       the head for your list
 */

#define dlist_foreach_safe_backward(pos, n, head)           \
    __dlist_foreach_safe (pos, n, head, prev)

#endif /* __LIST_H__ */

