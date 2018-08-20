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
#include "test_atiny_rpt.h"
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "liblwm2m.h" // need by atiny_rpt.h
#include "agenttiny.h"
#include "atiny_rpt.h"
#include "agent_list.h"

    typedef struct _atiny_rpt_list_t
    {
        atiny_dl_list node;
        lwm2m_uri_t uri;
        atiny_dl_list rpt_list;
        uint32_t rpt_node_cnt;
        uint32_t max_cnt;
    } atiny_rpt_list_t;

    typedef struct
    {
        atiny_dl_list list;
        data_report_t data;
    } atiny_rpt_node_t;

    extern void *lwm2m_malloc(size_t s);

    static void *stub_lwm2m_malloc(size_t s)
    {
        return NULL;
    }

    static void callback(atiny_report_type_e type, int cookie, data_send_status_e status)
    {
        printf("test data_report_t.callback\n");
    }

}
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestAtinyRpt::TestAtinyRpt()
{
    TEST_ADD(TestAtinyRpt::test_atiny_init_rpt);
    TEST_ADD(TestAtinyRpt::test_atiny_add_rpt_uri);
    TEST_ADD(TestAtinyRpt::test_atiny_rm_rpt_uri);

    TEST_ADD(TestAtinyRpt::test_atiny_dequeue_rpt_data);
    TEST_ADD(TestAtinyRpt::test_atiny_queue_rpt_data);
    TEST_ADD(TestAtinyRpt::test_atiny_clear_rpt_data);

    TEST_ADD(TestAtinyRpt::test_atiny_step_rpt);
    TEST_ADD(TestAtinyRpt::test_atiny_destroy_rpt);
    TEST_ADD(TestAtinyRpt::test_atiny_set_max_rpt_cnt);
}
TestAtinyRpt::~TestAtinyRpt()
{

}

void TestAtinyRpt::test_atiny_init_rpt(void)
{
    int ret = 0;
    ret = atiny_init_rpt();
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_rpt() failed");
}
void TestAtinyRpt::test_atiny_add_rpt_uri(void)
{
    lwm2m_uri_t uri;
    rpt_list_t header;

    uri.flag = LWM2M_URI_FLAG_OBJECT_ID | LWM2M_URI_FLAG_INSTANCE_ID | LWM2M_URI_FLAG_RESOURCE_ID;
    uri.objectId = 1;
    uri.instanceId = 0;
    uri.resourceId = 1;

    int ret = 0;
    ret = atiny_init_rpt();
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_rpt() failed");

    ret = atiny_add_rpt_uri(NULL, NULL);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_add_rpt_uri(...) failed");

    ret = atiny_add_rpt_uri(&uri, NULL);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_add_rpt_uri(...) failed");

    stubInfo stub_info;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &stub_info);
    ret = atiny_add_rpt_uri(&uri, &header);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_add_rpt_uri(...) failed");
    cleanStub(&stub_info);

    ret = atiny_add_rpt_uri(&uri, &header);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_add_rpt_uri(...) failed");

    atiny_destroy_rpt();
}
void TestAtinyRpt::test_atiny_rm_rpt_uri(void)
{
    lwm2m_uri_t uri;
    rpt_list_t header;

    uri.flag = LWM2M_URI_FLAG_OBJECT_ID | LWM2M_URI_FLAG_INSTANCE_ID | LWM2M_URI_FLAG_RESOURCE_ID;
    uri.objectId = 1;
    uri.instanceId = 0;
    uri.resourceId = 1;

    int ret = 0;
    ret = atiny_init_rpt();
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_rpt() failed");

    ret = atiny_rm_rpt_uri(NULL);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_rm_rpt_uri(...) failed");

    ret = atiny_rm_rpt_uri(&uri);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_rm_rpt_uri(...) failed");

    // add first
    ret = atiny_add_rpt_uri(&uri, &header);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_add_rpt_uri(...) failed");

    // remove next
    ret = atiny_rm_rpt_uri(&uri);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_rm_rpt_uri(...) failed");

    atiny_destroy_rpt();
}

void TestAtinyRpt::test_atiny_dequeue_rpt_data(void)
{
    lwm2m_uri_t uri;
    data_report_t data;
    rpt_list_t header;

    uri.flag = LWM2M_URI_FLAG_OBJECT_ID | LWM2M_URI_FLAG_INSTANCE_ID | LWM2M_URI_FLAG_RESOURCE_ID;
    uri.objectId = 1;
    uri.instanceId = 0;
    uri.resourceId = 1;

    int ret = 0;
    ret = atiny_init_rpt();
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_rpt() failed");

    ret = atiny_dequeue_rpt_data(NULL, NULL);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_dequeue_rpt_data(...) failed");

    header = (rpt_list_t)lwm2m_malloc(sizeof(struct _atiny_rpt_list_t));
    memset(header, 0, sizeof(struct _atiny_rpt_list_t));
    atiny_list_init(&header->rpt_list);

    ret = atiny_dequeue_rpt_data(header, &data);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_FOUND == ret), "atiny_dequeue_rpt_data(...) failed");

    lwm2m_free(header);

    // add first
    ret = atiny_add_rpt_uri(&uri, &header);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_add_rpt_uri(...) failed");

    // into queue
    memset(&data, 0, sizeof(data_report_t));
    ret = atiny_queue_rpt_data(&uri, &data);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_queue_rpt_data(...) failed");

    // dequeue
    ret = atiny_dequeue_rpt_data(header, &data);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_dequeue_rpt_data(...) failed");

    atiny_destroy_rpt();
}
void TestAtinyRpt::test_atiny_queue_rpt_data(void)
{
    lwm2m_uri_t uri;
    data_report_t data;
    rpt_list_t header;

    uri.flag = LWM2M_URI_FLAG_OBJECT_ID | LWM2M_URI_FLAG_INSTANCE_ID | LWM2M_URI_FLAG_RESOURCE_ID;
    uri.objectId = 1;
    uri.instanceId = 0;
    uri.resourceId = 1;

    int ret = 0;
    ret = atiny_init_rpt();
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_rpt() failed");

    ret = atiny_queue_rpt_data(NULL, NULL);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_queue_rpt_data(...) failed");

    ret = atiny_queue_rpt_data(&uri, &data);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_FOUND == ret), "atiny_queue_rpt_data(...) failed");

    // add first
    ret = atiny_add_rpt_uri(&uri, &header);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_add_rpt_uri(...) failed");

    // into queue
    memset(&data, 0, sizeof(data_report_t));
    ret = atiny_queue_rpt_data(&uri, &data);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_queue_rpt_data(...) failed");

    // dequeue
    ret = atiny_dequeue_rpt_data(header, &data);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_dequeue_rpt_data(...) failed");

    atiny_destroy_rpt();
}
void TestAtinyRpt::test_atiny_clear_rpt_data(void)
{
    lwm2m_uri_t uri;
    rpt_list_t header;

    uri.flag = LWM2M_URI_FLAG_OBJECT_ID | LWM2M_URI_FLAG_INSTANCE_ID | LWM2M_URI_FLAG_RESOURCE_ID;
    uri.objectId = 1;
    uri.instanceId = 0;
    uri.resourceId = 1;

    int ret = 0;
    ret = atiny_init_rpt();
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_rpt() failed");

    int result = 0;
    ret = atiny_clear_rpt_data(NULL, result);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_clear_rpt_data(...) failed");

    ret = atiny_clear_rpt_data(&uri, result);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_FOUND == ret), "atiny_clear_rpt_data(...) failed");

    // add first
    ret = atiny_add_rpt_uri(&uri, &header);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_add_rpt_uri(...) failed");

    ret = atiny_clear_rpt_data(&uri, result);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_clear_rpt_data(...) failed");

    atiny_destroy_rpt();
}

void TestAtinyRpt::test_atiny_step_rpt(void)
{
    lwm2m_context_t context;

    int ret = 0;
    ret = atiny_init_rpt();
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_rpt() failed");

    ret = atiny_step_rpt(&context);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_step_rpt() failed");

    /////// add rpt uri
    lwm2m_uri_t uri;
    rpt_list_t header;

    uri.flag = LWM2M_URI_FLAG_OBJECT_ID | LWM2M_URI_FLAG_INSTANCE_ID | LWM2M_URI_FLAG_RESOURCE_ID;
    uri.objectId = 1;
    uri.instanceId = 0;
    uri.resourceId = 1;

    ret = atiny_add_rpt_uri(&uri, &header);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_add_rpt_uri(...) failed");

    ret = atiny_step_rpt(&context);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_step_rpt() failed");

    atiny_destroy_rpt();
}
void TestAtinyRpt::test_atiny_destroy_rpt(void)
{
    int ret = 0;
    ret = atiny_init_rpt();
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_rpt() failed");

    ret = 0;
    atiny_destroy_rpt();
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy_rpt() failed");

    /////// add rpt uri
    lwm2m_uri_t uri;
    rpt_list_t header;

    uri.flag = LWM2M_URI_FLAG_OBJECT_ID | LWM2M_URI_FLAG_INSTANCE_ID | LWM2M_URI_FLAG_RESOURCE_ID;
    uri.objectId = 1;
    uri.instanceId = 0;
    uri.resourceId = 1;

    ret = atiny_add_rpt_uri(&uri, &header);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_add_rpt_uri(...) failed");

    // into queue
    data_report_t data;
    memset(&data, 0, sizeof(data_report_t));
    data.callback = callback;
    data.buf = (uint8_t *)lwm2m_malloc(1);

    ret = atiny_queue_rpt_data(&uri, &data);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_queue_rpt_data(...) failed");

    ret = 0;
    atiny_destroy_rpt();
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy_rpt() failed");
}
void TestAtinyRpt::test_atiny_set_max_rpt_cnt(void)
{
    lwm2m_uri_t uri;
    rpt_list_t header;

    uri.flag = LWM2M_URI_FLAG_OBJECT_ID | LWM2M_URI_FLAG_INSTANCE_ID | LWM2M_URI_FLAG_RESOURCE_ID;
    uri.objectId = 1;
    uri.instanceId = 0;
    uri.resourceId = 1;

    int ret = 0;
    ret = atiny_init_rpt();
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_rpt() failed");

    ret = atiny_set_max_rpt_cnt(&uri, 10);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_FOUND == ret), "atiny_set_max_rpt_cnt(...) failed");

    // add first
    ret = atiny_add_rpt_uri(&uri, &header);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_add_rpt_uri(...) failed");

    ret = atiny_set_max_rpt_cnt(&uri, 10);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_set_max_rpt_cnt(...) failed");

    atiny_destroy_rpt();
}

/* Private functions --------------------------------------------------------*/

