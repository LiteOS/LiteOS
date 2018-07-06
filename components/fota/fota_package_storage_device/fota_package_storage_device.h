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

/**@defgroup atiny_adapter Agenttiny Adapter
 * @ingroup agent
 */

#ifndef _FOTA_PACKAGE_STORAGE_DEVICE_H_
#define _FOTA_PACKAGE_STORAGE_DEVICE_H_

#include "atiny_fota_api.h"


typedef struct fota_hardware_api_tag_s
{
    uint32_t (*get_block_size)(struct fota_hardware_api_tag_s *thi, uint32_t offset);
    uint32_t (*get_max_size)(struct fota_hardware_api_tag_s *thi);
    int (*read_software)(struct fota_hardware_api_tag_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len);
}fota_hardware_s;

typedef struct
{
    atiny_fota_storage_device_s *storage_device;
    fota_hardware_s *hardware;
    void (*head_info_notify)(atiny_fota_storage_device_s *device, void *head_info, uint32_t info_len);
}fota_pack_device_info_s;


#if defined(__cplusplus)
extern "C" {
#endif

/**
 *@ingroup atiny_adapter
 *@brief get storage device.
 *
 *@par Description:
 *This API is used to get storage device.
 *@attention none.
 *
 *@param none.
 *
 *@retval #atiny_fota_storage_device_s *     storage device.
 *@par Dependency: none.
 *@see none
 */
atiny_fota_storage_device_s *fota_get_pack_device(void);
int fota_set_pack_device(atiny_fota_storage_device_s *device, fota_pack_device_info_s *device_info);

#if defined(__cplusplus)
}
#endif

#endif //_FOTA_PACKAGE_STORAGE_DEVICE_H_


