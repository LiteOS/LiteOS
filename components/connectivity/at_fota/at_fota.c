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
#include "at_fota.h"
#include "at_fota_hal.h"
#include "atadapter.h"
#include "ota.h"
#include <board.h>
#include "los_swtmr.h"
#include "hal_spi_flash.h"

extern at_task at;
#define VER_LEN  16
#define DOWNLOADTIME_LIMIT  100
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
static at_update_record_t g_at_update_record = {0};
unsigned char stmpbuf[1064];
char sbuf[1064];
unsigned char rabuf[1064];
static uint8_t tmr_ticks = 0;

//read write flash
int ota_cmd_ioctl(OTA_CMD_E cmd, char *arg, int len)
{
    switch(cmd)
    {
    case OTA_GET_VER:
    {
        char *ver_ret = arg;
        *ver_ret = 0;
        memcpy(ver_ret + 1, g_at_update_record.ver, 16);

        break;
    }
    case OTA_WRITE_BLOCK:
    {
        //AT_LOG("w flash off:%04X %04X",g_at_update_record.block_offset,len);
        at_fota_write(g_at_update_record.block_offset, arg, len);
        break;
    }
    case OTA_NOTIFY_NEW_VER:
        break;
    case OTA_UPDATE_EXC:
    {
        atiny_reboot();
    }
    }
    return 0;
}

#define htons_ota(x) ((((x) & 0x00ff) << 8) | (((x) & 0xff00) >> 8))

int valid_check(char *buf, int32_t buflen)
{
    ota_pcp_head_s *pbuf;
    //int ret;
    if(buf == NULL || buflen <= MIN_PKT_LEN)
    {
        AT_LOG("buf null");
        return -1;
    }
    //memset(buf+8,'0',4);
    sprintf(buf + 8, "%04X", '0');
    sprintf(buf + 10, "%02X", '0');
    memset(stmpbuf, 0, 1024);
    HexStrToByte((const unsigned char *)buf, stmpbuf, buflen); //strlen(buf)
    (void)crc_check(stmpbuf, buflen);
    memset(rabuf, 0, 1064);
    sota_str_to_hex(buf, buflen * 2, rabuf);
    pbuf = (ota_pcp_head_s *)rabuf;

    pbuf->ori_id = htons_ota(pbuf->ori_id);
    pbuf->chk_code = htons_ota(pbuf->chk_code);
    pbuf->data_len = htons_ota(pbuf->data_len);
    if(pbuf->ori_id != 0XFFFE || (pbuf->ver_num & 0xf) != 1 || \
            (pbuf->msg_code < MSG_GET_VER && pbuf->msg_code > MSG_NOTIFY_STATE))
    {
        AT_LOG("head wrong");
        return -1;
    }
    g_at_update_record.msg_code = pbuf->msg_code;
    g_at_update_record.chk_code = pbuf->chk_code;
    if(pbuf->msg_code == MSG_GET_BLOCK)
    {
        g_at_update_record.block_tolen += pbuf->data_len;
    }
    g_at_update_record.block_len = pbuf->data_len;
    return 0;
}

unsigned char atwbuf[1064];
unsigned char hbuf[64];

int at_fota_send(char *buf, int len)
{
    //平台的回应处理
    int ret;
    char crcretbuf[5] = {0};
    ota_pcp_head_s pcp_head = {0};
    if(len > 1024)
    {
        AT_LOG("payload too long");
        return -1;
    }
    pcp_head.ori_id = htons_ota(0xFFFE);
    pcp_head.ver_num = 1;
    pcp_head.msg_code = g_at_update_record.msg_code;
    pcp_head.data_len = htons_ota(len / 2);
    memset(hbuf, 0, 64);
    str_to_hex((const char *)&pcp_head, sizeof(ota_pcp_head_s), (char *)hbuf);

    memset(atwbuf, 0, 1064);

    memcpy(atwbuf, hbuf, 16);
    memcpy(atwbuf + 16, buf, len);

    memset(stmpbuf, 0, 1024);
    HexStrToByte(atwbuf, stmpbuf, len + 16); //strlen(atwbuf)
    ret = crc_check(stmpbuf, strlen((const char *)atwbuf) / 2);
    sprintf(crcretbuf, "%04X", ret);

    memcpy(atwbuf + 8, crcretbuf, 4);
    return nb_send_str((const char *)atwbuf, len + 16);
}

int ota_report_result(void)
{
    return -1;

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

int32_t ota_process_main(void *arg, int8_t *buf, int32_t buflen)
{
    uint8_t *pbuf = NULL;
    int ret = 0;

    if(valid_check((char *)buf, buflen) != 0)
    {
        AT_LOG("valid_check wrong");
        return -1;
    }

    if(g_at_update_record.block_len > 0)
        pbuf = rabuf + 8;
    switch(g_at_update_record.state)
    {
    case IDLE:
        if(g_at_update_record.msg_code == MSG_GET_VER)
        {
            char ver_ret[VER_LEN + 1] = {0};
            (void)ota_cmd_ioctl(OTA_GET_VER, ver_ret, 17);
            memset(sbuf, 0, 1024);
            ver_to_hex(ver_ret, (VER_LEN + 1), (char *)sbuf);
            at_fota_send((char *)sbuf, (VER_LEN + 1) * 2);
        }
        else if(g_at_update_record.msg_code == MSG_NOTIFY_NEW_VER)
        {
            ota_ver_notify_t *notify = (ota_ver_notify_t *)pbuf;
            char tmpbuf[2] = {0};
            char ver_ret[VER_LEN + 2] = {0};
            if(notify == NULL)
            {
                AT_LOG("no buf");
                return -1;
            }
            ret = (char)ota_cmd_ioctl(OTA_NOTIFY_NEW_VER, (char *)notify, sizeof(ota_ver_notify_t));
            memset(sbuf, 0, 1064);
            ver_to_hex(tmpbuf, 2, (char *)sbuf);
            at_fota_send((char *)sbuf, 2);
            if(ret == OTA_OK)
            {
                memcpy(ver_ret, notify->ver, VER_LEN);
                //AT_LOG("size:%X num:%X code:%X now code:%X",htons_ota(notify->block_size),htons_ota(notify->block_totalnum),htons_ota(notify->ver_chk_code),g_at_update_record.ver_chk_code);
                if(htons_ota(notify->ver_chk_code) != g_at_update_record.ver_chk_code)
                {
                    g_at_update_record.block_offset = 0;
                    g_at_update_record.block_size = htons_ota(notify->block_size);
                    g_at_update_record.block_totalnum = htons_ota(notify->block_totalnum);
                    memcpy(g_at_update_record.ver, notify->ver, VER_LEN);
                    ver_ret[16] = ver_ret[17] = 0;
                    g_at_update_record.block_num = 0;
                    //g_at_update_record清空操作
                    g_at_update_record.ver_chk_code = htons_ota(notify->ver_chk_code);//
                    g_at_update_record.state = DOWNLOADING;
                    g_at_update_record.download_tmr = tmr_ticks;
                }
                else
                {
                    ver_ret[16] = (g_at_update_record.block_num >> 8) & 0XFF;
                    ver_ret[17] = g_at_update_record.block_num & 0XFF;
                }
                memset(sbuf, 0, 1064);
                ver_to_hex(ver_ret, 18, sbuf);
                g_at_update_record.msg_code = MSG_GET_BLOCK;
                at_fota_send(sbuf, 18 * 2);
            }
        }
        else
        {
            char tmpbuf[2] = {1};
            AT_LOG("not cmd");
            memset(sbuf, 0, 1064);
            ver_to_hex(tmpbuf, 1, (char *)sbuf);
            at_fota_send((char *)sbuf, 2);
        }
        break;
    case DOWNLOADING:
    {
        char tmpbuf[VER_LEN + 2] = {0};
        uint16_t block_seq = 0;
        if(pbuf == NULL)
        {
            AT_LOG("no buf");
            return -1;
        }
        block_seq = ((*(pbuf + 1) << 8) & 0XFF00) | (*(pbuf + 2) & 0XFF);

        if(*pbuf != OTA_OK || g_at_update_record.block_num != block_seq \
                || g_at_update_record.state != DOWNLOADING)
        {
            AT_LOG("download wrong");
#if 1
            g_at_update_record.state = IDLE;
            memset(sbuf, 0, 1064);
            tmpbuf[1] = OTA_ERR;
            ver_to_hex(tmpbuf, 2, sbuf);
            at_fota_send(sbuf, 2 * 2);
            return -1;
#endif
        }
        g_at_update_record.download_tmr = tmr_ticks;
        //AT_LOG("pbuff:%02X%02X%02X-%02X%02X%02X%02X%02X%02X%02X%02X",*pbuf,*(pbuf+1),*(pbuf+2),*(pbuf+3),*(pbuf+4),*(pbuf+5),*(pbuf+6),*(pbuf+7),*(pbuf+8),*(pbuf+9),*(pbuf+10));
        (void)ota_cmd_ioctl(OTA_WRITE_BLOCK, (char *)(pbuf + 3), g_at_update_record.block_size);
        g_at_update_record.block_offset += g_at_update_record.block_size;
        if((++g_at_update_record.block_num) < g_at_update_record.block_totalnum)
        {
            //AT_LOG("DOWNLOADing");
            memset(sbuf, 0, 1064);
            memcpy(tmpbuf, g_at_update_record.ver, 16);
            tmpbuf[16] = (g_at_update_record.block_num >> 8 & 0XFF);
            tmpbuf[17] = g_at_update_record.block_num & 0XFF;
            ver_to_hex(tmpbuf, 18, sbuf);
            at_fota_send(sbuf, 18 * 2);
        }
        else//if((g_at_update_record.block_num) >= g_at_update_record.block_totalnum)
        {
            g_at_update_record.state = DOWNLOADED;
            AT_LOG("DOWNLOADED");
            if(g_at_update_record.ver_chk_code == do_crc(0, (unsigned char *)OTA_IMAGE_DOWNLOAD_ADDR, (int)g_at_update_record.block_tolen))
                ret = OTA_OK;
            else
                ret = OTA_CHK_FAILED;
            AT_LOG("ver_chk_code:%X ret:%d", (unsigned int)g_at_update_record.ver_chk_code, ret);
            g_at_update_record.msg_code = MSG_UPDATE_STATE;
            g_at_update_record.state = DOWNLOADED;
            memset(sbuf, 0, 1064);
            tmpbuf[0] = OTA_OK;
            ver_to_hex(tmpbuf, 1, sbuf);
            at_fota_send(sbuf, 2);
        }
        break;
    }
    case DOWNLOADED:
        if(g_at_update_record.msg_code == MSG_UPDATE_STATE)
        {
            AT_LOG("platform ack");
            g_at_update_record.msg_code = MSG_EXC_UPDATE;
            //break;//平台下发开始升级报文过快，板端无法处理
        }
        if(g_at_update_record.msg_code == MSG_EXC_UPDATE)
        {
            char tmpbuf[1] = {0};
            AT_LOG("begin update and send");
            g_at_update_record.state = UPDATING;
            memset(sbuf, 0, 1064);
            tmpbuf[0] = OTA_OK;
            ver_to_hex(tmpbuf, 1, sbuf);
            at_fota_send(sbuf, 2);
            g_at_update_record.msg_code = MSG_NOTIFY_STATE;
            ota_set_reboot(g_at_update_record.block_tolen);
        }
    //break;
    case UPDATING:
    {
        if(g_at_update_record.msg_code == MSG_NOTIFY_STATE)
        {
            ota_update_exc_s update_info;
            char tmpbuf[17] = {0};
            tmpbuf[0] = OTA_OK;
            memcpy(tmpbuf + 1, g_at_update_record.ver, VER_LEN);
            ver_to_hex(tmpbuf, 17, sbuf);
            at_fota_send(sbuf, 17 * 2);
            memcpy(update_info.ver, g_at_update_record.ver, VER_LEN);
            update_info.len = g_at_update_record.block_tolen;
            update_info.ver_chk_code = g_at_update_record.ver_chk_code;
            g_at_update_record.state = UPDATED;
            (void)ota_cmd_ioctl(OTA_UPDATE_EXC, (char *)&update_info, sizeof(ota_update_exc_s));
        }
        //break;
    }
    case UPDATED:
        AT_LOG("update success");
        if(pbuf == OTA_OK)
        {
            memset(&g_at_update_record, 0, sizeof(at_update_record_t));
        }
        break;
    default:
        AT_LOG("cmd invalid");
        break;
    }
    return 0;
}

static void sota_tmr(unsigned int argc)
{
    if(g_at_update_record.state == DOWNLOADING)
    {
        if(++tmr_ticks - g_at_update_record.download_tmr > DOWNLOADTIME_LIMIT)
        {
            AT_LOG("over time");
            //at_fota_send(sbuf,18*2);
        }
    }
    return;
}

int hal_init_sota(void)
{
    ota_assist assist;
    int ret;

    hal_spi_flash_config();
    assist.func_printf = printf;
    assist.func_ota_read = hal_spi_flash_read;
    assist.func_ota_write = hal_spi_flash_erase_write;
    ota_register_assist(&assist);

    ret = ota_init();
    if (ret != OTA_OK)
    {
        AT_LOG("read/write boot information failed");
    }

    return ret;

}

uint16_t at_fota_timer = -1;
int at_ota_init(char *featurestr, int cmdlen)
{
    if(featurestr == NULL || cmdlen <= 0 || cmdlen >= OOB_CMD_LEN - 1)
        return -1;

    if(LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_PERIOD, sota_tmr, &at_fota_timer, 1, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE) != 0)
    {
        AT_LOG("create stmr failed");
        return -1;
    }//deinit:delete timer.
    //LOS_SwtmrStart(at_fota_timer);
    memset(&g_at_update_record, 0, sizeof(at_update_record_t));
    memcpy(g_at_update_record.ver, "V0.0", strlen("V0.0"));
    hal_spi_flash_config();
    hal_init_sota();
    return at.oob_register(featurestr, cmdlen, ota_process_main);
}


