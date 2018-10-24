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
#include "sota/sota.h"
#include "sota_hal.h"
#include "at_frame/at_main.h"
#include "flag_manager.h"
#include "upgrade_flag.h"
#include "fota/fota_package_storage_device.h"

#include <board.h>
#include "los_swtmr.h"
#include "hal_spi_flash.h"
#include "atiny_lwm2m/atiny_fota_api.h"
#include "upgrade_flag.h"

extern at_task at;
#define VER_LEN  16
#define DOWNLOADTIME_LIMIT  200
#define STATUSTIME_LIMIT  300
typedef  uint8_t   BYTE;
typedef  uint16_t   WORD;
#define MIN_PKT_LEN 4
typedef enum
{
    MSG_GET_VER=19,
    MSG_NOTIFY_NEW_VER,
    MSG_GET_BLOCK,
    MSG_UPDATE_STATE,
    MSG_EXC_UPDATE,
    MSG_NOTIFY_STATE=24
}msg_code;

typedef struct ota_pcp_head_t
{
    WORD ori_id;
    BYTE ver_num;
    BYTE msg_code;
    WORD chk_code;
    WORD data_len;
}ota_pcp_head_s;

typedef struct ota_ver_notify
{
    BYTE ver[16];
    WORD block_size;
    WORD block_totalnum;
    WORD ver_chk_code;
}ota_ver_notify_t;

typedef struct ota_get_bloc
{
    BYTE ver[16];
    WORD block_seq;
}ota_get_block_t;

typedef struct ota_update_exc_t
{
    BYTE ver[16];
    WORD ver_chk_code;
    WORD len;
}ota_update_exc_s;

typedef struct ota_ret_t
{
    BYTE ret;
    BYTE ver[16];
}ota_ret;

typedef struct ota_block
{
    BYTE errcode;
    WORD block_seq;
    BYTE* data;
}ota_block_t;

typedef struct at_update_record
{
    uint16_t block_size;
    uint32_t block_num;
    uint32_t block_offset;
    uint32_t block_totalnum;
    uint32_t block_len;
    uint32_t block_tolen;
    uint32_t ver_chk_code;
    char ver[VER_LEN];
    uint8_t msg_code;
    uint32_t chk_code;
    uint8_t download_tmr;
    uint8_t state;
} at_update_record_t;

static sota_op_t g_flash_op;
static at_update_record_t g_at_update_record = {0};
static uint8_t tmr_ticks = 0;
unsigned char *flashbuf = NULL;
unsigned char* rabuf = NULL;
unsigned int image_download_addr;
unsigned int flash_block_size;
static atiny_fota_storage_device_s * g_storage_device;

#define LITTLE_DNEIAN
#ifdef LITTLE_DNEIAN
#define htons_ota(x) ((((x) & 0x00ff) << 8) | (((x) & 0xff00) >> 8))
#else
#define htons_ota(x)
#endif
#define PCP_HEAD 0xFFFE
#define BLOCK_HEAD 3

int valid_check(char *rcvbuf, int32_t len)
{
    ota_pcp_head_s *pbuf;
    char *databuf,*rlen;
    int buflen;
    int ret,cmd_crc_num;
    char *buf;
    char cmd[2]={0};
    if(rcvbuf == NULL|| len <= MIN_PKT_LEN)
    {
        SOTA_LOG("buf null:%p len:%d",rcvbuf,(int)len);
        goto END;
    }
    rlen = strstr(rcvbuf,":");
    if(rlen == NULL)
    {
        SOTA_LOG("buflen invalid");
        goto END;
    }
    buflen = chartoint(rlen+1);
    buflen = buflen * 2;
    databuf = strstr(rlen,",");
    if(databuf == NULL)
    {
        SOTA_LOG("buf invalid");
        goto END;
    }
    buf = databuf + 1;
    (void)HexStrToByte((const unsigned char *)(buf+8), (unsigned char*)cmd, 4);
    cmd_crc_num = cmd[0]<<8 | cmd[1];
    memset(buf + 8, '0', 4);
    memset(rabuf, 0, AT_DATA_LEN);
    HexStrToByte((const unsigned char *)buf, rabuf, buflen);
    ret = crc_check((const unsigned char *)rabuf, buflen/2);
    pbuf = (ota_pcp_head_s *)rabuf;

    pbuf->ori_id = htons_ota(pbuf->ori_id);
    pbuf->chk_code = htons_ota(pbuf->chk_code);
    pbuf->data_len = htons_ota(pbuf->data_len) - BLOCK_HEAD;
    if(pbuf->ori_id != PCP_HEAD || (pbuf->ver_num & 0xf) != 1 || (ret != cmd_crc_num) || \
            (pbuf->msg_code < MSG_GET_VER || pbuf->msg_code > MSG_NOTIFY_STATE))
    {
        SOTA_LOG("head wrong! ori_id:%X ver_num:%X ret:%X crc:%X",pbuf->ori_id,pbuf->ver_num,ret, cmd_crc_num);
        goto END;
    }
    g_at_update_record.msg_code = pbuf->msg_code;
    g_at_update_record.chk_code = pbuf->chk_code;
    if(pbuf->msg_code == MSG_GET_BLOCK)
    {
        g_at_update_record.block_tolen += pbuf->data_len;
    }
    g_at_update_record.block_len = pbuf->data_len;
    return AT_OK;
END:
    return AT_FAILED;
}

int ver_to_hex(const char *bufin, int len, char *bufout)
{
    int i = 0;
    if (NULL == bufin || len <= 0 || NULL == bufout)
    {
        return -1;
    }
    for(i = 0; i < len; i++)
    {
        sprintf(bufout + i * 2, "%02X", bufin[i]);
    }
    return 0;
}

int at_fota_send(char *buf, int len)
{
#define BUF_LEN 128
    uint32_t ret;
    char crcretbuf[5] = {0};
    char tmpbuf[BUF_LEN + VER_LEN] = {0};
    ota_pcp_head_s pcp_head = {0};
    unsigned char atwbuf[BUF_LEN + VER_LEN] = {0};
    unsigned char hbuf[64] = {0};
    if(len >= BUF_LEN)
    {
        SOTA_LOG("payload too long");
        return -1;
    }
    pcp_head.ori_id = htons_ota(PCP_HEAD);
    pcp_head.ver_num = 1;
    pcp_head.msg_code = g_at_update_record.msg_code;
    pcp_head.data_len = htons_ota(len / 2);
    (void)ver_to_hex((const char *)&pcp_head, sizeof(ota_pcp_head_s), (char *)hbuf);

    memcpy(atwbuf, hbuf, VER_LEN);
    memcpy(atwbuf + VER_LEN, buf, len);

    HexStrToByte(atwbuf, (unsigned char*)tmpbuf, len + VER_LEN); //strlen(atwbuf)
    ret = (uint32_t)crc_check((unsigned char*)tmpbuf, (len + VER_LEN) / 2);
    (void)snprintf(crcretbuf, sizeof(crcretbuf), "%04X", (unsigned int)ret);

    memcpy(atwbuf + 8, crcretbuf, 4);
    return g_flash_op.sota_send((char *)atwbuf, len + VER_LEN);
}

int ota_report_result(void)
{
    return -1;

}

int32_t sota_process_main(void *arg, int8_t *buf, int32_t buflen)
{
    char sbuf[64] = {0};
    uint8_t *pbuf = NULL;
    int ret = SOTA_OK;

    if(valid_check((char *)buf, buflen) != 0)
    {
        SOTA_LOG("valid_check wrong");
        return SOTA_FAILED;
    }

    if(g_at_update_record.block_len > 0)
        pbuf = rabuf + VER_LEN/2;
    switch(g_at_update_record.state)
    {
    case IDLE:
        if(g_at_update_record.msg_code == MSG_GET_VER)
        {
            char ver_ret[VER_LEN + 1] = {0};
            (void)g_flash_op.get_ver(ver_ret+1,VER_LEN);
            (void)ver_to_hex(ver_ret, (VER_LEN + 1), (char *)sbuf);
            (void)at_fota_send((char *)sbuf, (VER_LEN + 1) * 2);
        }
        else if(g_at_update_record.msg_code == MSG_NOTIFY_NEW_VER)
        {
            ota_ver_notify_t *notify = (ota_ver_notify_t *)pbuf;
            char tmpbuf[2] = {0};
            char ver_ret[VER_LEN + 2] = {0};
            if(notify == NULL)
            {
                SOTA_LOG("no buf");
                return SOTA_FAILED;
            }
            ret = g_flash_op.set_ver((const char*)notify, sizeof(ota_ver_notify_t));
            (void)ver_to_hex(tmpbuf, 2, (char *)sbuf);
            (void)at_fota_send((char *)sbuf, 2);
            if(ret == SOTA_OK)
            {
                memcpy(ver_ret, notify->ver, VER_LEN);
                if(htons_ota(notify->ver_chk_code) != g_at_update_record.ver_chk_code)
                {
                    g_at_update_record.block_offset = 0;
                    g_at_update_record.block_size = htons_ota(notify->block_size);
                    g_at_update_record.block_totalnum = htons_ota(notify->block_totalnum);
                    memcpy(g_at_update_record.ver, notify->ver, VER_LEN);
                    ver_ret[VER_LEN] = ver_ret[VER_LEN + 1] = 0;
                    g_at_update_record.block_num = 0;
                    g_at_update_record.ver_chk_code = htons_ota(notify->ver_chk_code);
                    g_at_update_record.state = DOWNLOADING;
                    g_at_update_record.download_tmr = tmr_ticks;
                }
                else
                {
                    ver_ret[VER_LEN] = (g_at_update_record.block_num >> 8) & 0XFF;
                    ver_ret[VER_LEN + 1] = g_at_update_record.block_num & 0XFF;
                }
                (void)ver_to_hex(ver_ret, (VER_LEN + 2), sbuf);
                g_at_update_record.msg_code = MSG_GET_BLOCK;
                (void)at_fota_send(sbuf, (VER_LEN + 2) * 2);
                (void)flag_write(FLAG_APP, (void*)&g_at_update_record, sizeof(at_update_record_t));
                ret = SOTA_DOWNLOADING;
            }
        }
        else
        {
            char tmpbuf[2] = {1};
            SOTA_LOG("not cmd");
            (void)ver_to_hex(tmpbuf, 1, (char *)sbuf);
            (void)at_fota_send((char *)sbuf, 2);
        }
        break;
    case DOWNLOADING:
    {
        char tmpbuf[VER_LEN + 2] = {0};
        uint16_t block_seq = 0;
        if(pbuf == NULL)
        {
            SOTA_LOG("no buf");
            return SOTA_FAILED;
        }
        block_seq = ((*(pbuf + 1) << 8) & 0XFF00) | (*(pbuf + 2) & 0XFF);

        if(*pbuf != SOTA_OK || g_at_update_record.block_num != block_seq \
                || g_at_update_record.state != DOWNLOADING)
        {
            SOTA_LOG("download wrong,we need %X, but is %X:",(int)g_at_update_record.block_num, (int)block_seq);
            g_at_update_record.state = IDLE;
            tmpbuf[1] = SOTA_FAILED;
            (void)ver_to_hex(tmpbuf, 2, sbuf);
            (void)at_fota_send(sbuf, 2 * 2);
            return SOTA_FAILED;
        }
        g_at_update_record.download_tmr = tmr_ticks;
        ret = g_storage_device->write_software(g_storage_device, g_at_update_record.block_offset,(const uint8_t *)(pbuf + BLOCK_HEAD), g_at_update_record.block_size);
        if(ret != SOTA_OK)
        {
            SOTA_LOG("write_software ret:%d! return", ret);
            return SOTA_FAILED;
        }
        g_at_update_record.block_offset += g_at_update_record.block_size;
        if((++g_at_update_record.block_num) < g_at_update_record.block_totalnum)
        {
            memcpy(tmpbuf, g_at_update_record.ver, VER_LEN);
            tmpbuf[VER_LEN] = (g_at_update_record.block_num >> 8 & 0XFF);
            tmpbuf[VER_LEN + 1] = g_at_update_record.block_num & 0XFF;
            (void)ver_to_hex(tmpbuf, (VER_LEN + 2), sbuf);
            (void)at_fota_send(sbuf, (VER_LEN + 2) * 2);
            break;
        }
        else//if((g_at_update_record.block_num) >= g_at_update_record.block_totalnum)
        {
            g_at_update_record.state = DOWNLOADED;
            SOTA_LOG("DOWNLOADED");
            ret = g_storage_device->write_software_end(g_storage_device, (atiny_download_result_e)ret,g_at_update_record.block_tolen);
            if(ret != SOTA_OK)
            {
                SOTA_LOG("write_software_end ret:%d! return", ret);
                return SOTA_FAILED;
            }
            g_at_update_record.msg_code = MSG_UPDATE_STATE;
            g_at_update_record.state = DOWNLOADED;
            tmpbuf[0] = SOTA_OK;
            (void)ver_to_hex(tmpbuf, 1, sbuf);
            (void)at_fota_send(sbuf, 2);
            g_at_update_record.msg_code = MSG_EXC_UPDATE;
        }
        break;
    }
    case DOWNLOADED:
    if(g_at_update_record.msg_code == MSG_EXC_UPDATE)
    {
        char tmpbuf[1] = {0};
        SOTA_LOG("begin update and send");
        g_at_update_record.state = UPDATING;
        tmpbuf[0] = SOTA_OK;
        (void)ver_to_hex(tmpbuf, 1, sbuf);
        (void)at_fota_send(sbuf, 2);
        g_at_update_record.msg_code = MSG_NOTIFY_STATE;
        (void)flag_write(FLAG_APP, (void*)&g_at_update_record, sizeof(at_update_record_t));
        ret = g_storage_device->active_software(g_storage_device);
        ret = SOTA_NEEDREBOOT;
        break;
    }
    default:
    {
        SOTA_LOG("cmd invalid");
        break;
    }
    }
    return ret;
}

void sota_tmr(void)
{
    if(g_at_update_record.state == DOWNLOADING)
    {
        if(++tmr_ticks - g_at_update_record.download_tmr > DOWNLOADTIME_LIMIT)
        {
            char tmpbuf[VER_LEN + 2] = {0};
            char sbuf[64] = {0};
            SOTA_LOG("over time");
            memcpy(tmpbuf, g_at_update_record.ver, 16);
            tmpbuf[16] = (g_at_update_record.block_num >> 8 & 0XFF);
            tmpbuf[17] = g_at_update_record.block_num & 0XFF;
            (void)ver_to_hex(tmpbuf, 18, sbuf);
            (void)at_fota_send(sbuf, 18 * 2);

        }
    }
    return;
}

int sota_status_check(void)
{
    upgrade_state_e state;
    char cbuf[64] = {0};
    if(flag_read(FLAG_APP, (char*)&g_at_update_record, sizeof(at_update_record_t)))
    {
        SOTA_LOG("flag read err");
        return SOTA_FAILED;
    }
    SOTA_LOG("state:%d flash ver:%s",g_at_update_record.state,g_at_update_record.ver);
    (void)flag_upgrade_get_result(&state);
    if(state == OTA_SUCCEED)
    {
        char sbuf[64] = {0};
        char tmpbuf[17] = {0};
        tmpbuf[0] = SOTA_OK;
        g_at_update_record.state = IDLE;
        (void)g_flash_op.set_ver(g_at_update_record.ver, VER_LEN);
        SOTA_LOG("we send the ver:%s",g_at_update_record.ver);
        memcpy(tmpbuf + 1, g_at_update_record.ver, VER_LEN);
        (void)ver_to_hex(tmpbuf, 17, sbuf);
        (void)at_fota_send(sbuf, 17 * 2);
    }
    if(g_at_update_record.state == DOWNLOADING)
    {
        return SOTA_BOOTLOADER_DOWNLOADING;
    }
    (void)flag_write(FLAG_APP, (const void*)cbuf, sizeof(at_update_record_t));
    return SOTA_OK;
}
sota_flag_opt_s g_sota_flag;

int func_flag_read(void *buf, int32_t len)
{
    return g_sota_flag.read_flash(OTA_UPDATE_INFO,buf, len, 0);
}

int func_flag_write(const void *buf, int32_t len)
{
    return g_sota_flag.write_flash(OTA_UPDATE_INFO,buf, len, 0);
}

int sota_init(sota_op_t* flash_op)
{
    flag_op_s flag;

    if(flash_op == NULL)
    {
        return SOTA_FAILED;
    }

    (void)ota_init_pack_device(&flash_op->ota_info);
    g_storage_device = fota_get_pack_device();

    g_sota_flag.read_flash = flash_op->ota_info.read_flash;
    g_sota_flag.write_flash = flash_op->ota_info.write_flash;
    flag.func_flag_read = func_flag_read;
    flag.func_flag_write = func_flag_write;
    (void)flag_init(&flag);
    (void)flag_upgrade_init();

    memcpy(&g_flash_op, flash_op, sizeof(sota_op_t));
    flash_block_size = flash_op->ota_info.flash_block_size;
    (void)sota_status_check();
    memset(&g_at_update_record, 0, sizeof(at_update_record_t));

    rabuf =  at_malloc(flash_op->user_data_len);
    if(rabuf == NULL)
    {
        SOTA_LOG("malloc rabuf failed");
        at_free(flashbuf);
        return SOTA_FAILED;
    }
    return SOTA_OK;
}

