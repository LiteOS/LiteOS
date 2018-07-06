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
#define MAKE_WORD(a, b)  ((((uint32_t)(a)) <<8) | ((uint32_t)(b)))

#define GET_DWORD(buf, pos) MAKE_DWORD(buf[pos], buf[pos + 1], buf[pos + 2], buf[pos + 3])
#define GET_WORD(buf, pos) MAKE_WORD(buf[pos], buf[pos + 1])


#define FOTA_PACK_HEADER_MIN_LEN 12
#define FOTA_TLV_START_POS FOTA_PACK_HEADER_MIN_LEN

#define FOTA_PACK_VERSION_POS 0
#define FOTA_PACK_HEADER_HEAD_LEN_POS 4
#define FOTA_PACK_HEADER_TOTAL_LEN_POS 8

#define FOTA_PACK_TLV_T_LEN 2
#define FOTA_PACK_TLV_L_LEN 2

#define FOTA_PACK_TLV_T_SHA256 1

#define VERSION_NO 0




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

    if(head->checksum_pos)
    {
        atiny_free(head->checksum_pos);
        head->checksum_pos = NULL;
    }

    if(head->checksum)
    {
        fota_pack_checksum_delete(head->checksum);
        head->checksum = NULL;
    }

    memset(&(head->buff), 0, sizeof(*head) - (((uint8_t *)&(head->buff)) - ((uint8_t *)head)));
}

int fota_pack_head_parse_head_len(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff,
                                 uint16_t len, uint16_t *used_len)
{
    *used_len = 0;
    if(offset < FOTA_PACK_HEADER_MIN_LEN)
    {
        uint32_t copy_len;
        uint32_t head_len;
        uint32_t total_len;
        uint32_t version;
        if(offset > head->stored_len)
        {
            FOTA_LOG("head not continuous, len %u offset %u", head->stored_len, offset);
            return FOTA_ERR;
        }

        if(NULL == head->buff)
        {
            head->buff = atiny_malloc(FOTA_PACK_HEADER_MIN_LEN);
            if(NULL == head->buff)
            {
                FOTA_LOG("atiny_malloc fail");
                return FOTA_ERR;
            }
            head->head_len = FOTA_PACK_HEADER_MIN_LEN;
        }
        copy_len = MIN(FOTA_PACK_HEADER_MIN_LEN - offset, len);
        memcpy(head->buff + offset, buff, copy_len);
        head->stored_len = offset + copy_len;
        *used_len = copy_len;
        if(head->stored_len < FOTA_PACK_HEADER_MIN_LEN)
        {
            return FOTA_OK;
        }

        version = GET_DWORD(head->buff, FOTA_PACK_VERSION_POS);
        if(version != VERSION_NO)
        {
            FOTA_LOG("invalid version %D", version);
            head->stored_len = 0;
            return FOTA_ERR;
        }

        head_len = GET_DWORD(head->buff, FOTA_PACK_HEADER_HEAD_LEN_POS);
        total_len = GET_DWORD(head->buff, FOTA_PACK_HEADER_TOTAL_LEN_POS);
        if(head_len < FOTA_PACK_HEADER_MIN_LEN || (head_len >= total_len))
        {
            FOTA_LOG("invalid head len %d, total len %d", head_len, total_len);
            head->stored_len = 0;
            return FOTA_ERR;
        }

        if(head_len > head->head_len)
        {
            uint8_t *new_buff = atiny_malloc(head_len);
            if(NULL == new_buff)
            {
                FOTA_LOG("atiny_malloc fail");
                return FOTA_ERR;
            }
            memcpy(new_buff, head->buff, head->stored_len);
            atiny_free(head->buff);
            head->buff = new_buff;
            head->head_len = head_len;
        }

    }
    return FOTA_OK;
}

static int fota_pack_parse_checksum(fota_pack_head_s *head, uint8_t *buff, uint32_t len)
{
    uint32_t attribute;
    uint32_t tlv_len;
    fota_pack_checksum_type_e checksum_type = FOTA_PACK_MAX_CHECKSUM_TYPE;
    uint8_t *cur = buff + FOTA_TLV_START_POS;
    uint32_t left_len = len - FOTA_TLV_START_POS;

    while(left_len > 0)
    {
        attribute = GET_WORD(cur, 0);
        cur += FOTA_PACK_TLV_T_LEN;
        tlv_len = GET_WORD(cur, 0);
        cur += FOTA_PACK_TLV_L_LEN;
        if(left_len < (FOTA_PACK_TLV_T_LEN  + FOTA_PACK_TLV_L_LEN + tlv_len))
        {
            FOTA_LOG("tvl err attribute %d, tlv_len %d", attribute, tlv_len);
            return FOTA_ERR;
        }
        if(FOTA_PACK_TLV_T_SHA256 == attribute)
        {
            checksum_type = FOTA_PACK_SHA_S56;
            if(head->checksum_pos)
            {
                atiny_free(head->checksum_pos);
                head->checksum_pos = NULL;
                head->checksum_len = 0;
            }
            if(tlv_len > 0)
            {

                head->checksum_pos = atiny_malloc(tlv_len);
                if(NULL == head->checksum_pos)
                {
                    FOTA_LOG("atiny_malloc %d fail", tlv_len);
                    return FOTA_ERR;
                }
                memcpy(head->checksum_pos, cur, tlv_len);
            }

            head->checksum_len = tlv_len;
            memset(cur, 0, tlv_len);
            break;
        }
        cur += tlv_len ;
        left_len -= (FOTA_PACK_TLV_T_LEN  + FOTA_PACK_TLV_L_LEN + tlv_len);
    }

    if(head->checksum_pos)
    {
        if(head->checksum)
        {
            fota_pack_checksum_delete(head->checksum);
            head->checksum = NULL;
        }
        head->checksum = fota_pack_checksum_create(checksum_type);
        if(head->checksum == NULL)
        {
            FOTA_LOG("fota_pack_checksum_create fail");
            return FOTA_ERR;
        }
        if(fota_pack_checksum_update_head(head->checksum, head) != FOTA_OK)
        {
            FOTA_LOG("fota_pack_checksum_update_head fail");
            return FOTA_ERR;
        }
    }
    return FOTA_OK;

}


int fota_pack_head_parse(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff,
                                 uint16_t len, uint16_t *used_len)
{
    int ret;
    uint16_t tmp_len = 0;

    *used_len = 0;
    ret = fota_pack_head_parse_head_len(head, offset, buff, len, &tmp_len);
    if(ret != FOTA_OK)
    {
        return ret;
    }

    len -= tmp_len;
    if((0 == len) && (head->stored_len < head->head_len))
    {
        *used_len = tmp_len;
        return FOTA_OK;
    }

    offset += tmp_len;
    buff += tmp_len;

    if((head->head_len < FOTA_PACK_HEADER_MIN_LEN)
        || (NULL == head->buff)
        || ((head->stored_len < head->head_len) && (offset > head->stored_len)))
    {
        FOTA_LOG("head not continuous, len %u offset %u", head->stored_len, offset);
            return FOTA_ERR;
    }

    if(offset < head->head_len)
    {
        uint16_t copy_len;

        copy_len = MIN(len, head->head_len - offset);
        (void)memcpy(head->buff + offset, buff, copy_len);
        *used_len = copy_len + tmp_len;
        head->stored_len = offset + copy_len;
        if(head->stored_len >= head->head_len)
        {
            uint32_t save_len;
            save_len = GET_DWORD(head->buff, FOTA_PACK_HEADER_TOTAL_LEN_POS);
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
                    FOTA_LOG("size exceed, save len %u head len %u max_len %u", save_len, head->head_len, max_len);
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

            return fota_pack_parse_checksum(head, head->buff, head->head_len);
        }
    }



    return FOTA_OK;
}

static bool fota_pack_head_is_done(const fota_pack_head_s *head)
{
    return ((head->head_len >= FOTA_PACK_HEADER_MIN_LEN) && (head->stored_len == head->head_len));
}

int fota_pack_head_check(const fota_pack_head_s *head, uint32_t len)
{
    uint32_t save_len;

    if(!fota_pack_head_is_done(head))
    {
        FOTA_LOG("head invalid get len, stored len %u, head len %u", head->stored_len, head->head_len);
        return FOTA_ERR;
    }

    save_len = GET_DWORD(head->buff, FOTA_PACK_HEADER_TOTAL_LEN_POS);
    if(len != save_len)
    {
        FOTA_LOG("len err save len %u, rcv len %u", save_len, len);
        return FOTA_ERR;
    }

    if((head->checksum_pos == NULL) || (head->checksum_len == 0))
    {
        FOTA_LOG("fota no checksum exist");
        return FOTA_OK;
    }

    return fota_pack_checksum_check(head->checksum, head->checksum_pos, head->checksum_len);
}
uint32_t fota_pack_head_get_head_len(const fota_pack_head_s *head)
{
    return fota_pack_head_is_done(head) ? head->head_len : 0;
}

const uint8_t* fota_pack_head_get_head_info(const fota_pack_head_s *head)
{
    return fota_pack_head_is_done(head) ? head->buff : 0;
}


int fota_pack_head_set_head_info(fota_pack_head_s *head, fota_hardware_s *hardware, head_update_check update_check, void *param)
{
    head->hardware = hardware;
    head->update_check = update_check;
    head->param = param;

    return FOTA_OK;
}

fota_pack_checksum_s *fota_pack_head_get_checksum(fota_pack_head_s *head)
{
    return head->checksum;
}


