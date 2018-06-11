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

#include "ota.h"
#include "ota_default.h"

ota_module g_ota_module;
ota_assist g_ota_assist;

void ota_register_module(ota_module* module)
{
    g_ota_module.func_init = module->func_init;
#ifndef USE_BOOTLOADER
    g_ota_module.func_set_reboot = module->func_set_reboot;
    g_ota_module.func_check_update_state = module->func_check_update_state;
#else
    g_ota_module.func_update_process = module->func_update_process;
    g_ota_module.func_jump_to_application = module->func_jump_to_application;
    g_ota_module.func_roll_back_image = module->func_roll_back_image;
#endif
}

void ota_register_assist(ota_assist* assist)
{
    g_ota_assist.func_printf = assist->func_printf;
    g_ota_assist.func_ota_read = assist->func_ota_read;
    g_ota_assist.func_ota_write = assist->func_ota_write;
}

int ota_init(void)
{
    if (g_ota_assist.func_printf == NULL
        || g_ota_assist.func_ota_read == NULL
        || g_ota_assist.func_ota_write == NULL)
    {
        OTA_LOG("must call ota_register_assist first");
        return OTA_ERRNO_INTERNEL;
    }

    if (g_ota_module.func_init == NULL)
    {
        g_ota_module.func_init = ota_default_init;
    }
    return g_ota_module.func_init();
}

#ifndef USE_BOOTLOADER

int ota_set_reboot(int32_t image_len, uint32_t image_crc, void (*func_reboot)(void))
{
    if (g_ota_module.func_set_reboot == NULL)
    {
        g_ota_module.func_set_reboot = ota_default_set_reboot;
    }
    return g_ota_module.func_set_reboot(image_len, image_crc, func_reboot);
}

int ota_check_update_state(ota_state* st)
{
    if (g_ota_module.func_check_update_state == NULL)
    {
        g_ota_module.func_check_update_state = ota_default_check_update_state;
    }
    return g_ota_module.func_check_update_state(st);
}

#else

int ota_update_process(void)
{
    if (g_ota_module.func_update_process == NULL)
    {
        g_ota_module.func_update_process = ota_default_update_process;
    }
    return g_ota_module.func_update_process();
}

int ota_jump_to_application(void)
{
    if (g_ota_module.func_jump_to_application == NULL)
    {
        g_ota_module.func_jump_to_application = ota_default_jump_to_application;
    }
    return g_ota_module.func_jump_to_application();
}

int ota_roll_back_image(void)
{
    if (g_ota_module.func_roll_back_image == NULL)
    {
        g_ota_module.func_roll_back_image = ota_default_roll_back_image;
    }
    return g_ota_module.func_roll_back_image();
}

#endif /* USE_BOOTLOADER */