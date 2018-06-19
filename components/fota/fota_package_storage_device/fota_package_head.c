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

#include "fota_package_head.h"
#include "fota_firmware_writer.h"
#include "stddef.h"

#define MAKE_DWORD(a, b, c, d) ((((uint32_t)(a)) << 24) | (((uint32_t)(b)) << 16) | (((uint32_t)(c)) <<8) | ((uint32_t)(d)))
#define GET_DWORD(buf, pos) MAKE_DWORD(buf[pos], buf[pos + 1], buf[pos + 2], buf[pos + 3])

#define FOTA_PACK_HEADER_LEN_POS 0
#define FOTA_PACK_HEADER_MIN_LEN 8


void fota_pack_head_init(fota_pack_head_s *head)
{
    (void)memset(head, 0 , sizeof(*head));
}
void fota_pack_head_destroy(fota_pack_head_s *head)
{
    if(head->buff)
    {
        atiny_free(head->buff);
        head->buff = NULL;
    }
    head->stored_len = 0;
}

int fota_pack_head_parse(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff,
                                 uint16_t len, uint16_t *used_len)
{
    *used_len = 0;

    if(head->stored_len < head->head_len)
    {
        if(offset > head->stored_len)
        {
            FOTA_LOG("head not continuous, len %u offset %u", head->stored_len, offset);
            return FOTA_ERR;
        }
    }

    if(offset < head->head_len)
    {
        uint16_t copy_len;

        if(NULL == head->buff)
        {
            head->buff = atiny_malloc(head->head_len);
            if(NULL == head->buff)
            {
                FOTA_LOG("atiny_malloc fail");
                return FOTA_ERR;
            }
        }


        copy_len = MIN(len, head->head_len - offset);
        (void)memcpy(head->buff + offset, buff, copy_len);
        *used_len = copy_len;
        head->stored_len = offset + copy_len;
        if(head->stored_len >= head->head_len)
        {
            uint32_t save_len;
            save_len = GET_DWORD(head->buff, FOTA_PACK_HEADER_LEN_POS);
            if(save_len <= head->head_len)
            {
                FOTA_LOG("head len err, save len % head len %u", save_len, head->head_len);
                return FOTA_ERR;
            }

            if(head->hardware && head->hardware->get_max_size)
            {
                uint32_t max_len = head->hardware->get_max_size(head->hardware);
                if(max_len < save_len - head->head_len)
                {
                    FOTA_LOG("size exceed, save len % head len %u max_len %u", save_len, head->head_len, max_len);
                    return FOTA_ERR;
                }
            }

            if(head->update_check)
            {
                if(head->update_check(head->buff, head->head_len, head->param) != FOTA_OK)
                {
                    return FOTA_ERR;
                }
            }
        }
    }



    return FOTA_OK;
}
uint32_t fota_pack_head_get_crc(const fota_pack_head_s *head)
{
    return 0;
}
int fota_pack_head_check_len(const fota_pack_head_s *head, uint32_t len)
{
    uint32_t save_len;
    if(head->stored_len != head->head_len)
    {
        FOTA_LOG("head invalid get len, stored len %u, head len %u", head->stored_len, head->head_len);
        return FOTA_ERR;
    }

    save_len = GET_DWORD(head->buff, FOTA_PACK_HEADER_LEN_POS);
    if(len != save_len)
    {
        FOTA_LOG("len err save len %u, rcv len %u", save_len, len);
        return FOTA_ERR;
    }

    return FOTA_OK;
}
uint32_t fota_pack_head_get_head_len(const fota_pack_head_s *head)
{
    return head->head_len;
}

int fota_pack_head_set_head_info(fota_pack_head_s *head, uint16_t head_len, fota_hardware_s *hardware, head_update_check update_check, void *param)
{
    if(head_len < FOTA_PACK_HEADER_MIN_LEN)
    {
        FOTA_LOG("invalid len %u", head_len);
        return FOTA_ERR;
    }

    head->head_len = head_len;
    head->hardware = hardware;
    head->update_check = update_check;
    head->param = param;

    return FOTA_OK;
}



