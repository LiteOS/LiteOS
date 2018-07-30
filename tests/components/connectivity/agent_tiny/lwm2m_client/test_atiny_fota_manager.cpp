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
#include "test_atiny_fota_manager.h"
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
typedef void (* notify_ack_callback)(atiny_report_type_e type, int cookie, data_send_status_e status);
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
extern void atiny_fota_manager_init(atiny_fota_manager_s *thi);
//extern int atiny_fota_start_download(atiny_fota_state_s * thi, const char *uri);
extern int atiny_fota_manager_finish_download(atiny_fota_manager_s *thi, int result);
extern void lwm2m_resource_value_changed(lwm2m_context_t * contextP, lwm2m_uri_t * uriP);
extern void atiny_fota_manager_update_notify(firmware_update_rst_e rst, void *param);
extern void atiny_fota_manager_destroy(atiny_fota_manager_s *thi);
extern lwm2m_context_t* atiny_fota_manager_get_lwm2m_context(atiny_fota_manager_s *thi);
extern atiny_fota_manager_s * atiny_fota_manager_get_instance(void);
extern char * atiny_fota_manager_get_pkg_uri(const atiny_fota_manager_s *thi);
extern int atiny_fota_manager_get_state(const atiny_fota_manager_s *thi);
extern int atiny_fota_manager_get_update_result(const atiny_fota_manager_s *thi);
extern void atiny_fota_manager_set_update_result(atiny_fota_manager_s *thi, atiny_update_result_e result);
extern int atiny_fota_manager_get_deliver_method(const atiny_fota_manager_s *thi);

//extern int atiny_fota_downloading_state_finish_download(atiny_fota_state_s *thi, int result);
//extern int atiny_fota_idle_state_report_result(atiny_fota_state_s * thi);
extern int lwm2m_stringToUri(const char * buffer, size_t buffer_len, lwm2m_uri_t * uriP);
extern atiny_update_info_s *atiny_update_info_get_instance(void);
extern int atiny_update_info_set(atiny_update_info_s *thi, atiny_fota_storage_device_s *device);
extern void agent_tiny_fota_init(void);
}


struct atiny_fota_manager_tag_s
{
    char *pkg_uri;
    atiny_fota_state_e state;
    atiny_update_result_e update_result;
    atiny_fota_idle_state_s idle_state;
    atiny_fota_downloading_state_s downloading_state;
    atiny_fota_downloaded_state_s downloaded_state;
    atiny_fota_updating_state_s updating_state;
    atiny_fota_state_s *current;
    atiny_fota_storage_device_s *device;
    lwm2m_context_t*  lwm2m_context;
    uint32_t cookie;
    bool wait_ack_flag;
    atiny_fota_state_e rpt_state;
    bool init_flag;
};


int si_atiny_fota_start_download()
{
    return -1;
}

int si_atiny_fota_downloading_state_finish_download(atiny_fota_state_s *thi, int result)
{
    return -1;
}

int si_atiny_fota_idle_state_report_result(atiny_fota_state_s * thi)
{
    return -1;
}

int si_lwm2m_stringToUri(const char * buffer, size_t buffer_len, lwm2m_uri_t * uriP)
{
    return 0;
}

void si_lwm2m_resource_value_changed(lwm2m_context_t * contextP, lwm2m_uri_t * uriP)
{
    return;
}

int si_atiny_update_info_set(atiny_update_info_s *thi, atiny_fota_storage_device_s *device)

{
    return 0;
}

atiny_update_info_s * si_atiny_update_info_get_instance(void)
{
    return NULL;
}






TestAtinyFotaManager::TestAtinyFotaManager()
{
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_start_download);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_execute_update);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_finish_download);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_repot_result);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_set_state);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_set_storage_device);
//    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_get_storage_device);
//    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_update_notify);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_destroy);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_set_lwm2m_context);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_get_lwm2m_context);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_get_instance);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_get_pkg_uri);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_get_data_cfg);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_get_update_result);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_get_deliver_method);
    TEST_ADD(TestAtinyFotaManager::test_atiny_fota_manager_update_notify);


}

void TestAtinyFotaManager::test_atiny_fota_manager_update_notify()
{
    atiny_fota_manager_s * test_thi = atiny_fota_manager_get_instance();
    atiny_fota_manager_update_notify(FIRMWARE_UPDATE_RST_SUCCESS, (void *)test_thi);
}

void TestAtinyFotaManager::test_atiny_fota_manager_get_deliver_method()
{
    atiny_fota_manager_s * test_thi;
    int ret = atiny_fota_manager_get_deliver_method(test_thi);
    TEST_ASSERT_MSG((0 == ret),"atiny_fota_manager_get_deliver_method() failed");
}

void TestAtinyFotaManager::test_atiny_fota_manager_get_update_result()
{
    atiny_fota_manager_s * test_thi = NULL;
    int ret = atiny_fota_manager_get_update_result(test_thi);
    TEST_ASSERT_MSG((ATINY_FIRMWARE_UPDATE_NULL == ret),"atiny_fota_manager_get_update_result() failed");
    
}

void TestAtinyFotaManager::test_atiny_fota_manager_get_pkg_uri()
{
    {   
        atiny_fota_manager_s * test_thi = NULL;
        char * ret = atiny_fota_manager_get_pkg_uri(test_thi);
        TEST_ASSERT_MSG((NULL == ret),"atiny_fota_manager_get_pkg_uri() failed");

        test_thi = atiny_fota_manager_get_instance();
        ret = atiny_fota_manager_get_pkg_uri(test_thi);
        TEST_ASSERT_MSG((NULL == ret),"atiny_fota_manager_get_pkg_uri() failed");
    }
    {
        atiny_fota_manager_s * test_thi = NULL;
        int ret = 0;
        ret = atiny_fota_manager_get_rpt_state(test_thi);
        TEST_ASSERT_MSG((ATINY_FOTA_IDLE == ret),"atiny_fota_manager_get_rpt_state() failed");

        test_thi = atiny_fota_manager_get_instance();
        ret = atiny_fota_manager_get_rpt_state(test_thi);
        TEST_ASSERT_MSG((test_thi->rpt_state == ret),"atiny_fota_manager_get_rpt_state() failed");
    }
}

void TestAtinyFotaManager::test_atiny_fota_manager_get_instance()
{
    atiny_fota_manager_s * test_thi = NULL;
    test_thi = atiny_fota_manager_get_instance();
    TEST_ASSERT_MSG((test_thi != NULL),"atiny_fota_manager_get_instance() failed");
}

void TestAtinyFotaManager::test_atiny_fota_manager_start_download()
{
    int ret = 0;
    atiny_fota_manager_s * test_thi = NULL;
    char test_uri[]  = "hello,world";
    char p[] = "hello,world";
    uint32_t test_len = 11;
    ret = atiny_fota_manager_start_download(test_thi, test_uri, test_len);
    TEST_ASSERT_MSG((ret == ATINY_ARG_INVALID), "atiny_fota_manager_start_download() failed");

    test_thi = (atiny_fota_manager_s*)malloc(sizeof(atiny_fota_manager_s));
    memset(test_thi, 0, sizeof(atiny_fota_manager_s));
    test_thi->rpt_state = ATINY_FOTA_DOWNLOADING;
    ret = atiny_fota_manager_start_download(test_thi, test_uri, test_len);
    TEST_ASSERT_MSG((ret == ATINY_ERR), "atiny_fota_manager_start_download() failed");

    test_thi->rpt_state = ATINY_FOTA_IDLE;
    ret = atiny_fota_manager_start_download(test_thi, test_uri, test_len);
    TEST_ASSERT_MSG((ret == ATINY_ERR), "atiny_fota_manager_start_download() failed");
    free(test_thi);

    test_thi = atiny_fota_manager_get_instance();
    test_thi->pkg_uri = (char *)malloc(10);
    memset(test_thi->pkg_uri, 0, sizeof(10));
    test_thi->lwm2m_context = (lwm2m_context_t *)malloc(sizeof(lwm2m_context_t));
    memset(test_thi->lwm2m_context, 0, sizeof(lwm2m_context_t));
  //  test_thi->lwm2m_context->objectList = (lwm2m_object_t*)malloc(sizeof(lwm2m_object_t));
    test_thi->lwm2m_context->objectList = NULL;
    ret = atiny_fota_manager_start_download(test_thi, test_uri, test_len);
    TEST_ASSERT_MSG((ret == ATINY_OK), "atiny_fota_manager_start_download() failed");
    free(test_thi->pkg_uri);
    free(test_thi->lwm2m_context);
 //   free(test_thi->lwm2m_context->objectList);
}

void TestAtinyFotaManager::test_atiny_fota_manager_execute_update()
{
    int ret = 0;
    atiny_fota_manager_s * test_thi = NULL;
    ret = atiny_fota_manager_execute_update(test_thi);
    TEST_ASSERT_MSG((ret == ATINY_ARG_INVALID), "atiny_fota_manager_execute_updates() failed");

    test_thi = (atiny_fota_manager_s*)malloc(sizeof(atiny_fota_manager_s));
    memset(test_thi, 0, sizeof(atiny_fota_manager_s));
    test_thi->rpt_state = ATINY_FOTA_DOWNLOADING;
    ret = atiny_fota_manager_execute_update(test_thi);
    TEST_ASSERT_MSG((ret == ATINY_ERR), "atiny_fota_manager_execute_updates() failed");

    test_thi->rpt_state = ATINY_FOTA_IDLE;
    test_thi->state = 0;
    test_thi->current = NULL;
    ret = atiny_fota_manager_execute_update(test_thi);
    TEST_ASSERT_MSG((ret == ATINY_ERR), "atiny_fota_manager_execute_updates() failed");
    free(test_thi);

    test_thi = atiny_fota_manager_get_instance();
    test_thi->state = 0;
    test_thi->rpt_state = 0;
    if(test_thi->current == NULL)
        printf("*******************************");
    ret = atiny_fota_manager_execute_update(test_thi);
    TEST_ASSERT_MSG((ret == ATINY_ERR), "atiny_fota_manager_start_download() failed");
}

void TestAtinyFotaManager::test_atiny_fota_manager_finish_download()
{
    int ret = 0;
    atiny_fota_manager_s * test_thi = NULL;     
    ret = atiny_fota_manager_finish_download(test_thi, 0);
    TEST_ASSERT_MSG((ret == ATINY_ARG_INVALID), "test_atiny_fota_manager_finish_download() failed");

    test_thi = (atiny_fota_manager_s*)malloc(sizeof(atiny_fota_manager_s));
    memset(test_thi, 0, sizeof(atiny_fota_manager_s));
    test_thi->rpt_state = ATINY_FOTA_DOWNLOADING;
    ret = atiny_fota_manager_finish_download(test_thi, 0);
    TEST_ASSERT_MSG((ret == ATINY_ERR), "test_atiny_fota_manager_finish_download() failed");

    test_thi->rpt_state = 0;
    test_thi->state = 0;
    test_thi->current = NULL;
    ret = atiny_fota_manager_finish_download(test_thi, 0);
    TEST_ASSERT_MSG((ret == ATINY_ERR), "test_atiny_fota_manager_finish_download() failed");

    free(test_thi);
    test_thi = atiny_fota_manager_get_instance();
    test_thi->state = 0;
    test_thi->rpt_state = 0;
    ret = atiny_fota_manager_finish_download(test_thi, 0);
    TEST_ASSERT_MSG((ret == ATINY_ERR), "test_atiny_fota_manager_finish_download() failed");
}

void TestAtinyFotaManager::test_atiny_fota_manager_repot_result()
{
    int ret = 0;
    atiny_fota_manager_s * test_thi = NULL;     
    ret = atiny_fota_manager_repot_result(test_thi);
    TEST_ASSERT_MSG((ret == ATINY_ARG_INVALID), "atiny_fota_manager_repot_result() failed");

    test_thi = (atiny_fota_manager_s*)malloc(sizeof(atiny_fota_manager_s));
    memset(test_thi, 0, sizeof(atiny_fota_manager_s));
    test_thi->current = NULL;
    ret = atiny_fota_manager_repot_result(test_thi);
    TEST_ASSERT_MSG((ret == ATINY_ERR), "atiny_fota_manager_repot_result() failed");
    free(test_thi);

    test_thi = atiny_fota_manager_get_instance();
    ret = atiny_fota_manager_repot_result(test_thi);
    TEST_ASSERT_MSG((ret == ATINY_OK), "atiny_fota_manager_repot_result() failed");
}

void TestAtinyFotaManager::test_atiny_fota_manager_set_state()
{
    int ret = 0;
    atiny_fota_manager_s * test_thi = NULL;     
    ret = atiny_fota_manager_set_state(test_thi, 0);
    TEST_ASSERT_MSG((ret == ATINY_ARG_INVALID), "atiny_fota_manager_set_state() failed");

    test_thi = (atiny_fota_manager_s*)malloc(sizeof(atiny_fota_manager_s));
    memset(test_thi, 0, sizeof(atiny_fota_manager_s));
    ret = atiny_fota_manager_set_state(test_thi, 5);
    TEST_ASSERT_MSG((ret == ATINY_ARG_INVALID), "atiny_fota_manager_set_state() failed");
    free(test_thi);

    test_thi->state = 0;
    ret = atiny_fota_manager_set_state(test_thi, 2);
    TEST_ASSERT_MSG((ret == ATINY_OK), "atiny_fota_manager_set_state() failed");
}

void TestAtinyFotaManager::test_atiny_fota_manager_set_storage_device()
{
    int ret = 0;
    atiny_fota_manager_s * test_thi = NULL;     
    atiny_fota_storage_device_s *test_device = NULL;
//    test_device = (atiny_fota_storage_device_s *)malloc(sizeof(atiny_fota_storage_device_s));
//    memset(test_device, 0, sizeof(atiny_fota_storage_device_s));
    ret = atiny_fota_manager_set_storage_device(test_thi, test_device);
    TEST_ASSERT_MSG((ret == ATINY_ARG_INVALID), "atiny_fota_manager_set_storage_device() failed");

    test_thi = atiny_fota_manager_get_instance();
    
    agent_tiny_fota_init();
    (void)atiny_cmd_ioctl(ATINY_GET_FOTA_STORAGE_DEVICE, (char * )&test_device, sizeof(test_device));
    
    
    ret = atiny_fota_manager_set_storage_device(test_thi, test_device);
    TEST_ASSERT_MSG((ret == ATINY_OK), "atiny_fota_manager_set_storage_device() failed");
}

void TestAtinyFotaManager::test_atiny_fota_manager_destroy()
{
    atiny_fota_manager_s * test_thi = NULL;     
    atiny_fota_manager_destroy(test_thi);
    TEST_ASSERT_MSG((NULL == test_thi), "atiny_fota_manager_destroy() failed");

    test_thi = atiny_fota_manager_get_instance();
    test_thi->pkg_uri = (char *)atiny_malloc(10);
    atiny_fota_manager_destroy(test_thi);
    TEST_ASSERT_MSG((NULL == test_thi->pkg_uri), "atiny_fota_manager_destroy() failed");
}

void TestAtinyFotaManager::test_atiny_fota_manager_set_lwm2m_context()
{
    int ret = 0;
    atiny_fota_manager_s * test_thi = NULL; 
    lwm2m_context_t * test_context = NULL;
    ret = atiny_fota_manager_set_lwm2m_context(test_thi, test_context);
    TEST_ASSERT_MSG((ret == ATINY_ARG_INVALID), "atiny_fota_manager_set_lwm2m_context() failed");

    test_thi = atiny_fota_manager_get_instance();
    //test_thi->init_flag = false;
    ret = atiny_fota_manager_set_lwm2m_context(test_thi, test_context);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_fota_manager_set_lwm2m_context() failed");
}

void TestAtinyFotaManager::test_atiny_fota_manager_get_lwm2m_context()
{
    atiny_fota_manager_s * test_thi = NULL; 
    lwm2m_context_t * test_context = atiny_fota_manager_get_lwm2m_context(test_thi);
    TEST_ASSERT_MSG((NULL == test_context), "atiny_fota_manager_get_lwm2m_context() failed");

    test_thi = atiny_fota_manager_get_instance();
    test_thi->lwm2m_context = NULL;
    TEST_ASSERT_MSG((NULL == test_context), "atiny_fota_manager_get_lwm2m_context() failed");
}

void TestAtinyFotaManager::test_atiny_fota_manager_get_data_cfg()
{
    atiny_fota_manager_s * test_thi = NULL;
    lwm2m_data_cfg_t * test_cfg = NULL;
    atiny_fota_manager_get_data_cfg(test_thi, test_cfg);

    test_thi = atiny_fota_manager_get_instance();
    //test_thi->downloading_state = 0;
    atiny_fota_manager_get_data_cfg(test_thi, test_cfg);

    test_cfg = (lwm2m_data_cfg_t *)malloc(sizeof(lwm2m_data_cfg_t));
    memset(test_cfg, 0, sizeof(lwm2m_data_cfg_t));
    atiny_fota_manager_get_data_cfg(test_thi, test_cfg);
    TEST_ASSERT_MSG((0 == test_cfg->type), "atiny_fota_manager_get_lwm2m_context() failed");

    test_thi->wait_ack_flag = true;
    test_thi->cookie = 1;
    data_send_status_e test_status = SENT_SUCCESS;
    notify_ack_callback callback = (notify_ack_callback)test_cfg->callback;
    callback(FIRMWARE_UPDATE_STATE, 1, NOT_SENT);
    //test_cfg->callback((void *)FIRMWARE_UPDATE_STATE);
    free(test_cfg);
}



TestAtinyFotaManager::~TestAtinyFotaManager()
{
}

void TestAtinyFotaManager::setup()
{
    std::cout<<"in steup\n";
}

void TestAtinyFotaManager::tear_down()
{
    std::cout<<"in teardown\n";
}




