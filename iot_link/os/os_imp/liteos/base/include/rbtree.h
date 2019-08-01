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

#ifndef __RBTREE_H__
#define __RBTREE_H__

#include "__binary_tree.h"

/* defines */

#define RBTREE_RED      0
#define RBTREE_BLACK    1

#define RBTREE_DEBUG

/* typedefs */

typedef struct rb_node
{
    bi_node_t  bin;
    int        c;
} rb_node_t;

typedef struct rb_tree
{
    bi_tree_t  bit;

#ifdef RBTREE_DEBUG
    uint32_t   nodes;
#endif
} rb_tree_t;

/* inlines */

extern int         rb_insert   (rb_tree_t * t, rb_node_t * n);
extern void        rb_delete   (rb_tree_t * t, rb_node_t * n);
extern void        rb_init     (rb_tree_t * t,
                                int (* compare_nn) (bi_node_t *, bi_node_t *),
                                int (* compare_nk) (bi_node_t *, uintptr_t));
extern rb_node_t * rb_find_eq  (rb_tree_t * t, uintptr_t k);
extern rb_node_t * rb_find_ge  (rb_tree_t * t, uintptr_t k);
extern rb_node_t * rb_find_gt  (rb_tree_t * t, uintptr_t k);
extern rb_node_t * rb_find_le  (rb_tree_t * t, uintptr_t k);
extern rb_node_t * rb_find_lt  (rb_tree_t * t, uintptr_t k);
extern rb_node_t * rb_first    (rb_tree_t * t);
extern rb_node_t * rb_last     (rb_tree_t * t);
extern rb_node_t * rb_next     (rb_node_t * n);
extern rb_node_t * rb_prev     (rb_node_t * n);
extern rb_node_t * rb_node_get (rb_tree_t * t, uintptr_t k,
                                bi_node_t * (* create) (uintptr_t, uintptr_t),
                                uintptr_t arg);
extern void        rb_replace  (rb_tree_t * t, rb_node_t * o, rb_node_t * n);

#endif /* __RBTREE_H__ */

