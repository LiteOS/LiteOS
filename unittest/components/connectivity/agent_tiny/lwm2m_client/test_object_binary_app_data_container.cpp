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
#include "test_object_binary_app_data_container.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "object_comm.h"
#include "atiny_rpt.h"

    extern void display_binary_app_data_object(lwm2m_object_t *object);
    extern lwm2m_object_t *get_binary_app_data_object(atiny_param_t *atiny_params);
    extern void free_binary_app_data_object(lwm2m_object_t *object);
    extern void set_binary_app_data_object_rpt_max_cnt(uint32_t max_rpt_cnt);

    extern lwm2m_data_t *lwm2m_data_new(int size);
    extern void *lwm2m_malloc(size_t s);

    extern int atiny_init_rpt(void);
    extern int atiny_dequeue_rpt_data(rpt_list_t rpt_list,  data_report_t *data);

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

    static int stub_atiny_dequeue_rpt_data(rpt_list_t rpt_list,  data_report_t *data)
    {
        uint8_t buf[] = {0x0, 0x1, 0x2, 0x3, 0x4};
        memset(data, 0, sizeof(data_report_t));
        data->type = APP_DATA;
        data->len = sizeof(buf);
        data->buf = (uint8_t *)lwm2m_malloc(data->len);
        memcpy(data->buf, buf, data->len);
        return ATINY_OK;
    }
}
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestObjectBinaryAppDataContainer::TestObjectBinaryAppDataContainer()
{
    TEST_ADD(TestObjectBinaryAppDataContainer::test_get_binary_app_data_object);
    TEST_ADD(TestObjectBinaryAppDataContainer::test_display_binary_app_data_object);
    TEST_ADD(TestObjectBinaryAppDataContainer::test_free_binary_app_data_object);
    TEST_ADD(TestObjectBinaryAppDataContainer::test_set_binary_app_data_object_rpt_max_cnt);

    TEST_ADD(TestObjectBinaryAppDataContainer::test_prv_read);
    TEST_ADD(TestObjectBinaryAppDataContainer::test_prv_discover);
    TEST_ADD(TestObjectBinaryAppDataContainer::test_prv_write);
    TEST_ADD(TestObjectBinaryAppDataContainer::test_prv_create);
    TEST_ADD(TestObjectBinaryAppDataContainer::test_prv_delete);
    TEST_ADD(TestObjectBinaryAppDataContainer::test_prv_exec);
}
TestObjectBinaryAppDataContainer::~TestObjectBinaryAppDataContainer()
{

}

void TestObjectBinaryAppDataContainer::test_display_binary_app_data_object(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pAppDataObject = NULL;

    /* pAppDataObject == NULL is not checked in function display_binary_app_data_object(lwm2m_object_t *) */

    atiny_init_rpt(); // must before get_binary_app_data_object()
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject != NULL), "get_binary_app_data_object(...) failed");
    uint8_t ret = 0;
    display_binary_app_data_object(pAppDataObject);
    TEST_ASSERT_MSG((ret == 0), "display_binary_app_data_object(...) failed");
    free_binary_app_data_object(pAppDataObject);
}
void TestObjectBinaryAppDataContainer::test_get_binary_app_data_object(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pAppDataObject = NULL;

    stubInfo stub_info;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &stub_info);
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject == NULL), "get_binary_app_data_object(...) failed");
    cleanStub(&stub_info);

    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc_ex, &stub_info);
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject == NULL), "get_binary_app_data_object(...) failed");
    cleanStub(&stub_info);

    atiny_init_rpt(); // must before get_binary_app_data_object()
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject != NULL), "get_binary_app_data_object(...) failed");
    // two instances

    free_binary_app_data_object(pAppDataObject);
}
void TestObjectBinaryAppDataContainer::test_free_binary_app_data_object(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pAppDataObject = NULL;

    /* pAppDataObject == NULL is not checked in function free_binary_app_data_object(lwm2m_object_t *) */

    atiny_init_rpt(); // must before get_binary_app_data_object()
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject != NULL), "get_binary_app_data_object(...) failed");
    uint8_t ret = 0;
    free_binary_app_data_object(pAppDataObject);
    TEST_ASSERT_MSG((ret == 0), "free_binary_app_data_object(...) failed");
}

void TestObjectBinaryAppDataContainer::test_set_binary_app_data_object_rpt_max_cnt(void)
{
    uint8_t ret = 0;
    set_binary_app_data_object_rpt_max_cnt(10);
    TEST_ASSERT_MSG((ret == 0), "set_binary_app_data_object_rpt_max_cnt(...) failed");
}

void TestObjectBinaryAppDataContainer::test_prv_read(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pAppDataObject = NULL;

    atiny_init_rpt(); // must before get_binary_app_data_object()
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject != NULL && pAppDataObject->readFunc != NULL),
                    "get_binary_app_data_object(...) failed");
    // two instances 0,1

    int num = 0;
    lwm2m_data_t *pData = NULL;
    lwm2m_data_cfg_t pDataCfg;
    uint8_t ret = 0;
    // not found instance
    ret = pAppDataObject->readFunc(2, &num, &pData, NULL, pAppDataObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_read(...) failed in object_binary_app_data_container.c");
    // instance 0
    ret = pAppDataObject->readFunc(0, &num, &pData, NULL, pAppDataObject);
    TEST_ASSERT_MSG((COAP_205_CONTENT == ret && pData != NULL),
                    "prv_read(...) failed in object_binary_app_data_container.c");
    // here num=1, pData!=NULL

    // instance 0
    // COAP_404_NOT_FOUND == prv_read_data(...), because of "dequeue empty rpt list"
    ret = pAppDataObject->readFunc(0, &num, &pData, &pDataCfg, pAppDataObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret && pData != NULL),
                    "prv_read(...) failed in object_binary_app_data_container.c");

    // instance 0
    stubInfo stub_info;
    setStub((void *)atiny_dequeue_rpt_data, (void *)stub_atiny_dequeue_rpt_data, &stub_info);
    ret = pAppDataObject->readFunc(0, &num, &pData, &pDataCfg, pAppDataObject);
    TEST_ASSERT_MSG((COAP_205_CONTENT == ret && pData != NULL),
                    "prv_read(...) failed in object_binary_app_data_container.c");
    cleanStub(&stub_info);

    // free pData
    if(pData != NULL)
    {
        lwm2m_data_free(num, pData);
        pData = NULL;
    }

    free_binary_app_data_object(pAppDataObject);
}
void TestObjectBinaryAppDataContainer::test_prv_discover(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pAppDataObject = NULL;

    atiny_init_rpt(); // must before get_binary_app_data_object()
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject != NULL && pAppDataObject->discoverFunc != NULL),
                    "get_binary_app_data_object(...) failed");
    // two instances 0,1

    int num = 0;
    lwm2m_data_t *pData = NULL;
    uint8_t ret = 0;
    ret = pAppDataObject->discoverFunc(0, &num, &pData, pAppDataObject);
    TEST_ASSERT_MSG((COAP_205_CONTENT == ret && pData != NULL),
                    "prv_discover(...) failed in object_binary_app_data_container.c");
    // here num=3, pData!=NULL

    ret = pAppDataObject->discoverFunc(0, &num, &pData, pAppDataObject);
    TEST_ASSERT_MSG((COAP_205_CONTENT == ret && pData != NULL),
                    "prv_discover(...) failed in object_binary_app_data_container.c");

    if(pData != NULL)
    {
        lwm2m_data_free(num, pData);
        pData = NULL;
    }

    free_binary_app_data_object(pAppDataObject);
}
void TestObjectBinaryAppDataContainer::test_prv_write(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pAppDataObject = NULL;

    atiny_init_rpt(); // must before get_binary_app_data_object()
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject != NULL && pAppDataObject->writeFunc != NULL),
                    "get_binary_app_data_object(...) failed");
    // two instances 0,1

    int num = 0;
    lwm2m_data_t data;
    uint8_t ret = 0;
    // write non-existent instance 2
    ret = pAppDataObject->writeFunc(2, num, &data, pAppDataObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_write(...) failed in object_binary_app_data_container.c");

    // write exist instance 1, but no data
    ret = pAppDataObject->writeFunc(1, num, &data, pAppDataObject);
    TEST_ASSERT_MSG((COAP_204_CHANGED == ret),
                    "prv_write(...) failed in object_binary_app_data_container.c");

    // write exist instance 1, one data
    num = 1;
    data.id = 0;
    ret = pAppDataObject->writeFunc(1, num, &data, pAppDataObject);
    TEST_ASSERT_MSG((COAP_204_CHANGED == ret),
                    "prv_write(...) failed in object_binary_app_data_container.c");

    data.id = 1;
    ret = pAppDataObject->writeFunc(1, num, &data, pAppDataObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_write(...) failed in object_binary_app_data_container.c");

    free_binary_app_data_object(pAppDataObject);
}
void TestObjectBinaryAppDataContainer::test_prv_create(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pAppDataObject = NULL;

    atiny_init_rpt(); // must before get_binary_app_data_object()
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject != NULL && pAppDataObject->createFunc != NULL),
                    "get_binary_app_data_object(...) failed");
    // two instances 0,1

    int num = 0;
    lwm2m_data_t *pData = NULL;
    uint8_t ret = 0;
    // create exist instance 0
    // atiny_add_rpt_uri() error!
    ret = pAppDataObject->createFunc(0, num, pData, pAppDataObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_create(...) failed in object_binary_app_data_container.c");

    // create new instance 2
    ret = pAppDataObject->createFunc(2, num, pData, pAppDataObject);
    TEST_ASSERT_MSG((COAP_201_CREATED == ret),
                    "prv_create(...) failed in object_binary_app_data_container.c");

    free_binary_app_data_object(pAppDataObject);
}
void TestObjectBinaryAppDataContainer::test_prv_delete(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pAppDataObject = NULL;

    atiny_init_rpt(); // must before get_binary_app_data_object()
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject != NULL && pAppDataObject->deleteFunc != NULL),
                    "get_binary_app_data_object(...) failed");
    // two instances 0,1

    uint8_t ret = 0;
    // delete non-existent instance 2
    // atiny_add_rpt_uri() error!
    ret = pAppDataObject->deleteFunc(2, pAppDataObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_delete(...) failed in object_binary_app_data_container.c");

    // delete exist instance 1
    ret = pAppDataObject->deleteFunc(1, pAppDataObject);
    TEST_ASSERT_MSG((COAP_202_DELETED == ret),
                    "prv_delete(...) failed in object_binary_app_data_container.c");

    free_binary_app_data_object(pAppDataObject);

}
void TestObjectBinaryAppDataContainer::test_prv_exec(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pAppDataObject = NULL;

    atiny_init_rpt(); // must before get_binary_app_data_object()
    pAppDataObject = get_binary_app_data_object(&atiny_params);
    TEST_ASSERT_MSG((pAppDataObject != NULL && pAppDataObject->executeFunc != NULL),
                    "get_binary_app_data_object(...) failed");
    // two instances 0,1

    uint8_t buffer[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    int length = sizeof(buffer);
    int ret = 0;
    uint16_t resourceId = 0;
    ret = pAppDataObject->executeFunc(0, resourceId, buffer, length, pAppDataObject);
    TEST_ASSERT_MSG((COAP_204_CHANGED == ret),
                    "prv_exec(...) failed in object_binary_app_data_container.c");
    resourceId = 1;
    ret = pAppDataObject->executeFunc(0, resourceId, buffer, length, pAppDataObject);
    TEST_ASSERT_MSG((COAP_405_METHOD_NOT_ALLOWED == ret),
                    "prv_exec(...) failed in object_binary_app_data_container.c");
    resourceId = 2;
    ret = pAppDataObject->executeFunc(0, resourceId, buffer, length, pAppDataObject);
    TEST_ASSERT_MSG((COAP_204_CHANGED == ret),
                    "prv_exec(...) failed in object_binary_app_data_container.c");
    resourceId = 3;
    ret = pAppDataObject->executeFunc(0, resourceId, buffer, length, pAppDataObject);
    TEST_ASSERT_MSG((COAP_405_METHOD_NOT_ALLOWED == ret),
                    "prv_exec(...) failed in object_binary_app_data_container.c");
    resourceId = 4;
    ret = pAppDataObject->executeFunc(0, resourceId, buffer, length, pAppDataObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_exec(...) failed in object_binary_app_data_container.c");

    free_binary_app_data_object(pAppDataObject);
}

/* Private functions --------------------------------------------------------*/

