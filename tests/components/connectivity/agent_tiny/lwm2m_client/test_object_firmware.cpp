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

#include "test_object_firmware.h"

/* testcase for read 5/0 in object_app.c
*/

void TestObjectFirmware::test_prv_firmware_read()
{
    int result;
    lwm2m_uri_t uri = {.flag = 0x07, .objectId = 5, .instanceId = 0, .resourceId = 0};
    int len = 0;
    int i = 0;
    lwm2m_data_t *data = NULL;
    lwm2m_data_t *data1 = NULL;
    lwm2m_object_t *testObj = NULL;
    atiny_param_t *atiny_pa = NULL;

    testObj = get_object_firmware(atiny_pa);
    TEST_ASSERT(testObj->readFunc != NULL);

    lwm2m_list_t *list = lwm2m_list_find(testObj->instanceList, uri.instanceId);
    TEST_ASSERT(list != NULL);

    uri.instanceId = 1;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_404_NOT_FOUND, result);
    uri.instanceId = 0;
    
    len = 1; 
    data1 = lwm2m_data_new(len);
    data1->id = 0;
    result = testObj->readFunc(uri.instanceId, &len, &data1, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_405_METHOD_NOT_ALLOWED, result);
    data1->id = 2;
    result = testObj->readFunc(uri.instanceId, &len, &data1, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_405_METHOD_NOT_ALLOWED, result);
    data1->id = 9;
    result = testObj->readFunc(uri.instanceId, &len, &data1, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_404_NOT_FOUND, result);
    lwm2m_data_free(1, data1);

    len = 0; 
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    lwm2m_data_free(3, data);

    free_object_firmware(testObj);

}
void TestObjectFirmware::test_prv_firmware_execute()
{
    int result;
    lwm2m_uri_t uri = {.flag = 0x07, .objectId = 5, .instanceId = 0, .resourceId = 0};
    int len = 0;
    uint8_t *buffer = NULL;
    lwm2m_object_t *testObj = NULL;
    atiny_param_t *atiny_pa = NULL;

    testObj = get_object_firmware(atiny_pa);
    TEST_ASSERT(testObj->executeFunc != NULL);
    testObj->userData = (uint8_t*)malloc(sizeof(uint8_t));

    lwm2m_list_t *list = lwm2m_list_find(testObj->instanceList, uri.instanceId);
    TEST_ASSERT(list != NULL);

    uri.resourceId = 2;
    result = testObj->executeFunc(uri.instanceId, uri.resourceId, buffer, len, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);

    uri.resourceId = 0;
    result = testObj->executeFunc(uri.instanceId, uri.resourceId, buffer, len, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_405_METHOD_NOT_ALLOWED, result);

    free_object_firmware(testObj);
}

void TestObjectFirmware::test_prv_firmware_write()
{
    int result;
    lwm2m_uri_t uri = {.flag = 0x07, .objectId = 5, .instanceId = 0, .resourceId = 0};
    lwm2m_object_t *testObj = NULL;
    atiny_param_t *atiny_pa = NULL;
    lwm2m_data_t* dataArray = NULL;
    int len = 1;
 
    testObj = get_object_firmware(atiny_pa);
    TEST_ASSERT_MSG(testObj != NULL, "get object device failed\r\n");
    dataArray = (lwm2m_data_t*)malloc(sizeof(lwm2m_data_t));


    uri.instanceId = 1;
    result = testObj->writeFunc(uri.instanceId, 1, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_404_NOT_FOUND, result);

    uri.instanceId = 0;


    dataArray->id = 1;
    dataArray->type = LWM2M_TYPE_BOOLEAN;
    result = testObj->writeFunc(uri.instanceId, 1, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);

    dataArray->id = 0;
    result = testObj->writeFunc(uri.instanceId, 1, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_405_METHOD_NOT_ALLOWED, result);


    free(dataArray);
    free_object_firmware(testObj);
}

TestObjectFirmware::TestObjectFirmware()
{
    TEST_ADD(TestObjectFirmware::test_prv_firmware_read);
    TEST_ADD(TestObjectFirmware::test_prv_firmware_execute);
    TEST_ADD(TestObjectFirmware::test_prv_firmware_write);
}

void TestObjectFirmware::setup()
{
    std::cout << "in steup\n";
}

void TestObjectFirmware::tear_down()
{
    std::cout << "in teardown\n";
}


