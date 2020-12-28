/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Agent Tiny Mqtt Demo
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

#include "los_base.h"
#include "los_task_pri.h"
#include "los_typedef.h"
#include "los_sys.h"
#include "atiny_mqtt/mqtt_client.h"
#include "osdepends/atiny_osdep.h"
#include "log/atiny_log.h"
#include "cJSON.h"

#include "agent_tiny_mqtt_demo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_DTLS_PSK_ENCRYPT
#define MQTT_DEMO_USE_PSK 1
#elif LOSCFG_DTLS_CRET_ENCRYPT
#define MQTT_DEMO_USE_CERT 1
#endif /* LOSCFG_DTLS_PSK_ENCRYPT */

#define DEFAULT_SERVER_IPV4 "192.168.3.8"
#ifdef LOSCFG_COMPONENTS_SECURITY_MBEDTLS

#if (MQTT_DEMO_USE_PSK == 1)
#define DEFAULT_SERVER_PORT "8883"
#define AGENT_TINY_DEMO_PSK_ID "testID"
#define AGENT_TINY_DEMO_PSK_LEN (3)
unsigned char g_demo_psk[AGENT_TINY_DEMO_PSK_LEN] = {0xab, 0xcd, 0xef};
#elif (MQTT_DEMO_USE_CERT == 1)

#define DEFAULT_SERVER_PORT "8883"

static char g_mqtt_ca_crt[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIEQjCCAyqgAwIBAgIJAPd1i7XQtMI6MA0GCSqGSIb3DQEBBQUAMHMxCzAJBgNV\r\n"
"BAYTAmNuMQswCQYDVQQIEwJ6ajELMAkGA1UEBxMCaHoxDTALBgNVBAoTBHRlc3Qx\r\n"
"DTALBgNVBAsTBHRlc3QxEDAOBgNVBAMTBzAuMC4wLjAxGjAYBgkqhkiG9w0BCQEW\r\n"
"C3Rlc3RAcXEuY29tMB4XDTIwMTAxOTAzNTQyNFoXDTMwMTAxNzAzNTQyNFowczEL\r\n"
"MAkGA1UEBhMCY24xCzAJBgNVBAgTAnpqMQswCQYDVQQHEwJoejENMAsGA1UEChME\r\n"
"dGVzdDENMAsGA1UECxMEdGVzdDEQMA4GA1UEAxMHMC4wLjAuMDEaMBgGCSqGSIb3\r\n"
"DQEJARYLdGVzdEBxcS5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB\r\n"
"AQC60eLKxhugNt4qW8/F+Gw2PJr6KRwJ+GtuCMRHkEEDNTKjWWBDJp91HTgBgKL/\r\n"
"1j4M0QpvFjJk9FGeLPm3jNi/TP3n9cYbd4zd92Dg28O9d20EwEvFv1AwCcRPXMrK\r\n"
"e/7sknH7kCOd8wQPGM+3UbyK1apzkZXkIA3kjvMVs4thOteBRtdjM1EJL0ZP7Me1\r\n"
"EnOfOrHBAHSJU8uHozPfQB+YJhLQADSIdjrxk2kMW+XHhDNSUZIkB9rFD7Uhbad1\r\n"
"s9dUnBVWbS/sZaWKeMM7bJpfdub5JvLKLDHvNLCtdQ51jI605ZQrn/C3JJKFL96R\r\n"
"PjPE+m1rTsOYMH06pdOoiitjAgMBAAGjgdgwgdUwHQYDVR0OBBYEFHqPmqO2OhSw\r\n"
"PpVR2ypkNNJ1m5v3MIGlBgNVHSMEgZ0wgZqAFHqPmqO2OhSwPpVR2ypkNNJ1m5v3\r\n"
"oXekdTBzMQswCQYDVQQGEwJjbjELMAkGA1UECBMCemoxCzAJBgNVBAcTAmh6MQ0w\r\n"
"CwYDVQQKEwR0ZXN0MQ0wCwYDVQQLEwR0ZXN0MRAwDgYDVQQDEwcwLjAuMC4wMRow\r\n"
"GAYJKoZIhvcNAQkBFgt0ZXN0QHFxLmNvbYIJAPd1i7XQtMI6MAwGA1UdEwQFMAMB\r\n"
"Af8wDQYJKoZIhvcNAQEFBQADggEBAAI+zigjJskLs0baSfu8blM35P6Z7Ns5sl4f\r\n"
"xXnm9YFPi79NnDV379pTzzOy80Szh9A1k5QhAp3yELnKaK5KfZsVoh15YzEHNLep\r\n"
"j97LGFmXC7M4Fbr95+8nraKf+bDg5vhvDfFNabxpm0+/ZJp7VLFsWk5y9SrfON7Y\r\n"
"wSlNtpAfG2EPYs3wGwPsp8tb4I461QiitFb6tp97TdKS92YLRqVK5u2YABYRMQu+\r\n"
"72ij6tZ3+XE8eWBBSCJeCr9NNNyOJ669ct2/oe6Bw7sxJvJ1GfsOd2cdD5RkTt5R\r\n"
"Z7Jm8H5nVFHmIEt3yilOXkjZGdn58dBzKaSk2y2IXYQjxwuQ6og=\r\n"
"-----END CERTIFICATE-----\r\n";

#endif /* MQTT_DEMO_USE_PSK */
#endif /* LOSCFG_COMPONENTS_SECURITY_MBEDTLS */

#ifndef DEFAULT_SERVER_PORT
#define DEFAULT_SERVER_PORT "1883"
#endif

#ifndef MQTT_DEMO_CONNECT_DYNAMIC
#define AGENT_TINY_DEMO_PASSWORD  "ed9317ebf381bbaf0000"
#else
#define AGENT_TINY_DEMO_PASSWORD  "c6f16270c5bbf00063ab"
#endif /* MQTT_DEMO_CONNECT_DYNAMIC */

#define AGENT_TINY_DEMO_DEVICEID  "54a6ba67-c023-4d26-9a1c-41adef662550"

#define AGENT_TINY_DEMO_PRODUCTID "8ab780ed11330c2ce2acdf23dbab4d06"

#define AGENT_TINY_DEMO_NODEID    "11223344"

#define AGENT_TINY_DEMO_TIME      "20201103"

#define MAX_LEN 2048

#ifndef array_size
#define array_size(a) (sizeof(a) / sizeof(*(a)))
#endif

static mqtt_client_s *g_phandle = NULL;
static demo_param_s g_demo_param;

static int create_profile_data(cJSON *profile_data[], int value)
{
    cJSON *item = NULL;
    cJSON *tmp = NULL;
    const char *define_data_name_1 = "batteryLevel";
    int define_data_value_1 = (((uint32_t)value) % 100);
    int ret = ATINY_ERR;
    item  = cJSON_CreateObject();
    if (item == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateObject null");
        return ATINY_ERR;
    }
    tmp = cJSON_CreateNumber(define_data_value_1);
    if (tmp == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateNumber null");
        goto EXIT;
    }
    cJSON_AddItemToObject(item, define_data_name_1, tmp);
    profile_data[0] = item;
    ret = ATINY_OK;
EXIT:
    if (ret != ATINY_OK) {
        cJSON_Delete(item);
    }
    return ret;
}

static cJSON *create_service_data(cJSON *profile_data[], uint32_t num)
{
    cJSON *service_data = NULL;
    int ret = ATINY_ERR;
    const char *serviceid = "Battery";
    const char *evet_time = "20201103T114920Z";
    uint32_t i;

    service_data = cJSON_CreateArray();
    if (service_data == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateArray null");
        goto EXIT;
    }

    for (i = 0; i < num; i++) {
        cJSON *item = NULL;
        cJSON *tmp = NULL;

        item = cJSON_CreateObject();
        if (item == NULL) {
            ATINY_LOG(LOG_ERR, "cJSON_CreateObject null");
            goto EXIT;
        }
        cJSON_AddItemToArray(service_data, item);

        tmp = cJSON_CreateString(serviceid);
        if (tmp == NULL) {
            ATINY_LOG(LOG_ERR, "cJSON_CreateString null");
            goto EXIT;
        }
        cJSON_AddItemToObject(item, MQTT_SERVICEID, tmp);

        cJSON_AddItemToObject(item, MQTT_SERVICE_DATA, profile_data[i]);
        profile_data[i] = NULL;

        tmp = cJSON_CreateString(evet_time);
        if (tmp == NULL) {
            ATINY_LOG(LOG_ERR, "cJSON_CreateString null");
            goto EXIT;
        }
        cJSON_AddItemToObject(item, MQTT_EVENT_TIME, tmp);
    }

    ret = ATINY_OK;

EXIT:
    if (ret != ATINY_OK) {
        if (service_data != NULL) {
            cJSON_Delete(service_data);
            service_data = NULL;
        }
    }

    for (i = 0; i < num; i++) {
        if (profile_data[i]) {
            cJSON_Delete(profile_data[i]);
        }
    }

    return service_data;
}

static char *create_json_data(cJSON *profile_data[], uint32_t num)
{
    cJSON *tmp = NULL;
    char *ret = NULL;
    cJSON *service_data = NULL;
    cJSON *root = NULL;

    service_data = create_service_data(profile_data, num);
    if (service_data == NULL) {
        goto EXIT;
    }

    root = cJSON_CreateObject();
    if (root == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateObject null");
        goto EXIT;
    }

    tmp = cJSON_CreateString(MQTT_DEVICE_REQ);
    if (tmp == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateString MQTT_DEVICE_REQ null");
        goto EXIT;
    }
    cJSON_AddItemToObject(root, MQTT_MSG_TYPE, tmp);

    tmp = cJSON_CreateNumber(MQTT_NO_MORE_DATA);
    if (tmp == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateNumber MQTT_NO_MORE_DATA null");
        goto EXIT;
    }
    cJSON_AddItemToObject(root, MQTT_HAS_MORE, tmp);

    cJSON_AddItemToObject(root, MQTT_DATA, service_data);
    service_data = NULL;
    ret = cJSON_Print(root);
    if (ret == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_Print fail");
    }

EXIT:
    if (root != NULL) {
        cJSON_Delete(root);
    }
    if (service_data != NULL) {
        cJSON_Delete(service_data);
    }
    return ret;
}

static void app_data_report(void)
{
    int cnt = 0;

    while (1) {
        if (g_phandle != NULL) {
            cJSON *profile_data[1] = {0};
            if (create_profile_data(profile_data, cnt) == ATINY_OK) {
                char *msg = NULL;
                msg = create_json_data(profile_data, 1);
                if (msg != NULL) {
                    int ret = atiny_mqtt_data_send(g_phandle, msg, strnlen(msg, MAX_LEN), MQTT_QOS_LEAST_ONCE);
                    ATINY_LOG(LOG_INFO, "report ret:%d, cnt %d", ret, cnt);
                    if (ret == ATINY_OK) {
                        ATINY_LOG(LOG_DEBUG, "report data %s\n", msg);
                    }
                    atiny_free(msg);
                }
            }
        } else {
            ATINY_LOG(LOG_ERR, "g_phandle null");
        }
        (void)LOS_TaskDelay(10 * 1000);
        cnt++;
    }
}

static INT32 creat_report_task(void)
{
    UINT32 ret;
    TSK_INIT_PARAM_S task_init_param;
    UINT32 TskHandle;

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "app_data_report";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)app_data_report;
    task_init_param.uwStackSize = 0x2000;

    ret = LOS_TaskCreate(&TskHandle, &task_init_param);
    if (ret != LOS_OK) {
        return ret;
    }
    return ret;
}

#define MAX_TIME_DST 11
static int demo_get_time(char *time, int32_t len)
{
    const int32_t min_time_len = 11;
    if ((time == NULL) || (len < min_time_len)) {
        ATINY_LOG(LOG_ERR, "invalid param len %d", len);
        return ATINY_ERR;
    }

    (void)strcpy_s(time, MAX_TIME_DST, AGENT_TINY_DEMO_TIME); // 11 : time buffer size

    return ATINY_OK;
}

static int proc_rcv_msg(const char *serviceid, const char *cmd, cJSON *paras, int has_more, int mid)
{
    ATINY_LOG(LOG_INFO, "proc_rcv_msg call");
    return ATINY_OK;
}

static int send_msg_resp(int mid, int errcode, int has_more, cJSON *body)
{
    cJSON *tmp = NULL;
    int ret = ATINY_ERR;
    cJSON *msg = NULL;
    char *str_msg = NULL;

    msg = cJSON_CreateObject();
    if (msg == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateObject null");
        goto EXIT;
    }

    tmp = cJSON_CreateString(MQTT_DEVICE_RSP);
    if (tmp == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateString  null");
        goto EXIT;
    }
    cJSON_AddItemToObject(msg, MQTT_MSG_TYPE, tmp);

    tmp = cJSON_CreateNumber(mid);
    if (tmp == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateNumber null");
        goto EXIT;
    }
    cJSON_AddItemToObject(msg, MQTT_MID, tmp);

    tmp = cJSON_CreateNumber(errcode);
    if (tmp == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateNumber null");
        goto EXIT;
    }
    cJSON_AddItemToObject(msg, MQTT_ERR_CODE, tmp);

    tmp = cJSON_CreateNumber(has_more);
    if (tmp == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateNumber null");
        goto EXIT;
    }
    cJSON_AddItemToObject(msg, MQTT_HAS_MORE, tmp);

    cJSON_AddItemToObject(msg, MQTT_BODY, body);
    body = NULL;

    str_msg = cJSON_Print(msg);
    if (str_msg == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateNumber null");
        goto EXIT;
    }

    ret = atiny_mqtt_data_send(g_phandle, str_msg, strnlen(str_msg, MAX_LEN), MQTT_QOS_LEAST_ONCE);
    if (str_msg == NULL) {
        ATINY_LOG(LOG_ERR, "atiny_mqtt_data_send fail ret %d",  ret);
    }

    ATINY_LOG(LOG_INFO, "send rsp ret %d, rsp: %s", ret, str_msg);

EXIT:
    cJSON_Delete(msg);
    if (body != NULL) {
        cJSON_Delete(body);
    }
    if (str_msg != NULL) {
        atiny_free(str_msg);
    }
    return ret;
}

static cJSON *get_resp_body(void)
{
    cJSON *body = NULL;
    cJSON *tmp = NULL;
    const char *body_para = "body_para";

    body = cJSON_CreateObject();
    if (body == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateObject");
        return NULL;
    }

    tmp = cJSON_CreateString(body_para);
    if (tmp == NULL) {
        ATINY_LOG(LOG_ERR, "cJSON_CreateString null");
        goto EXIT;
    }
    cJSON_AddItemToObject(body, body_para, tmp);

    return body;

EXIT:
    cJSON_Delete(body);
    return NULL;
}

enum {
    SERVERID_IDX,
    CMD_IDX,
    PARAS_IDX,
    HAS_MORE_IDX,
    MID_IDX,
};

static int handle_rcv_msg(cJSON *msg)
{
    cJSON *items[5] = {0};
    const char *names[5] = {MQTT_SERVICEID, MQTT_CMD, MQTT_PARAS, MQTT_HAS_MORE, MQTT_MID};
    int ret;
    cJSON *body = NULL;
    uint32_t i;

    for (i = 0; i < array_size(items); i++) {
        items[i] = cJSON_GetObjectItem(msg, names[i]);
        if (names[i] == NULL) {
            ATINY_LOG(LOG_ERR, "cJSON_GetObjectItem %s fail", names[i]);
            return ATINY_ERR;
        }
    }

    if ((items[SERVERID_IDX]->string == NULL)
        || (items[CMD_IDX]->string == NULL)
        || ((items[HAS_MORE_IDX]->valueint != MQTT_NO_MORE_DATA) && (items[HAS_MORE_IDX]->valueint != MQTT_MORE_DATA))) {
        ATINY_LOG(LOG_ERR, "null or err para hasMore %d", items[3]->valueint);
        return ATINY_ERR;
    }

    ret = proc_rcv_msg(items[SERVERID_IDX]->string, items[CMD_IDX]->string,
        items[PARAS_IDX], items[HAS_MORE_IDX]->valueint, items[MID_IDX]->valueint);
    body = get_resp_body();
    if (body == NULL) {
        ATINY_LOG(LOG_ERR, "get_resp_body fail");
        return ATINY_ERR;
    }

    return send_msg_resp(items[MID_IDX]->valueint, (ret == ATINY_OK) ? MQTT_ERR_CODE_OK : MQTT_ERR_CODE_ERR,
        MQTT_NO_MORE_DATA, body);
}

static int demo_rcv_msg(const uint8_t *msg, int32_t len)
{
    cJSON *parse_msg = NULL;
    int ret = ATINY_ERR;
    if ((msg == NULL) || (len <= 0)) {
        ATINY_LOG(LOG_ERR, "invalid param len %ld", len);
        return ATINY_ERR;
    }

    ATINY_LOG(LOG_INFO, "recv msg %s", msg);
    parse_msg = cJSON_Parse((const char *)msg);
    if (parse_msg != NULL) {
        ret = handle_rcv_msg(parse_msg);
    } else {
        ATINY_LOG(LOG_ERR, "cJSON_Parse fail");
    }

    if (parse_msg != NULL) {
        cJSON_Delete(parse_msg);
    }

    return ret;
}

static int demo_cmd_ioctl(mqtt_cmd_e cmd, void *arg, int32_t len)
{
    int result = ATINY_ERR;

    switch (cmd) {
        case MQTT_GET_TIME:
            result = demo_get_time(arg, len);
            break;
        case MQTT_RCV_MSG:
            result = demo_rcv_msg(arg, len);
            break;
        case MQTT_SAVE_SECRET_INFO:
            if (g_demo_param.write_flash_info == NULL) {
                result = ATINY_ERR;
                ATINY_LOG(LOG_ERR, "write_flash_info null");
            } else {
                result = g_demo_param.write_flash_info(arg, len);
            }
            break;
        case MQTT_READ_SECRET_INFO:
            if (g_demo_param.read_flash_info == NULL) {
                result = ATINY_ERR;
                ATINY_LOG(LOG_ERR, "read_flash_info null");
            } else {
                result = g_demo_param.read_flash_info(arg, len);
            }
            break;
        default:
            break;
        }
    return result;
}

void agent_tiny_mqtt_entry(void)
{
    UINT32 ret;
    mqtt_param_s atiny_params;
    mqtt_device_info_s device_info;
    if (g_demo_param.init != NULL) {
        g_demo_param.init();
    }
    atiny_params.server_ip = DEFAULT_SERVER_IPV4;
    atiny_params.server_port = DEFAULT_SERVER_PORT;
#ifdef LOSCFG_COMPONENTS_SECURITY_MBEDTLS
#if (MQTT_DEMO_USE_PSK == 1)
    atiny_params.info.security_type = MQTT_SECURITY_TYPE_PSK;
    atiny_params.info.u.psk.psk_id = (unsigned char *)AGENT_TINY_DEMO_PSK_ID;
    atiny_params.info.u.psk.psk_id_len = strlen(AGENT_TINY_DEMO_PSK_ID);
    atiny_params.info.u.psk.psk = g_demo_psk;
    atiny_params.info.u.psk.psk_len = AGENT_TINY_DEMO_PSK_LEN;
#elif (MQTT_DEMO_USE_CERT == 1)
    atiny_params.info.security_type = MQTT_SECURITY_TYPE_CA;
    atiny_params.info.u.ca.ca_crt = g_mqtt_ca_crt;
    atiny_params.info.u.ca.ca_len = sizeof(g_mqtt_ca_crt);
#else
    atiny_params.info.security_type = MQTT_SECURITY_TYPE_NONE;
#endif /* MQTT_DEMO_USE_PSK */
#else
    atiny_params.info.security_type = MQTT_SECURITY_TYPE_NONE;
#endif /* LOSCFG_COMPONENTS_SECURITY_MBEDTLS */
    atiny_params.cmd_ioctl = demo_cmd_ioctl;
    if (atiny_mqtt_init(&atiny_params, &g_phandle) != ATINY_OK) {
        return;
    }
    ret = creat_report_task();
    if (ret != LOS_OK) {
        return;
    }
    device_info.codec_mode = MQTT_CODEC_MODE_JSON;
    device_info.sign_type = MQTT_SIGN_TYPE_HMACSHA256_NO_CHECK_TIME;
    device_info.password = AGENT_TINY_DEMO_PASSWORD;
#ifndef MQTT_DEMO_CONNECT_DYNAMIC
    device_info.connection_type = MQTT_STATIC_CONNECT;
    device_info.u.s_info.deviceid = AGENT_TINY_DEMO_DEVICEID;
#else
    device_info.connection_type = MQTT_DYNAMIC_CONNECT;
    device_info.u.d_info.productid = AGENT_TINY_DEMO_PRODUCTID;
    device_info.u.d_info.nodeid = AGENT_TINY_DEMO_NODEID;
#endif
    (void)atiny_mqtt_bind(&device_info, g_phandle);
    return;
}

void agent_tiny_demo_init(const demo_param_s *param)
{
    g_demo_param = *param;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
