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
//#include "fota/ota_api.h"
#if USE_DIFF_UPGRADE
#include "ota_diff_upgrade.h"
#endif
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
static sota_op_t g_flash_op;
static at_update_record_t g_at_update_record = {0};
static uint8_t tmr_ticks = 0;
unsigned char *flashbuf = NULL;
unsigned char* rabuf = NULL;
unsigned int image_download_addr;
unsigned int flash_block_size;

void sota_deinit(void)
{
   
    if(rabuf != NULL)
        at_free(rabuf);
    if(flashbuf != NULL)
        at_free(flashbuf);
}

int sota_flash_write(const void* buffer, int32_t len, uint32_t offset)
{
    static uint32_t writed_len = 0;
    static int cnt = 0;
    int ret;
    ret = g_flash_op.read_block_flash(flashbuf, offset - writed_len, image_download_addr + writed_len);
    if(ret)
        AT_LOG("flash read err:%d", ret);
    if(offset + len < writed_len + flash_block_size)
    {
        memcpy(flashbuf + offset - writed_len, buffer, len);
        ret = g_flash_op.write_block_flash(flashbuf, flash_block_size, image_download_addr + writed_len);
        if(ret)
            AT_LOG("flash write err in:%d", ret);
        cnt++;
    }
    else
    {
        memcpy(flashbuf + offset - writed_len, buffer, flash_block_size - (offset - writed_len));
        ret = g_flash_op.write_block_flash(flashbuf, flash_block_size, image_download_addr + writed_len);
        if(ret)
            AT_LOG("flash write err ex1000:%d", ret);
        memset(flashbuf, 0, flash_block_size);
        memcpy(flashbuf, (unsigned char *)buffer + flash_block_size - (offset - writed_len), len + (offset - writed_len) - flash_block_size);
        ret = g_flash_op.write_block_flash(flashbuf, flash_block_size, image_download_addr + writed_len + flash_block_size);
        if(ret)
            AT_LOG("flash write err ex2:%d", ret);
        writed_len += flash_block_size;
        cnt = 0;
    }
    AT_LOG("writ:%d", cnt);
    return 0;
}

#define htons_ota(x) ((((x) & 0x00ff) << 8) | (((x) & 0xff00) >> 8))

int valid_check(char *rcvbuf, int32_t len)
{
    ota_pcp_head_s *pbuf;
    char *databuf,*rlen;
    int buflen;
    int ret,cmd_crc_num;
    char *buf;
    char cmd[2]={0};
    unsigned char tmpbuf[AT_DATA_LEN] = {0};
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
    sota_str_to_hex(buf+8, 4, (unsigned char*)cmd);
    cmd_crc_num = cmd[0]<<8 | cmd[1];
    memset(buf + 8, '0', 4);

    HexStrToByte((const unsigned char *)buf, tmpbuf, buflen);
    ret = crc_check(tmpbuf, buflen/2);
    memset(rabuf, 0, AT_DATA_LEN);
    sota_str_to_hex(buf, buflen, rabuf);
    pbuf = (ota_pcp_head_s *)rabuf;

    pbuf->ori_id = htons_ota(pbuf->ori_id);
    pbuf->chk_code = htons_ota(pbuf->chk_code);
    pbuf->data_len = htons_ota(pbuf->data_len);
    if(pbuf->ori_id != 0XFFFE || (pbuf->ver_num & 0xf) != 1 || (ret != cmd_crc_num) || \
            (pbuf->msg_code < MSG_GET_VER && pbuf->msg_code > MSG_NOTIFY_STATE))
    {
        SOTA_LOG("head wrong");
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

int at_fota_send(char *buf, int len)
{
    //平台的回应处理
    int ret;
    char crcretbuf[5] = {0};
    char tmpbuf[AT_DATA_LEN/4] = {0};
    ota_pcp_head_s pcp_head = {0};
    unsigned char atwbuf[AT_DATA_LEN/4] = {0};
    unsigned char hbuf[64] = {0};
    if(len > AT_DATA_LEN)
    {
        SOTA_LOG("payload too long");
        return -1;
    }
    pcp_head.ori_id = htons_ota(0xFFFE);
    pcp_head.ver_num = 1;
    pcp_head.msg_code = g_at_update_record.msg_code;
    pcp_head.data_len = htons_ota(len / 2);
    ver_to_hex((const char *)&pcp_head, sizeof(ota_pcp_head_s), (char *)hbuf);

    memcpy(atwbuf, hbuf, 16);
    memcpy(atwbuf + 16, buf, len);

    HexStrToByte(atwbuf, (unsigned char*)tmpbuf, len + 16); //strlen(atwbuf)
    ret = crc_check((unsigned char*)tmpbuf, (len + 16) / 2);
    sprintf(crcretbuf, "%04X", ret);

    memcpy(atwbuf + 8, crcretbuf, 4);
    return g_flash_op.sota_send((char *)atwbuf, len + 16);
    //nb_send_str((const char *)atwbuf, len + 16);
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
    char sbuf[64] = {0};
    uint8_t *pbuf = NULL;
    int ret = SOTA_OK;
    static int crc_code = 0;

    if(valid_check((char *)buf, buflen) != 0)
    {
        SOTA_LOG("valid_check wrong");
        return SOTA_FAILED;
    }

    if(g_at_update_record.block_len > 0)
        pbuf = rabuf + 8;
    switch(g_at_update_record.state)
    {
    case IDLE:
        if(g_at_update_record.msg_code == MSG_GET_VER)
        {
            char ver_ret[VER_LEN + 1] = {0};
            (void)g_flash_op.read_ver(ver_ret+1,VER_LEN);
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
                SOTA_LOG("no buf");
                return SOTA_FAILED;
            }
            ret = g_flash_op.notify_new_ver((char*)notify, sizeof(ota_ver_notify_t));
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
                    g_at_update_record.ver_chk_code = htons_ota(notify->ver_chk_code);
                    g_at_update_record.state = DOWNLOADING;
                    g_at_update_record.download_tmr = tmr_ticks;
                }
                else
                {
                    ver_ret[16] = (g_at_update_record.block_num >> 8) & 0XFF;
                    ver_ret[17] = g_at_update_record.block_num & 0XFF;
                }
                ver_to_hex(ver_ret, 18, sbuf);
                g_at_update_record.msg_code = MSG_GET_BLOCK;
                at_fota_send(sbuf, 18 * 2);
            }
        }
        else
        {
            char tmpbuf[2] = {1};
            SOTA_LOG("not cmd");
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
            SOTA_LOG("no buf");
            return SOTA_FAILED;
        }
        block_seq = ((*(pbuf + 1) << 8) & 0XFF00) | (*(pbuf + 2) & 0XFF);

        if(*pbuf != OTA_OK || g_at_update_record.block_num != block_seq \
                || g_at_update_record.state != DOWNLOADING)
        {
            SOTA_LOG("download wrong,we need %X, but is %X:",(int)g_at_update_record.block_num, (int)block_seq);
            g_at_update_record.state = IDLE;
            tmpbuf[1] = OTA_ERR;
            ver_to_hex(tmpbuf, 2, sbuf);
            at_fota_send(sbuf, 2 * 2);
            return SOTA_FAILED;
        }
        g_at_update_record.download_tmr = tmr_ticks;
#ifdef USE_DIFF
        #define HEAD_LEN 4 //????
        if(g_at_update_record.block_num == 0)
        {
            g_flash_op.write_block_flash((const char *)(pbuf + 3 + HEAD_LEN), g_at_update_record.block_size - HEAD_LEN,g_at_update_record.block_offset);
            crc_code = do_crc(crc_code, (unsigned char *)(pbuf + 3 + HEAD_LEN), (int)g_at_update_record.block_size - HEAD_LEN);
            g_at_update_record.block_offset += (g_at_update_record.block_size - HEAD_LEN);
            memcpy(tmpbuf, g_at_update_record.ver, 16);
            tmpbuf[16] = (g_at_update_record.block_num >> 8 & 0XFF);
            tmpbuf[17] = g_at_update_record.block_num & 0XFF;
            ver_to_hex(tmpbuf, 18, sbuf);
            at_fota_send(sbuf, 18 * 2);
            break;
        }
#endif
        sota_flash_write((const char *)(pbuf + 3), g_at_update_record.block_size,g_at_update_record.block_offset);
        crc_code = do_crc(crc_code, (unsigned char *)(pbuf + 3), (int)g_at_update_record.block_size);
        g_at_update_record.block_offset += g_at_update_record.block_size;
        if((++g_at_update_record.block_num) < g_at_update_record.block_totalnum)
        {
            memcpy(tmpbuf, g_at_update_record.ver, 16);
            tmpbuf[16] = (g_at_update_record.block_num >> 8 & 0XFF);
            tmpbuf[17] = g_at_update_record.block_num & 0XFF;
            ver_to_hex(tmpbuf, 18, sbuf);
            at_fota_send(sbuf, 18 * 2);
            break;
        }
        else//if((g_at_update_record.block_num) >= g_at_update_record.block_totalnum)
        {
            g_at_update_record.state = DOWNLOADED;
            SOTA_LOG("DOWNLOADED");
            if(g_at_update_record.ver_chk_code == crc_code)
                ret = OTA_OK;
            else
                ret = OTA_CHK_FAILED;
            SOTA_LOG("crc_code:%X ver_chk_code:%X ret:%d",crc_code, (unsigned int)g_at_update_record.ver_chk_code, ret);
            g_at_update_record.msg_code = MSG_UPDATE_STATE;
            g_at_update_record.state = DOWNLOADED;
            tmpbuf[0] = OTA_OK;
            ver_to_hex(tmpbuf, 1, sbuf);
            at_fota_send(sbuf, 2);
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
        tmpbuf[0] = OTA_OK;
        ver_to_hex(tmpbuf, 1, sbuf);
        at_fota_send(sbuf, 2);
        g_at_update_record.msg_code = MSG_NOTIFY_STATE;
        //ota_set_reboot(g_at_update_record.block_tolen);
    }
    //break;
    case UPDATING:
    {
        if(g_at_update_record.msg_code == MSG_NOTIFY_STATE)//this process should preserve in flash
        {
            g_flash_op.write_block_flash((void*)&g_at_update_record, sizeof(at_update_record_t),g_flash_op.sota_flag_addr);
            g_flash_op.sota_exec();
        }
        break;
    }
    case UPDATED:
        SOTA_LOG("this is the state after rebooting! wrong!");
        break;
    default:
        SOTA_LOG("cmd invalid");
        break;
    }
    return SOTA_OK;
}

static void sota_tmr(unsigned int argc)
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
            ver_to_hex(tmpbuf, 18, sbuf);
            at_fota_send(sbuf, 18 * 2);

        }
    }
    return;
}

void sota_status_check()
{
    if(g_flash_op.read_block_flash((char*)&g_at_update_record, sizeof(at_update_record_t),g_flash_op.sota_flag_addr))
    {
        SOTA_LOG("flag read err");
    }
    SOTA_LOG("state:%d flash ver:%s",g_at_update_record.state,g_at_update_record.ver);
    if(g_at_update_record.state == UPDATING)
    {
        char sbuf[64] = {0};
        char tmpbuf[17] = {0};
        tmpbuf[0] = OTA_OK;
        g_at_update_record.state = IDLE;
        g_flash_op.write_ver(g_at_update_record.ver, VER_LEN);
        SOTA_LOG("we send the ver:%s",g_at_update_record.ver);
        memcpy(tmpbuf + 1, g_at_update_record.ver, VER_LEN);
        g_flash_op.write_block_flash((const void*)sbuf, sizeof(at_update_record_t),g_flash_op.sota_flag_addr);//??state??
        ver_to_hex(tmpbuf, 17, sbuf);
        at_fota_send(sbuf, 17 * 2);
    }
}

int sota_init(sota_op_t* flash_op)
{
    if(flash_op == NULL)
        return SOTA_FAILED;

    memcpy(&g_flash_op, flash_op, sizeof(sota_op_t));
    image_download_addr = flash_op->image_addr;
    flash_block_size = flash_op->flash_block_size;
    sota_status_check();
    memset(&g_at_update_record, 0, sizeof(at_update_record_t));
    memcpy(g_at_update_record.ver, "V0.0", strlen("V0.0"));//
    flashbuf = at_malloc(FLASH_LEN);
    if(flashbuf == NULL)
    {
        SOTA_LOG("malloc flashbuf failed");
        return SOTA_FAILED;
    }
    rabuf =  at_malloc(AT_DATA_LEN);
    if(rabuf == NULL)
    {
        SOTA_LOG("malloc rabuf failed");
        at_free(flashbuf);
        return SOTA_FAILED;
    }
    return SOTA_OK;
}

