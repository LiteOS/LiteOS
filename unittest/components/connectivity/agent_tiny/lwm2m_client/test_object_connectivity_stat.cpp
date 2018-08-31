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

#include "test_object_connectivity_stat.h"

void TestObjectObjectConnectivityStat::test_prv_read()
{
    int result = 0;
    lwm2m_uri_t uri = {.flag = 0x07, .objectId = 7, .instanceId = 0, .resourceId = 0};
    lwm2m_object_t *testObj = NULL;
    int len = 0;
    lwm2m_data_t *dataArray = NULL;

    testObj = get_object_conn_s();
    TEST_ASSERT_MSG(testObj != NULL, "get object device failed\r\n");
    dataArray = lwm2m_data_new(len);
    TEST_ASSERT("dataArray != NULL");

    uri.instanceId = 1;
    result = testObj->readFunc(uri.instanceId, &len, &dataArray, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_404_NOT_FOUND, result);

    uri.instanceId = 0;
    len = 0;
    result = testObj->readFunc(uri.instanceId, &len, &dataArray, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    lwm2m_data_free(len, dataArray);

    free_object_conn_s(testObj);

}


void TestObjectObjectConnectivityStat::test_prv_exec()
{
    int result = 0;
    int len = 0;
    lwm2m_uri_t uri = {.flag = 0x07, .objectId = 7, .instanceId = 0, .resourceId = 0};
    lwm2m_object_t *testObj = NULL;


    testObj = get_object_conn_s();
    TEST_ASSERT_MSG(testObj != NULL, "get object device failed\r\n");

    uri.instanceId = 6;
    result = testObj->executeFunc(uri.instanceId, uri.resourceId, NULL, len, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_404_NOT_FOUND, result);

    uri.instanceId = 0;
    uri.resourceId = 6;
    result = testObj->executeFunc(uri.instanceId, uri.resourceId, NULL, len, testObj);
    printf("46result = %d\n", result);
    TEST_ASSERT_EQUALS_MSG(result, COAP_204_CHANGED, result);

    uri.resourceId = 6;
    len = 1;
    result = testObj->executeFunc(uri.instanceId, uri.resourceId, NULL, len, testObj);
    printf("52result = %d\n", result);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);

    uri.resourceId = 7;
    len = 0;
    result = testObj->executeFunc(uri.instanceId, uri.resourceId, NULL, len, testObj);
    printf("56result = %d\n", result);
    TEST_ASSERT_EQUALS_MSG(result, COAP_405_METHOD_NOT_ALLOWED, result);

    free_object_conn_s(testObj);
}

void TestObjectObjectConnectivityStat::test_conn_s_updateTxStatistic()
{
    int result = 0;
    lwm2m_object_t *testObj = NULL;
    uint16_t txDataByte = 8;

    testObj = get_object_conn_s();
    TEST_ASSERT_MSG(testObj != NULL, "get object device failed\r\n");
    ((conn_s_data_t *) (testObj->userData))->collectDataStarted = true;
    conn_s_updateTxStatistic(testObj, txDataByte, true);

    free_object_conn_s(testObj);
}
void TestObjectObjectConnectivityStat::test_conn_s_updateRxStatistic()
{
    int result = 0;
    lwm2m_object_t *testObj = NULL;
    uint16_t rxDataByte = 8;

    testObj = get_object_conn_s();
    TEST_ASSERT_MSG(testObj != NULL, "get object device failed\r\n");
    ((conn_s_data_t *) (testObj->userData))->collectDataStarted = true;
    conn_s_updateRxStatistic(testObj, rxDataByte, true);

    free_object_conn_s(testObj);
}






TestObjectObjectConnectivityStat::TestObjectObjectConnectivityStat()
{
    TEST_ADD(TestObjectObjectConnectivityStat::test_prv_read);
    TEST_ADD(TestObjectObjectConnectivityStat::test_prv_exec);
    TEST_ADD(TestObjectObjectConnectivityStat::test_conn_s_updateTxStatistic);
    TEST_ADD(TestObjectObjectConnectivityStat::test_conn_s_updateRxStatistic);
}

void TestObjectObjectConnectivityStat::setup()
{
    std::cout << "in steup\n";
}

void TestObjectObjectConnectivityStat::tear_down()
{
    std::cout << "in teardown\n";
}