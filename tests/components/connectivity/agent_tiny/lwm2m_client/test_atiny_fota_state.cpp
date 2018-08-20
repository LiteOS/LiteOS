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

/* Define to prevent recursive inclusion ------------------------------------*/

#include <fstream>
#include <memory>
#include "agenttiny.h"
#include "adapter_layer.h"
#include "atiny_log.h"
#include "test_atiny_fota_state.h"
extern "C"
{
    #include "atiny_fota_state.h" 
    #include "firmware_update.h"
    
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
    int stub_atiny_update_info_read(atiny_update_info_s *thi, atiny_update_info_e type, uint8_t *info, uint32_t len)
    {
        ((lwm2m_observe_info_t*)info)->tokenLen = 10;
        return 0;
    }
    int stub_start_firmware_download(lwm2m_context_t *contextP, char *uri, atiny_fota_storage_device_s *storage_device_p)
    {
        return 0;
    }
    int stub_start_firmware_download2(lwm2m_context_t *contextP, char *uri, atiny_fota_storage_device_s *storage_device_p)
    {
        return -1;
    }
    void stub_lwm2m_resource_value_changed(lwm2m_context_t * contextP, lwm2m_uri_t * uriP)
    {
        return;
    }
    uint8_t stub_lwm2m_send_notify(lwm2m_context_t * contextP, lwm2m_observe_info_t *observe_info, int firmware_update_state)
    {
        return 0;
    }
    
    int stub_atiny_update_info_write(atiny_update_info_s *thi, atiny_update_info_e type, const uint8_t *info, uint32_t len)
    {
        return 0;
    }
    int test_atiny_update_info_read(atiny_update_info_s *thi, atiny_update_info_e type, uint8_t *info, uint32_t len)
    {
        ((lwm2m_observe_info_t *)info)->tokenLen = 1;
        return 0;
    }
    atiny_update_info_s * test_atiny_update_info_get_instance(void)
    {
        return NULL;
    }
    
    int test_atiny_update_info_write(atiny_update_info_s *thi, atiny_update_info_e type, const uint8_t *info, uint32_t len)
    {
        return 0;
    }
    
    int test_get_software_result(atiny_fota_storage_device_s *thi)
    {
        return 0;
    }
    
    int test_active_software(atiny_fota_storage_device_s *thi)
    {
        return 0;
    }
    extern int write_update_info_test(atiny_fota_storage_device_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len);
    int active_software_test(atiny_fota_storage_device_s *thi)
    {
        return 0;
    }
    
    int get_software_result_test(atiny_fota_storage_device_s *thi)
    {
        return 0;
    }
}

void TestAtinyFotaState::test_atiny_fota_state_default_handle()
{
    atiny_fota_state_s thi;
    atiny_fota_state_tag_s thi_pare;
    atiny_fota_manager_s manager;

//    atiny_fota_state_init(&thi, &manager);
//    thi.execute_update(&thi_pare);
}
void TestAtinyFotaState::test_atiny_fota_state_init()
{
    atiny_fota_state_s thi;
    atiny_fota_manager_s manager;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    thi.execute_update = NULL;
    thi.start_download = NULL;
    thi.finish_download = NULL;
    thi.repot_result = NULL;
    thi.recv_notify_ack = NULL;
    thi.manager = NULL;
    
    atiny_fota_state_init(&thi, &manager);
    
    TEST_ASSERT(thi.execute_update != NULL);
    TEST_ASSERT(thi.start_download != NULL);
    TEST_ASSERT(thi.finish_download != NULL);
    TEST_ASSERT(thi.repot_result != NULL);
    TEST_ASSERT(thi.recv_notify_ack != NULL);
    TEST_ASSERT(thi.manager != NULL);
}

void TestAtinyFotaState::test_atiny_fota_idle_state_init()
{
    atiny_fota_state_s thi;
    atiny_fota_manager_s manager;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    thi.execute_update = NULL;
    thi.start_download = NULL;
    thi.finish_download = NULL;
    thi.repot_result = NULL;
    thi.recv_notify_ack = NULL;
    thi.manager = NULL;
    
    atiny_fota_state_init(&thi, &manager);
    
    TEST_ASSERT(thi.execute_update != NULL);
    TEST_ASSERT(thi.start_download != NULL);
    TEST_ASSERT(thi.finish_download != NULL);
    TEST_ASSERT(thi.repot_result != NULL);
    TEST_ASSERT(thi.recv_notify_ack != NULL);
    TEST_ASSERT(thi.manager != NULL);
}

void TestAtinyFotaState::test_atiny_fota_start_download()
{
    atiny_fota_idle_state_s thi;
    atiny_fota_state_s* thi_pare;
    atiny_fota_manager_s manager;
    atiny_fota_manager_s manager_pare;
    char *uri = "/19/1/0";
    int result = 0;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    memset(&manager_pare, 0, sizeof(atiny_fota_manager_s));
    atiny_fota_idle_state_init(&thi, &manager);
    
    thi_pare = (atiny_fota_state_s *)malloc(sizeof(atiny_fota_state_s));
    TEST_ASSERT(thi_pare != NULL);
    thi_pare->manager = &manager_pare;
    
    manager_pare.update_result = 0;
    manager_pare.lwm2m_context = (lwm2m_context_t *)malloc(sizeof(lwm2m_context_t));
    TEST_ASSERT(manager_pare.lwm2m_context != NULL);
    
    stubInfo si;
    stubInfo si1;
    setStub((void *)lwm2m_resource_value_changed, (void *)stub_lwm2m_resource_value_changed, &si1);
    setStub((void *)start_firmware_download, (void *)stub_start_firmware_download, &si);
    result = thi.interface.start_download(thi_pare, uri);
    TEST_ASSERT(result == ATINY_OK);
    
    cleanStub(&si1);
    cleanStub(&si);
    free(manager_pare.lwm2m_context);
    free(thi_pare);
}
void TestAtinyFotaState::test_atiny_fota_idle_state_report_result()
{
    atiny_fota_idle_state_s thi;
    atiny_fota_idle_state_s* thi_pare;
    atiny_fota_manager_s manager;
    atiny_fota_manager_s manager_para;
    atiny_fota_storage_device_s device;
    int result = 0;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    memset(&manager_para, 0, sizeof(atiny_fota_manager_s));
    atiny_fota_idle_state_init(&thi, &manager);
    thi_pare = (atiny_fota_idle_state_s *)malloc(sizeof(atiny_fota_idle_state_s));
    TEST_ASSERT(thi_pare != NULL);
    atiny_fota_idle_state_init(thi_pare, &manager);
    thi_pare->report_flag = TRUE;
    ((atiny_fota_state_s *)thi_pare)->manager->device = &device;
    ((atiny_fota_state_s *)thi_pare)->manager->lwm2m_context = (lwm2m_context_t *)malloc(sizeof(lwm2m_context_t));
    TEST_ASSERT(((atiny_fota_state_s *)thi_pare)->manager->lwm2m_context != NULL);

    stubInfo si1;
    stubInfo si;
    stubInfo si2;
    stubInfo si3;
    setStub((void *)atiny_update_info_read, (void *)test_atiny_update_info_read, &si1);
    setStub((void *)lwm2m_send_notify, (void *)stub_lwm2m_send_notify, &si);
    setStub((void *)lwm2m_resource_value_changed, (void *)stub_lwm2m_resource_value_changed, &si2);
    setStub((void *)atiny_update_info_write, (void *)test_atiny_update_info_write, &si3);
    result = thi.interface.repot_result((atiny_fota_state_tag_s*)thi_pare);
    TEST_ASSERT(result == ATINY_OK);
    
    thi_pare->report_flag = FALSE;
    ((atiny_fota_state_s *)thi_pare)->manager->device = &device;
    ((atiny_fota_state_s *)thi_pare)->manager->device->get_software_result = get_software_result_test;
    ((atiny_fota_state_s *)thi_pare)->manager->device->write_update_info = write_update_info_test;
    result = thi.interface.repot_result((atiny_fota_state_tag_s*)thi_pare);
    TEST_ASSERT(result == ATINY_OK);
    cleanStub(&si3);
    cleanStub(&si2);
    cleanStub(&si);
    cleanStub(&si1);

    free(((atiny_fota_state_s *)thi_pare)->manager->lwm2m_context);
    free(thi_pare);
}

void TestAtinyFotaState::test_atiny_fota_manager_update_notify()
{
    atiny_fota_idle_state_s thi;
    atiny_fota_manager_s manager;

    atiny_fota_idle_state_init(&thi, &manager);
}

void TestAtinyFotaState::test_atiny_fota_downloading_state_finish_download()
{
    atiny_fota_state_tag_s* thi;
    atiny_fota_downloading_state_s thi_pare;
    atiny_fota_manager_s manager;
    atiny_fota_manager_s manager_pare;
    int result = 0;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    memset(&manager_pare, 0, sizeof(atiny_fota_manager_s));
    thi_pare.interface.execute_update = NULL;
    thi_pare.interface.finish_download = NULL;
    thi_pare.interface.manager = NULL;
    thi_pare.interface.repot_result = NULL;
    thi_pare.interface.start_download = NULL;
    atiny_fota_downloading_state_init(&thi_pare, &manager_pare);
    

    thi = (atiny_fota_state_tag_s *)malloc(sizeof(atiny_fota_state_tag_s));
    TEST_ASSERT(thi != NULL);
    thi->manager = &manager;

    thi->manager->lwm2m_context = (lwm2m_context_t*)malloc(sizeof(lwm2m_context_t));
    TEST_ASSERT(thi->manager->lwm2m_context != NULL);

    stubInfo si1;
    setStub((void *)lwm2m_resource_value_changed, (void *)stub_lwm2m_resource_value_changed, &si1);
    result = thi_pare.interface.finish_download(thi, ATINY_BUF_OVERFLOW);
    TEST_ASSERT(result == ATINY_FOTA_IDLE);
    cleanStub(&si1);

    free(thi->manager->lwm2m_context);
    free(thi);
}

void TestAtinyFotaState::test_atiny_fota_downloaded_state_execute_update()
{
    atiny_fota_state_tag_s* thi;
    atiny_fota_downloaded_state_s thi_pare;
    int result = 0;
    lwm2m_observed_t * targetP;
    lwm2m_watcher_t watcherP;
    atiny_fota_storage_device_s device;   
    atiny_fota_manager_s manager;
    atiny_fota_manager_s manager_pare;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    memset(&manager_pare, 0, sizeof(atiny_fota_manager_s));
    atiny_fota_downloaded_state_init(&thi_pare, &manager_pare);
    
    thi = (atiny_fota_state_tag_s *)malloc(sizeof(atiny_fota_state_tag_s));
    TEST_ASSERT(thi != NULL);
    thi->manager = &manager;
    thi->manager->update_result = 0;

    stubInfo si1;
    setStub((void *)lwm2m_resource_value_changed, (void *)stub_lwm2m_resource_value_changed, &si1);
    result = thi_pare.interface.execute_update(thi);
    TEST_ASSERT(result == ATINY_OK);
    cleanStub(&si1);
    free(thi);
}
void TestAtinyFotaState::test_atiny_fota_idle_state_recv_notify_ack()
{
    atiny_fota_state_tag_s* thi;
    atiny_fota_idle_state_s thi_para;
    data_send_status_e status;
    atiny_fota_manager_s manager;
    atiny_fota_manager_s manager_para;
    int result = 0;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    memset(&manager_para, 0, sizeof(atiny_fota_manager_s));
    atiny_fota_idle_state_init(&thi_para, &manager_para);
    
    thi = (atiny_fota_state_tag_s *)malloc(sizeof(atiny_fota_state_tag_s));
    TEST_ASSERT(thi != NULL);
    thi->manager = &manager;
    status = SENT_FAIL;
    result = thi_para.interface.recv_notify_ack(thi, status);
    TEST_ASSERT(result == ATINY_OK);

    status = SENT_SUCCESS;
    thi->manager->rpt_state = ATINY_FOTA_IDLE;
    result = thi_para.interface.recv_notify_ack(thi, status);
    TEST_ASSERT(result == ATINY_OK);

    thi->manager->rpt_state = ATINY_FOTA_UPDATING;
    result = thi_para.interface.recv_notify_ack(thi, status);
    TEST_ASSERT(result == ATINY_ERR);

    thi->manager->rpt_state = ATINY_FOTA_DOWNLOADING;
    thi->manager->lwm2m_context = (lwm2m_context_t *)malloc(sizeof(lwm2m_context_t));
    TEST_ASSERT(thi->manager->lwm2m_context != NULL);
    stubInfo si;
    setStub((void*)start_firmware_download, (void*)stub_start_firmware_download, &si);
    stubInfo si1;
    setStub((void *)lwm2m_resource_value_changed, (void *)stub_lwm2m_resource_value_changed, &si1);
    result = thi_para.interface.recv_notify_ack(thi, status);
    TEST_ASSERT(result == ATINY_OK);
    cleanStub(&si);
    cleanStub(&si1);

    free(thi->manager->lwm2m_context);
    free(thi);
    
}
void TestAtinyFotaState::test_atiny_fota_idle_state_int_report_result()
{
    atiny_fota_idle_state_s thi;
    int result = 0;
    atiny_fota_manager_s manager;
    atiny_fota_storage_device_s device;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    thi.interface.manager = &manager;
    thi.interface.manager->device = &device;
    thi.interface.manager->device->get_software_result = test_get_software_result;
    stubInfo si;
    stubInfo si1;
    setStub((void*)atiny_update_info_read, (void*)stub_atiny_update_info_read, &si);
    setStub((void *)atiny_update_info_write, (void *)test_atiny_update_info_write, &si1); 
    result = atiny_fota_idle_state_int_report_result(&thi);
    TEST_ASSERT(result == ATINY_OK);
    
    cleanStub(&si1);
    cleanStub(&si);
    
}
void TestAtinyFotaState::atiny_fota_downloading_state_recv_notify_ack()
{
    atiny_fota_downloading_state_s thi_para;
    atiny_fota_manager_s manager_para;
    atiny_fota_manager_s manager;
    atiny_fota_state_s thi;
    data_send_status_e status;
    int result = 0;
    
    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    memset(&manager_para, 0, sizeof(atiny_fota_manager_s));
    atiny_fota_downloading_state_init(&thi_para, &manager_para);
    thi.manager = &manager;
    
    status = SENT_FAIL;
    result = thi_para.interface.recv_notify_ack(&thi, status);
    TEST_ASSERT(result == ATINY_OK);

    status = SENT_SUCCESS;
    thi.manager->rpt_state = ATINY_FOTA_IDLE;
    result = thi_para.interface.recv_notify_ack(&thi, status);
    TEST_ASSERT(result == ATINY_OK);

    
    status = SENT_SUCCESS;
    thi.manager->rpt_state = ATINY_FOTA_DOWNLOADING;
    result = thi_para.interface.recv_notify_ack(&thi, status);
    TEST_ASSERT(result == ATINY_ERR);
}
void TestAtinyFotaState::test_atiny_fota_downloaded_state_recv_notify_ack()
{
    atiny_fota_state_s thi;
    atiny_fota_manager_s manager;
    data_send_status_e status;
    atiny_fota_downloaded_state_s thi_para;
    atiny_fota_manager_s manager_para;
    lwm2m_observed_t * targetP;
    lwm2m_watcher_t watcherP;
    atiny_fota_storage_device_s device;
    int result = 0;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    memset(&manager_para, 0, sizeof(atiny_fota_manager_s));
    thi.manager = &manager;
    atiny_fota_downloaded_state_init(&thi_para, &manager_para);

    
    status = SENT_FAIL;
    result = thi_para.interface.recv_notify_ack(&thi, status);
    TEST_ASSERT(result == ATINY_OK);

    status = SENT_SUCCESS;
    thi.manager->device = &device;
    thi.manager->lwm2m_context = (lwm2m_context_t *)malloc(sizeof(lwm2m_context_t));
    TEST_ASSERT(thi.manager->lwm2m_context != NULL);
    
    stubInfo si1;
    stubInfo si3;
    setStub((void*)start_firmware_download, (void *)stub_start_firmware_download2, &si3);
    setStub((void *)lwm2m_resource_value_changed, (void *)stub_lwm2m_resource_value_changed, &si1);
    thi.manager->rpt_state = ATINY_FOTA_DOWNLOADING;
    result = thi_para.interface.recv_notify_ack(&thi, status);
    TEST_ASSERT(result == ATINY_ERR);
    cleanStub(&si3);
    
    stubInfo si;
    setStub((void*)start_firmware_download, (void *)stub_start_firmware_download, &si);
//    setStub((void *)lwm2m_resource_value_changed, (void *)stub_lwm2m_resource_value_changed, &si1);
    thi.manager->rpt_state = ATINY_FOTA_DOWNLOADING;
    result = thi_para.interface.recv_notify_ack(&thi, status);
    TEST_ASSERT(result == ATINY_OK);

    thi.manager->rpt_state = ATINY_FOTA_IDLE;
    result = thi_para.interface.recv_notify_ack(&thi, status);
    TEST_ASSERT(result == ATINY_OK);

    thi.manager->rpt_state = ATINY_FOTA_DOWNLOADED;
    result = thi_para.interface.recv_notify_ack(&thi, status);
    TEST_ASSERT(result == ATINY_OK);
    

    thi.manager->rpt_state = ATINY_FOTA_UPDATING;
    
    thi.manager->update_result = 1;
    thi.manager->lwm2m_context->observedList = (lwm2m_observed_t *)malloc(sizeof(lwm2m_observed_t));
    TEST_ASSERT(thi.manager->lwm2m_context->observedList != NULL);
    
    targetP = thi.manager->lwm2m_context->observedList;
    targetP->uri.flag = 0x07;
    targetP->uri.objectId = LWM2M_FIRMWARE_UPDATE_OBJECT_ID;
    targetP->uri.instanceId = 0;
    targetP->uri.resourceId = 3;

    targetP->watcherList = &watcherP;
    watcherP.counter = 1;
    watcherP.token[0] = 1;
    watcherP.tokenLen = 8;
    watcherP.format = LWM2M_CONTENT_TEXT;
    stubInfo si2;
    setStub((void *)atiny_update_info_write, (void *)stub_atiny_update_info_write, &si2);
    
    device.active_software = test_active_software;
    result = thi_para.interface.recv_notify_ack(&thi, status);
    TEST_ASSERT(result == ATINY_OK);
    
    device.active_software = NULL;
    result = thi_para.interface.recv_notify_ack(&thi, status);
    TEST_ASSERT(result == ATINY_ERR);

    cleanStub(&si2);
    cleanStub(&si1);
    cleanStub(&si);
    
    free(thi.manager->lwm2m_context->observedList);
    free(thi.manager->lwm2m_context);
    
}
void TestAtinyFotaState::test_atiny_fota_downloading_state_init()
{
    atiny_fota_downloading_state_s thi;
    atiny_fota_manager_s manager;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    thi.interface.execute_update = NULL;
    thi.interface.finish_download = NULL;
    thi.interface.manager = NULL;
    thi.interface.repot_result = NULL;
    thi.interface.start_download = NULL;
    
    atiny_fota_downloading_state_init(&thi, &manager);
    
    TEST_ASSERT(thi.interface.execute_update != NULL);
    TEST_ASSERT(thi.interface.finish_download != NULL);
    TEST_ASSERT(thi.interface.manager != NULL);
    TEST_ASSERT(thi.interface.repot_result != NULL);
    TEST_ASSERT(thi.interface.start_download != NULL);
}

void TestAtinyFotaState::test_atiny_fota_downloaded_state_init()
{
    atiny_fota_downloaded_state_s thi;
    atiny_fota_manager_s manager;

    thi.interface.execute_update = NULL;
    thi.interface.finish_download = NULL;
    thi.interface.manager = NULL;
    thi.interface.repot_result = NULL;
    thi.interface.start_download = NULL;
    
    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    atiny_fota_downloaded_state_init(&thi, &manager);
    
    TEST_ASSERT(thi.interface.execute_update != NULL);
    TEST_ASSERT(thi.interface.finish_download != NULL);
    TEST_ASSERT(thi.interface.manager != NULL);
    TEST_ASSERT(thi.interface.repot_result != NULL);
    TEST_ASSERT(thi.interface.start_download != NULL);
}
void TestAtinyFotaState::test_atiny_fota_updating_state_init()
{
    atiny_fota_updating_state_s thi;
    atiny_fota_manager_s manager;

    thi.interface.execute_update = NULL;
    thi.interface.finish_download = NULL;
    thi.interface.manager = NULL;
    thi.interface.repot_result = NULL;
    thi.interface.start_download = NULL;

    memset(&manager, 0, sizeof(atiny_fota_manager_s));
    atiny_fota_updating_state_init(&thi, &manager);

    TEST_ASSERT(thi.interface.execute_update != NULL);
    TEST_ASSERT(thi.interface.finish_download != NULL);
    TEST_ASSERT(thi.interface.manager != NULL);
    TEST_ASSERT(thi.interface.repot_result != NULL);
    TEST_ASSERT(thi.interface.start_download != NULL);
}
TestAtinyFotaState::TestAtinyFotaState()
{
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_state_default_handle);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_state_init);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_idle_state_init);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_start_download);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_idle_state_report_result);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_manager_update_notify);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_downloading_state_finish_download);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_downloading_state_init);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_downloaded_state_init);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_updating_state_init);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_downloaded_state_execute_update);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_idle_state_recv_notify_ack);
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_idle_state_int_report_result);
    TEST_ADD(TestAtinyFotaState::atiny_fota_downloading_state_recv_notify_ack)
    TEST_ADD(TestAtinyFotaState::test_atiny_fota_downloaded_state_recv_notify_ack)
}


void TestAtinyFotaState::setup()
{
    std::cout << "in steup\n";
}

void TestAtinyFotaState::tear_down()
{
    std::cout << "in teardown\n";
}






