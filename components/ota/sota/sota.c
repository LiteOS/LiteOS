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
#include "flag_manager.h"

#include<stdio.h>
#include<string.h>
#include<stdint.h>

#include "ota/package.h"
#include "upgrade_flag.h"

#define VER_LEN      16
#define SEND_BUF_LEN 128

typedef  uint8_t   BYTE;
typedef  uint16_t  WORD;

typedef enum
{
    MSG_GET_VER=19,
    MSG_NOTIFY_NEW_VER,
    MSG_GET_BLOCK,
    MSG_UPDATE_STATE,
    MSG_EXC_UPDATE,
    MSG_NOTIFY_STATE=24
}msg_code_e;

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

typedef struct ota_get_block
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

typedef struct sota_update_info
{
    uint16_t block_size;
    uint32_t block_num;
    uint32_t block_offset;
    uint32_t block_totalnum;
    uint32_t block_tolen;
    uint32_t ver_chk_code;
    char ver[VER_LEN];
    uint8_t state;
} sota_update_info_t;

static sota_opt_t                  g_flash_op;
static sota_update_info_t          g_at_update_record;
static pack_storage_device_api_s * g_storage_device;
static unsigned char* rabuf = NULL;

#define LITTLE_DNEIAN
#ifdef LITTLE_DNEIAN
#define htons_ota(x) ((((x) & 0x00ff) << 8) | (((x) & 0xff00) >> 8))
#else
#define htons_ota(x) (x)
#endif
#define PCP_HEAD 0xFFFE
#define BLOCK_HEAD 3

static int chartoint(const char* port)
{
	int tmp=0;
	while(*port >= '0' && *port <= '9')
	{
		tmp = tmp*10+*port-'0';
		port++;
	}
	return tmp;
}

static int packet_parse(const char *rcvbuf, int32_t len)
{
    ota_pcp_head_s *phead;
    char *databuf;
    char *rlen;
    int buflen;
    int ret,cmd_crc_num;
    char *buf;

    if(rcvbuf == NULL || len < sizeof(ota_pcp_head_s) || rabuf == NULL)
    {
        SOTA_LOG("buf null:%p len:%d",rcvbuf,(int)len);
        goto END;
    }

    rlen = strstr(rcvbuf,":");/*lint !e158*/
    if(rlen == NULL)/*lint !e158*/
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

    memset(rabuf, 0, g_flash_op.frame_buf_len);
    HexStrToByte((const unsigned char *)buf, rabuf, buflen);
    phead = (ota_pcp_head_s *)rabuf;

    cmd_crc_num = htons_ota(phead->chk_code);
    phead->chk_code = 0;
    ret = crc_check((const unsigned char *)rabuf, buflen/2);
    phead->ori_id = htons_ota(phead->ori_id);
    if(phead->data_len != 0 && phead->msg_code == MSG_GET_BLOCK)
        phead->data_len = htons_ota(phead->data_len) - BLOCK_HEAD;
    if(phead->ori_id != PCP_HEAD || (ret != cmd_crc_num) || \
            (phead->msg_code < MSG_GET_VER || phead->msg_code > MSG_NOTIFY_STATE))
    {
        SOTA_LOG("head wrong! msg_code:%X ver_num:%X ret:%X crc:%X",phead->ori_id,phead->msg_code,ret, cmd_crc_num);
        goto END;
    }

    return SOTA_OK;
END:
    return SOTA_FAILED;
}

static int ver_to_hex(const char *bufin, int len, char *bufout)
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

static int sota_at_send(msg_code_e msg_code, char *buf, int len)
{
    uint32_t ret;
    char crcretbuf[5] = {0};
    char tmpbuf[SEND_BUF_LEN + VER_LEN] = {0};
    ota_pcp_head_s pcp_head = {0};
    unsigned char atwbuf[SEND_BUF_LEN + VER_LEN] = {0};
    unsigned char hbuf[64] = {0};
    if(len >= SEND_BUF_LEN)
    {
        SOTA_LOG("payload too long");
        return -1;
    }
    pcp_head.ori_id = htons_ota(PCP_HEAD);
    pcp_head.ver_num = 1;
    pcp_head.msg_code = msg_code;
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

static void sota_request_block(char *ver)
{
     char ver_ret[VER_LEN + 2] = {0};
     char sbuf[64] = {0};

     memcpy(ver_ret, ver, VER_LEN);
     ver_ret[VER_LEN] = (g_at_update_record.block_num >> 8) & 0XFF;
     ver_ret[VER_LEN + 1] = g_at_update_record.block_num & 0XFF;
     (void)ver_to_hex(ver_ret, (VER_LEN + 2), sbuf);
     (void)sota_at_send(MSG_GET_BLOCK, sbuf, (VER_LEN + 2) * 2);
}

int32_t sota_process_main(void *arg, const int8_t *buf, int32_t buflen)
{
    char sbuf[64] = {0};
    uint8_t *pbuf = NULL;
    int ret = SOTA_OK;
    ota_pcp_head_s *phead;
    unsigned char  msg_code;

    if(packet_parse((const char *)buf, buflen) != 0)
    {
        SOTA_LOG("valid_check wrong");
        return SOTA_FAILED;
    }

    phead =(ota_pcp_head_s *)rabuf;
    msg_code = phead->msg_code;

    if(phead->data_len > 0)
    {
        pbuf = rabuf + VER_LEN/2;
    }

    switch(g_at_update_record.state)
    {
    case IDLE:
        if(msg_code == MSG_GET_VER)
        {
            char ver_ret[VER_LEN + 1] = {0};
            (void)g_flash_op.get_ver(ver_ret+1,VER_LEN);
            (void)ver_to_hex(ver_ret, (VER_LEN + 1), (char *)sbuf);
            (void)sota_at_send(MSG_GET_VER, (char *)sbuf, (VER_LEN + 1) * 2);
        }
        else if(msg_code == MSG_NOTIFY_NEW_VER)
        {
            ota_ver_notify_t *notify = (ota_ver_notify_t *)pbuf;
            char tmpbuf[2] = {0};
            (void)ver_to_hex(tmpbuf, 2, (char *)sbuf);
            (void)sota_at_send(MSG_NOTIFY_NEW_VER, (char *)sbuf, 2);

            if((g_at_update_record.ver_chk_code == 0)
                || htons_ota(notify->ver_chk_code) != g_at_update_record.ver_chk_code)
            {
                g_at_update_record.block_offset = 0;
                g_at_update_record.block_size = htons_ota(notify->block_size);
                g_at_update_record.block_totalnum = htons_ota(notify->block_totalnum);
                g_at_update_record.block_num = 0;
                g_at_update_record.ver_chk_code = htons_ota(notify->ver_chk_code);
            }
            g_at_update_record.state = DOWNLOADING;
            memcpy(g_at_update_record.ver, notify->ver, VER_LEN);
            (void)flag_write(FLAG_APP, (void*)&g_at_update_record, sizeof(sota_update_info_t));
            if (g_flash_op.run_mode == APP_MODE)
            {
                sota_request_block((char*)notify->ver);
            }
            ret = SOTA_DOWNLOADING;

        }
        else
        {
            char tmpbuf[2] = {1};
            (void)ver_to_hex(tmpbuf, 1, (char *)sbuf);
            (void)sota_at_send(MSG_NOTIFY_NEW_VER, (char *)sbuf, 2);
        }
        break;
    case DOWNLOADING:
    {
        char tmpbuf[VER_LEN + 2] = {0};
        uint16_t block_seq = 0;

        block_seq = ((*(pbuf + 1) << 8) & 0XFF00) | (*(pbuf + 2) & 0XFF);
        if((*pbuf != SOTA_OK) || (msg_code != MSG_GET_BLOCK)
            || (g_at_update_record.block_num != block_seq))
        {
            SOTA_LOG("download wrong,we need %X, but is %X:",(int)g_at_update_record.block_num, (int)block_seq);
            g_at_update_record.state = IDLE;
            tmpbuf[1] = (char)SOTA_FAILED;
            (void)ver_to_hex(tmpbuf, 2, sbuf);
            (void)sota_at_send(MSG_GET_BLOCK, sbuf, 2 * 2);
            return SOTA_FAILED;
        }
        SOTA_LOG("off:%lx size:%x",g_at_update_record.block_offset,g_at_update_record.block_size);
        ret = g_storage_device->write_software(g_storage_device, g_at_update_record.block_offset,(const uint8_t *)(pbuf + BLOCK_HEAD), g_at_update_record.block_size);
        if(ret != SOTA_OK)
        {
            tmpbuf[1] = (char)SOTA_FAILED;
            SOTA_LOG("write_software ret:%d! return", ret);
            (void)ver_to_hex(tmpbuf, 2, sbuf);
            (void)sota_at_send(MSG_GET_BLOCK, sbuf, 2 * 2);
            return SOTA_FAILED;
        }
        g_at_update_record.block_offset += g_at_update_record.block_size;
        g_at_update_record.block_tolen += phead->data_len;
        if((++g_at_update_record.block_num) < g_at_update_record.block_totalnum)
        {
            memcpy(tmpbuf, g_at_update_record.ver, VER_LEN);
            tmpbuf[VER_LEN] = (g_at_update_record.block_num >> 8 & 0XFF);
            tmpbuf[VER_LEN + 1] = g_at_update_record.block_num & 0XFF;
            (void)ver_to_hex(tmpbuf, (VER_LEN + 2), sbuf);
            (void)sota_at_send(MSG_GET_BLOCK, sbuf, (VER_LEN + 2) * 2);
            break;
        }
        else//if((g_at_update_record.block_num) >= g_at_update_record.block_totalnum)
        {
            g_at_update_record.state = DOWNLOADED;
            SOTA_LOG("DOWNLOADED");
            ret = g_storage_device->write_software_end(g_storage_device, (pack_download_result_e)ret,g_at_update_record.block_tolen);
            if(ret != SOTA_OK)
            {
                tmpbuf[1] = (char)SOTA_FAILED;
                SOTA_LOG("write_software_end ret:%d! return", ret);
                (void)ver_to_hex(tmpbuf, 2, sbuf);
                (void)sota_at_send(MSG_GET_BLOCK, sbuf, 2 * 2);
                return SOTA_FAILED;
            }
            tmpbuf[0] = SOTA_OK;
            (void)ver_to_hex(tmpbuf, 1, sbuf);
            (void)sota_at_send(MSG_UPDATE_STATE, sbuf, 2);
        }
        break;
    }
    case DOWNLOADED:
    if(msg_code == MSG_EXC_UPDATE)
    {
        char tmpbuf[1] = {0};
        SOTA_LOG("begin update and send");
        g_at_update_record.state = UPDATING;
        tmpbuf[0] = SOTA_OK;
        (void)ver_to_hex(tmpbuf, 1, sbuf);
        (void)sota_at_send(MSG_EXC_UPDATE,sbuf, 2);
        (void)flag_write(FLAG_APP, (void*)&g_at_update_record, sizeof(sota_update_info_t));
        ret = g_storage_device->active_software(g_storage_device);
        if(ret != SOTA_OK)
        {
            SOTA_LOG("active_software ret:%d! return", ret);
            tmpbuf[1] = (char)SOTA_FAILED;
            (void)ver_to_hex(tmpbuf, 2, sbuf);
            (void)sota_at_send(MSG_EXC_UPDATE, sbuf, 2 * 2);
            return SOTA_FAILED;
        }

        if (g_flash_op.run_mode == BOOTLOADER_MODE)
        {
            g_flash_op.sota_free(rabuf);
            rabuf = NULL;
        }
        ret = SOTA_NEEDREBOOT;
    }
    break;
    default:
    {
        SOTA_LOG("cmd invalid");
        break;
    }
    }
    return ret;
}

void sota_timeout_handler(void)
{
    SOTA_LOG("sota over time");
    g_at_update_record.state = IDLE;
    return;
}

static int sota_status_check(void)
{
    upgrade_state_e state;
    char sbuf[64] = {0};
    char tmpbuf[VER_LEN+1] = {0};

    memset(&g_at_update_record, 0, sizeof(sota_update_info_t));
    if(flag_read(FLAG_APP, (char*)&g_at_update_record, sizeof(sota_update_info_t)))
    {
        SOTA_LOG("flag read err");
        return SOTA_FAILED;
    }
    SOTA_LOG("state:%d flash ver:%s",g_at_update_record.state,g_at_update_record.ver);

    if(g_at_update_record.state == DOWNLOADING && g_flash_op.run_mode == BOOTLOADER_MODE)
    {
        sota_request_block(g_at_update_record.ver);
        return SOTA_BOOTLOADER_DOWNLOADING;
    }

    (void)flag_upgrade_get_result(&state);
    if(state == OTA_SUCCEED)
    {
        (void)g_flash_op.set_ver(g_at_update_record.ver, VER_LEN);
        SOTA_LOG("we send the ver:%s",g_at_update_record.ver);
        memcpy(tmpbuf + 1, g_at_update_record.ver, VER_LEN);
        (void)ver_to_hex(tmpbuf, VER_LEN+1, sbuf);
        (void)sota_at_send(MSG_NOTIFY_STATE, sbuf, (VER_LEN+1) * 2);
    }

    memset(&g_at_update_record, 0, sizeof(sota_update_info_t));
    (void)flag_write(FLAG_APP, (const void*)&g_at_update_record, sizeof(sota_update_info_t));
    return SOTA_OK;
}

static int func_flag_read(void *buf, int32_t len)
{
    return g_flash_op.ota_info.read_flash(OTA_UPDATE_INFO,buf, len, 0);
}

static int func_flag_write(const void *buf, int32_t len)
{
    return g_flash_op.ota_info.write_flash(OTA_UPDATE_INFO,buf, len, 0);
}

int sota_init(sota_opt_t* flash_opt)
{
    int  ret;
    flag_op_s flag_op;

    if(flash_opt == NULL)
    {
        return SOTA_FAILED;
    }

    ret = pack_init_device(&flash_opt->ota_info);
    if (ret != SOTA_OK)
    {
        return SOTA_FAILED;
    }

    g_storage_device = pack_get_device();

    memcpy(&g_flash_op, flash_opt, sizeof(sota_opt_t));

    flag_op.func_flag_read = func_flag_read;
    flag_op.func_flag_write = func_flag_write;
    (void)flag_init(&flag_op);
    (void)flag_upgrade_init();

    rabuf = flash_opt->sota_malloc(flash_opt->frame_buf_len);
    if(rabuf == NULL)
    {
        SOTA_LOG("malloc rabuf failed");
        return SOTA_FAILED;
    }
    return sota_status_check();
}

