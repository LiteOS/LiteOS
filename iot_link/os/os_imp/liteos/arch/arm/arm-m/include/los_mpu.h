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

#ifndef _LOS_MPU_H
#define _LOS_MPU_H

#if LOSCFG_ENABLE_MPU == YES

#include <los_compiler.h>
#include <los_typedef.h>

#define MPU_TYPE_NR_DREG_SHIFT  (8u)
#define MPU_TYPE_NR_DREG_MASK   (0xffu << MPU_TYPE_NR_DREG_SHIFT)

#define MPU_CTRL_PRIVDEFENA     (1u << 2)
#define MPU_CTRL_HFNMIENA       (1u << 1)
#define MPU_CTRL_ENABLE         (1u << 0)

#define MPU_RBAR_REGION(n)      ((n) & 0xf)
#define MPU_RBAR_VALID          (1u << 4)

#define MPU_ATTR_XN             (1u << 28)

/*      attr macro defination                 privileged unprivileged */

#define MPU_ATTR______          (0u << 24) /* none       none         */
#define MPU_ATTR_RW___          (1u << 24) /* RW         none         */
#define MPU_ATTR_RW_R_          (2u << 24) /* RW         R            */
#define MPU_ATTR_RW_RW          (3u << 24) /* RW         RW           */
#define MPU_ATTR_R____          (5u << 24) /* R          none         */
#define MPU_ATTR_R__R_          (6u << 24) /* R          R            */

#define MPU_ATTR_STRONGLY       (4u << 16) /* strongly always s, so set S */
#define MPU_ATTR_DEVICE         (5u << 16) /* device always s, so set S */
#define MPU_ATTR_WT             (2u << 16)
#define MPU_ATTR_WB             (3u << 16)

#define MPU_ATTR_SZ(s)          ((32 - CLZ (s - 1) - 1) << 1)

#define MPU_ATTR_EN             (1u << 00)

#define MPU_NR_USR_ENTRIES      5
#define MPU_FIRST_USR_REGION    2

#define LOS_DO_PRIVILEDGED(expr0, expr1...)                     \
    do                                                          \
    {                                                           \
        UINT32 __control_flags = osEnterPrivileged();           \
        expr0 ## expr1;                                         \
        osExitPrivileged(__control_flags);                      \
    } while (0)

typedef struct
{
    UINT32 uwRegionAddr;
    UINT32 uwRegionAttr;
} LOS_MPU_ENTRY;

typedef struct
{
    UINT32 uwRegionAddr;
    UINT32 uwRegionSize;
    UINT32 uwRegionAttr;
} LOS_MPU_PARA;

extern UINT32 osEnterPrivileged (void);
extern void   osExitPrivileged  (UINT32);
#else
#define LOS_DO_PRIVILEDGED(expr0, expr1...)                     \
    do                                                          \
    {                                                           \
        expr0 ## expr1;                                         \
    } while (0)
#endif

#endif

