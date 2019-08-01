/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
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

#include "los_config.h"

#if LOSCFG_ENABLE_MPU == YES

#include <los_mpu.h>

void mpu_init (void)
    {
    if (((MPU->TYPE & MPU_TYPE_NR_DREG_MASK) >> MPU_TYPE_NR_DREG_SHIFT) != 8)
        {
        return;
        }

    /*
     * the background map region attributes for privileged only:
     *
     * 0x00000000 ~ 0x1fffffff XRW WT       ROM/FLASH
     * 0x20000000 ~ 0x3fffffff XRW WB       SRAM
     * 0x40000000 ~ 0x5fffffff -RW device   on-chip peripheral
     * 0x60000000 ~ 0x7fffffff XRW WB       RAM
     * 0x80000000 ~ 0x9fffffff -RW WT       RAM
     * 0xa0000000 ~ 0xbfffffff -RW device   shared device
     * 0xc0000000 ~ 0xdfffffff -RW device   non-shared device
     * 0xe0000000 ~ 0xe00fffff -RW strongly PPB
     * 0xe0100000 ~ 0xffffffff -RW device   vendor system region
     *
     * refer to ValidateAddress, DefaultMemoryAttributes and DefaultPermissions
     * in DDI0419C and DDI0403E
     *
     * the default background map is just fine for privileged regions so only
     * use MPU entries for unprivileged tasks
     */

    MPU->CTRL = MPU_CTRL_PRIVDEFENA | MPU_CTRL_ENABLE;

    MPU->RBAR = 0x00000000 | MPU_RBAR_VALID | MPU_RBAR_REGION (0);
    MPU->RASR = MPU_ATTR_RW_R_ | MPU_ATTR_WT |
                MPU_ATTR_SZ (0x20000000) | MPU_ATTR_EN;

    MPU->RBAR = 0x40000000 | MPU_RBAR_VALID | MPU_RBAR_REGION (1);
    MPU->RASR = MPU_ATTR_RW_RW | MPU_ATTR_DEVICE | MPU_ATTR_XN |
                MPU_ATTR_SZ (0x20000000) | MPU_ATTR_EN;
    }

#endif

