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
#include "test_object_server.h"
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "object_comm.h"

    extern void copy_server_object(lwm2m_object_t *objectDest, lwm2m_object_t *objectSrc);
    extern void display_server_object(lwm2m_object_t *object);
    extern lwm2m_object_t *get_server_object(int serverId,
            const char *binding,
            int lifetime,
            bool storing);
    extern void clean_server_object(lwm2m_object_t *object);

    extern int atiny_cmd_ioctl(atiny_cmd_e cmd, char *arg, int len);
    extern lwm2m_data_t *lwm2m_data_new(int size);
    extern void *lwm2m_malloc(size_t s);
    extern int atiny_set_max_rpt_cnt(const lwm2m_uri_t *uri, uint32_t max_rpt_cnt);

    static int stub_atiny_cmd_ioctl(atiny_cmd_e cmd, char *arg, int len)
    {
        return ATINY_ARG_INVALID;
    }

    static lwm2m_data_t *stub_lwm2m_data_new(int size)
    {
        return NULL;
    }

    static void *stub_lwm2m_malloc(size_t s)
    {
        return NULL;
    }

    static void *stub_lwm2m_malloc_ex(size_t s)
    {
        static int i = 0;
        if( i++ == 0)
        {
            void *mem = NULL;
            mem = atiny_malloc(s);
            return mem;
        }
        return NULL;
    }

    static int stub_atiny_set_max_rpt_cnt(const lwm2m_uri_t *uri, uint32_t max_rpt_cnt)
    {
        return ATINY_RESOURCE_NOT_FOUND;
    }
}
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestObjectServer::TestObjectServer()
{
    TEST_ADD(TestObjectServer::test_copy_server_object);
    TEST_ADD(TestObjectServer::test_display_server_object);
    TEST_ADD(TestObjectServer::test_get_server_object);
    TEST_ADD(TestObjectServer::test_clean_server_object);

    TEST_ADD(TestObjectServer::test_prv_server_read);
    TEST_ADD(TestObjectServer::test_prv_server_discover);
    TEST_ADD(TestObjectServer::test_prv_server_write);
    TEST_ADD(TestObjectServer::test_prv_server_create);
    TEST_ADD(TestObjectServer::test_prv_server_delete);
    TEST_ADD(TestObjectServer::test_prv_server_execute);
}

TestObjectServer::~TestObjectServer()
{
}

void TestObjectServer::test_copy_server_object(void)
{
    uint16_t serverId = 123;
    const char *binding = "UQ";
    int life_time = 20;
    lwm2m_object_t *pSrcServerObject = NULL;
    lwm2m_object_t *pDestServerObject = NULL;

    pSrcServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pSrcServerObject != NULL), "get_server_object(...) failed");
    pDestServerObject = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
    TEST_ASSERT_MSG((pDestServerObject != NULL), "lwm2m_malloc(...) failed");
    // p == NULL is not checked in function copy_server_object(lwm2m_object_t *, lwm2m_object_t *)
    uint8_t ret = 0;
    copy_server_object(pDestServerObject, pSrcServerObject);
    TEST_ASSERT_MSG((ret == 0), "copy_server_object(...) failed");

    clean_server_object(pSrcServerObject);
    clean_server_object(pDestServerObject);
}
void TestObjectServer::test_display_server_object(void)
{
    uint16_t serverId = 123;
    char *binding = (char *)"UQ";
    int life_time = 20;
    lwm2m_object_t *pServerObject = NULL;
    uint8_t ret = 0;

    // pServerObject == NULL is not checked in function display_server_object(lwm2m_object_t *)

    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject != NULL), "get_server_object(...) failed");

    display_server_object(pServerObject);
    TEST_ASSERT_MSG((ret == 0), "display_server_object(...) failed");

    clean_server_object(pServerObject);
}
void TestObjectServer::test_get_server_object(void)
{
    uint16_t serverId = 123;
    char *binding = (char *)"UQ";
    int life_time = 20;
    lwm2m_object_t *pServerObject = NULL;

    stubInfo stub_info;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &stub_info);
    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject == NULL), "get_server_object(...) failed");
    cleanStub(&stub_info);

    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc_ex, &stub_info);
    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject == NULL), "get_server_object(...) failed");
    cleanStub(&stub_info);

    binding = (char *)"MJ";
    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject == NULL), "get_server_object(...) failed");

    binding = (char *)"UQ";
    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject != NULL), "get_server_object(...) failed");

    clean_server_object(pServerObject);
}
void TestObjectServer::test_clean_server_object(void)
{
    uint16_t serverId = 123;
    const char *binding = "UQ";
    int life_time = 20;
    lwm2m_object_t *pServerObject = NULL;
    uint8_t ret = 0;

    stubInfo stub_info;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &stub_info);
    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject == NULL), "get_server_object(...) failed");
    cleanStub(&stub_info);
    clean_server_object(pServerObject);
    TEST_ASSERT_MSG((ret == 0), "clean_server_object(...) failed");

    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject != NULL), "get_server_object(...) failed");
    clean_server_object(pServerObject);
    TEST_ASSERT_MSG((ret == 0), "clean_server_object(...) failed");
}

void TestObjectServer::test_prv_server_read(void)
{

    uint16_t serverId = 123;
    char *binding = (char *)"UQ";
    int life_time = 20;
    lwm2m_object_t *pServerObject = NULL;

    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject != NULL && pServerObject->readFunc != NULL),
                    "get_server_object(...) failed");
    // one instance 0

    int num = 0;
    lwm2m_data_t *pData = NULL;
    uint8_t ret = 0;
    // read non-existent instance 1
    ret = pServerObject->readFunc(1, &num, &pData, NULL, pServerObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_server_read(...) failed in object_binary_app_data_container.c");

    // read exist instance 0
    ret = pServerObject->readFunc(0, &num, &pData, NULL, pServerObject);
    TEST_ASSERT_MSG((COAP_205_CONTENT == ret && pData != NULL),
                    "prv_server_read(...) failed in object_binary_app_data_container.c");
    // here num=7, pData!=NULL

    if(pData != NULL)
    {
        lwm2m_data_free(num, pData);
        pData = NULL;
    }

    clean_server_object(pServerObject);
}
void TestObjectServer::test_prv_server_discover(void)
{

    uint16_t serverId = 123;
    char *binding = (char *)"UQ";
    int life_time = 20;
    lwm2m_object_t *pServerObject = NULL;

    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject != NULL && pServerObject->discoverFunc != NULL),
                    "get_server_object(...) failed");
    // one instance 0

    int num = 0;
    lwm2m_data_t *pData = NULL;
    uint8_t ret = 0;
    // discover non-existent instance 1
    // note: pre_server_discover() is not check instance !!
    ret = pServerObject->discoverFunc(1, &num, &pData, pServerObject);
    TEST_ASSERT_MSG((COAP_205_CONTENT == ret),
                    "prv_server_discover(...) failed in object_server.c");

    // discover exist instance 0
    ret = pServerObject->discoverFunc(0, &num, &pData, pServerObject);
    TEST_ASSERT_MSG((COAP_205_CONTENT == ret),
                    "prv_server_discover(...) failed in object_server.c");

    if(pData != NULL)
    {
        lwm2m_data_free(num, pData);
        pData = NULL;
    }

    clean_server_object(pServerObject);
}
void TestObjectServer::test_prv_server_write(void)
{

    uint16_t serverId = 123;
    char *binding = (char *)"UQ";
    int life_time = 20;
    lwm2m_object_t *pServerObject = NULL;

    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject != NULL && pServerObject->writeFunc != NULL),
                    "get_server_object(...) failed");
    // one instance 0

    int num = 0;
    lwm2m_data_t data, *pData = NULL;
    uint8_t ret = 0;
    // write non-existent instance 1
    memset(&data, 0, sizeof(lwm2m_data_t));
    ret = pServerObject->writeFunc(1, num, &data, pServerObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_server_write(...) failed in object_server.c");

    // write exist instance 0
    ret = pServerObject->writeFunc(0, num, &data, pServerObject);
    TEST_ASSERT_MSG((COAP_400_BAD_REQUEST == ret),
                    "prv_server_write(...) failed in object_server.c");

    num = 1;
    data.id = LWM2M_SERVER_SHORT_ID_ID;
    data.type = LWM2M_TYPE_INTEGER;
    data.value.asInteger = 10;
    ret = pServerObject->writeFunc(0, num, &data, pServerObject);
    TEST_ASSERT_MSG((COAP_204_CHANGED == ret),
                    "prv_server_write(...) failed in object_server.c");

    // through read
    if(pServerObject->readFunc != NULL)
    {
        stubInfo stub_info;
        setStub((void *)atiny_set_max_rpt_cnt, (void *)stub_atiny_set_max_rpt_cnt, &stub_info);
        num = 0;
        ret = pServerObject->readFunc(0, &num, &pData, NULL, pServerObject);
        TEST_ASSERT_MSG((COAP_205_CONTENT == ret && pData != NULL),
                        "prv_server_read(...) failed in object_binary_app_data_container.c");

        ret = pServerObject->writeFunc(0, num, pData, pServerObject);
        TEST_ASSERT_MSG((COAP_204_CHANGED == ret),
                        "prv_server_write(...) failed in object_server.c");

        if(pData != NULL)
        {
            lwm2m_data_free(num, pData);
            pData = NULL;
        }
        cleanStub(&stub_info);
    }

    clean_server_object(pServerObject);
}
void TestObjectServer::test_prv_server_create(void)
{

    uint16_t serverId = 123;
    char *binding = (char *)"UQ";
    int life_time = 20;
    lwm2m_object_t *pServerObject = NULL;

    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject != NULL && pServerObject->createFunc != NULL),
                    "get_server_object(...) failed");
    // one instance 0

    int num = 0;
    lwm2m_data_t *pData = NULL;
    uint8_t ret = 0;

    // malloc error test
    stubInfo stub_info;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &stub_info);
    ret = pServerObject->createFunc(0, num, pData, pServerObject);
    TEST_ASSERT_MSG((COAP_500_INTERNAL_SERVER_ERROR == ret),
                    "prv_server_create(...) failed in object_binary_app_data_container.c");
    cleanStub(&stub_info);

    // create exist instance 0
    // note:
    //  1,The existing instance was not checked
    //  2,pData should be not NULL was not checked
#if 0
    ret = pServerObject->createFunc(0, num, pData, pServerObject);
    TEST_ASSERT_MSG((COAP_201_CREATED == ret),
                    "prv_server_create(...) failed in object_binary_app_data_container.c");
#endif
    num = 1;
    pData = lwm2m_data_new(1);
    // test integer type
    pData->id = LWM2M_SERVER_SHORT_ID_ID;
    pData->type = LWM2M_TYPE_INTEGER;
    ret = pServerObject->createFunc(0, num, pData, pServerObject);
    // ret = COAP_406_NOT_ACCEPTABLE
    TEST_ASSERT_MSG((COAP_201_CREATED != ret),
                    "prv_server_create(...) failed in object_binary_app_data_container.c");

    pData->value.asInteger = 100;
    // create new instance 1
    ret = pServerObject->createFunc(1, num, pData, pServerObject);
    TEST_ASSERT_MSG((COAP_201_CREATED == ret),
                    "prv_server_create(...) failed in object_binary_app_data_container.c");

    if(pData != NULL)
    {
        lwm2m_data_free(num, pData);
        pData = NULL;
    }

    clean_server_object(pServerObject);
}
void TestObjectServer::test_prv_server_delete(void)
{

    uint16_t serverId = 123;
    char *binding = (char *)"UQ";
    int life_time = 20;
    lwm2m_object_t *pServerObject = NULL;

    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject != NULL && pServerObject->deleteFunc != NULL),
                    "get_server_object(...) failed");
    // one instance 0

    uint8_t ret = 0;
    // delete non-existent instance 1
    ret = pServerObject->deleteFunc(1, pServerObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_server_delete(...) failed in object_server.c");

    // delete exist instance 0
    ret = pServerObject->deleteFunc(0, pServerObject);
    TEST_ASSERT_MSG((COAP_202_DELETED == ret),
                    "prv_server_delete(...) failed in object_server.c");

    clean_server_object(pServerObject);
}
void TestObjectServer::test_prv_server_execute(void)
{

    uint16_t serverId = 123;
    char *binding = (char *)"UQ";
    int life_time = 20;
    lwm2m_object_t *pServerObject = NULL;

    pServerObject = get_server_object(serverId, binding, life_time, false);
    TEST_ASSERT_MSG((pServerObject != NULL && pServerObject->executeFunc != NULL),
                    "get_server_object(...) failed");
    // one instance 0

    uint8_t ret = 0;
    uint8_t buffer[] = {0x0, 0x1, 0x2};
    int length = sizeof(buffer);
    uint16_t resourceId = 0;
    // execute non-existent instance 1
    ret = pServerObject->executeFunc(1, resourceId, buffer, length, pServerObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_server_execute(...) failed in object_server.c");

    // execute exist instance 0
    ret = pServerObject->executeFunc(0, resourceId, buffer, length, pServerObject);
    TEST_ASSERT_MSG((COAP_405_METHOD_NOT_ALLOWED == ret),
                    "prv_server_execute(...) failed in object_server.c");

    resourceId = LWM2M_SERVER_DISABLE_ID;
    ret = pServerObject->executeFunc(0, resourceId, buffer, length, pServerObject);
    TEST_ASSERT_MSG((COAP_405_METHOD_NOT_ALLOWED == ret),
                    "prv_server_execute(...) failed in object_server.c");

    resourceId = LWM2M_SERVER_UPDATE_ID;
    ret = pServerObject->executeFunc(0, resourceId, buffer, length, pServerObject);
    TEST_ASSERT_MSG((COAP_204_CHANGED == ret),
                    "prv_server_execute(...) failed in object_server.c");

    clean_server_object(pServerObject);
}


/* Private functions --------------------------------------------------------*/

