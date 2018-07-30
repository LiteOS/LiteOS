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

#include "test_at_api_interface.h"

extern "C" {

    #include "atadapter.h"
    
    int32_t demo_init()
    {
        printf("%s:%d, OK\r\n", __func__, __LINE__);
        return AT_OK;
    }

    int32_t demo_connect(const int8_t * host, const int8_t *port, int32_t proto)
    {
        return AT_OK;
    }

    int32_t demo_send(int32_t id , const uint8_t  *buf, uint32_t len)
    {
        return AT_OK;
    }

    int32_t demo_recv(int32_t id , int8_t  *buf, uint32_t len)
    {
        return AT_OK;
    }

    int32_t demo_recv_timeout(int32_t id , int8_t  *buf, uint32_t len, int32_t timeout)
    {
        return AT_OK;
    }

    int32_t demo_close(int32_t id)
    {
        return AT_OK;
    }
    
    at_adaptor_api api = {
        .init = NULL,
        .get_localmac = NULL,
        .get_localip = NULL,
        .connect = NULL,
        .send = NULL,
        .recv_timeout = NULL,
        .recv = NULL,
        .close = NULL,
        .recv_cb = NULL,
        .deinit = NULL,
    };
}

void TestAtApiInterface::test_at_api_register()
{
    int ret = 0;
    ret = at_api_register(NULL);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_apt_register for passing NULL failed!");

    ret = at_api_register(&api);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_api_register for init is NULL, failed!");

    api.init = demo_init;
    ret = at_api_register(&api);
    TEST_ASSERT_MSG((ret == AT_OK), "test at_api_register for init normal failde!");

    return;
}

void TestAtApiInterface::test_at_api_connect()
{
    int ret = 0;
    int8_t * host = (int8_t*)"localhost";
    int8_t * port = (int8_t*)"9999";
    int32_t proto = 0;
    ret = at_api_connect((char*)host, (char*)port, proto);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_api_connect for connect is NULL failed!");
    
    api.connect = demo_connect;
    ret = at_api_connect((char*)host, (char*)port, proto);
    TEST_ASSERT_MSG((ret == AT_OK), "test at_api_connect for connect is normal failed!");
    return;
}

void TestAtApiInterface::test_at_api_send()
{
    int ret = 0;
    
    ret = at_api_send(0, NULL, 0);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_api_send for send_func is NULL failed!");
    
    api.send = demo_send;
    ret = at_api_send(0, NULL, 0);
    TEST_ASSERT_MSG((ret == AT_OK), "test at_api_send for send_func is normal failed!");
    return;
}
void TestAtApiInterface::test_at_api_recv()
{
    int ret = 0;
    
    ret = at_api_recv(0, NULL, 0);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_api_recv for recv_func is NULL failed!");
    
    api.recv = demo_recv;
    ret = at_api_recv(0, NULL, 0);
    TEST_ASSERT_MSG((ret == AT_OK), "test at_api_recv for recv_func is normal failed!");
    return;
}

void TestAtApiInterface::test_at_api_recv_timeout()
{
    int ret = 0;
    
    ret = at_api_recv_timeout(0, NULL, 0, 0xffff);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_api_recv_timeout for recv_timeout_func is NULL failed!");
    
    api.recv_timeout = demo_recv_timeout;
    ret = at_api_recv_timeout(0, NULL, 0, 0xffff);
    TEST_ASSERT_MSG((ret == AT_OK), "test at_api_recv_timeout for recv_timeout_func is normal failed!");
    return;
}

void TestAtApiInterface::test_at_api_close()
{
    int ret = 0;
    
    ret = at_api_close(0);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_api_close for close_func is NULL failed!");
    
    api.close = demo_close;
    ret = at_api_close(0);
    TEST_ASSERT_MSG((ret == AT_OK), "test at_api_close for send_close is normal failed!");
    return;
}


TestAtApiInterface::TestAtApiInterface()
{
    TEST_ADD(TestAtApiInterface::test_at_api_register);
    TEST_ADD(TestAtApiInterface::test_at_api_connect);
    TEST_ADD(TestAtApiInterface::test_at_api_send);
    TEST_ADD(TestAtApiInterface::test_at_api_recv);
    TEST_ADD(TestAtApiInterface::test_at_api_recv_timeout);
    TEST_ADD(TestAtApiInterface::test_at_api_close);
}

void TestAtApiInterface::setup()
{
    return;
}
void TestAtApiInterface::tear_down()
{
    return ;
}


