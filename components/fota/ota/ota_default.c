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
#include "ota_errno.h"
#include "ota_crc.h"
#include "ota_sha256.h"
#include "ota_default.h"
#include "board.h"

#include <string.h>

#define MAX_RESTART_CNT 5
#define OTA_INTEGRITY_BUF_SIZE 0x1000

static ota_default_flag g_ota_flag;

static int prv_read_specific(uint32_t addr, ota_default_flag* flag)
{
    int ret;
    int crc;

    ret = g_ota_assist.func_ota_read(flag, sizeof(ota_default_flag), addr);
    if (0 == ret)
    {
        crc = calc_crc32(0, flag, sizeof(ota_default_flag) - sizeof(uint32_t));
        if (crc != flag->crc)
        {
            ret = -1;
        }
    }

    return ret;
}

static int prv_write_specific(uint32_t addr, ota_default_flag* flag)
{
    flag->crc = calc_crc32(0, flag, sizeof(ota_default_flag) - sizeof(uint32_t));
    return g_ota_assist.func_ota_write(flag, sizeof(ota_default_flag), addr);
}

static int prv_read_ota_default_flag(void)
{
    int ret;

    ret = prv_read_specific(OTA_FLAG_ADDR1, &g_ota_flag);
    if (ret != 0)
    {
        ret = prv_read_specific(OTA_FLAG_ADDR2, &g_ota_flag);
    }

    return ret;
}

static int prv_write_ota_default_flag(void)
{
    int ret;
    ota_default_flag flag_tmp;

    ret = prv_read_specific(OTA_FLAG_ADDR1, &flag_tmp);
    if (0 == ret)
    {
        ret = prv_write_specific(OTA_FLAG_ADDR2, &flag_tmp);
        if (0 != ret)
        {
            return ret;
        }
    }

    ret = prv_write_specific(OTA_FLAG_ADDR1, &g_ota_flag);

    return ret;
}

uint8_t g_integrity_buf[OTA_INTEGRITY_BUF_SIZE];

static int prv_image_integrity(uint8_t* integrity)
{
    int ret = -1;
    uint32_t image_addr = OTA_IMAGE_DOWNLOAD_ADDR;
    uint32_t image_len = g_ota_flag.image_length;
    ota_sha256_context ctx;
    uint32_t check_len;

    ota_sha256_init(&ctx);
    ota_sha256_starts(&ctx, 0);

    while (image_len > 0)
    {
        check_len = image_len > OTA_INTEGRITY_BUF_SIZE
                    ? OTA_INTEGRITY_BUF_SIZE : image_len;
        if (g_ota_assist.func_ota_read(g_integrity_buf, check_len, image_addr) != 0)
        {
            OTA_LOG("read image failed during integrity check");
            goto exit;
        }
        ota_sha256_update(&ctx, (const unsigned char*)g_integrity_buf, check_len);
        image_addr += check_len;
        image_len -= check_len;
    }
    ota_sha256_finish(&ctx, (unsigned char*)integrity);

    ret = 0;

exit:
    ota_sha256_free(&ctx);
    return ret;
}

#ifdef USE_BOOTLOADER
static void prv_get_update_record(uint8_t* state, uint32_t* offset)
{
    if (NULL != state)
    {
        *state = g_ota_flag.cur_state;
    }
    if (NULL != offset)
    {
        *offset = g_ota_flag.cur_offset;
    }
}


static int prv_set_update_record(uint8_t state, uint32_t offset)
{
    g_ota_flag.cur_state = state;
    g_ota_flag.cur_offset = offset;

    return prv_write_ota_default_flag();
}

static int prv_update_state(ota_state st)
{
    g_ota_flag.state = st;
    if (prv_write_ota_default_flag() != 0)
    {
        OTA_LOG("write ota flag failed");
        return OTA_ERRNO_SPI_FLASH_WRITE;
    }
    return OTA_ERRNO_OK;
}

static int prv_check_integrity_value(const uint8_t* v1, const uint8_t* v2)
{
    int i;

    for (i = 0; i < OTA_IMAGE_INTEGRITY_LENGTH; ++i)
    {
        if (v1[i] != v2[i])
        {
            return -1;
        }
    }

    return 0;
}
#endif
int ota_default_init(void)
{
    if (prv_read_ota_default_flag() != 0)
    {
        g_ota_flag.state = OTA_S_IDLE;
        g_ota_flag.restart_cnt = 0;
        g_ota_flag.cur_state = 0;
        g_ota_flag.cur_offset = 0;
        g_ota_flag.image_length = 0;
        g_ota_flag.old_image_length = OTA_IMAGE_DOWNLOAD_SIZE;
        memset(g_ota_flag.image_integrity, 0, OTA_IMAGE_INTEGRITY_LENGTH);
        if (prv_write_ota_default_flag() != 0)
        {
            OTA_LOG("write ota flag failed");
            return OTA_ERRNO_SPI_FLASH_WRITE;
        }
    }
    OTA_LOG("state: %d", g_ota_flag.state);
    OTA_LOG("restart_cnt: %d", g_ota_flag.restart_cnt);

    return OTA_ERRNO_OK;
}

int ota_default_set_reboot(int32_t image_len)
{
    if (image_len <= 0)
    {
        OTA_LOG("ilegal image_len: %d", image_len);
        return OTA_ERRNO_ILEGAL_PARAM;
    }

    g_ota_flag.cur_state = 0;
    g_ota_flag.cur_offset = 0;
    g_ota_flag.image_length = image_len;
    g_ota_flag.state = OTA_S_NEEDUPDATE;
    if (prv_image_integrity(g_ota_flag.image_integrity) != 0)
    {
        OTA_LOG("do integrity check failed");
        return OTA_ERRNO_INTEGRITY_CHECK;
    }

    if (prv_write_ota_default_flag() != 0)
    {
        OTA_LOG("write ota flag failed");
        return OTA_ERRNO_SPI_FLASH_WRITE;
    }

    return OTA_ERRNO_OK;
}

int ota_default_check_update_state(ota_state* st)
{
    if (NULL != st)
    {
        *st = g_ota_flag.state;
    }

    if (OTA_S_SUCCEED == g_ota_flag.state || OTA_S_FAILED == g_ota_flag.state)
    {
        g_ota_flag.state = OTA_S_IDLE;
        g_ota_flag.restart_cnt = 0;
        if (OTA_S_SUCCEED == g_ota_flag.state)
        {
            g_ota_flag.old_image_length = g_ota_flag.image_length;
        }
        if (prv_write_ota_default_flag() != 0)
        {
            OTA_LOG("write ota flag failed");
            return OTA_ERRNO_SPI_FLASH_WRITE;
        }
    }

    return OTA_ERRNO_OK;
}
#ifdef USE_BOOTLOADER
int ota_default_update_process(void)
{
/*lint -e616 */
    int ret;
    uint8_t integrity[OTA_IMAGE_INTEGRITY_LENGTH];

    switch (g_ota_flag.state)
    {
    case OTA_S_IDLE:
    case OTA_S_FAILED:
        return OTA_ERRNO_OK;
    case OTA_S_NEEDUPDATE:
        if (prv_image_integrity(integrity) != 0)
        {
            OTA_LOG("image integrity check failed");
            (void)prv_update_state(OTA_S_FAILED);
            return OTA_ERRNO_INTEGRITY_CHECK;
        }
        if (prv_check_integrity_value(integrity, g_ota_flag.image_integrity) != 0)
        {
            OTA_LOG("incomplete image detected");
            (void)prv_update_state(OTA_S_FAILED);
            return OTA_ERRNO_INTEGRITY_CHECK;
        }
    case OTA_S_UPDATING:
        (void)prv_update_state(OTA_S_UPDATING);
        ret = board_update_copy(g_ota_flag.old_image_length, g_ota_flag.image_length,
                                prv_get_update_record, prv_set_update_record);
        if (OTA_ERRNO_OK != ret)
        {
            OTA_LOG("update failed");
            (void)prv_update_state(OTA_S_FAILED);
            return ret;
        }
        return prv_update_state(OTA_S_SUCCEED);
    case OTA_S_SUCCEED:
        if (g_ota_flag.restart_cnt > MAX_RESTART_CNT)
        {
            return ota_default_roll_back_image();
        }
        else
        {
            g_ota_flag.restart_cnt++;
            if (prv_write_ota_default_flag() != 0)
            {
                OTA_LOG("write ota flag failed");
                return OTA_ERRNO_SPI_FLASH_WRITE;
            }
            return OTA_ERRNO_OK;
        }
    default:
        OTA_LOG("unknown state:%d", g_ota_flag.state);
        return OTA_ERRNO_INTERNEL;
    }
/*lint +e616 */
}

int ota_default_jump_to_application(void)
{
    return board_jump2app();
}

int ota_default_roll_back_image(void)
{
    int ret;
    if ((ret = board_rollback_copy(g_ota_flag.old_image_length,
                                   prv_get_update_record, prv_set_update_record)) != 0)
    {
        OTA_LOG("image rollback failed");
        return ret;
    }
    return prv_update_state(OTA_S_FAILED);
}
#endif

