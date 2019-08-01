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

#ifndef __LOS_COMMON_H__
#define __LOS_COMMON_H__

/*
 * offset_of - caculate the offset for a member in a struct
 * @t: the type of the container struct this is embedded in
 * @m: the name of the member within the struct
 */

#define offset_of(t, m)         ((uintptr_t) &((t *) 0)->m)

/*
 * container_of - cast a member of a structure out to the containing structure
 * @p: the pointer to the member
 * @t: the type of the container struct this is embedded in
 * @m: the name of the member within the struct
 */

#define container_of(p, t, m)   ((t *) (((char *) (p)) - (offset_of (t, m))))

/*
 * round_up - make a value to be aligned forward
 * @x: the value to be aligned
 * @a: the alignment value
 */

#define round_up(x, a)          (((uintptr_t) (x) + ((uintptr_t) (a) - 1)) & ~((uintptr_t) (a) - 1))

/*
 * round_down - make a value to be aligned backward
 * @x: the value to be aligned
 * @a: the alignment value
 */

#define round_down(x, a)        ((x) & ~((uintptr_t) (a) - 1))


#endif /* __LOS_COMMON_H__ */

