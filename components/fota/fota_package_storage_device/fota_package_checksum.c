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

#include "fota_package_checksum.h"
#include <string.h>
#include "fota_package_head.h"
#include "ota_sha256.h"

struct fota_pack_checksum_tag_s
{
    ota_sha256_context sha256_context;
    uint32_t offset;
    bool offset_flag;
    fota_pack_head_s *head;
};


static void fota_pack_checksum_init(fota_pack_checksum_s *thi)
{
    memset(thi, 0, sizeof(*thi));
}


fota_pack_checksum_s * fota_pack_checksum_create(fota_pack_checksum_type_e type)
{
    if(FOTA_PACK_SHA_S56 == type)
    {
        fota_pack_checksum_s *thi = atiny_malloc(sizeof(fota_pack_checksum_s));
        if(NULL == thi)
        {
            FOTA_LOG("atiny_malloc fail");
            return NULL;
        }
        fota_pack_checksum_init(thi);
        return thi;
    }
    FOTA_LOG("checksum type %d invalid", type);
    return NULL;
}

void fota_pack_checksum_delete(fota_pack_checksum_s *thi)
{
    if(NULL == thi)
    {
        return;
    }
    ota_sha256_free(&thi->sha256_context);
    atiny_free(thi);
}

static int fota_pack_checksum_init_head_data(fota_pack_checksum_s *thi)
{
    int len;
    const uint8_t *buff;

    ota_sha256_init(&thi->sha256_context);
    ota_sha256_starts(&thi->sha256_context, false);


    len = fota_pack_head_get_head_len(thi->head);
    if(0 == len)
    {
        return FOTA_OK;
    }

    buff = fota_pack_head_get_head_info(thi->head);
    if(NULL == buff)
    {
        FOTA_LOG("buff null");
        return FOTA_ERR;
    }

    ota_sha256_update(&thi->sha256_context, buff, len);

    return FOTA_OK;
}

int fota_pack_checksum_update_head(fota_pack_checksum_s *thi, fota_pack_head_s *head)
{
    ASSERT_THIS(return FOTA_ERR);

    thi->head = head;

    return fota_pack_checksum_init_head_data(thi);
}


static int fota_pack_checksum_restore_checksum(fota_pack_checksum_s *thi, uint32_t offset, fota_hardware_s *hardware)
{
    uint8_t *buff  = NULL;
    const uint32_t max_size = 4096;
    uint32_t total_size = 0;
    uint32_t left_size;
    uint32_t read_size;
    int ret = FOTA_ERR;

    buff = atiny_malloc(max_size);
    do
    {
        if(NULL ==buff)
        {
            FOTA_LOG("malloc null");
            break;
        }
        left_size = offset - total_size;
        read_size = MIN(left_size, max_size);
        ret = hardware->read_software(hardware, total_size, buff, read_size);
        if(ret != FOTA_OK)
        {
            FOTA_LOG("read_software fail, ret %d, offset %d, read_size %d", ret, total_size, read_size);
            break;
        }
        ota_sha256_update(&thi->sha256_context, buff, read_size);
        total_size += read_size;
    }while(total_size < offset);

    if(buff)
    {
        atiny_free(buff);
    }

    return FOTA_OK;
}

int fota_pack_checksum_update_data(fota_pack_checksum_s *thi, uint32_t offset, const uint8_t *buff,
                                            uint16_t len, fota_hardware_s *hardware)
{
    int ret;

    ASSERT_THIS(return FOTA_ERR);

    if(0 == len)
    {
        return FOTA_OK;
    }

    if(NULL == buff)
    {
        FOTA_LOG("buff null");
        return FOTA_ERR;
    }

    if(((thi->offset_flag) && (thi->offset == offset))
        || (fota_pack_head_get_head_len(thi->head) == offset))
    {
        ota_sha256_update(&thi->sha256_context, buff, len);
        thi->offset_flag = true;
        thi->offset = offset + len;
        return FOTA_OK;
    }

     if((NULL == hardware) || (NULL == hardware->read_software))
    {
        FOTA_LOG("hardware null");
        return FOTA_ERR;
    }

    ret = fota_pack_checksum_init_head_data(thi);
    if(ret != FOTA_OK)
    {
        return ret;
    }

    ret = fota_pack_checksum_restore_checksum(thi, offset, hardware);
    if(ret != FOTA_OK)
    {
        return ret;
    }
    ota_sha256_update(&thi->sha256_context, buff, len);

    thi->offset_flag = true;
    thi->offset = offset + len;
    return FOTA_OK;
}

int fota_pack_checksum_check(fota_pack_checksum_s *thi, const uint8_t *expected_value, uint16_t len)
{
    uint8_t real_value[32];

    ASSERT_THIS(return FOTA_ERR);

    if(sizeof(real_value) != len)
    {
        FOTA_LOG("len %d not the same", len);
        return FOTA_ERR;
    }
    ota_sha256_finish(&thi->sha256_context, real_value);
    if(memcmp(real_value, expected_value, len) != 0)
    {
        FOTA_LOG("checksum err");
        return FOTA_ERR;
    }
    return FOTA_OK;
}


