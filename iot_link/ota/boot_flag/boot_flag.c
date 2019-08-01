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
/**
 *  DATE                AUTHOR      INSTRUCTION
 *  2019-05-30 14:20  zhangqianfu  The first version
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <ota_storage.h>
#include <boot_flag.h>

typedef struct
{
    en_ota_upgrade_type upgrade_type;
    en_ota_upgrade_state upgrade_state;
    uint32_t   runimg_size;
    uint32_t   bakimg_size;
    uint32_t   newimg_size;
    uint32_t   crc;
}ota_boot_flag_t;

static ota_boot_flag_t s_ota_boot_flag;

static int ota_boot_flag_save()
{
    s_ota_boot_flag.crc = calc_crc32(0, &s_ota_boot_flag,sizeof(s_ota_boot_flag) - sizeof(uint32_t));

    return ota_write_msg(en_ota_storage_msg_type_bootflag,&s_ota_boot_flag,sizeof(s_ota_boot_flag),0);
}

int ota_boot_flag_init()
{
    int ret = -1;

    ota_storage_device *device;

    device = ota_storage_get_info();

    ret = ota_read_msg(en_ota_storage_msg_type_bootflag,&s_ota_boot_flag,sizeof(s_ota_boot_flag),0);

    if(ret == 0)
    {
        if(s_ota_boot_flag.crc != calc_crc32(0,&s_ota_boot_flag,sizeof(s_ota_boot_flag)-sizeof(uint32_t)))
        {
            s_ota_boot_flag.upgrade_type = en_ota_upgrade_type_none;
            s_ota_boot_flag.upgrade_state = en_ota_upgrade_state_idle;
            s_ota_boot_flag.newimg_size = 0;
            s_ota_boot_flag.bakimg_size = 0;

            device = ota_storage_get_info();
            if(NULL != device)
            {
                s_ota_boot_flag.runimg_size = device->run_img_max_size;
            }
            ota_boot_flag_save();
        }
    }

    return ret;
}



int ota_boot_flag_trigger(en_ota_upgrade_type type,int newimg_size)
{
    s_ota_boot_flag.newimg_size = newimg_size;
    s_ota_boot_flag.upgrade_state = en_ota_upgrade_state_trigerring;
    s_ota_boot_flag.upgrade_type = type;

    return ota_boot_flag_save();
}

///< the loader use this to set the success of failed
int ota_boot_flag_set_status(en_ota_upgrade_state state,int runimg_size, int bakimg_size,int newimg_size)
{
    s_ota_boot_flag.upgrade_type = en_ota_upgrade_type_none;
    s_ota_boot_flag.upgrade_state = state;
    s_ota_boot_flag.runimg_size = runimg_size;
    s_ota_boot_flag.newimg_size = newimg_size;
    s_ota_boot_flag.bakimg_size = bakimg_size;

    return ota_boot_flag_save();
}

///< the  protocols use this to get the upgrade status
en_ota_upgrade_state ota_boot_flag_get_status(void)
{
   return s_ota_boot_flag.upgrade_state;
}

int ota_boot_flag_get(en_ota_upgrade_type *type,en_ota_upgrade_state *state,\
                     int *newimg_size,int *runimg_size, int *bakimg_size)
{
    if(NULL != type)
    {
        *type = s_ota_boot_flag.upgrade_type;
    }
    if(NULL != state)
    {
        *type = s_ota_boot_flag.upgrade_state;
    }
    if(NULL != newimg_size)
    {
        *type = s_ota_boot_flag.newimg_size;
    }
    if(NULL != runimg_size)
    {
        *type = s_ota_boot_flag.runimg_size;
    }
    if(NULL != bakimg_size)
    {
        *type = s_ota_boot_flag.bakimg_size;
    }

    return 0;
}
