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

#ifndef _FOTA_PACKAGE_HEAD_H_
#define _FOTA_PACKAGE_HEAD_H_
#include "fota_package_storage_device.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <atiny_adapter.h>
#include "fota_package_checksum.h"

#define FOTA_LOG(fmt, ...) \
(void)atiny_printf("[%s:%d][%lu]" fmt "\r\n",  __FUNCTION__, __LINE__, (uint32_t)atiny_gettime_ms(),  ##__VA_ARGS__)


#define ASSERT_THIS(do_something) \
        if(NULL == thi)\
        {\
            FOTA_LOG("this null pointer");\
            do_something;\
        }

#ifndef MIN
#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#endif

enum
{
    FOTA_OK,
    FOTA_ERR
};

struct fota_pack_head_tag_s;
typedef int (*head_update_check)(const uint8_t *head_buff , uint16_t len, void *param);


typedef struct fota_pack_head_tag_s
{
    fota_hardware_s *hardware;
    head_update_check update_check;
    void *param;

    uint8_t *buff;
    uint16_t stored_len;
    uint16_t head_len;
    fota_pack_checksum_s *checksum;
    uint8_t *checksum_pos;
    uint32_t checksum_len;
}fota_pack_head_s;

#if defined(__cplusplus)
extern "C" {
#endif


void fota_pack_head_init(fota_pack_head_s *head);
void fota_pack_head_destroy(fota_pack_head_s *head);
int fota_pack_head_parse(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff, uint16_t len,
                    uint16_t *used_len);
int fota_pack_head_check(const fota_pack_head_s *head, uint32_t len);
uint32_t fota_pack_head_get_head_len(const fota_pack_head_s *head);
const uint8_t* fota_pack_head_get_head_info(const fota_pack_head_s *head);

int fota_pack_head_set_head_info(fota_pack_head_s *head, fota_hardware_s *hardware,
                                            head_update_check updat_check, void *param);
fota_pack_checksum_s *fota_pack_head_get_checksum(fota_pack_head_s *head);


#if defined(__cplusplus)
}
#endif

#endif //_FOTA_PACKAGE_HEAD_H_


