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

#include <stdbool.h>

#include <rbtree.h>

/* defines */

/* inlines */

/**
 * __color - get the color of a given node
 * @n: the given node
 *
 * return: the color of <n>
 */

static inline int __color (rb_node_t * n)
{
    return n->c;
}

/**
 * __raw_is_red - check if a node is red, assume <n> is not NULL
 * @n: the given node
 *
 * return: true if <n> is red, false if not.
 */

static inline int __raw_is_red (rb_node_t * n)
{
    return __color (n) == RBTREE_RED;
}

/**
 * __is_red - check if a node is red
 * @n: the given node
 *
 * return: true if <n> is red, false if not, or <n> is NULL.
 */

static inline int __is_red (rb_node_t * n)
{
    return (n != NULL) && __raw_is_red (n);
}

/**
 * __raw_is_black - check if a node is black, assume <n> is not NULL
 * @n: the given node
 *
 * return: true if <n> is red, false if not.
 */

static inline int __raw_is_black (rb_node_t * n)
{
    return __color (n) == RBTREE_BLACK;
}

/**
 * __is_black - check if a node is black
 * @n: the given node
 *
 * return: true if <n> is black or NULL, false otherwise.
 */

static inline int __is_black (rb_node_t * n)
{
    return (n == NULL) || __raw_is_black (n);
}

/**
 * __set_color - set a node's color
 * @n: the given node
 * @c: the new color
 *
 * return: NA
 */

static inline void __set_color (rb_node_t * n, int c)
{
    n->c = c;
}

/**
 * __set_red - set a node to red
 * @n: the given node
 *
 * return: NA
 */

static inline void __set_red (rb_node_t * n)
{
    __set_color (n, RBTREE_RED);
}

/**
 * __set_black - set a node to black
 * @n: the given node
 *
 * return: NA
 */

static inline void __set_black (rb_node_t * n)
{
    __set_color (n, RBTREE_BLACK);
}

/**
 * __parent - get the parent of a node
 * @n: the given node
 *
 * return: NA
 */

static inline rb_node_t * __parent (rb_node_t * n)
{
    return container_of (n->bin.p, rb_node_t, bin);
}

/**
 * __left - get the left child of a node
 * @n: the given node
 *
 * return: NA
 */

static inline rb_node_t * __left (rb_node_t * n)
{
    return container_of (n->bin.l, rb_node_t, bin);
}
/**
 * __right - get the right child of a node
 * @n: the given node
 *
 * return: NA
 */

static inline rb_node_t * __right (rb_node_t * n)
{
    return container_of (n->bin.r, rb_node_t, bin);
}

/**
 * __root - get the root of a tree
 * @n: the given tree
 *
 * return: NA
 */

static inline rb_node_t * __root (rb_tree_t * t)
{
    return container_of (t->bit.r, rb_node_t, bin);
}

/**
 * __set_parent - set the parent of a node
 * @n: the given node
 * @p: the new parent
 *
 * return: NA
 */

static inline void __set_parent (rb_node_t * n, rb_node_t * p)
{
    n->bin.p = &p->bin;
}

/**
 * __set_left - set the left child of a node
 * @n: the given node
 * @l: the new left child
 *
 * return: NA
 */

static inline void __set_left (rb_node_t * n, rb_node_t * l)
{
    n->bin.l = &l->bin;
}

/**
 * __set_right - set the right child of a node
 * @n: the given node
 * @r: the new right child
 *
 * return: NA
 */

static inline void __set_right (rb_node_t * n, rb_node_t * r)
{
    n->bin.r = &r->bin;
}

/**
 * __set_root - set the root of a tree
 * @n: the given tree
 * @r: the new root
 *
 * return: NA
 */

static inline void __set_root (rb_tree_t * t, rb_node_t * r)
{
    t->bit.r = &r->bin;
}

/**
 * __raw_grand - get the grand parent for a node, assume n and p(n) are not NULL
 * @n: the given node
 *
 * return: grand parent of the node <n>
 */

static inline rb_node_t * __raw_grand (rb_node_t * n)
{
    return container_of (__bit_raw_grand (&n->bin), rb_node_t, bin);
}

/**
 * __raw_uncle - get the uncle for a node, assume n, p(n) and g(n) are not NULL
 * @n: the given node
 *
 * return: uncle of the node <n>
 */

static inline rb_node_t * __raw_uncle (rb_node_t * n)
{
    return container_of (__bit_raw_uncle (&n->bin), rb_node_t, bin);
}

/**
 * __rotate_right - rotate the sub-tree right
 * @t: the given tree
 * @n: the given node (indcated the sub-tree)
 *
 * return: NA
 */

static inline void __rotate_right (rb_tree_t * t, rb_node_t * n)
{
    __bit_rotate_right (&t->bit, &n->bin);
}

/**
 * __rotate_left - rotate the sub-tree left
 * @t: the given tree
 * @n: the given node (indcated the sub-tree)
 *
 * return: NA
 */

static inline void __rotate_left (rb_tree_t * t, rb_node_t * n)
{
    __bit_rotate_left (&t->bit, &n->bin);
}

/**
 * __successor - the successor node of a node
 * @n: the given node
 *
 * return: the successor of <n>
 */

static inline rb_node_t * __successor (rb_node_t * n)
{
    return container_of (__bit_successor (&n->bin), rb_node_t, bin);
}

/**
 * __predecessor - the predecessor node of a node
 * @n: the given node
 *
 * return: the predecessor of <n>
 */

static inline rb_node_t * __predecessor (rb_node_t * n)
{
    return container_of (__bit_predecessor (&n->bin), rb_node_t, bin);
}

/**
 * rb_find_eq - find a given key in a rb-tree
 * @t: the given tree
 * @k: the key value
 *
 * return: the node with key = <k>, NULL or key not found.
 */

rb_node_t * rb_find_eq (rb_tree_t * t, uintptr_t k)
{
    return container_of (__bit_find_eq (&t->bit, k), rb_node_t, bin);
}

/**
 * rb_find_ge - find a node that is just greater or equal to the given value
 * @t: the given tree
 * @k: the key value
 *
 * return: the nearest node with key >= <k>, NULL or key not found.
 */

rb_node_t * rb_find_ge (rb_tree_t * t, uintptr_t k)
{
    return container_of (__bit_find_ge (&t->bit, k), rb_node_t, bin);
}

/**
 * rb_find_gt - find a node that is just greater to the given value
 * @t: the given tree
 * @k: the key value
 *
 * return: the nearest node with key > <k>, NULL or key not found.
 */

rb_node_t * rb_find_gt (rb_tree_t * t, uintptr_t k)
{
    return container_of (__bit_find_gt (&t->bit, k), rb_node_t, bin);
}

/**
 * rb_find_le - find a node that is just less or equal to the given value
 * @t: the given tree
 * @k: the key value
 *
 * return: the nearest node with key <= <k>, NULL or key not found.
 */

rb_node_t * rb_find_le (rb_tree_t * t, uintptr_t k)
{
    return container_of (__bit_find_le (&t->bit, k), rb_node_t, bin);
}

/**
 * rb_find_lt - find a node that is just less to the given value
 * @t: the given tree
 * @k: the key value
 *
 * return: the nearest node with key < <k>, NULL or key not found.
 */

rb_node_t * rb_find_lt (rb_tree_t * t, uintptr_t k)
{
    return container_of (__bit_find_lt (&t->bit, k), rb_node_t, bin);
}

/**
 * rb_first - find the first node of the tree
 * @t: the given tree
 *
 * return: the first node, or NULL if the tree is empty
 */

rb_node_t * rb_first (rb_tree_t * t)
{
    return container_of (__bit_first (&t->bit), rb_node_t, bin);
}

/**
 * rb_last - find the last node of the tree
 * @t: the given tree
 *
 * return: the last node, or NULL if the tree is empty
 */

rb_node_t * rb_last (rb_tree_t * t)
{
    return container_of (__bit_last (&t->bit), rb_node_t, bin);
}

/**
 * rb_next - find the next node
 * @t: the given node
 *
 * return: the next node, or NULL if the node is already last
 */

rb_node_t * rb_next (rb_node_t * n)
{
    return __successor (n);
}

/**
 * rb_prev - find the prevous node
 * @t: the given node
 *
 * return: the prevous node, or NULL if the node is already first
 */

rb_node_t * rb_prev (rb_node_t * n)
{
    return __predecessor (n);
}

/**
 * rb_replace - replace a new node in the same place
 * @t: the given tree
 * @o: the old node to be replaced
 * @n: the new node
 *
 * return: the nearest node with key < <k>, NULL or key not found.
 */

void rb_replace (rb_tree_t * t, rb_node_t * o, rb_node_t * n)
{
    rb_node_t * p, * l, * r;

    *n = *o;

    p = __parent (n);
    l = __left (n);
    r = __right (n);

    if (l)
    {
        __set_parent (l, n);
    }

    if (r)
    {
        __set_parent (r, n);
    }

    if (!p)
    {
        t->bit.r = &n->bin;
        return;
    }

    if (__left (p) == o)
    {
        __set_left (p, n);
    }
    else
    {
        __set_right (p, n);
    }
}

/**
 * __rb_insadj - re-balance the tree after insertion
 * @t: the given tree
 * @n: the new inserted node
 *
 * return: NA
 */

static void __rb_insadj (rb_tree_t * t, rb_node_t * n)
{
    rb_node_t * p, * g, * u;

    while (1)
    {
        p = __parent (n);

        if (p == NULL)
        {
            __set_black (n);
            return;
        }

        if (__raw_is_black (p))
        {
            return;
        }

        /*
         * when we reach here, the parent is red, so:
         * 1) the parent must not be root
         * 2) the grand father mast not be NULL
         * 3) the grand father must be black
         */

        g = __raw_grand (n);
        u = __raw_uncle (n);

        /* case 1, 2 and 3 should set grand parent as RED */

        __set_red (g);

        if (!__is_red (u))
        {
            break;
        }

        /* case 1: z's uncle y is red */

        __set_black (p);
        __set_black (u);

        n = g;
    }

    /* case 2 and case 3 */

    if (p == __left (g))
    {
        if (n == __right (p))
        {
            __rotate_left (t, p);
        }
        else
        {
            n = p;
        }

        __rotate_right (t, g);
    }
    else
    {
        if (n == __left (p))
        {
            __rotate_right (t, p);
        }
        else
        {
            n = p;
        }

        __rotate_left (t, g);
    }

    __set_black (n);
}

/**
 * rb_insert - insert a new node into a tree
 * @t: the given tree
 * @n: the new inserted node
 *
 * return: 0 insert done, -1 insert fail (for double insertion)
 */

int rb_insert (rb_tree_t * t, rb_node_t * n)
{
    __set_red (n);

    if (__bit_insert (&t->bit, &n->bin) < 0)
    {
        return -1;
    }

#ifdef RBTREE_DEBUG
    t->nodes++;
#endif

    __rb_insadj (t, n);

    return 0;
}

/**
 * rb_node_get - find a existing node or create a new one for it
 * @r:      the given tree root
 * @k:      the key value
 * @new:    pointer to a bool value, set to true if node not found and created a
 *          new one
 * @create: the node creating routine, <k> is the seconde argument
 * @arg:    the first argument of the <create> routine
 *
 * return: the node with key = <k>, or NULL if node not found and allocate fail.
 */

rb_node_t * rb_node_get (rb_tree_t * t, uintptr_t k,
                         bi_node_t * (* create) (uintptr_t, uintptr_t),
                         uintptr_t arg)
{
    rb_node_t * n;
    bool        new = false;

    n = container_of (__bit_node_get (&t->bit, k, &new, create, arg),
                      rb_node_t, bin);

    if (new)
    {
        __set_red (n);
        __rb_insadj (t, n);

#ifdef RBTREE_DEBUG
        t->nodes++;
#endif
    }

    return n;
}

/**
 * __rb_deladj - re-balance a tree after deleting a black leaf
 * @t: the given tree
 * @p: the parent node of the deleted leaf
 * @n: the double colored node
 *
 * return: NA
 */

static void __rb_deladj (rb_tree_t * t, rb_node_t * p, rb_node_t * n)
{

    /*
     * for a non-root black node, there must be a sibling for it, so must a parent
     * for it
     */

    while (__is_black (n) && (n != __root (t)))
    {
        rb_node_t * w;

        /*
         * n should be double colored, so black hight for it's sibling tree
         * must be great than 0, so it sibling must not be NULL
         */

        if (__left (p) == n)
        {
            w = __right (p);

            if (__raw_is_red (w))
            {

                /*
                 * case 1: n's sibling w is red
                 *
                 * w's child must be black and not NULL
                 */

                __set_black (w);
                __set_red (p);

                w = __left (w);

                __rotate_left (t, p);
            }

            /* now n is double black, and w is black */

            if (__is_black (__right (w)))
            {
                if (__is_black (__left (w)))
                {

                    /*
                     * case 2: n's sibling w is black, and both of w's children
                     *         are black
                     */

                    __set_red (w);
                    n = p;
                    p = __parent (p);

                    continue;
                }
                else
                {

                    /*
                     * case 3: n's sibling w is black, w's left child is red,
                     *         and w's right child is black
                     */

                    __rotate_right (t, w);
                    w = __parent (w);

                    /* fall thru to case 4 */
                }
            }

            /*
             * case 4: n's sibling w is black, and w's right child is red
             */

            __set_color (w, __color (p));
            __set_black (p);
            __set_black (__right (w));
            __rotate_left (t, p);
        }
        else
        {
            w = __left (p);

            if (__raw_is_red (w))
            {

                /*
                 * case 1: n's sibling w is red
                 *
                 * w's child must be black and not NULL
                 */

                __set_black (w);
                __set_red (p);

                w = __right (w);

                __rotate_right (t, p);
            }

            /* now n is double black, and w is black */

            if (__is_black (__left (w)))
            {
                if (__is_black (__right (w)))
                {

                    /*
                     * case 2: n's sibling w is black, and both of w's children
                     *         are black
                     */

                    __set_red (w);
                    n = p;
                    p = __parent (p);

                    continue;
                }
                else
                {

                    /*
                     * case 3: n's sibling w is black, w's left child is red,
                     *         and w's right child is black
                     */

                    __rotate_left (t, w);
                    w = __parent (w);

                    /* fall thru to case 4 */
                }
            }

            /*
             * case 4: n's sibling w is black, and w's right child is red
             */

            __set_color (w, __color (p));
            __set_black (p);
            __set_black (__left (w));
            __rotate_right (t, p);
        }

        n = __root (t);
    }

    if (n)
    {
        __set_black (n);
    }
}

/**
 * rb_delete - delete a node from a tree
 * @t: the given tree
 * @n: the node to be deleted
 *
 * return: NA
 */

void rb_delete (rb_tree_t * t, rb_node_t * n)
{
    rb_node_t * p;
    rb_node_t * c;
    rb_node_t * o = n;
    bool        two_child = false;
    bool        adj;

    if ((t == NULL) || (n == NULL))
    {
        return;
    }

    /* find the real 'n' to be deleted, and its child */

    if (__right (n) == NULL)
    {
        c = __left (n);
    }
    else if (__left (n) == NULL)
    {
        c = __right (n);
    }
    else
    {
        two_child = true;

        n = __successor (n);

        /* n->left must be NIL, so n's child must be right (also may be NIL) */

        c = __right (n);
    }

    p   = __parent (n);
    adj = __is_black (n);

    if ((c != NULL) && (p != o))
    {
        __set_parent (c, p);
    }

    if (p == NULL)
    {
        __set_root (t, c);
    }
    else if (__left (p) == n)
    {
        __set_left (p, c);      /* set even p == o */
    }
    else
    {
        __set_right (p, c);     /* set even p == o */
    }

    if (two_child)
    {
        rb_node_t * op = __parent (o);

        /* 'n' will take the place of the old node 'o' */

        *n = *o;

        if (op != NULL)
        {
            if (__left (op) == o)
            {
                __set_left (op, n);
            }
            else
            {
                __set_right (op, n);
            }
        }
        else
        {
            __set_root (t, n);
        }

        __set_parent (__left (n), n);

        /*
         * __right (n) will be NULL when:
         *
         *     p == o
         *     n->r == NULL
         *
         *    op
         *    /\
         *   ?  o (p)
         *     / \
         *    ?   n
         *       / \
         *      NIL NIL (c)
         */

        if (__right (n) != NULL)
        {
            __set_parent (__right (n), n);
        }

        if (p == o)
        {
            p = n;
        }
    }

    if (adj)
    {
        __rb_deladj (t, p, c);
    }

#ifdef RBTREE_DEBUG
    t->nodes--;
#endif
}

/**
 * rb_init - init a rb-tree
 * @t: the given tree
 * @c: the comparison method
 * @k: the method to get the key of a node
 *
 * return: NA
 */

void rb_init (rb_tree_t * t,
              int (* compare_nn) (bi_node_t *, bi_node_t *),
              int (* compare_nk) (bi_node_t *, uintptr_t))
{
    __bit_init (&t->bit, compare_nn, compare_nk);

#ifdef RBTREE_DEBUG
    t->nodes = 0;
#endif
}

