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
 *  2019-05-30 14:19  zhangqianfu  The first version
 *
 */
#ifndef LITEOS_LAB_IOT_LINK_OTA_BOOT_FLAG_BOOT_FLAG_H_
#define LITEOS_LAB_IOT_LINK_OTA_BOOT_FLAG_BOOT_FLAG_H_

typedef enum
{
    en_ota_upgrade_type_none = 0,
    en_ota_upgrade_type_full,
    en_ota_upgrade_type_diff,
}en_ota_upgrade_type;

typedef enum
{
    en_ota_upgrade_state_idle = 0,    ///< which means could be upgrade
    en_ota_upgrade_state_trigerring,  ///< which means has been triggered and need upgrade
    en_ota_upgrade_state_success,     ///< which means upgrade success
    en_ota_upgrade_state_failed,      ///< which means upgrade failed
}en_ota_upgrade_state;

int ota_boot_flag_trigger(en_ota_upgrade_type,int img_size);  ///< trans protocol use this to set to trigger state

///< the loader use this to set the success of failed
int ota_boot_flag_set_status(en_ota_upgrade_state state,int runimg_size, int bakimg_size,int newimg_size);

///< the  protocols use this to get the upgrade status
int ota_boot_flag_get_status(en_ota_upgrade_state *state);

///< the loader use this function to get the upgrade information
int ota_boot_flag_get(en_ota_upgrade_type *type,en_ota_upgrade_state *state,\
                     int *newimg_size,int *runimg_size, int *bakimg_size);

int ota_boot_flag_init();


#endif /* LITEOS_LAB_IOT_LINK_OTA_BOOT_FLAG_BOOT_FLAG_H_ */
