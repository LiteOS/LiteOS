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

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <sota.h>

#include <crc.h>
#include <osal.h>
#include <link_misc.h>

#define cn_ver_length      16
#define cn_snd_buf_length  128


#ifdef SOTA_DEBUG
#define SOTA_LOG(fmt, ...) \
    do \
    { \
        osal_printf("[%s:%d][I]"fmt"\n", \
                              __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#else
#define SOTA_LOG(fmt, ...) ((void)0)
#endif

typedef enum
{
    MSG_GET_VER = 19,
    MSG_NOTIFY_NEW_VER,
    MSG_GET_BLOCK,
    MSG_DOWNLOAD_STATE,
    MSG_EXC_UPDATE,
    MSG_NOTIFY_STATE = 24
}en_msg_code;

typedef enum
{
    IDLE = 0,
    DOWNLOADING,
    UPDATING,
    UPDATED,
}en_sota_state;

typedef enum
{
    DEV_OK = 0x0,
    DEV_BUSYING = 0x1,
    DEV_WEAK_SIGNAL = 0x2,
    DEV_LATEST_VER = 0x3,
    DEV_LOW_BATTERY = 0x4,
    DEV_NO_SPACE = 0x5,
    DOWNLOAD_TIME_OUT = 0x6,
    FIRMWARE_CHECK_ERROR = 0x7,
    FIRMWARE_NOT_MATCH = 0x8,
    DEV_MEMORY_EXHAUSTED = 0x9,
    DEV_INNER_ERROR = 0x7f,
    UPDATE_TASK_EXIT = 0x80,
    REQUEST_BLOCK_INVALID = 0x81
}response_code_e;

typedef enum
{
    SOTA_OK = 0,
    SOTA_DOWNLOADING = 1,
    SOTA_UPDATING    = 2,
    SOTA_UPDATED     = 3,
    SOTA_FAILED             = 101,
    SOTA_EXIT               = 102,
    SOTA_INVALID_PACKET     = 103,
    SOTA_UNEXPECT_PACKET    = 104,
    SOTA_WRITE_FLASH_FAILED = 105
} en_sota_ret;


#pragma pack(1)


typedef struct sota_update_info
{
    uint16_t block_size;
    uint32_t block_num;
    uint32_t block_offset;
    uint32_t block_totalnum;
    uint32_t block_tolen;
    uint32_t ver_chk_code;
    char           ver[cn_ver_length];
    en_sota_state  state;
    uint32_t crc;
} sota_upgrade_info_t;

typedef struct
{
    uint16_t  ori_id;
    uint8_t   ver_num;
    uint8_t   msg_code;
    uint16_t  chk_code;
    uint16_t  data_len;
}sota_pcp_head_t;

typedef struct
{
    int8_t   ver[cn_ver_length];
    uint16_t block_size;
    uint16_t block_totalnum;
    uint16_t ver_chk_code;
}sota_notify_version_t;

typedef struct
{
    uint8_t   ver[cn_ver_length];
    uint16_t  blocknum;
}sota_block_request_t;

typedef struct
{
    uint8_t retcode;
}sota_response_t;


#pragma pack()

typedef int (*fn_sota_msg_send)(void *msg,int len);
typedef struct
{
    sota_upgrade_info_t  record;
    fn_sota_msg_send     msg_send;
}sota_cb_t;

static sota_cb_t  s_sota_cb;

#define cn_pcp_head           0xFFFE
#define cn_block_head_size    3


static int sota_flag_save()
{
    return 0;
}


static int msg_send(void *msg, int len)
{
    int ret = -1;

    if(NULL != s_sota_cb.msg_send)
    {
        ret = s_sota_cb.msg_send(msg,len);
    }

    return ret;
}


static int sota_msg_send(en_msg_code msg_code, uint8_t *buf, int32_t len)
{
    int32_t ret;
    sota_pcp_head_t  *pcp;
    uint8_t         *msg_buf;
    uint32_t         msg_len;

    ret = -1;
    msg_len = len  + sizeof(sota_pcp_head_t);
    if (msg_len>= cn_snd_buf_length)
    {
        SOTA_LOG("payload too long");
        return ret;
    }

    msg_buf = osal_malloc(msg_len);
    if(NULL == msg_buf)
    {
        SOTA_LOG("message memory buffer error");
        return ret;
    }

    pcp->ori_id = htons_ota(cn_pcp_head);
    pcp->ver_num =1;
    pcp->msg_code = msg_code;
    pcp->data_len = len;
    memcpy(msg_buf+sizeof(sota_pcp_head_t),buf,len);
    ret = msg_send(msg_buf,msg_len);

    osal_free(msg_buf);

    return ret;
}

static void sota_send_request_block(char *ver)
{
    sota_block_request_t request;
    memcpy(request.ver,s_sota_cb.record.ver,cn_ver_length);
    request.blocknum = htons(s_sota_cb.record.block_num);

    sota_msg_send(MSG_GET_BLOCK, &request, sizeof(request));
}

static void sota_send_response_code(en_msg_code msg_code, response_code_e code)
{
    
    sota_response_t  response;

    response.retcode = code;

    sota_msg_send(msg_code, &response, sizeof(response));
}

static void sota_reset_record_info(sota_notify_version_t *notify)
{
    s_sota_cb.record.block_offset = 0;
    s_sota_cb.record.block_size = htons_ota(notify->block_size);
    s_sota_cb.record.block_totalnum = htons_ota(notify->block_totalnum);
    s_sota_cb.record.block_num = 0;
    s_sota_cb.record.block_tolen = 0;
    s_sota_cb.record.ver_chk_code = notify->ver_chk_code;
    memcpy(s_sota_cb.record.ver, notify->ver, cn_ver_length);
    s_sota_cb.record.state = DOWNLOADING;

    sota_flag_save();
}

static int32_t sota_new_ver_process(const sota_pcp_head_t *head, const uint8_t *pbuf)
{
    char ver[cn_ver_length];
    sota_notify_version_t *notify = (sota_notify_version_t *)pbuf;
    
    (void)g_flash_op.get_ver(ver, cn_ver_length);
    if (strncmp(ver, (const char*)notify->ver, cn_ver_length) == 0)
    {
        SOTA_LOG("Already latest version %s", notify->ver);
        sota_send_response_code(MSG_NOTIFY_NEW_VER, DEV_LATEST_VER);
        s_sota_cb.record.state = IDLE;
        return SOTA_OK;
    }

    SOTA_LOG("Notify ver %s,%x, record ver:%s,%x", notify->ver, notify->ver_chk_code, 
        s_sota_cb.record.ver,s_sota_cb.record.ver_chk_code);
    if ((strncmp(s_sota_cb.record.ver, (const char *)notify->ver, cn_ver_length) == 0)
        && (notify->ver_chk_code == s_sota_cb.record.ver_chk_code))
    {
        SOTA_LOG("state %d, downloaded %d blocks", s_sota_cb.record.state, s_sota_cb.record.block_num);
        if (s_sota_cb.record.block_num < s_sota_cb.record.block_totalnum
            && s_sota_cb.record.state == DOWNLOADING)
        {
            sota_send_request_block((char*)notify->ver);
            return SOTA_DOWNLOADING;
        }
        else if (s_sota_cb.record.block_num == s_sota_cb.record.block_totalnum
            && s_sota_cb.record.state == UPDATING)
        {
            sota_send_response_code(MSG_DOWNLOAD_STATE, DEV_OK);
            return SOTA_UPDATING;
        }
        else if (s_sota_cb.record.block_num == s_sota_cb.record.block_totalnum
            && s_sota_cb.record.state == UPDATED)
        {
            return SOTA_UPDATED;
        }
    }
    
    sota_reset_record_info(notify);
    sota_send_request_block((char*)notify->ver);
    return SOTA_DOWNLOADING;
}

static int32_t sota_data_block_process(const sota_pcp_head_t *head, const uint8_t *pbuf)
{
    uint16_t block_seq = 0;
    int ret = SOTA_OK;
    
    if (s_sota_cb.record.state != DOWNLOADING)
    {
       return SOTA_UNEXPECT_PACKET;
    }
    
    if (*pbuf == UPDATE_TASK_EXIT)
    {
        s_sota_cb.record.state = IDLE;
        return SOTA_EXIT;
    }

    block_seq = ((*(pbuf + 1) << 8) & 0XFF00) | (*(pbuf + 2) & 0XFF);
    if (s_sota_cb.record.block_num != block_seq)
    {
        SOTA_LOG("Download wrong,we need block %X, but rx %X:",(int)s_sota_cb.record.block_num, (int)block_seq);
        return SOTA_UNEXPECT_PACKET;
    }
    SOTA_LOG("off:%lx size:%x ",s_sota_cb.record.block_offset,head->data_len);
    ret = g_storage_device->write_software(g_storage_device, s_sota_cb.record.block_offset,(const uint8_t *)(pbuf + cn_block_head_size), head->data_len);
    if (ret != SOTA_OK)
    {
        SOTA_LOG("write software failed. ret:%d", ret);
        sota_send_response_code(MSG_DOWNLOAD_STATE, DEV_NO_SPACE);
        return SOTA_WRITE_FLASH_FAILED;
    }
    
    s_sota_cb.record.block_offset += s_sota_cb.record.block_size;
    s_sota_cb.record.block_tolen += head->data_len;
    s_sota_cb.record.block_num++;
    
    if ((s_sota_cb.record.block_num) < s_sota_cb.record.block_totalnum)
    {
        SOTA_LOG("Rx total %d bytes downloading\r\n", s_sota_cb.record.block_tolen);
        sota_send_request_block(s_sota_cb.record.ver);
        return SOTA_DOWNLOADING;
    } 
    else
    { 
        SOTA_LOG("Rx total %d bytes, UPDATING...\r\n", s_sota_cb.record.block_tolen);
        ret = g_storage_device->write_software_end(g_storage_device, PACK_DOWNLOAD_OK, s_sota_cb.record.block_tolen);
        if (ret != SOTA_OK)
        {
            SOTA_LOG("write software end ret:%d", ret);
            sota_send_response_code(MSG_DOWNLOAD_STATE, FIRMWARE_CHECK_ERROR);
            return SOTA_WRITE_FLASH_FAILED;
        }
        else
        {
            s_sota_cb.record.state = UPDATING;
            sota_send_response_code(MSG_DOWNLOAD_STATE, DEV_OK);
            return SOTA_UPDATING;
        }
    } 
}

static int32_t sota_update_exc_process(const sota_pcp_head_t *head, const uint8_t *pbuf)
{
    int ret = SOTA_OK;

    SOTA_LOG("Begin excute update");
    if (s_sota_cb.record.state != UPDATING)
    {
        return SOTA_UNEXPECT_PACKET;
    }
   
    ret = g_storage_device->active_software(g_storage_device);
    if (ret != SOTA_OK)
    {
        SOTA_LOG("Active software failed ret:%d.", ret);
        sota_send_response_code(MSG_EXC_UPDATE, DEV_INNER_ERROR);
        return SOTA_WRITE_FLASH_FAILED;
    }
    else
    {
        s_sota_cb.record.state = UPDATED;
       (void)flag_write(FLAG_APP, (void*)&s_sota_cb.record, sizeof(sota_upgrade_info_t));
        sota_send_response_code(MSG_EXC_UPDATE, DEV_OK);
        return SOTA_UPDATED;
    }
}
int32_t sota_process(void *arg, const int8_t *buf, int32_t buflen)
{
    char sbuf[64] = {0};
    const uint8_t *pbuf = NULL;
    int ret = SOTA_OK;
    sota_pcp_head_t *phead;
    unsigned char  msg_code;

    phead =(sota_pcp_head_t *)buf;
    msg_code = phead->msg_code;

    if (phead->data_len > 0)
    {
        pbuf = (uint8_t *)buf + cn_ver_length/2;
    }

    SOTA_LOG("process sota msg %d", msg_code);
    
    switch (msg_code)
    {
        case MSG_GET_VER:
        { 
            char ver_ret[cn_ver_length + 1] = {0};
            (void)g_flash_op.get_ver(ver_ret+1, cn_ver_length);
            (void)ver_to_hex(ver_ret, (cn_ver_length + 1), (char *)sbuf);
            (void)sota_msg_send(MSG_GET_VER, (char *)sbuf, (cn_ver_length + 1) * 2);
            ret = SOTA_OK;
            break;
        }
        case MSG_NOTIFY_NEW_VER:
        {
            if (phead->data_len > sizeof(sota_notify_version_t))
            {
               ret = sota_new_ver_process(phead, pbuf);
            }
            else
            {
                ret = SOTA_INVALID_PACKET;
            }
            break;
        }  
        case MSG_GET_BLOCK:
        {
            if (phead->data_len > 0)
            {
               ret = sota_data_block_process(phead, pbuf);
            }
            else
            {
                ret = SOTA_INVALID_PACKET;
            }
            break;
        }
        case MSG_EXC_UPDATE:
        {
             ret = sota_update_exc_process(phead, pbuf);
             break;
        }
        default:
        {
            SOTA_LOG("Rx invalid packet");
            ret = SOTA_INVALID_PACKET;
            break;
        }
    }
    return ret;
}

void sota_timeout_handler(void)
{
    if (s_sota_cb.record.state == DOWNLOADING)
    {
        SOTA_LOG("Download block %d over time", s_sota_cb.record.block_num);
        sota_send_response_code(MSG_EXC_UPDATE, DOWNLOAD_TIME_OUT);
        sota_send_request_block(s_sota_cb.record.ver);
    }
    else if (s_sota_cb.record.state == UPDATING)
    {
        SOTA_LOG("Download finish. excute over time");
        sota_send_response_code(MSG_EXC_UPDATE, DOWNLOAD_TIME_OUT);
        sota_send_response_code(MSG_DOWNLOAD_STATE, DEV_OK);
    }
}

static int sota_status_check(void)
{
    upgrade_state_e state;
    char sbuf[64] = {0};
    char tmpbuf[cn_ver_length+1] = {0};

    memset(&s_sota_cb.record, 0, sizeof(sota_upgrade_info_t));
    if (flag_read(FLAG_APP, (char*)&s_sota_cb.record, sizeof(sota_upgrade_info_t)))
    {
        SOTA_LOG("flag read err");
        return SOTA_FAILED;
    }
    SOTA_LOG("state:%d flash ver:%s",s_sota_cb.record.state, s_sota_cb.record.ver);

    if (g_flash_op.firmware_download_stage == BOOTLOADER
        && g_flash_op.current_run_stage == BOOTLOADER)
    {
        if (s_sota_cb.record.state == DOWNLOADING)
        {
            sota_send_request_block(s_sota_cb.record.ver);
            return SOTA_DOWNLOADING;
        }
    }
    else
    {
        (void)flag_upgrade_get_result(&state);
        SOTA_LOG("upgrade result: %d", state);
        if (state == OTA_SUCCEED)
        {
            SOTA_LOG("Update version %s success", s_sota_cb.record.ver);
            memcpy(tmpbuf + 1, s_sota_cb.record.ver, cn_ver_length);
            (void)ver_to_hex(tmpbuf, cn_ver_length+1, sbuf);
            (void)sota_msg_send(MSG_NOTIFY_STATE, sbuf, (cn_ver_length+1) * 2);
        }
    }

    memset(&s_sota_cb.record, 0, sizeof(sota_upgrade_info_t));
    (void)flag_write(FLAG_APP, (const void*)&s_sota_cb.record, sizeof(sota_upgrade_info_t));
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

int32_t sota_init(const sota_arg_s* sota_arg)
{
    int  ret;
    flag_op_s flag_op;
    pack_params_s pack_param;

    if (sota_arg == NULL || sota_arg->sota_malloc == NULL || sota_arg->sota_free == NULL)
    {
        return SOTA_FAILED;
    }

    memcpy(&pack_param.ota_opt, &sota_arg->ota_info, sizeof(pack_param.ota_opt));
    pack_param.malloc = sota_arg->sota_malloc;
    pack_param.free = sota_arg->sota_free;
    pack_param.printf = sota_arg->sota_printf;
    ret = pack_init_device(&pack_param);
    if (ret != SOTA_OK)
    {
        return SOTA_FAILED;
    }

    g_storage_device = pack_get_device();

    memcpy(&g_flash_op, sota_arg, sizeof(sota_arg_s));

    flag_op.func_flag_read = func_flag_read;
    flag_op.func_flag_write = func_flag_write;
    (void)flag_init(&flag_op);
    (void)flag_upgrade_init();

    return sota_status_check();
}

