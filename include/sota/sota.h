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
#ifndef __SOTA_H__
#define __SOTA_H__

#include<stdint.h>

typedef enum{
    OTA_GET_VER,
    OTA_NOTIFY_NEW_VER,
    OTA_WRITE_BLOCK,
    OTA_UPDATE_EXC,
    OTA_ERASE_INFO,
    OTA_WRITE_INFO,
    OTA_FLAG_ADDR_SET,
    OTA_FLAG_ADDR_GET,
    OTA_GET_INFO,
    //OTA_GET_TIME_OUT,
    //OTA_NOTIFY_STATE
}OTA_CMD_E;

typedef enum{
    MSG_GET_VER=19,
    MSG_NOTIFY_NEW_VER,
    MSG_GET_BLOCK,
    MSG_UPDATE_STATE,
    MSG_EXC_UPDATE,
    MSG_NOTIFY_STATE=24
}MSG_CODE;

typedef  unsigned char   BYTE;
typedef  unsigned short   WORD;
#define USE_DIFF_UPGRADE  0
int ota_cmd_ioctl(OTA_CMD_E cmd, char* arg, int len);

#define MIN_PKT_LEN 4
#define FLASH_LEN 1024*4

typedef struct ota_pcp_head_t
{
    WORD ori_id;
    BYTE ver_num;
    BYTE msg_code;
    WORD chk_code;
    WORD data_len;
    //BYTE* data;
}ota_pcp_head_s;

typedef struct ota_ver_notify
{
    BYTE ver[16];
    WORD block_size;
    WORD block_totalnum;
    WORD ver_chk_code;
}ota_ver_notify_t;

typedef struct ota_get_block//send to cloud
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

typedef enum{
    IDLE=0,
    DOWNLOADING,
    DOWNLOADED,
    UPDATING,
    UPDATED,
}at_fota_state;

typedef enum
{
    OTA_OK = 0X0,
    OTA_BUSY = 0X1,
    OTA_DOWNLOAD_OVERTIME = 0X6,
    OTA_CHK_FAILED = 0X7,
    OTA_ERR,
}OTA_ERROR_E;

typedef struct
{
    int (*read_ver)(char* buf, uint32_t len);
    int (*notify_new_ver)(char* buf, uint32_t len);
    int (*write_ver)(char* buf, uint32_t len);
    int (*sota_send)(const char* buf, int len);
    void (*sota_exec)(void);
    int (*read_block_flash)(void* buf, int32_t len, uint32_t offset);
    int (*write_block_flash)(const void* buf, int32_t len, uint32_t offset);
    int (*erase_block_flash)(void* buf, int32_t len, uint32_t offset);
    //int (*read_upgrade_info)(char* buf, uint32_t len);
    //int (*write_upgrade_info)(char* buf, uint32_t len);
    unsigned int image_addr;
    unsigned int sota_flag_addr;
    unsigned int flash_block_size;
} sota_op_t;

int sota_init(sota_op_t* flash_op);
int32_t ota_process_main(void *arg, int8_t *buf, int32_t buflen);
void    sota_timer(void);

#define SOTA_DEBUG
#ifdef SOTA_DEBUG
#define SOTA_LOG(fmt, arg...)  printf("[%s:%d][I]"fmt"\n", __func__, __LINE__, ##arg)
#else
#define SOTA_LOG(fmt, arg...)
#endif

//extern void atiny_reboot(void);
//extern int nb_send_str(const char* buf, int len);
//extern int str_to_hex(const char *bufin, int len, char *bufout);
int ver_to_hex(const char *bufin, int len, char *bufout);

//int ota_cmd_ioctl(OTA_CMD_E cmd, char* arg, int len);

#define SOTA_OK    		 0
#define SOTA_FAILED 		-1
#define SOTA_TIMEOUT      -2
#endif
