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

#include "test_connection.h"
#include "atiny_socket.h"
#include "stub.h"
#include <unistd.h>
#include <fcntl.h>
extern "C" {
#include "connection.h"
#include "object_comm.h"

#define SERVER_URI_MAX_LEN 64
    typedef struct
    {
        int fd;
    }
    atiny_net_context;

    
    extern void agent_tiny_fota_init(void);

    void *stub_atiny_net_connect(const char *host, const char *port, int proto)
    {
        return NULL;
    }
    void *stub_atiny_mutex_create(void)
    {
        return (void *)"a";
    }

    connection_t *stub_connection_create(connection_t *connList,
                                         lwm2m_object_t *securityObj,
                                         int instanceId,
                                         lwm2m_context_t *lwm2mH)
    {
        return ((connection_t *)malloc(sizeof(connection_t)));
    }
    int	stub_atiny_net_recv_timeout( void *ctx, unsigned char *buf, size_t len, uint32_t timeout )
    {
        return -1;
    }

    
    void test_lwm2m_connection_err_notify_t(lwm2m_context_t* context, connection_err_e err_type, bool boostrap_flag)
    {
        return;
    }
}
void TestConnection::test_connection_create()
{
    connection_t conn;
    lwm2m_object_t securityObj0;
    lwm2m_list_t instanceList;

    securityObj0.instanceList = NULL;
    connection_t *connP = connection_create(&conn, &securityObj0, 0, NULL);
    TEST_ASSERT((connP == NULL));

    memset(&instanceList, 0, sizeof(lwm2m_list_t));
    securityObj0.instanceList = &instanceList;
    ((security_instance_t*)(securityObj0.instanceList))->uri = NULL;
    connP = connection_create(&conn, &securityObj0, 0, NULL);
    TEST_ASSERT((connP == NULL));

    ((security_instance_t*)(securityObj0.instanceList))->uri = "coaps://";
    connP = connection_create(&conn, &securityObj0, 0, NULL);
    TEST_ASSERT((connP == NULL));

    ((security_instance_t*)(securityObj0.instanceList))->uri = "coa://";
    connP = connection_create(&conn, &securityObj0, 0, NULL);
    TEST_ASSERT((connP == NULL));

    char uri[] = "coap://[192.168.1.12]:[5683]";
    ((security_instance_t*)(securityObj0.instanceList))->uri = uri;
    connP = connection_create(&conn, &securityObj0, 0, NULL);
    TEST_ASSERT((connP == NULL));

    connP = NULL;
    lwm2m_context_t context;
    atiny_param_t *atiny_params = &this->prv_atiny_params;
    int serverId = 123;
    char serverUri[SERVER_URI_MAX_LEN];

    lwm2m_object_t *securityObj = get_security_object(serverId, atiny_params, &context);
    TEST_ASSERT((securityObj != NULL));
    
    connP = connection_create(&conn, securityObj, 0, &context);
    TEST_ASSERT_MSG((connP != NULL), "Test in connection_create when atiny_net_connect is Failed!");
    TEST_ASSERT((connP->next == &conn));
    TEST_ASSERT((connP->securityObj == securityObj));
    TEST_ASSERT((connP->securityInstId == 0));
    TEST_ASSERT((connP->lwm2mH == &context));
    lwm2m_free(connP->net_context);
    lwm2m_free(connP);

    stubInfo si;
    setStub((void *)atiny_net_connect, (void *)stub_atiny_net_connect, &si);
    
    connP = connection_create(&conn, securityObj, 0, &context);
    TEST_ASSERT_MSG((connP == NULL), "Test in connection_create when atiny_net_connect return NULL is Failed!");
    lwm2m_free(connP);
    cleanStub(&si);

    clean_security_object(securityObj);
}

int stub_atiny_net_send1( void *ctx, const unsigned char *buf, size_t len )
{
    //stub func for test lwm2m_buffer_send(), return len
    std::cout << "return " << len << "\n";
    return len;
}


void TestConnection::test_lwm2m_buffer_send()
{
    connection_t conn;
    uint8_t buf[64] = "test str";
    stubInfo si;

    uint8_t ret = lwm2m_buffer_send(NULL, NULL, 0, NULL);
    TEST_ASSERT(ret == COAP_500_INTERNAL_SERVER_ERROR);

    setStub((void *)atiny_net_send, (void *)stub_atiny_net_send1, &si);
    ret = lwm2m_buffer_send(&conn, buf, 0, NULL);
    TEST_ASSERT(ret == 0);
    cleanStub(&si);


    atiny_net_context anc;
    char buf2[64] = {0};

    conn.net_context = &anc;
    anc.fd = open("./demo", O_CREAT | O_RDWR, 0777);
    unlink("./demo");
    ret = lwm2m_buffer_send(&conn, buf, strlen((char *)buf), NULL);

    read(anc.fd, buf2, 64);
    TEST_ASSERT((strncmp((char *)buf, buf2, strlen((char *)buf))));
    close(anc.fd);
}

void TestConnection::test_lwm2m_connect_server()
{
    client_data_t *data = NULL;
    uint16_t ObjInstID = 1;
    void *newConnP = NULL;

    data = (client_data_t *)malloc(sizeof(*data));
    TEST_ASSERT(data != NULL);

    data->securityObjP = (lwm2m_object_t *)malloc(sizeof(lwm2m_object_t));
    TEST_ASSERT(data->securityObjP != NULL);
    data->securityObjP->instanceList = (lwm2m_list_t *)malloc(sizeof(lwm2m_list_t));
    TEST_ASSERT(data->securityObjP->instanceList != NULL);
    data->securityObjP->instanceList->id = 1;

    stubInfo si;
    setStub((void *)connection_create, (void *)stub_connection_create, &si);
    newConnP = lwm2m_connect_server(ObjInstID, data, false);
    free(newConnP);
    cleanStub(&si);

    free(data->securityObjP->instanceList);
    free(data->securityObjP);
    free(data);
}
void TestConnection::test_lwm2m_close_connection()
{
    int result = 0;
    client_data_t *app_data = NULL;
    connection_t  *targetP = NULL;

    app_data = (client_data_t *)malloc(sizeof(client_data_t));
    TEST_ASSERT(app_data != NULL);

    targetP = (connection_t *)malloc(sizeof(connection_t));
    TEST_ASSERT(targetP != NULL);
    targetP->net_context = (int *)malloc(sizeof(int));
    TEST_ASSERT(targetP->net_context != NULL);
    ((atiny_net_context *)(targetP->net_context))->fd = -1;
    targetP->next = (connection_t *)malloc(sizeof(connection_t));
    TEST_ASSERT(targetP->next != NULL);

    app_data->connList = targetP;
    lwm2m_close_connection(targetP, app_data);

    free(app_data->connList);

    app_data->connList = (connection_t *)malloc(sizeof(connection_t));
    TEST_ASSERT(app_data->connList != NULL);
    targetP = (connection_t *)malloc(sizeof(connection_t));
    TEST_ASSERT(targetP != NULL);
    targetP->net_context = (int *)malloc(sizeof(int));
    TEST_ASSERT(targetP->net_context != NULL);
    ((atiny_net_context *)(targetP->net_context))->fd = -1;
    targetP->next = (connection_t *)malloc(sizeof(connection_t));
    TEST_ASSERT(targetP->next != NULL);
    app_data->connList->next = targetP;

    lwm2m_close_connection(targetP, app_data);
    printf("[%s:%d]\n", __FILE__, __LINE__);
    free(app_data->connList->next);
    free(app_data->connList);
    free(app_data);

}
void TestConnection::test_lwm2m_buffer_recv()
{
    int result = 0;
    connection_t *sessionH = NULL;

    sessionH = (connection_t *)malloc(sizeof(connection_t));
    TEST_ASSERT(sessionH != NULL);
    sessionH->net_context = (int *)malloc(sizeof(int));

    stubInfo si;
    setStub((void *)atiny_net_recv_timeout, (void *)stub_atiny_net_recv_timeout, &si);
    sessionH->dtls_flag = false;
    sessionH->errs[1] = 0;
    result = lwm2m_buffer_recv(sessionH, NULL, 0, 1);
    TEST_ASSERT_EQUALS_MSG(result, -1, result);
    sessionH->errs[1] = 11;
    result = lwm2m_buffer_recv(sessionH, NULL, 0, 1);
    TEST_ASSERT_EQUALS_MSG(result, -1, result);
    cleanStub(&si);

    free(sessionH->net_context);
    free(sessionH);

}
void TestConnection::test_connection_free()
{
    connection_t *connP = NULL;

    connection_free(connP);
    connP = (connection_t *)malloc(sizeof(connection_t));
    TEST_ASSERT(connP != NULL);
    connP->net_context = NULL;
    connP->net_context = (atiny_net_context *)malloc(sizeof(atiny_net_context));
    TEST_ASSERT(connP->net_context != NULL);
    ((atiny_net_context *)(connP->net_context))->fd = -1;
    connection_free(connP);

    free(connP);

}
void TestConnection::test_lwm2m_session_is_equal()
{
    connection_t connA;
    connection_t connB;


    bool ret = lwm2m_session_is_equal(NULL, NULL, NULL);
    TEST_ASSERT(ret == true);

    connection_t *p_connB = &connA;
    ret = lwm2m_session_is_equal((void *)&connA, (void *)p_connB, NULL);

    TEST_ASSERT(ret == true);

    ret = lwm2m_session_is_equal((void *)&connA, (void *)&connB, NULL);

    TEST_ASSERT(ret == false);

}
void TestConnection::test_lwm2m_register_connection_err_notify()
{
    lwm2m_register_connection_err_notify(test_lwm2m_connection_err_notify_t);
}
TestConnection::TestConnection()
{

    TEST_ADD(TestConnection::test_connection_create);
    TEST_ADD(TestConnection::test_lwm2m_buffer_send);
    TEST_ADD(TestConnection::test_lwm2m_session_is_equal);
    TEST_ADD(TestConnection::test_lwm2m_connect_server);
    TEST_ADD(TestConnection::test_lwm2m_close_connection);
    TEST_ADD(TestConnection::test_lwm2m_buffer_recv);
    TEST_ADD(TestConnection::test_connection_free);
    TEST_ADD(TestConnection::test_lwm2m_register_connection_err_notify);
}

void TestConnection::setup()
{
    atiny_device_info_t *device_info = &this->prv_dev_info;
    atiny_param_t *atiny_params = &this->prv_atiny_params;
    atiny_security_param_t  *iot_security_param = NULL;
    atiny_security_param_t  *bs_security_param = NULL;
    void *handle = this->prv_handle;

    memset(atiny_params, 0, sizeof(atiny_param_t));

    std::cout << "init for TestConnection\n";
    device_info->endpoint_name = (char *)"xxxxxxxx";
    device_info->manufacturer = (char *)"test";

    atiny_params->server_params.binding = (char *)"UQS";
    atiny_params->server_params.life_time = 50000;
    atiny_params->server_params.storing_cnt = 0;

    iot_security_param = &(atiny_params->security_params[0]);
    bs_security_param = &(atiny_params->security_params[1]);

    iot_security_param->server_ip = (char *)"192.168.0.106";
    bs_security_param->server_ip = (char *)"192.168.0.106";

    iot_security_param->server_port = (char *)"5683";
    bs_security_param->server_port = (char *)"5683";

    iot_security_param->psk_Id = NULL;
    iot_security_param->psk = NULL;
    iot_security_param->psk_len = 0;

    bs_security_param->psk_Id = NULL;
    bs_security_param->psk = NULL;
    bs_security_param->psk_len = 0;
    
    stubInfo si_atiny_mutex_create;
    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create, &si_atiny_mutex_create);
    agent_tiny_fota_init();
    int ret = atiny_init(atiny_params, &handle);
    cleanStub(&si_atiny_mutex_create);
    printf("[%s:%d] ret:%d\n", __FILE__, __LINE__, ret);
    TEST_ASSERT(ret == ATINY_OK);

}

void TestConnection::tear_down()
{
    std::cout << "deinit for TestConnection\n";
}



