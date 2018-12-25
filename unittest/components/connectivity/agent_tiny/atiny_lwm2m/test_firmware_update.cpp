/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>

 ******************************************************************************
  File Name     : test_firmware_update.cpp
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/7/31
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2018/7/31
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
#include "test_firmware_update.h"

extern "C"
{
    #include "ota/package.h" 
    #include "internals.h"
    #include "atiny_lwm2m/agenttiny.h"
    #include "firmware_update.h"
    #include "er-coap-13.h"
    #include "connection.h"

    static uint32_t s_block_num_flag = 0;
    static int s_write_ret = 0;

    extern int atiny_net_send(void* ctx, const unsigned char* buf, size_t len);
    extern int parse_firmware_uri(char *uri, int uri_len);
    extern void transaction_remove(lwm2m_context_t * contextP, lwm2m_transaction_t * transacP);
    extern void *lwm2m_malloc(size_t s);

    static void firmware_update_notify_callback(firmware_update_rst_e rst, void *param)
    {
        
    }
    int test_write_software_end(pack_storage_device_api_s *thi, pack_download_result_e result, uint32_t total_len)
    {
        return 0;
    } 


    
    static int test_write_software(pack_storage_device_api_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len)
    {
        return s_write_ret;
    }


    static int stub_atiny_net_send(void* ctx, const unsigned char* buf, size_t len)
    {
        return 0;
    }
    static int8_t stub_lwm2m_buffer_send(void* sessionH,
                          uint8_t* buffer,
                          size_t length,
                          void* userdata)
    {
        return 0;
    }
    static int stub_coap_get_header_block2(void *packet, uint32_t *num, uint8_t *more, uint16_t *size, uint32_t *offset)
    {
        coap_packet_t *const coap_pkt = (coap_packet_t *) packet;
        
        if (num!=NULL) *num = s_block_num_flag ? 0 : coap_pkt->block2_num;
        if (more!=NULL) *more = coap_pkt->block2_more;
        if (size!=NULL) *size = coap_pkt->block2_size;
        if (offset!=NULL) *offset = coap_pkt->block2_offset;
        return 1;
    }

    static int stub_coap_set_header_block2(void *packet, uint32_t num, uint8_t more, uint16_t size)
    {
        return -1;
    }

    static int stub_coap_set_header_uri_path(void *packet, const char *path)
    {
        return -1;
    }
    static int stub_transaction_send(lwm2m_context_t * contextP, lwm2m_transaction_t * transacP)
    {
        return -1;
    }
    
    static void *stub_lwm2m_malloc(size_t s)
    {
        return NULL;
    }
}
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
void TestFirmwareUpdate::test_set_firmware_update_notify(void)
{
    int ret = 0;
    char param[] = "123abc";
    set_firmware_update_notify(firmware_update_notify_callback, param);
    TEST_ASSERT_MSG((0 == ret), "set_firmware_update_notify(...) failed");
}
void TestFirmwareUpdate::test_start_firmware_download(void)
{
    int ret = 0;

    lwm2m_context_t context;
    char *uri = (char *)"coap://192.168.1.106:5368";
    pack_storage_device_api_s storage_device;
    lwm2m_server_t serverList;
    lwm2m_transaction_t transac;
    coap_packet_t message;
    lwm2m_context_t userData;
    connection_t sessionH;
    lwm2m_transaction_callback_t callback = NULL;

    memset(&context, 0, sizeof(lwm2m_context_t));
    memset(&storage_device, 0, sizeof(pack_storage_device_api_s));
    memset(&serverList, 0, sizeof(lwm2m_server_t));
    memset(&transac, 0, sizeof(transac));
    memset(&message, 0, sizeof(coap_packet_t));
    memset(&userData, 0, sizeof(lwm2m_context_t));
    memset(&sessionH, 0, sizeof(connection_t));

    ret = start_firmware_download(NULL, uri, &storage_device);
    TEST_ASSERT_MSG((-1 == ret), "start_firmware_download(...) failed");

    ret = start_firmware_download(&context, uri, &storage_device);
    TEST_ASSERT_MSG((-1 == ret), "start_firmware_download(...) failed");

    ///////////////////////////////////////////////////
    serverList.status = STATE_REGISTERED;
    context.serverList = &serverList;
    
    ret = start_firmware_download(&context, uri, &storage_device);
    TEST_ASSERT_MSG((-1 == ret), "start_firmware_download(...) failed");
    clean_firmware_record();
    
    uri = (char *)"coap://192.168.1.106:5368/ota";

    ret = start_firmware_download(&context, uri, &storage_device);
    TEST_ASSERT_MSG((-1 == ret), "start_firmware_download(...) failed");
    printf("[%s:%d] ******* ret=%d\n", __func__, __LINE__, ret);

    ret = start_firmware_download(&context, uri, &storage_device);
    TEST_ASSERT_MSG((-1 == ret), "start_firmware_download(...) failed");
    printf("[%s:%d] ******* ret=%d\n", __func__, __LINE__, ret);
    clean_firmware_record();

   
    //////////////////////////////////////////////////////
    serverList.sessionH = (void*)&sessionH;

    stubInfo si_net_send;
//    setStub((void *)atiny_net_send, (void *)stub_atiny_net_send, &si_net_send); 
    setStub((void *)transaction_send, (void *)stub_transaction_send, &si_net_send);  
    ret = start_firmware_download(&context, uri, &storage_device);
    TEST_ASSERT_MSG((-1 == ret), "start_firmware_download(...) failed");
    cleanStub(&si_net_send);
    transaction_remove(&context, context.transactionList);
    clean_firmware_record();
    printf("[%s:%d] ******* ret=%d\n", __func__, __LINE__, ret);

    stubInfo si; 
    setStub((void*)coap_set_header_uri_path, (void*)stub_coap_set_header_uri_path, &si);
    ret = start_firmware_download(&context, uri, &storage_device);
    TEST_ASSERT_MSG((-1 == ret), "start_firmware_download(...) failed");
    cleanStub(&si);
    clean_firmware_record();
    printf("[%s:%d] ******* ret=%d\n", __func__, __LINE__, ret);

    ///////////
    stubInfo si_trans_send;
    setStub((void *)transaction_send, (void *)stub_transaction_send, &si_trans_send);
    ret = start_firmware_download(&context, uri, &storage_device);
    TEST_ASSERT_MSG((-1 == ret), "start_firmware_download(...) failed");
    printf("[%s:%d] ******* ret=%d\n", __func__, __LINE__, ret);
  
    if(NULL != context.transactionList){
        callback = context.transactionList->callback;
    }
    if(NULL == callback) return;

    callback(&transac, NULL);

    message.block2_num = 1;
    message.block2_more = 1;
    message.block2_size = 1;
    message.block2_offset = 1;
    transac.peerH = (void*)&sessionH;
    userData.nextMID = 1;
    transac.userData = &userData;
    callback(&transac, &message);

    stubInfo si_coap_block;
    setStub((void *)coap_get_header_block2, (void *)stub_coap_get_header_block2, &si_coap_block);
    callback(&transac, &message);
    transaction_remove(&userData, userData.transactionList);

    s_block_num_flag = 1;
    callback(&transac, &message);
    s_block_num_flag = 0;
    
    storage_device.write_software = test_write_software;

    s_write_ret = 1;
    callback(&transac, &message);
    s_write_ret = 0;

    setStub((void*)lwm2m_malloc, (void*)stub_lwm2m_malloc, &si);
    callback(&transac, &message);
    cleanStub(&si);

    setStub((void*)coap_set_header_block2, (void*)stub_coap_set_header_block2, &si);
    callback(&transac, &message);
    cleanStub(&si);

    setStub((void*)coap_set_header_uri_path, (void*)stub_coap_set_header_uri_path, &si);
    //storage_device.write_software_end = test_write_software_end;
    callback(&transac, &message);
    storage_device.write_software_end = NULL;
    cleanStub(&si);
    transaction_remove(&userData, userData.transactionList);
   
    setStub((void*)transaction_send, (void*)stub_transaction_send, &si);
    callback(&transac, &message);
    cleanStub(&si);
    
    message.block2_more = 0;
    callback(&transac, &message);
    
    storage_device.write_software_end = test_write_software_end;
    callback(&transac, &message);    

    transaction_remove(&userData, userData.transactionList);

    cleanStub(&si_coap_block);
    cleanStub(&si_trans_send);

    storage_device.write_software_end = test_write_software_end;
    setStub((void*)transaction_send, (void*)stub_transaction_send, &si);     
    callback(&transac, &message);
    cleanStub(&si);
    
    
    transaction_remove(&context, context.transactionList);
    clean_firmware_record();
}

void TestFirmwareUpdate::test_parse_firmware_uri(void)
{
    int ret = 0;
    char *uri = (char *)"coap://192.168.1.106:5368";

    ret = parse_firmware_uri(NULL, 0);
    TEST_ASSERT_MSG((-1 == ret), "parse_firmware_uri(...) failed");

    ret = parse_firmware_uri(uri, strlen(uri));
    TEST_ASSERT_MSG((-1 == ret), "parse_firmware_uri(...) failed");

    uri = (char*)"coaps://192.168.1.106:5368/ota";
    ret = parse_firmware_uri(uri, strlen(uri));
    TEST_ASSERT_MSG((0 == ret), "parse_firmware_uri(...) failed");
    clean_firmware_record();

    uri = (char*)"coap://";
    ret = parse_firmware_uri(uri, strlen(uri));
    TEST_ASSERT_MSG((-1 == ret), "parse_firmware_uri(...) failed");
    
    uri = (char*)"abcd";
    ret = parse_firmware_uri(uri, strlen(uri));
    TEST_ASSERT_MSG((-1 == ret), "parse_firmware_uri(...) failed");

    stubInfo si;
    setStub((void*)lwm2m_malloc, (void*)stub_lwm2m_malloc, &si);
    uri = (char*)"coap://192.168.1.106:5368/ota";
    ret = parse_firmware_uri(uri, strlen(uri));
    TEST_ASSERT_MSG((-1 == ret), "parse_firmware_uri(...) failed");
    cleanStub(&si);
}

TestFirmwareUpdate::TestFirmwareUpdate()
{
    TEST_ADD(TestFirmwareUpdate::test_set_firmware_update_notify);
    TEST_ADD(TestFirmwareUpdate::test_start_firmware_download);
    TEST_ADD(TestFirmwareUpdate::test_parse_firmware_uri);
}

void TestFirmwareUpdate::setup()
{
    return;
}
void TestFirmwareUpdate::tear_down()
{
    return;
}


