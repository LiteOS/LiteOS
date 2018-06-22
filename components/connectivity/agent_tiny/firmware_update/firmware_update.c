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

#include "internals.h"
#include "agenttiny.h"
#include "firmware_update.h"
#include "atiny_update_info.h"

#define FW_PRODUCT_ID "ATINY"
#define FW_PRODUCT_ID_MAX_LEN (8)
#define FW_BLOCK_SIZE (512)

#define COAP_PROTO_PREFIX "coap://"
#define COAPS_PROTO_PREFIX "coaps://"

typedef enum fw_record_data_type
{
    FW_RECORD_BLOCK_NUM,
    FW_RECORD_BLOCK_MORE,
    FW_RECORD_BLOCK_SIZE,
    FW_RECORD_BLOCK_OFFSET,
    FW_RECORD_RUI,
    FW_RECORD_TYPE_MAX
}fw_record_data_type_e;

typedef struct fw_update_record
{
    uint32_t record_len;
    uint8_t in_use;
    unsigned char product_id[FW_PRODUCT_ID_MAX_LEN];
    uint32_t block_num;
    uint8_t block_more;
    uint16_t block_size;
    uint32_t block_offset;
    uint32_t uri_len;
    char *uri;
}fw_update_record_t;

static char *g_ota_uri = NULL;
static atiny_fota_storage_device_s *g_fota_storage_device = NULL;
static fw_update_record_t g_fw_update_record;
static firmware_update_notify g_firmware_update_notify = NULL;
static void *g_firmware_update_notify_param = NULL;

static void firmware_download_reply(lwm2m_transaction_t * transacP,
                                        void * message)
{
    coap_packet_t *packet = (coap_packet_t *)message;
    lwm2m_context_t *contextP = (lwm2m_context_t *)(transacP->userData);
    lwm2m_transaction_t * transaction;
    uint32_t len = 0;
    uint32_t block_num = 0;
    uint8_t block2_more = 0;
    uint16_t block_size = FW_BLOCK_SIZE;
    uint32_t block_offset = 0;

    if(1 != coap_get_header_block2(message, &block_num, &block2_more, &block_size, &block_offset))
    {
        printf("[%s][%d] coap_get_header_block2 failed\n", __FUNCTION__, __LINE__);
        goto failed_exit;
    }
    printf("[%s][%d] block_num : %lu, block2_more : %lu, block_size : %lu, block_offset : %lu\n", __FUNCTION__, __LINE__, block_num, (uint32_t)block2_more, (uint32_t)block_size, block_offset);
    printf("[%s][%d] payload_len : %u\n", __FUNCTION__, __LINE__, packet->payload_len);

    if(0 == block_num)
    {
        g_fw_update_record.block_size = block_size;
        g_fw_update_record.block_num = block_num;
        g_fw_update_record.block_offset = block_offset;
        g_fw_update_record.block_more = block2_more;
    }

    len = (uint32_t)(packet->payload_len);
    if(g_fota_storage_device && g_fota_storage_device->write_software)
        g_fota_storage_device->write_software(g_fota_storage_device, block_offset, packet->payload, len);
    else if(NULL == g_fota_storage_device)
        printf("[%s][%d] g_fota_storage_device NULL\n", __FUNCTION__, __LINE__);
    else
        printf("[%s][%d] g_fota_storage_device->write_software NULL\n", __FUNCTION__, __LINE__);

    len = block_offset + (uint32_t)(packet->payload_len);

    if(block2_more)
    {
        transaction = transaction_new(transacP->peerH, COAP_GET, NULL, NULL, contextP->nextMID++, 4, NULL);
        if(!transaction)
        {
            printf("[%s][%d] transaction_new failed\n", __FUNCTION__, __LINE__);
            goto failed_exit;
        }
        coap_set_header_uri_path(transaction->message, g_ota_uri);
        //coap_set_header_uri_query(transaction->message, query);
        coap_set_header_block2(transaction->message, block_num+1, 0, g_fw_update_record.block_size);
        printf("[%s][%d] get next : %lu\n", __FUNCTION__, __LINE__, block_num+1);

        transaction->callback = firmware_download_reply;
        transaction->userData = (void *)contextP;

        contextP->transactionList = (lwm2m_transaction_t *)LWM2M_LIST_ADD(contextP->transactionList, transaction);

        if(transaction_send(contextP, transaction) != 0)
        {
            printf("[%s][%d] transaction_send failed\n", __FUNCTION__, __LINE__);
            goto failed_exit;
        }
    }
    else
    {
        if(g_fota_storage_device && g_fota_storage_device->write_software_end)
            g_fota_storage_device->write_software_end(g_fota_storage_device, ATINY_FOTA_DOWNLOAD_OK, len);
        else if(NULL == g_fota_storage_device)
            printf("[%s][%d] g_fota_storage_device NULL\n", __FUNCTION__, __LINE__);
        else
            printf("[%s][%d] g_fota_storage_device->write_software_end NULL\n", __FUNCTION__, __LINE__);
        printf("[%s][%d] g_firmware_update_notify FIRMWARE_UPDATE_RST_SUCCESS\n", __FUNCTION__, __LINE__);
        if(g_firmware_update_notify)
            g_firmware_update_notify(FIRMWARE_UPDATE_RST_SUCCESS, g_firmware_update_notify_param);
        printf("[%s][%d] download %s success, total size : %lu\n", __FUNCTION__, __LINE__, g_fw_update_record.uri, len);
    }
    return;
failed_exit:
    if(g_fota_storage_device && g_fota_storage_device->write_software_end)
        g_fota_storage_device->write_software_end(g_fota_storage_device, ATINY_FOTA_DOWNLOAD_FAIL, len);
    else if(NULL == g_fota_storage_device)
        printf("[%s][%d] g_fota_storage_device NULL\n", __FUNCTION__, __LINE__);
    else
        printf("[%s][%d] g_fota_storage_device->write_software_end NULL\n", __FUNCTION__, __LINE__);
    printf("[%s][%d] g_firmware_update_notify FIRMWARE_UPDATE_RST_FAILED\n", __FUNCTION__, __LINE__);
    if(g_firmware_update_notify)
            g_firmware_update_notify(FIRMWARE_UPDATE_RST_FAILED, g_firmware_update_notify_param);
    return;
}

static int init_fota_storage_device(void)
{
    unsigned char product_id[FW_PRODUCT_ID_MAX_LEN] = {0};
    uint32_t offset = 0;
    int ret = -1;

    memset(&g_fw_update_record, 0x0, sizeof(g_fw_update_record));

    (void)atiny_cmd_ioctl(ATINY_GET_FOTA_STORAGE_DEVICE, (char * )&g_fota_storage_device, sizeof(g_fota_storage_device));
    if(NULL == g_fota_storage_device)
    {
        printf("[%s][%d] fota_get_pack_device failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    offset = sizeof(g_fw_update_record.in_use) + sizeof(g_fw_update_record.record_len);
    ret = g_fota_storage_device->read_update_info(g_fota_storage_device, offset, product_id, FW_PRODUCT_ID_MAX_LEN);
    if(0 != ret)
    {
        printf("[%s][%d] read_update_info failed\n", __FUNCTION__, __LINE__);
        goto read_info_failed;
    }
    if(0 == memcmp(product_id, FW_PRODUCT_ID, strlen(FW_PRODUCT_ID)))
    {
        printf("[%s][%d] has been recorded\n", __FUNCTION__, __LINE__);
        offset = sizeof(g_fw_update_record) - sizeof(g_fw_update_record.uri);
        ret = g_fota_storage_device->read_update_info(g_fota_storage_device, 0, (uint8_t *)(&g_fw_update_record), offset);
        if(0 != ret)
        {
            printf("[%s][%d] read_update_info failed\n", __FUNCTION__, __LINE__);
            goto read_info_failed;
        }
        if(1 == g_fw_update_record.in_use)
        {
            g_fw_update_record.uri = (char *)lwm2m_malloc(g_fw_update_record.uri_len + 1);
            if(NULL == g_fw_update_record.uri)
            {
                printf("[%s][%d] lwm2m_malloc failed\n", __FUNCTION__, __LINE__);
                goto read_info_failed;
            }
            memset(g_fw_update_record.uri, 0x0, g_fw_update_record.uri_len + 1);
            ret = g_fota_storage_device->read_update_info(g_fota_storage_device, offset, (uint8_t *)(g_fw_update_record.uri), g_fw_update_record.uri_len);
            if(0 != ret)
            {
                printf("[%s][%d] read_update_info failed\n", __FUNCTION__, __LINE__);
                goto read_info_failed;
            }
            printf("[%s][%d] record uri : %s\n", __FUNCTION__, __LINE__, g_fw_update_record.uri);
        }
        else
        {
            printf("[%s][%d] read_update_info failed\n", __FUNCTION__, __LINE__);
            goto read_info_failed;
        }
    }
    else
    {
        printf("[%s][%d] has not been recorded\n", __FUNCTION__, __LINE__);
    }
    return 0;
read_info_failed:
    printf("[%s][%d] read_update_info failed\n", __FUNCTION__, __LINE__);
    if(NULL != g_fw_update_record.uri)
    {
        lwm2m_free(g_fw_update_record.uri);
        g_fw_update_record.uri = NULL;
    }
    memset(&g_fw_update_record, 0x0, sizeof(g_fw_update_record));
    return 0;
}

static int record_fw_uri(char *uri, int uri_len)
{
    if(!uri || *uri == '\0' || uri_len <= 0)
    {
        return -1;
    }

    if(NULL != g_fw_update_record.uri)
    {
        lwm2m_free(g_fw_update_record.uri);
        g_fw_update_record.uri = NULL;
    }

    g_fw_update_record.uri = (char *)lwm2m_malloc(uri_len + 1);
    if(NULL == g_fw_update_record.uri)
    {
        printf("[%s][%d] lwm2m_malloc failed\n", __FUNCTION__, __LINE__);
        return -1;
    }
    memset(g_fw_update_record.uri, 0x0, uri_len + 1);
    memcpy(g_fw_update_record.uri, uri, uri_len);
    g_fw_update_record.uri_len = uri_len;

    return 0;
}

static int update_uri_info(char *uri, int uri_len, unsigned char *update_flag)
{
    int ret = -1;

    if(!uri || *uri == '\0' || uri_len <= 0 || !update_flag)
    {
        return -1;
    }

    if(1 == g_fw_update_record.in_use && NULL != g_fw_update_record.uri && 0 == memcmp(g_fw_update_record.uri, uri, uri_len))
    {
        *update_flag = 0;
        return 0;
    }
    else
    {
        memset(&g_fw_update_record, 0x0, sizeof(g_fw_update_record));
        ret = record_fw_uri(uri, uri_len);
        if(0 != ret)
        {
            printf("[%s][%d] record_fw_uri failed\n", __FUNCTION__, __LINE__);
            return -1;
        }
        *update_flag = 1;
        g_fw_update_record.in_use = 1;
        return 0;
    }
}

void set_firmware_update_notify(firmware_update_notify notify_cb, void *param)
{
    g_firmware_update_notify = notify_cb;
    g_firmware_update_notify_param = param;

    return;
}

int parse_firmware_uri(char *uri, int uri_len)
{
    char *char_p, *path;
    int path_len, proto_len;

    if(!uri || *uri == '\0' || uri_len <= 0)
    {
        return -1;
    }

    if(0 == strncmp(uri, COAP_PROTO_PREFIX, strlen(COAP_PROTO_PREFIX)))
        proto_len = strlen(COAP_PROTO_PREFIX);
    else if(0 == strncmp(uri, COAPS_PROTO_PREFIX, strlen(COAPS_PROTO_PREFIX)))
        proto_len = strlen(COAPS_PROTO_PREFIX);
    else
    {
        printf("[%s][%d] unsupported proto\n", __FUNCTION__, __LINE__);
        return -1;
    }
    //ÔÝ²»¿¼ÂÇquery
    char_p = uri + proto_len;
    path = strchr(char_p, '/');
    path_len = uri_len - (path - uri);

    g_ota_uri = (char *)lwm2m_malloc(path_len + 1);
    if(!g_ota_uri)
    {
        printf("[%s][%d] lwm2m_malloc failed\n", __FUNCTION__, __LINE__);
        return -1;
    }
    memset(g_ota_uri, 0x0, path_len + 1);
    memcpy(g_ota_uri, path, path_len);

    return 0;
}

int start_firmware_download(lwm2m_context_t *contextP, char *uri)
{
    lwm2m_transaction_t * transaction;
    unsigned char update_flag = 0;
    int ret = -1;
    int uri_len;
    lwm2m_server_t *server;

    if(!contextP || !uri || *uri == '\0')
    {
        printf("[%s][%d] invalid params\n", __FUNCTION__, __LINE__);
        return -1;
    }
    printf("[%s][%d] start download %s\n", __FUNCTION__, __LINE__, uri);
    uri_len = strlen(uri);
    server = registration_get_registered_server(contextP);
    if(NULL == server)
    {
        printf("[%s][%d] registration_get_registered_server failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if(NULL == g_fota_storage_device)
    {
        ret = init_fota_storage_device();
        if(ret != 0)
        {
            printf("[%s][%d] init_fota_storage_device failed\n", __FUNCTION__, __LINE__);
            return -1;
        }
    }

    ret = update_uri_info(uri, uri_len, &update_flag);
    if(0 != ret)
    {
        ;// continue to although update_uri_info failed
    }

    printf("[%s][%d] update_flag = %u\n", __FUNCTION__, __LINE__, update_flag);
    if(1 == update_flag)
    {
        if(NULL != g_ota_uri)
        {
            lwm2m_free(g_ota_uri);
            g_ota_uri = NULL;
        }
        ret = parse_firmware_uri(uri, uri_len);
        if(0 != ret)
        {
            printf("[%s][%d] parse_firmware_uri failed\n", __FUNCTION__, __LINE__);
            return -1;
        }
    }

    transaction = transaction_new(server->sessionH, COAP_GET, NULL, NULL, contextP->nextMID++, 4, NULL);
    if(!transaction)
    {
        printf("[%s][%d] transaction_new failed\n", __FUNCTION__, __LINE__);
        return -1;
    }
    coap_set_header_uri_path(transaction->message, g_ota_uri);
    //coap_set_header_uri_query(transaction->message, query);
    // now just download the whole firmware
    coap_set_header_block2(transaction->message, 0, 0, FW_BLOCK_SIZE);

    transaction->callback = firmware_download_reply;
    transaction->userData = (void *)contextP;

    contextP->transactionList = (lwm2m_transaction_t *)LWM2M_LIST_ADD(contextP->transactionList, transaction);

    if(transaction_send(contextP, transaction) != 0)
    {
        printf("[%s][%d] transaction_send failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    return 0;
}


