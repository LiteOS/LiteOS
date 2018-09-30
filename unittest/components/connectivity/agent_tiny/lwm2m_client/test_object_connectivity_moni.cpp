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
#include "test_object_connectivity_moni.h"
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "object_comm.h"
#include "atiny_rpt.h"

    extern lwm2m_object_t *get_object_conn_m(atiny_param_t *atiny_params);
    extern void free_object_conn_m(lwm2m_object_t *objectP);
    extern uint8_t connectivity_moni_change(lwm2m_data_t *dataArray,
                                            lwm2m_object_t *objectP);

    extern void *lwm2m_malloc(size_t s);
    extern lwm2m_data_t *lwm2m_data_new(int size);

    static lwm2m_data_t *stub_lwm2m_data_new(int size)
    {
        return NULL;
    }

    static void *stub_lwm2m_malloc(size_t s)
    {
        return NULL;
    }

    static void *stub_lwm2m_malloc_2(size_t s)
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

    static void *stub_lwm2m_malloc_3(size_t s)
    {
        static int i = 0;
        if( i++ <= 1)
        {
            void *mem = NULL;
            mem = atiny_malloc(s);
            return mem;
        }
        return NULL;
    }
}

/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/

TestObjectConnectivityMoni::TestObjectConnectivityMoni()
{
    TEST_ADD(TestObjectConnectivityMoni::test_get_object_conn_m);
    TEST_ADD(TestObjectConnectivityMoni::test_free_object_conn_m);
    TEST_ADD(TestObjectConnectivityMoni::test_connectivity_moni_change);

    TEST_ADD(TestObjectConnectivityMoni::test_prv_read);
}
TestObjectConnectivityMoni::~TestObjectConnectivityMoni()
{

}

void TestObjectConnectivityMoni::test_get_object_conn_m(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pConnObject = NULL;

    stubInfo stub_info;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &stub_info);
    pConnObject = get_object_conn_m(&atiny_params);
    TEST_ASSERT_MSG((pConnObject == NULL), "get_object_conn_m(...) failed");
    cleanStub(&stub_info);

    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc_2, &stub_info);
    pConnObject = get_object_conn_m(&atiny_params);
    TEST_ASSERT_MSG((pConnObject == NULL), "get_object_conn_m(...) failed");
    cleanStub(&stub_info);

    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc_3, &stub_info);
    pConnObject = get_object_conn_m(&atiny_params);
    TEST_ASSERT_MSG((pConnObject == NULL), "get_object_conn_m(...) failed");
    cleanStub(&stub_info);

    pConnObject = get_object_conn_m(&atiny_params);
    TEST_ASSERT_MSG((pConnObject != NULL), "get_object_conn_m(...) failed");

    free_object_conn_m(pConnObject);

}
void TestObjectConnectivityMoni::test_free_object_conn_m(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pConnObject = NULL;

    pConnObject = get_object_conn_m(&atiny_params);
    TEST_ASSERT_MSG((pConnObject != NULL), "get_object_conn_m(...) failed");

    uint8_t ret = 0;
    free_object_conn_m(pConnObject);
    TEST_ASSERT_MSG((ret == 0), "free_object_conn_m(...) failed");
}
void TestObjectConnectivityMoni::test_connectivity_moni_change(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pConnObject = NULL;

    pConnObject = get_object_conn_m(&atiny_params);
    TEST_ASSERT_MSG((pConnObject != NULL), "get_object_conn_m(...) failed");

    lwm2m_data_t data;
    uint8_t ret = 0;
    memset(&data, 0, sizeof(lwm2m_data_t));
    ret = connectivity_moni_change(&data, pConnObject);
    TEST_ASSERT_MSG((COAP_405_METHOD_NOT_ALLOWED == ret), "connectivity_moni_change(...) failed");

    // through read
    if(pConnObject->readFunc != NULL)
    {
        int num = 0;
        lwm2m_data_t *pData = NULL;
        ret = pConnObject->readFunc(0, &num, &pData, NULL, pConnObject);
        TEST_ASSERT_MSG((COAP_205_CONTENT == ret),
                        "prv_read(...) failed in object_connectivity_moni.c");

        for(int i = 0; i < num; i++)
        {
            ret = connectivity_moni_change(&pData[i], pConnObject);
            TEST_ASSERT_MSG((COAP_204_CHANGED == ret || COAP_405_METHOD_NOT_ALLOWED == ret),
                            "connectivity_moni_change(...) failed");
        }

        // free pData
        if(pData != NULL)
        {
            lwm2m_data_free(num, pData);
            pData = NULL;
        }
    }
    else
    {
        // TODO
        // fill data to test
    }

    free_object_conn_m(pConnObject);
}

void TestObjectConnectivityMoni::test_prv_read(void)
{
    atiny_param_t atiny_params;
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;
    lwm2m_object_t *pConnObject = NULL;

    pConnObject = get_object_conn_m(&atiny_params);
    TEST_ASSERT_MSG((pConnObject != NULL && pConnObject->readFunc != NULL),
                    "get_object_conn_m(...) failed");

    int num = 0;
    lwm2m_data_t *pData = NULL;
    uint8_t ret = 0;
    // read invalid instance id
    ret = pConnObject->readFunc(1, &num, &pData, NULL, pConnObject);
    TEST_ASSERT_MSG((COAP_404_NOT_FOUND == ret),
                    "prv_read(...) failed in object_connectivity_moni.c");

    // read valid instance 0
    stubInfo stub_info;
    setStub((void *)lwm2m_data_new, (void *)stub_lwm2m_data_new, &stub_info);
    ret = pConnObject->readFunc(0, &num, &pData, NULL, pConnObject);
    TEST_ASSERT_MSG((COAP_500_INTERNAL_SERVER_ERROR == ret),
                    "prv_read(...) failed in object_connectivity_moni.c");
    cleanStub(&stub_info);

    ret = pConnObject->readFunc(0, &num, &pData, NULL, pConnObject);
    TEST_ASSERT_MSG((COAP_205_CONTENT == ret),
                    "prv_read(...) failed in object_connectivity_moni.c");

    // free pData
    if(pData != NULL)
    {
        lwm2m_data_free(num, pData);
        pData = NULL;
    }

    free_object_conn_m(pConnObject);
}

/* Private functions --------------------------------------------------------*/

