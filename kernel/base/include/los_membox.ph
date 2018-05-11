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

#ifndef _LOS_MEMBOX_PH
#define _LOS_MEMBOX_PH

#include "los_membox.h"

#define OS_MEMBOX_NEXT(addr, uwBlkSize) (LOS_MEMBOX_NODE *)((UINT8 *)(addr) + (uwBlkSize))

#ifdef LOS_MEMBOX_CHECK
#define OS_MEMBOX_MAGIC 0xa55a5aa5
#define OS_MEMBOX_SET_MAGIC(addr) *((UINT32 *)(addr)) = OS_MEMBOX_MAGIC
#define OS_MEMBOX_CHECK_MAGIC(addr) ((*((UINT32 *)(addr)) == OS_MEMBOX_MAGIC) ? LOS_OK : LOS_NOK)
#else
#define OS_MEMBOX_SET_MAGIC(addr)
#define OS_MEMBOX_CHECK_MAGIC(addr) LOS_OK
#endif

#define OS_MEMBOX_USER_ADDR(addr) ((VOID *)((UINT8 *)(addr) + LOS_MEMBOX_MAGIC_SIZE))
#define OS_MEMBOX_NODE_ADDR(addr) ((LOS_MEMBOX_NODE *)((UINT8 *)(addr) - LOS_MEMBOX_MAGIC_SIZE))


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MEMBOX_PH */
