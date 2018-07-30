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

/* Includes -----------------------------------------------------------------*/
#include "test_esp8266.h"
/* Defines ------------------------------------------------------------------*/
#define TEST_STATE_OK   0
#define TEST_STATE_ERR  1
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
char g_state = TEST_STATE_OK;
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
extern "C"
{

    #include "atiny_socket.h"
    #include "atiny_adapter.h"
    #include "esp8266.h"

    extern at_task at;

    // in esp8266.c
    extern int32_t esp8266_init();
    extern int32_t esp8266_echo_off(void);
    extern int32_t esp8266_reset(void);
    extern int32_t esp8266_choose_net_mode(enum_net_mode m);
    extern int32_t esp8266_set_mux_mode(int32_t m);
    extern int32_t esp8266_joinap(char * pssid, char * ppasswd);
    extern int32_t esp8266_connect(const int8_t * host, const int8_t *port, int32_t proto);
    extern int32_t esp8266_send(int32_t id , const uint8_t  *buf, uint32_t len);
    extern int32_t esp8266_recv(int32_t id, int8_t * buf, uint32_t len);
    extern int32_t esp8266_recv_timeout(int32_t id, int8_t * buf, uint32_t len, int32_t timeout);
    extern int32_t esp8266_close(int32_t id);
    extern int32_t esp8266_data_handler(void * arg, int8_t * buf, int32_t len);
    extern int8_t  esp8266_get_localip(int8_t * ip, int8_t * gw, int8_t * mask);
    extern int8_t  esp8266_get_localmac(int8_t * mac);
    extern int32_t esp8266_recv_cb(int32_t id);
    extern int32_t esp8266_deinit(void);

    // in atadapter.c
    extern int32_t at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf);
    extern int32_t at_write(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len);
    extern int32_t at_get_unuse_linkid();

    // in liteos queue
    extern UINT32 LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
                            UINT32 uwFlags,UINT16 usMaxMsgSize );
    extern UINT32 LOS_QueueWriteCopy( UINT32 uwQueueID, VOID * pBufferAddr,
                            UINT32 uwBufferSize, UINT32 uwTimeOut );
    extern UINT32 LOS_QueueReadCopy(UINT32  uwQueueID, VOID *  pBufferAddr,
                            UINT32 * puwBufferSize, UINT32  uwTimeOut);
    // in atadapter.c
    extern void* at_malloc(size_t size);
    
    // stubs
    static int32_t stub_esp8266_joinap(char *pssid, char *ppasswd)
    {
        static int i = 0;
        if(g_state == TEST_STATE_OK){
            return AT_OK;
        }
        if(++i > 5){
            return AT_OK;
        }
        return AT_FAILED;
    }

    static int32_t stub_at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf)
    {
        char *tmp = NULL;

        if(g_state == TEST_STATE_ERR){
            return AT_FAILED;
        }
        if(0 == memcmp(cmd, AT_CMD_CHECK_IP, strlen(AT_CMD_CHECK_IP))){
            tmp = (char*)"+CIPSTA_CUR:ip:\"192.168.1.10\",gateway:\"192.168.1.1\",netmask:\"255.255.255.0\"";
            memcpy(rep_buf, tmp, strlen(tmp));
        }else if(0 == memcmp(cmd, AT_CMD_CHECK_MAC, strlen(AT_CMD_CHECK_MAC))){
            tmp = (char*)"+CIPSTAMAC:\"18:fe:35:98:d3:7b\"";
            memcpy(rep_buf, tmp, strlen(tmp));
        }
        return AT_OK;
    }

    static int32_t stub_at_write(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len)
    {
        return AT_FAILED;
    }

    static int32_t stub_at_get_unuse_linkid()
    {
        return -1;
    }

    static UINT32 stub_LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
            UINT32 uwFlags,UINT16 usMaxMsgSize )
    {
        return 1;
    }

    static UINT32 stub_LOS_QueueWriteCopy( UINT32 uwQueueID, VOID * pBufferAddr,
                     UINT32 uwBufferSize, UINT32 uwTimeOut )
    {
        if(g_state == TEST_STATE_ERR){
            return 1;
        }
        // simulate memory release
        QUEUE_BUFF *pqbuf = (QUEUE_BUFF *)pBufferAddr;
        if(pqbuf && pqbuf->addr){
            at_free(pqbuf->addr);
        }
        return 0;
    }

    static void* stub_at_malloc(size_t size)
    {
        return NULL;
    }

    static UINT32 stub_LOS_QueueReadCopy(UINT32  uwQueueID, VOID *  pBufferAddr,
                    UINT32 * puwBufferSize, UINT32  uwTimeOut)
    {
        
        QUEUE_BUFF qbuf = {0, NULL};
        char buf[] = { 0x1,0x2,0x3,0x4,0x5 };

        if(g_state == TEST_STATE_ERR){
            return 1;
        }
        qbuf.addr = (uint8_t*)atiny_malloc(sizeof(buf));
        if (NULL == qbuf.addr)
        {
            printf("malloc for qbuf failed!");
            goto END;
        }

        qbuf.len = sizeof(buf);
        memcpy(qbuf.addr, buf, sizeof(buf));
        memcpy(pBufferAddr, &qbuf, sizeof(QUEUE_BUFF));
        *puwBufferSize = qbuf.len;
        
        END:
        return 0;
    }
}

TestEsp8266::TestEsp8266()
{
    TEST_ADD(TestEsp8266::test_esp8266_init);
    TEST_ADD(TestEsp8266::test_esp8266_echo_off);
    TEST_ADD(TestEsp8266::test_esp8266_choose_net_mode);
    TEST_ADD(TestEsp8266::test_esp8266_set_mux_mode);
    TEST_ADD(TestEsp8266::test_esp8266_joinap);
    TEST_ADD(TestEsp8266::test_esp8266_connect);
    TEST_ADD(TestEsp8266::test_esp8266_send);
    TEST_ADD(TestEsp8266::test_esp8266_recv);
    TEST_ADD(TestEsp8266::test_esp8266_recv_timeout);
    TEST_ADD(TestEsp8266::test_esp8266_close);
    TEST_ADD(TestEsp8266::test_esp8266_data_handler);
    TEST_ADD(TestEsp8266::test_esp8266_get_localip);
    TEST_ADD(TestEsp8266::test_esp8266_get_localmac);
    TEST_ADD(TestEsp8266::test_esp8266_recv_cb);
    TEST_ADD(TestEsp8266::test_esp8266_deinit);
}

TestEsp8266::~TestEsp8266()
{
    esp8266_deinit();
}

void TestEsp8266::test_esp8266_init(void)
{
    uint32_t ret = AT_OK;
    
    stubInfo stub_info;
    setStub((void *)esp8266_joinap,(void *)stub_esp8266_joinap,&stub_info);
    esp8266_deinit();
    ret = esp8266_init();
    TEST_ASSERT_MSG((ret == AT_OK), "esp8266_init() failed");
    cleanStub(&stub_info);

    // abnormal test
    g_state = TEST_STATE_ERR;
    setStub((void *)esp8266_joinap,(void *)stub_esp8266_joinap,&stub_info);
    esp8266_deinit();
    ret = esp8266_init();
    TEST_ASSERT_MSG((ret == AT_OK), "esp8266_init() failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;
}
void TestEsp8266::test_esp8266_deinit(void)
{
    int32_t ret = AT_OK;
    ret = esp8266_deinit();
    TEST_ASSERT_MSG((ret == AT_OK), "esp8266_deinit() failed");
}
void TestEsp8266::test_esp8266_echo_off(void)
{
    int32_t ret = AT_OK;
    ret = esp8266_echo_off();
    TEST_ASSERT_MSG((ret == AT_OK), "esp8266_echo_off() failed");

    // abnormal test
    /* test command send failure */
    stubInfo stub_info;
    g_state = TEST_STATE_ERR;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
    ret = esp8266_echo_off();
    TEST_ASSERT_MSG((ret == AT_FAILED), "esp8266_echo_off() failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;
}

void TestEsp8266::test_esp8266_reset(void)
{
    int32_t ret = AT_OK;
    ret = esp8266_reset();
    TEST_ASSERT_MSG((ret == AT_OK), "esp8266_reset() failed");

    // abnormal test
    /* test command send failure */
    stubInfo stub_info;
    g_state = TEST_STATE_ERR;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
    ret = esp8266_reset();
    TEST_ASSERT_MSG((ret == AT_FAILED), "esp8266_reset() failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;
}
void TestEsp8266::test_esp8266_choose_net_mode(void)
{
    int32_t ret = AT_OK;
    ret = esp8266_choose_net_mode(STA);
    TEST_ASSERT_MSG((ret == AT_OK), "esp8266_choose_net_mode(STA) failed");

    // abnormal test
    /* test command send failure */
    stubInfo stub_info;
    g_state = TEST_STATE_ERR;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
    ret = esp8266_choose_net_mode(STA);
    TEST_ASSERT_MSG((ret == AT_FAILED), "esp8266_choose_net_mode(STA) failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;
}
void TestEsp8266::test_esp8266_set_mux_mode(void)
{
    int32_t ret = AT_OK;
    ret = esp8266_set_mux_mode(AT_MUXMODE_SINGLE);
    TEST_ASSERT_MSG((ret == AT_OK), "esp8266_set_mux_mode(AT_MUXMODE_SINGLE) failed");

    // abnormal test
    /* test command send failure */
    stubInfo stub_info;
    g_state = TEST_STATE_ERR;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
    ret = esp8266_set_mux_mode(AT_MUXMODE_SINGLE);
    TEST_ASSERT_MSG((ret == AT_FAILED), "esp8266_set_mux_mode(AT_MUXMODE_SINGLE) failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;
}
void TestEsp8266::test_esp8266_joinap(void)
{
    int32_t ret = AT_OK;
    ret = esp8266_joinap((char*)WIFI_SSID, (char*)WIFI_PASSWD);
    TEST_ASSERT_MSG((ret == AT_OK), "esp8266_joinap(WIFI_SSID, WIFI_PASSWD) failed");

    // abnormal test
    /* test command send failure */
    stubInfo stub_info;
    g_state = TEST_STATE_ERR;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
    ret = esp8266_joinap((char*)WIFI_SSID, (char*)WIFI_PASSWD);
    TEST_ASSERT_MSG((ret == AT_FAILED), "esp8266_joinap(WIFI_SSID, WIFI_PASSWD) failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;
}
void TestEsp8266::test_esp8266_connect(void)
{
    char host[] = "coap://192.168.1.106";
    char port[] = "5683";
    uint32_t tmp;
    uint32_t ret = AT_OK;

    // normal
    tmp = at.mux_mode;
    at.mux_mode = AT_MUXMODE_SINGLE;
    ret = esp8266_connect((int8_t *)host, (int8_t *)port, ATINY_PROTO_UDP);
    TEST_ASSERT_MSG((ret != AT_FAILED), "esp8266_connect(host, port, ATINY_PROTO_UDP) failed");
    at.mux_mode = AT_MUXMODE_MULTI;
    ret = esp8266_connect((int8_t *)host, (int8_t *)port, ATINY_PROTO_UDP);
    TEST_ASSERT_MSG((ret != AT_FAILED), "esp8266_connect(host, port, ATINY_PROTO_UDP) failed");
    at.mux_mode = tmp;

    // abnormal test
    /* test unuse linkid */
    stubInfo stub_info;
    setStub((void *)at_get_unuse_linkid,(void *)stub_at_get_unuse_linkid,&stub_info);
    ret = esp8266_connect((int8_t *)host, (int8_t *)port, ATINY_PROTO_UDP);
    TEST_ASSERT_MSG((ret == -1), "esp8266_connect(host, port, ATINY_PROTO_UDP) failed");
    cleanStub(&stub_info);

    /* test queue create failure */
    setStub((void *)LOS_QueueCreate,(void *)stub_LOS_QueueCreate,&stub_info);
    ret = esp8266_connect((int8_t *)host, (int8_t *)port, ATINY_PROTO_UDP);
    TEST_ASSERT_MSG((ret == -1), "esp8266_connect(host, port, ATINY_PROTO_UDP) failed");
    cleanStub(&stub_info);

    /* test command send failure */
    g_state = TEST_STATE_ERR;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
    ret = esp8266_connect((int8_t *)host, (int8_t *)port, ATINY_PROTO_UDP);
    TEST_ASSERT_MSG((ret == -1), "esp8266_connect(host, port, ATINY_PROTO_UDP) failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;
}
void TestEsp8266::test_esp8266_send(void)
{
    int32_t id = 0;
    uint8_t buf[100] = {0};
    uint32_t len = sizeof(buf);
    uint32_t tmp;
    uint32_t ret = AT_OK;
    
    tmp = at.mux_mode;
    at.mux_mode = AT_MUXMODE_SINGLE;
    ret = esp8266_send(id, buf, len);
    TEST_ASSERT_MSG((ret != AT_FAILED), "esp8266_send(id, buf, len) failed");
    at.mux_mode = AT_MUXMODE_MULTI;
    ret = esp8266_send(id, buf, len);
    TEST_ASSERT_MSG((ret != AT_FAILED), "esp8266_send(id, buf, len) failed");
    at.mux_mode = tmp;

    // abnormal test
    /* test send error */
    stubInfo stub_info;
    setStub((void *)at_write,(void *)stub_at_write,&stub_info);
    ret = esp8266_send(id, buf, len);
    TEST_ASSERT_MSG((ret == AT_FAILED), "esp8266_send(id, buf, len) failed");
    cleanStub(&stub_info);
}
void TestEsp8266::test_esp8266_recv(void)
{
    int32_t id = 0;
    int8_t buf[100] = {0};
    uint32_t len = sizeof(buf);
    uint32_t ret = AT_OK;

    /* test empty data */
    ret = esp8266_recv(id, buf, len);
    TEST_ASSERT_MSG((ret != -1), "esp8266_recv(id, buf, len) failed");

    /* test queue read copy */
    stubInfo stub_info;
    setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy,&stub_info);
    ret = esp8266_recv(id, buf, len);
    TEST_ASSERT_MSG((ret != -1), "esp8266_recv(id, buf, len) failed");
    cleanStub(&stub_info);

    // abnormal
    /* test queue read copy failure */
    g_state = TEST_STATE_ERR;
    setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy,&stub_info);
    ret = esp8266_recv(id, buf, len);
    TEST_ASSERT_MSG((ret == -1), "esp8266_recv(id, buf, len) failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;
}
void TestEsp8266::test_esp8266_recv_timeout(void)
{
    int32_t id = 0;
    int8_t buf[100] = {0};
    uint32_t len = sizeof(buf);
    int32_t timeout = 1000;
    uint32_t ret = AT_OK;
    
    /* test empty data */
    ret = esp8266_recv_timeout(id, buf, len, timeout);
    TEST_ASSERT_MSG((ret != -1), "esp8266_recv_timeout(id, buf, len, timeout) failed");

    /* test queue read copy */
    stubInfo stub_info;
    setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy,&stub_info);
    ret = esp8266_recv_timeout(id, buf, len, timeout);
    TEST_ASSERT_MSG((ret != -1), "esp8266_recv_timeout(id, buf, len, timeout) failed");
    cleanStub(&stub_info);

    // abnormal test
    /* test queue read copy failure */
    g_state = TEST_STATE_ERR;
    setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy,&stub_info);
    ret = esp8266_recv_timeout(id, buf, len, timeout);
    TEST_ASSERT_MSG((ret == -1), "esp8266_recv_timeout(id, buf, len, timeout) failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;
}
void TestEsp8266::test_esp8266_close(void)
{
    uint32_t tmp;
    uint32_t ret = AT_OK;
    
    tmp = at.mux_mode;
    at.mux_mode = AT_MUXMODE_SINGLE;
    ret = esp8266_close(0);
    TEST_ASSERT_MSG((ret == AT_OK), "esp8266_close(0) failed");
    at.mux_mode = AT_MUXMODE_MULTI;
    ret = esp8266_close(0);
    TEST_ASSERT_MSG((ret == AT_OK), "esp8266_close(0) failed");
    at.mux_mode = tmp;

    // abnormal test
    stubInfo stub_info;
    g_state = TEST_STATE_ERR;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
    ret = esp8266_close(1);
    TEST_ASSERT_MSG((ret == AT_FAILED), "esp8266_close(1) failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;
}
void TestEsp8266::test_esp8266_data_handler(void)
{
    char *buf = NULL;
    char tmp[100] = {0};
    int32_t len = -1;
    uint32_t ret = AT_OK;
    char data[] = {0x1,0x2,0x3,0x4,0x5,0x0};
    
    ret = esp8266_data_handler(NULL, (int8_t*)buf, len);
    TEST_ASSERT_MSG((ret == -1), "esp8266_data_handler(NULL, NULL, -1) failed");

    /* prefix error */
    buf = (char*)"esp8266";
    len = strlen(buf);
    ret = esp8266_data_handler(NULL, (int8_t*)buf, len);
    TEST_ASSERT_MSG((ret == -1), "esp8266_data_handler(NULL, buf, len) failed");

    /* only perfix */
    buf = (char*)"\r\n+IPD";
    len = strlen(buf);
    ret = esp8266_data_handler(NULL, (int8_t*)buf, len);
    TEST_ASSERT_MSG((ret == -1), "esp8266_data_handler(NULL, buf, len) failed");

    /* complete data */
    buf = (char*)"\r\n+IPD,0,%d:";
    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, buf, sizeof(data));
    len = strlen(tmp);
    memcpy(tmp+len, data, sizeof(data));
    len = len + sizeof(data);
//    for(int i = 0; i < len; i++){
//        printf("%d_", tmp[i]);
//    }
//    printf("\n");

    stubInfo stub_info;
    setStub((void *)LOS_QueueWriteCopy,(void *)stub_LOS_QueueWriteCopy,&stub_info);
    ret = esp8266_data_handler(NULL, (int8_t*)tmp, len);
    TEST_ASSERT_MSG((ret != -1), "esp8266_data_handler(NULL, tmp, len) failed");
    cleanStub(&stub_info);
    
    // abnormal test
    /* test queue wirte copy failure */
    g_state = TEST_STATE_ERR;
    setStub((void *)LOS_QueueWriteCopy,(void *)stub_LOS_QueueWriteCopy,&stub_info);
    ret = esp8266_data_handler(NULL, (int8_t*)tmp, len);
    TEST_ASSERT_MSG((ret == 1), "esp8266_data_handler(NULL, tmp, len) failed");
    cleanStub(&stub_info);
    g_state = TEST_STATE_OK;

    /* test memory allocation failure */
    setStub((void *)at_malloc,(void *)stub_at_malloc,&stub_info);
    ret = esp8266_data_handler(NULL, (int8_t*)tmp, len);
    TEST_ASSERT_MSG((ret == -1), "esp8266_data_handler(NULL, tmp, len) failed");
    cleanStub(&stub_info);
}
void TestEsp8266::test_esp8266_get_localip(void)
{
    int8_t ip[32] = {0};
    int8_t gw[32] = {0};
    int8_t mask[32] = {0};
    int8_t ret = 0;

    /* test no response */
    ret = esp8266_get_localip(ip, gw, mask);
    TEST_ASSERT_MSG((ret == 0), "esp8266_get_localip(ip, gw, mask) failed");

    /* test correct response */
    stubInfo stub_info;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
    ret = esp8266_get_localip(ip, gw, mask);
    TEST_ASSERT_MSG((ret == 0), "esp8266_get_localip(ip, gw, mask) failed");
    cleanStub(&stub_info);
//    printf("ip:%s, gateway:%s, netmask:%s\n",ip, gw, mask);
}
void TestEsp8266::test_esp8266_get_localmac(void)
{
    int8_t mac[32] = {0};
    int8_t ret = 0;

    /* test no response */
    ret = esp8266_get_localmac(mac);
    TEST_ASSERT_MSG((ret == 0), "esp8266_get_localmac(mac) failed");

    /* test correct response */
    stubInfo stub_info;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
    ret = esp8266_get_localmac(mac);
    TEST_ASSERT_MSG((ret == 0), "esp8266_get_localmac(mac) failed");
    cleanStub(&stub_info);
//    printf("mac:%s\n",mac);
}
void TestEsp8266::test_esp8266_recv_cb(void)
{
    int32_t ret = AT_OK;
    ret = esp8266_recv_cb(1);
    TEST_ASSERT_MSG((ret == -1), "esp8266_recv_cb(1) failed");
}

/* Private functions --------------------------------------------------------*/

