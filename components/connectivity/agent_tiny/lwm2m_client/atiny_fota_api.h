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

#ifndef _ATINY_FOTA_API_H_
#define _ATINY_FOTA_API_H_
#include <stdbool.h>
#include <stdint.h>


struct atiny_fota_storage_device_tag_s;
typedef struct atiny_fota_storage_device_tag_s atiny_fota_storage_device_s;

typedef int atiny_fota_state_e;
enum //atiny_fota_state_e
{
    ATINY_FOTA_IDLE = 0,
    ATINY_FOTA_DOWNLOADING,
    ATINY_FOTA_DOWNLOADED,
    ATINY_FOTA_UPDATING
};

typedef enum
{
    ATINY_FOTA_DOWNLOAD_OK,
    ATINY_FOTA_DOWNLOAD_FAIL
}atiny_download_result_e;
struct atiny_fota_storage_device_tag_s
{
    int (*write_software)(atiny_fota_storage_device_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len);
    int (*write_software_end)(atiny_fota_storage_device_s *thi, atiny_download_result_e result, uint32_t total_len);
    int (*active_software)(atiny_fota_storage_device_s *thi);
    int (*get_software_result)(atiny_fota_storage_device_s *thi);
    int (*write_update_info)(atiny_fota_storage_device_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len);
    int (*read_update_info)(atiny_fota_storage_device_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len);
};



#if defined(__cplusplus)
extern "C" {
#endif


#if defined(__cplusplus)
}
#endif

#endif //_ATINY_FOTA_API_H_


