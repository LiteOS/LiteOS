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

#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "los_common.h"

typedef struct bi_node
{
    struct bi_node     * p;     /* parent       */
    struct bi_node     * l;     /* left         */
    struct bi_node     * r;     /* right        */
} bi_node_t;

typedef struct bi_tree
{
    bi_node_t          * r;     /* root         */

    /*
     * compare two nodes or a node with a key to determine which is bigger,
     *
     * returning negative values means 'a < b' or 'n->k < k'
     * returning positive values means 'a > b' or 'n->k > k'
     * returning zero means 'a == b' or 'n->k == k'
     */

    int               (* compare_nn) (bi_node_t * a, bi_node_t * b);
    int               (* compare_nk) (bi_node_t * n, uintptr_t k);
} bi_tree_t;

/* defines */

/**
 * __bit_parent - get the parent for a node, assume n is not NULL
 * @n: the given node
 *
 * return: the parent of the node <n>
 */

static inline bi_node_t * __bit_parent (bi_node_t * n)
{
    return n->p;
}

/**
 * __bit_set_parent - set the parent for a node, assume n is not NULL
 * @n: the given node
 *
 * return: NA
 */

static inline void __bit_set_parent (bi_node_t * n, bi_node_t * p)
{
    n->p = p;
}

/**
 * __bit_raw_grand - get the grand parent for a node, assume n and p(n) are not NULL
 * @n: the given node
 *
 * return: the grand parent of the node <n>
 */

static inline bi_node_t * __bit_raw_grand (bi_node_t * n)
{
    return __bit_parent (__bit_parent (n));
}

/**
 * __bit_grand - get the grand parent for a node
 * @n: the given node
 *
 * return: the grand parent of the node <n>, NULL if n or p(n) is NULL
 */

static inline bi_node_t * __bit_grand (bi_node_t * n)
{
    bi_node_t * p;

    if ((n == NULL) || ((p = __bit_parent (n)) == NULL))
    {
        return NULL;
    }

    return __bit_parent (p);
}

/**
 * __bit_raw_uncle - get the uncle for a node, assume n, p(n) and g(n) are not NULL
 * @n: the given node
 *
 * return: the uncle of the node <n>
 */

static inline bi_node_t * __bit_raw_uncle (bi_node_t * n)
{
    bi_node_t * p = __bit_parent (n);
    bi_node_t * g = __bit_raw_grand (n);

    return p == g->l ? g->r : g->l;
}

/**
 * __bit_uncle - get the uncle for a node
 * @n: the given node
 *
 * return: the uncle of the node <n>, NULL if n, p(n) or g(n) is NULL
 */

static inline bi_node_t * __bit_uncle (bi_node_t * n)
{
    bi_node_t * p;
    bi_node_t * g;

    if (n == NULL)
    {
        return NULL;
    }

    if ((p = __bit_parent (n)) == NULL)
    {
        return NULL;
    }

    if ((g = __bit_parent (p)) == NULL)
    {
        return NULL;
    }

    return p == g->l ? g->r : g->l;
}

/**
 * __bit_successor - the successor node of a node
 * @n: the given node
 *
 * return: the successor of <n>
 */

static inline bi_node_t * __bit_successor (bi_node_t * n)
{
    bi_node_t * s = n->r;

    if (s != NULL)
    {
        while (s->l)
        {
            s = s->l;
        }

        return s;
    }

    while (n->p && (n == n->p->r))
    {
        n = n->p;
    }

    return n->p;
}

/**
 * __bit_predecessor - the predecessor node of a node
 * @n: the given node
 *
 * return: the predecessor of <n>
 */

static inline bi_node_t * __bit_predecessor (bi_node_t * n)
{
    bi_node_t * p = n->l;

    if (p != NULL)
    {
        while (p->r)
        {
            p = p->r;
        }

        return p;
    }

    while (n->p && (n == n->p->l))
    {
        n = n->p;
    }

    return n->p;
}

/**
 * __bit_find_eq - find a given key in a tree
 * @r: the given tree root
 * @k: the key value
 *
 * return: the node with key = <k>, or NULL if key not found.
 */

static inline bi_node_t * __bit_find_eq (bi_tree_t * t, uintptr_t k)
{
    bi_node_t * w = t->r;

    while (w != NULL)
    {
        int          c = t->compare_nk (w, k);

        if (c == 0)
        {
            break;
        }

        w = c < 0 ? w->r : w->l;
    }

    return w;
}

/**
 * __bit_find_cond_xt - find a node that is just greater or less than the given value
 * @t:    the given tree
 * @k:    the key value
 * @cond: the condition, '>' or '<'
 *
 * return: the nearest node with key greater or less than <k>, or NULL if key not found.
 */

#define __bit_find_cond_xt(t, k, cond)                  \
do {                                                    \
    bi_node_t * l = NULL; /* last one */                \
    bi_node_t * w = t->r;                               \
                                                        \
    while (w != NULL)                                   \
    {                                               \
        int     c = t->compare_nk (w, k);               \
                                                        \
        if (c cond 0)                                   \
        {                                           \
            l = w;                                      \
        }                                           \
                                                        \
        w = c < 0 ? w->r : w->l;                        \
    }                                               \
                                                        \
    return l;                                           \
} while (0)

/**
 * __bit_find_gt - find a node that is just greater to the given value
 * @t: the given tree
 * @k: the key value
 *
 * return: the nearest node with key > <k>, NULL or if key not found.
 */

static inline bi_node_t * __bit_find_gt (bi_tree_t * t, uintptr_t k)
{
    __bit_find_cond_xt (t, k, >);
}

/**
 * __bit_find_lt - find a node that is just less to the given value
 * @t: the given tree
 * @k: the key value
 *
 * return: the nearest node with key < <k>, NULL or if key not found.
 */

static inline bi_node_t * __bit_find_lt (bi_tree_t * t, uintptr_t k)
{
    __bit_find_cond_xt (t, k, <);
}

/**
 * __bit_first - find the first node of the tree
 * @t: the given tree
 *
 * return: the first node, or NULL if the tree is empty
 */

static inline bi_node_t * __bit_first (bi_tree_t * t)
{
    bi_node_t * n = t->r;

    if (n != NULL)
    {
        while (n->l != NULL)
        {
            n = n->l;
        }
    }

    return n;
}

/**
 * __bit_last - find the last node of the tree
 * @t: the given tree
 *
 * return: the last node, or NULL if the tree is empty
 */

static inline bi_node_t * __bit_last (bi_tree_t * t)
{
    bi_node_t * n = t->r;

    if (n != NULL)
    {
        while (n->r != NULL)
        {
            n = n->r;
        }
    }

    return n;
}

/**
 * __bit_find_cond_xe - find a node that is just greater equal or less equal than
 *                      the given value
 * @t:    the given tree
 * @k:    the key value
 * @cond: the condition, '<=' or '>='
 *
 * return: the nearest node with key greater equal or less equal than <k>, or NULL
 *         if key not found.
 */

#define __bit_find_cond_xe(t, k, cond)                  \
do {                                                    \
    bi_node_t * l = NULL; /* last one */                \
    bi_node_t * w = t->r;                               \
                                                        \
    while (w != NULL)                                   \
    {                                               \
        int     c = t->compare_nk (w, k);               \
                                                        \
        if (c == 0)                                     \
        {                                           \
            return w;                                   \
        }                                           \
                                                        \
        if (c cond 0)                                   \
        {                                           \
            l = w;                                      \
        }                                           \
                                                        \
        w = c < 0 ? w->r : w->l;                        \
    }                                               \
                                                        \
    return l;                                           \
} while (0)

/**
 * __bit_find_ge - find a node that is just greater or equal to the given value
 * @t: the given tree
 * @k: the key value
 *
 * return: the nearest node with key >= <k>, or NULL if key not found.
 */

static inline bi_node_t * __bit_find_ge (bi_tree_t * t, uintptr_t k)
{
    __bit_find_cond_xe (t, k, >=);
}

/**
 * __bit_find_le - find a node that is just less or equal to the given value
 * @t: the given tree
 * @k: the key value
 *
 * return: the nearest node with key <= <k>, of NULL if key not found.
 */

static inline bi_node_t * __bit_find_le (bi_tree_t * t, uintptr_t k)
{
    __bit_find_cond_xe (t, k, <=);
}

/**
 * __bit_node_get - find a existing node or create a new one for it
 * @r:      the given tree root
 * @k:      the key value
 * @new:    pointer to a bool value, set to true if node not found and created a
 *          new one
 * @create: the node creating routine, <k> is the seconde argument
 * @arg:    the first argument of the <create> routine
 *
 * return: the node with key = <k>, or NULL if node not found and allocate fail.
 */

static inline bi_node_t * __bit_node_get (bi_tree_t * t, uintptr_t k, bool * new,
                                          bi_node_t * (* create) (uintptr_t, uintptr_t),
                                          uintptr_t arg)
{
    bi_node_t *  w = t->r;      /* walker, iterator */
    bi_node_t *  l;             /* last one */
    bi_node_t ** p;
    int          c;

    if (w == NULL)
    {
        w = create (k, arg);

        w->p = w->l = w->r = NULL;
        t->r = w;

        *new = true;

        return w;
    }

    while (1)
    {
        c = t->compare_nk (w, k);

        if (c == 0)
        {
            break;
        }

        l = w;

        p = c < 0 ? &l->r : &l->l;
        w = *p;

        if (w == NULL)
        {
            w = create (k, arg);

            if (w != NULL)
            {
                w->p = l;
                w->l = w->r = NULL;

                *p   = w;

                *new = true;
            }

            break;
        }
    }

    return w;
}

/**
 * __bit_rotate - rotate a sub-tree from <f> to <t>
 * @T: the given tree
 * @n: the sub-tree
 * @f: the form-side
 * @t: the to-side
 *
 * return: NA
 *
 * see also: __bit_rotate_right, __bit_rotate_left
 */

#define __bit_rotate(T, n, f, t)                                        \
    do                                                                  \
    {                                                               \
        bi_node_t * a = n->f;                                           \
        bi_node_t * p = __bit_parent (n);                               \
                                                                        \
        if (a->t)   /* a->t is the 'z' in the comments of */            \
        {       /* __bit_rotate_right and __bit_rotate_left */      \
            __bit_set_parent (a->t, n);                                 \
        }                                                           \
                                                                        \
        __bit_set_parent (n, a);                                        \
        n->f = a->t;                                                    \
        a->t = n;                                                       \
                                                                        \
        __bit_set_parent (a, p);                                        \
                                                                        \
        if (p == NULL)                                                  \
        {                                                           \
            T->r = a;                                                   \
        }                                                           \
        else                                                            \
        {                                                           \
            if (n == p->l)                                              \
            {                                                       \
                p->l = a;                                               \
            }                                                       \
            else                                                        \
            {                                                       \
                p->r = a;                                               \
            }                                                       \
        }                                                           \
    } while (0)

/**
 * __bit_rotate_right - rotate a sub-tree right
 * @T: the given tree
 * @n: the sub-tree
 *
 *      n              a
 *     / \            / \
 *    a   x    ->    y   n
 *   / \                / \
 *  y   z              z   x
 *
 * return: NA
 */

static inline void __bit_rotate_right (bi_tree_t * t, bi_node_t * n)
{
    __bit_rotate (t, n, l, r);
}

/**
 * __bit_rotate_left - rotate a sub-tree left
 * @T: the given tree
 * @n: the sub-tree
 *
 *    n                  a
 *   / \                / \
 *  x   a      ->      n   y
 *     / \            / \
 *    z   y          x   z
 */

static inline void __bit_rotate_left (bi_tree_t * t, bi_node_t * n)
{
    __bit_rotate (t, n, r, l);
}

/**
 * __bit_insert - insert a new node into a tree
 * @t: the given tree
 * @n: the new inserted node
 *
 * return: 0 insert done, -1 insert fail
 */

static inline int __bit_insert (bi_tree_t * t, bi_node_t * n)
{
    bi_node_t * p = t->r;

    n->r = n->l = NULL;

    if (p == NULL)
    {
        n->p = (bi_node_t *) NULL;
        t->r = n;

        return 0;
    }

    while (1)
    {
        int c = t->compare_nn (n, p);

        if (c == 0)
        {
            return -1;
        }

        if (c > 0)
        {
            if (p->r == NULL)
            {
                p->r = n;
                break;
            }

            p = p->r;
        }
        else
        {
            if (p->l == NULL)
            {
                p->l = n;
                break;
            }

            p = p->l;
        }
    }

    n->p = p;

    return 0;
}

/**
 * __bit_for_each - iterate over a binary search tree from most left to most right
 * @t:   the given tree
 * @pfn: the callback function for each found node, when pfn return non-zero value
 *       the loop end
 *
 * return: NA
 */

static inline void __bit_for_each (bi_tree_t * t, int (* pfn) (bi_node_t *))
{
    bi_node_t * n = t->r;

    if (n == NULL)
    {
        return;
    }

    while (1)
    {
        while (n->l != NULL)
        {
            n = n->l;
        }

        while (1)
        {
            if (pfn (n))
            {
                return;
            }

            if (n->r != NULL)
            {
                n = n->r;
                break;
            }

            while (1)
            {
                if (n->p == NULL)
                {
                    return;
                }

                if (n == n->p->l)
                {
                    n = n->p;
                    break;
                }

                n = n->p;
            }
        }
    }
}

/**
 * __bit_init - init a bit-tree
 * @t: the given tree
 * @c: the comparison method
 * @k: the method to get the key of a node
 *
 * return: 0 on success, -1 on error
 */

static inline int __bit_init (bi_tree_t * t,
                              int (* compare_nn) (bi_node_t *, bi_node_t *),
                              int (* compare_nk) (bi_node_t *, uintptr_t))
{
    t->r = NULL;

    if (!compare_nn || !compare_nk)
    {
        return -1;
    }

    t->compare_nn = compare_nn;
    t->compare_nk = compare_nk;

    return 0;
}

#endif /* __BINARY_TREE_H__ */

