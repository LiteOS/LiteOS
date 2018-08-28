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

#include "test_object_device.h"
#include <inttypes.h>

/* testcase for read 3/0 in object_app.c
*/
extern "C" {
    int stub_discover_serialize(lwm2m_context_t *contextP,
                                lwm2m_uri_t *uriP,
                                lwm2m_server_t *serverP,
                                int size,
                                lwm2m_data_t *dataP,
                                uint8_t **bufferP)
    {
        return 1;
    }
    int stub_atiny_cmd_ioctl(atiny_cmd_e cmd, char *arg, int len)
    {
        return ATINY_ARG_INVALID;
    }
}

void TestObjectDevice::test_prv_device_read()
{
    int result;
    int len = 1;
    lwm2m_uri_t uri = {.flag = 0x07, .objectId = 3, .instanceId = 0, .resourceId = 0};
    lwm2m_data_t *data = NULL;
    lwm2m_object_t *testObj = NULL;
    atiny_param_t *atiny_pa = NULL;
    const char *facturer = "uuuuu";

    testObj = get_object_device(atiny_pa, facturer);
    TEST_ASSERT(testObj->readFunc != NULL);

    lwm2m_list_t *list = lwm2m_list_find(testObj->instanceList, uri.instanceId);
    TEST_ASSERT(list != NULL);

    data = lwm2m_data_new(1);
    uri.resourceId = 0;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    lwm2m_data_free(1, data);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);

    data = lwm2m_data_new(1);
    uri.resourceId = 1;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    stubInfo si_atiny_cmd_ioctl;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_atiny_cmd_ioctl);
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    cleanStub(&si_atiny_cmd_ioctl);
    lwm2m_data_free(1, data);

    data = lwm2m_data_new(1);
    uri.resourceId = 2;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    stubInfo si_atiny_cmd_ioctl1;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_atiny_cmd_ioctl1);
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    cleanStub(&si_atiny_cmd_ioctl1);
    lwm2m_data_free(1, data);
    
    data = lwm2m_data_new(1);
    uri.resourceId = 3;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    stubInfo si_atiny_cmd_ioctl2;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_atiny_cmd_ioctl2);
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    cleanStub(&si_atiny_cmd_ioctl2);
    lwm2m_data_free(1, data);

    data = lwm2m_data_new(1);
    uri.resourceId = 6;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    stubInfo si_atiny_cmd_ioctl3;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_atiny_cmd_ioctl3);
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    cleanStub(&si_atiny_cmd_ioctl3);
    lwm2m_data_free(1, data);

    data = lwm2m_data_new(1);
    uri.resourceId = 7;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    stubInfo si_atiny_cmd_ioctl4;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_atiny_cmd_ioctl4);
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    cleanStub(&si_atiny_cmd_ioctl4);
    lwm2m_data_free(1, data);

    data = lwm2m_data_new(1);
    uri.resourceId = 8;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    stubInfo si_atiny_cmd_ioctl5;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_atiny_cmd_ioctl5);
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    cleanStub(&si_atiny_cmd_ioctl5);
    lwm2m_data_free(1, data);

    data = lwm2m_data_new(1);
    uri.resourceId = 9;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    stubInfo si_atiny_cmd_ioctl6;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_atiny_cmd_ioctl6);
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    cleanStub(&si_atiny_cmd_ioctl6);
    lwm2m_data_free(1, data);

    data = lwm2m_data_new(1);
    uri.resourceId = 10;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    stubInfo si_atiny_cmd_ioctl7;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_atiny_cmd_ioctl7);
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    cleanStub(&si_atiny_cmd_ioctl7);
    lwm2m_data_free(1, data);

    data = lwm2m_data_new(1);
    uri.resourceId = 11;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    stubInfo si_atiny_cmd_ioctl8;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_atiny_cmd_ioctl8);
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    cleanStub(&si_atiny_cmd_ioctl8);
    lwm2m_data_free(1, data);

    data = lwm2m_data_new(1);
    uri.resourceId = 13;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    lwm2m_data_free(1, data);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);

    data = lwm2m_data_new(1);
    uri.resourceId = 15;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    lwm2m_data_free(1, data);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);

    data = lwm2m_data_new(1);
    uri.resourceId = 16;
    data->id = uri.resourceId;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    stubInfo si_atiny_cmd_ioctl9;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_atiny_cmd_ioctl9);
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    cleanStub(&si_atiny_cmd_ioctl9);
    lwm2m_data_free(1, data);

    len = 0;
    result = testObj->readFunc(uri.instanceId, &len, &data, NULL, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);
    lwm2m_data_free(14, data);

    free_object_device(testObj);

}

void TestObjectDevice::test_prv_device_execute()
{
    int result;
    int len = 0;
    uint8_t *buffer = NULL;
    lwm2m_uri_t uri = {.flag = 0x07, .objectId = 3, .instanceId = 0, .resourceId = 0};
    lwm2m_object_t *testObj = NULL;
    atiny_param_t *atiny_pa = NULL;
    const char *facturer = "uuuuu";

    testObj = get_object_device(atiny_pa, facturer);
    TEST_ASSERT(testObj->executeFunc != NULL);
    testObj->userData = (uint8_t*)malloc(sizeof(uint8_t));

    lwm2m_list_t *list = lwm2m_list_find(testObj->instanceList, uri.instanceId);
    TEST_ASSERT(list != NULL);

    uri.resourceId = 4;
    result = testObj->executeFunc(uri.instanceId, uri.resourceId, buffer, len, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_204_CHANGED, result);

    uri.resourceId = 5;
    result = testObj->executeFunc(uri.instanceId, uri.resourceId, buffer, len, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_204_CHANGED, result);

    uri.resourceId = 12;
    result = testObj->executeFunc(uri.instanceId, uri.resourceId, buffer, len, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_204_CHANGED, result);

    uri.resourceId = 0;
    result = testObj->executeFunc(uri.instanceId, uri.resourceId, buffer, len, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_405_METHOD_NOT_ALLOWED, result);

    free_object_device(testObj);

}
void TestObjectDevice::test_prv_device_discover()
{
    int result;
    lwm2m_uri_t uri1 = {.flag = 0x07, .objectId = 3, .instanceId = 0, .resourceId = 0};
    lwm2m_uri_t uri2 = {.flag = 0x06, .objectId = 3, .instanceId = 0, .resourceId = 0};
    lwm2m_context_t *contextP = NULL;
    lwm2m_object_t *testObj = NULL;
    atiny_param_t *atiny_pa = NULL;
    const char *facturer = "uuuuu";
    size_t *length = NULL;

    contextP = (lwm2m_context_t *)malloc(sizeof(*contextP));
    TEST_ASSERT_MSG(contextP != NULL, "contextP malloc failed\r\n");
    length = (size_t *)malloc(sizeof(*length));
    TEST_ASSERT_MSG(length != NULL, "length malloc failed\r\n");

    contextP->objectList = NULL;
    testObj = get_object_device(atiny_pa, facturer);
    TEST_ASSERT_MSG(testObj != NULL, "get object device failed\r\n");
    contextP->objectList = testObj;


    stubInfo si_discover_serialize;
    setStub((void *)discover_serialize, (void *)stub_discover_serialize, &si_discover_serialize);

    result = object_discover(contextP, &uri1, NULL, NULL, length);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);

    result = object_discover(contextP, &uri2, NULL, NULL, length);
    TEST_ASSERT_EQUALS_MSG(result, COAP_205_CONTENT, result);

    cleanStub(&si_discover_serialize);

    free(length);
    free_object_device(testObj);
    free(contextP);

}
void TestObjectDevice::test_prv_device_write()
{
    int result = 0;
    lwm2m_uri_t uri = {.flag = 0x07, .objectId = 3, .instanceId = 0, .resourceId = 0};
    lwm2m_object_t *testObj = NULL;
    lwm2m_data_t *dataArray = NULL;
    atiny_param_t *atiny_pa = NULL;
    const char *facturer = "uuuuu";

    dataArray = (lwm2m_data_t *)malloc(sizeof(*dataArray));
    memset(dataArray, 0, sizeof(*dataArray));
    TEST_ASSERT_MSG(dataArray != NULL, "dataArray malloc failed\r\n");
    testObj = get_object_device(atiny_pa, facturer);
    TEST_ASSERT_MSG(testObj != NULL, "get object device failed\r\n");

    dataArray->id = 13;
    dataArray->type = LWM2M_TYPE_INTEGER;
    result = testObj->writeFunc(uri.instanceId, 0, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_204_CHANGED, result);

    dataArray->id = 14;
    dataArray->value.asBuffer.buffer = (uint8_t *)"+03:2";
    dataArray->value.asBuffer.length = 3;
    result = testObj->writeFunc(uri.instanceId, 0, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_204_CHANGED, result);
    dataArray->value.asBuffer.buffer = (uint8_t *)"-11:02";
    dataArray->value.asBuffer.length = 6;
    result = testObj->writeFunc(uri.instanceId, 0, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_204_CHANGED, result);
    dataArray->value.asBuffer.buffer = (uint8_t *)"-11:1";
    dataArray->value.asBuffer.length = 5;
    result = testObj->writeFunc(uri.instanceId, 0, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    dataArray->value.asBuffer.buffer = (uint8_t *)"-21:1";
    dataArray->value.asBuffer.length = 5;
    result = testObj->writeFunc(uri.instanceId, 0, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);
    dataArray->value.asBuffer.buffer = (uint8_t *)"-11:02";
    dataArray->value.asBuffer.length = 7;
    result = testObj->writeFunc(uri.instanceId, 0, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_400_BAD_REQUEST, result);

    dataArray->id = 15;
    result = testObj->writeFunc(uri.instanceId, 0, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_204_CHANGED, result);

    dataArray->id = 16;
    result = testObj->writeFunc(uri.instanceId, 0, dataArray, testObj);
    TEST_ASSERT_EQUALS_MSG(result, COAP_405_METHOD_NOT_ALLOWED, result);

    free(dataArray);
    free_object_device(testObj);

}

void TestObjectDevice::test_display_device_object()
{
    lwm2m_object_t *testObj = NULL;
    atiny_param_t *atiny_pa = NULL;
    const char *facturer = "uuuuu";

    testObj = get_object_device(atiny_pa, facturer);
    TEST_ASSERT_MSG(testObj != NULL, "get object device failed\r\n");
    display_device_object(testObj);
    free_object_device(testObj);
}
TestObjectDevice::TestObjectDevice()
{
    TEST_ADD(TestObjectDevice::test_prv_device_read);
    TEST_ADD(TestObjectDevice::test_prv_device_execute);
    TEST_ADD(TestObjectDevice::test_prv_device_discover);
    TEST_ADD(TestObjectDevice::test_prv_device_write);
    TEST_ADD(TestObjectDevice::test_display_device_object);

}

void TestObjectDevice::setup()
{
    std::cout << "in steup\n";
}

void TestObjectDevice::tear_down()
{
    std::cout << "in teardown\n";

}




