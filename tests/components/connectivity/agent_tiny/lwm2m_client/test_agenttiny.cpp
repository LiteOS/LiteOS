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
#include "test_agenttiny.h"
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "liblwm2m.h" // lwm2m_context_t
#include "object_comm.h" // client_data_t
#include "agenttiny.h"
#include "atiny_adapter.h"
#include "atiny_rpt.h"
#include "los_sem.ph"
#include "connection.h"
#include "atiny_fota_manager.h"

    enum
    {
        OBJ_SECURITY_INDEX = 0,
        OBJ_SERVER_INDEX,
        OBJ_DEVICE_INDEX,
        OBJ_FIRMWARE_INDEX,
        OBJ_CONNECT_INDEX,
        OBJ_APP_INDEX,
        OBJ_MAX_NUM,
    };

    typedef struct
    {
        lwm2m_context_t  *lwm2m_context;
        atiny_param_t     atiny_params;
        client_data_t     client_data;
        lwm2m_object_t   *obj_array[OBJ_MAX_NUM];
        int atiny_quit;
        int reconnect_flag;
        void *quit_sem;
        int reboot_flag;
        uint8_t *recv_buffer;
    } handle_data_t;

    extern int g_reboot;

    static handle_data_t *s_handle = NULL;
    static SEM_CB_S s_sem;
    static atiny_param_t s_atiny_params;
    static int s_malloc_failed_index = 0;
    static int s_create_mutex_failed_index = 0;

    extern void atiny_event_handle(module_type_t type, int code, const char *arg, int arg_len);
    extern void observe_handle_ack(lwm2m_transaction_t *transacP, void *message);
    extern int atiny_init_objects(atiny_param_t *atiny_params, const atiny_device_info_t *device_info,
                                  handle_data_t *handle);
    extern void atiny_set_bootstrap_sequence_state(atiny_param_t* atiny_params, lwm2m_context_t* lwm2m_context);
    extern void lwm2m_close(lwm2m_context_t *contextP);
    extern void *lwm2m_malloc(size_t s);

    extern void atiny_destroy(void *handle);
    extern void atiny_destroy_rpt(void);
    extern void atiny_set_reboot_flag();
    extern void agent_tiny_fota_init(void);

    extern void *atiny_mutex_create(void);
    extern void atiny_mutex_lock(void *mutex);
    extern void atiny_mutex_unlock(void *mutex);
    extern void atiny_mutex_destroy(void *mutex);
    extern int atiny_cmd_ioctl(atiny_cmd_e cmd, char *arg, int len);

    extern void ack_callback(atiny_report_type_e type, int cookie, data_send_status_e status);

    static void *stub_atiny_mutex_create(void)
    {
        memset(&s_sem, 0, sizeof(SEM_CB_S));
        return (void *)&s_sem;
    }

    static void *stub_atiny_mutex_create_ex(void)
    {
        static int i = 0;
        if( i++ == s_create_mutex_failed_index)
        {
            i = 0;
            return NULL;
        }
        if(s_create_mutex_failed_index == 0xff)
        {
            i = 0;
        }
        memset(&s_sem, 0, sizeof(SEM_CB_S));
        return (void *)&s_sem;
    }

    static int stub_atiny_cmd_ioctl(atiny_cmd_e cmd, char *arg, int len)
    {
        if(cmd == ATINY_GET_FOTA_STORAGE_DEVICE)
        {
            arg = NULL;
        }
        else if(NULL != s_handle)
        {
            s_handle->atiny_quit = 1;
            s_handle = NULL;
        }
        return 0;
    }

    static void transaction_callback(void *message)
    {
        printf("test lwm2m_transaction_t.callback\n");
    }

    static int stub_atiny_fota_manager_repot_result(atiny_fota_manager_s *thi)
    {
        return ATINY_ERR;
    }

    static void *stub_lwm2m_malloc(size_t s)
    {
        static int i = 0;
        if( i++ == s_malloc_failed_index)
        {
            i = 0;
            return NULL;
        }
        if( s_malloc_failed_index == 0xff)
        {
            i = 0;
        }
        return atiny_malloc(s);
    }

    static lwm2m_object_t * stub_get_security_object(uint16_t serverId,atiny_param_t* atiny_params,lwm2m_context_t* lwm2m_context)
    {
        return NULL;
    }
    static lwm2m_object_t * stub_get_server_object(int serverId, const char* binding, int lifetime,bool storing)
    {
        return NULL;
    }
    static lwm2m_object_t * stub_get_object_device(atiny_param_t *atiny_params, const char* manufacturer)
    {
        return NULL;
    }
    static lwm2m_object_t * stub_get_object_firmware(atiny_param_t *atiny_params)
    {
        return NULL;
    }
    static lwm2m_object_t * stub_get_object_conn_m(atiny_param_t* atiny_params)
    {
        return NULL;
    }
    static lwm2m_object_t * stub_get_binary_app_data_object(atiny_param_t* atiny_params)
    {
        return NULL;
    }
}
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestAgenttiny::TestAgenttiny()
{
    TEST_ADD(TestAgenttiny::test_atiny_event_notify);
    TEST_ADD(TestAgenttiny::test_atiny_init);
    TEST_ADD(TestAgenttiny::test_atiny_bind);
    TEST_ADD(TestAgenttiny::test_atiny_deinit);
    TEST_ADD(TestAgenttiny::test_atiny_data_report);
    TEST_ADD(TestAgenttiny::test_atiny_data_change);
    TEST_ADD(TestAgenttiny::test_atiny_set_log_level);
    TEST_ADD(TestAgenttiny::test_atiny_get_log_level);
    TEST_ADD(TestAgenttiny::test_atiny_reconnect);
    TEST_ADD(TestAgenttiny::test_atiny_event_handle);
    TEST_ADD(TestAgenttiny::test_observe_handle_ack);
    TEST_ADD(TestAgenttiny::test_atiny_init_objects);
    TEST_ADD(TestAgenttiny::test_atiny_set_bootstrap_sequence_state);
    TEST_ADD(TestAgenttiny::test_atiny_destroy);
}

TestAgenttiny::~TestAgenttiny()
{

}

void TestAgenttiny::test_atiny_event_notify(void)
{
    const char *arg = "atiny_event_notify";
    int len = strlen(arg);
    
    atiny_event_notify(ATINY_REG_OK, arg, len);
}

void TestAgenttiny::test_atiny_init(void)
{
    void *pHandle = NULL;

    atiny_param_t atiny_params;
    memset(&atiny_params, 0, sizeof(atiny_param_t));
    atiny_params.server_params.binding = (char *)"UQ";
    atiny_params.server_params.life_time = 20;
    atiny_params.server_params.storing_cnt = 0;

    int ret = 0;

    ret = atiny_init(NULL, NULL);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_init(NULL, NULL) failed");

    /** no server_ip or server_port test **/
    atiny_params.bootstrap_mode = BOOTSTRAP_FACTORY;
    ret = atiny_init(&atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_init(...) failed");

    atiny_params.bootstrap_mode = BOOTSTRAP_CLIENT_INITIATED;
    ret = atiny_init(&atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_init(...) failed");

    atiny_params.bootstrap_mode = BOOTSTRAP_SEQUENCE;
    ret = atiny_init(&atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init(...) failed");

    atiny_params.bootstrap_mode = (atiny_bootstrap_type_e)0xff;
    ret = atiny_init(&atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_init(...) failed");

    // use s_atiny_params to test
    ret = atiny_init(&s_atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init(...) failed");

    stubInfo si_mutex_create;
    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create, &si_mutex_create);
    agent_tiny_fota_init();

    stubInfo si_ioctl;
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_ioctl);
    ret = atiny_init(&s_atiny_params, &pHandle);
    cleanStub(&si_ioctl);
    TEST_ASSERT_MSG((ATINY_ERR == ret), "atiny_init(...) failed");

    ret = atiny_init(&s_atiny_params, &pHandle);
    cleanStub(&si_mutex_create);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");

    // destory
    ret = 0;
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");
}

void TestAgenttiny::test_atiny_bind(void)
{
    void *pHandle = NULL;
    atiny_device_info_t device_info;

    memset(&device_info, 0, sizeof(atiny_device_info_t));

    int ret = 0;
    ret = atiny_bind(NULL, NULL);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_bind(NULL, NULL) failed");

    // atiny_init()
    stubInfo si_mutex_create;
    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create, &si_mutex_create);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    cleanStub(&si_mutex_create);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");
    /////////////////////

    ret = atiny_bind(&device_info, pHandle);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_bind(NULL, NULL) failed");

    device_info.endpoint_name = (char *)"44440003";

    ret = atiny_bind(&device_info, pHandle);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_bind(NULL, NULL) failed");

    device_info.manufacturer = (char *)"Agent_Tiny";

    // atiny_init_rpt() failed
    ret = atiny_bind(&device_info, pHandle);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_bind(NULL, NULL) failed");

    s_handle = (handle_data_t *)pHandle;
    //g_reboot = 1;
    atiny_set_reboot_flag();
    stubInfo si_cmd_ioctl;
    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create, &si_mutex_create);
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &si_cmd_ioctl);
    ret = atiny_bind(&device_info, pHandle);
    cleanStub(&si_mutex_create);
    cleanStub(&si_cmd_ioctl);
    g_reboot = false;
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_bind(NULL, NULL) failed");

    // destory
    ret = 0;
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");
}

void TestAgenttiny::test_atiny_deinit(void)
{
    void *pHandle = NULL;

    int ret = 0;
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");

    // through atiny_init(...) initialize pHandle
    stubInfo si_mutex_create;
    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create, &si_mutex_create);    
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    cleanStub(&si_mutex_create);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");

    ret = 0;
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");

}

void TestAgenttiny::test_atiny_data_report(void)
{
    uint8_t buf[5] = {0, 1, 6, 5, 9};
    data_report_t report_data;
    int ret;
    int cnt = 0;
    report_data.buf = buf;
    report_data.callback = ack_callback;
    report_data.cookie = 0;
    report_data.len = sizeof(buf);
    report_data.type = APP_DATA;
    report_data.cookie = cnt;

    void *pHandle = NULL;
    // through atiny_init(...) initialize pHandle
    stubInfo si_mutex_create;
    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create, &si_mutex_create);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    cleanStub(&si_mutex_create);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");
    //////////

    ret = atiny_data_report(NULL, NULL);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_data_report(...) failed");

    atiny_init_rpt(); // must
    ret = atiny_data_report(pHandle, &report_data);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_FOUND == ret), "atiny_data_report(...) failed");

    report_data.type = FIRMWARE_UPDATE_STATE;
    ret = atiny_data_report(pHandle, &report_data);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_FOUND == ret), "atiny_data_report(...) failed");

    report_data.type = (atiny_report_type_e)0xff;
    ret = atiny_data_report(pHandle, &report_data);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_FOUND == ret), "atiny_data_report(...) failed");

    stubInfo si_malloc;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &si_malloc);
    s_malloc_failed_index = 0;
    report_data.type = APP_DATA;
    ret = atiny_data_report(pHandle, &report_data);
    s_malloc_failed_index = 0xff;
    TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "atiny_data_report(...) failed");
    cleanStub(&si_malloc);

    ret = 0;
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");
}

void TestAgenttiny::test_atiny_data_change(void)
{
    int ret = 0;
    void *pHandle = NULL;
    // through atiny_init(...) initialize pHandle
    stubInfo si_mutex_create;
    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create, &si_mutex_create);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");
    //////////

    char *data_type = (char *)DEVICE_MEMORY_FREE;

    ret = atiny_data_change(NULL, NULL);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_data_change(...) failed");

    // NULL == handle->lwm2m_context
    //    ret = atiny_data_change(pHandle, data_type);
    //    TEST_ASSERT_MSG((ATINY_CLIENT_UNREGISTERED == ret), "atiny_data_change(...) failed");

    handle_data_t *pHandleData = (handle_data_t *)pHandle;

    atiny_device_info_t device_info;
    memset(&device_info, 0, sizeof(atiny_device_info_t));
    device_info.endpoint_name = (char *)"44440003";
    device_info.manufacturer = (char *)"Agent_Tiny";

//    pHandleData->atiny_params.bootstrap_mode = BOOTSTRAP_SEQUENCE;
//    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
//    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init_objects(...) failed");

//    pHandleData->atiny_params.bootstrap_mode = BOOTSTRAP_CLIENT_INITIATED;
//    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
//    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init_objects(...) failed");

//    pHandleData->atiny_params.bootstrap_mode = BOOTSTRAP_FACTORY;
    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init_objects(...) failed");


    ret = atiny_data_change(pHandle, data_type);
    TEST_ASSERT_MSG((ATINY_CLIENT_UNREGISTERED == ret), "atiny_data_change(...) failed");

    if(pHandleData->lwm2m_context)
    {
        pHandleData->lwm2m_context->state = STATE_READY;
    }
    ret = atiny_data_change(pHandle, data_type);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_data_change(...) failed");

    cleanStub(&si_mutex_create);

    ret = 0;
    atiny_destroy(pHandle);
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");
}

void TestAgenttiny::test_atiny_set_log_level(void)
{
    // like test_atiny_get_log_level
}

void TestAgenttiny::test_atiny_get_log_level(void)
{
    atiny_log_e logLevel = LOG_INFO;
    atiny_log_e ori_logLevel;
    int ret = 0;

    ori_logLevel = logLevel = atiny_get_log_level();
    TEST_ASSERT_MSG((LOG_INFO == logLevel), "atiny_get_log_level() failed");

    logLevel = LOG_DEBUG;
    atiny_set_log_level(logLevel);
    TEST_ASSERT_MSG((0 == ret), "atiny_set_log_level(...) failed");

    logLevel = atiny_get_log_level();
    TEST_ASSERT_MSG((LOG_DEBUG == logLevel), "atiny_get_log_level() failed");

    atiny_set_log_level(ori_logLevel);
    TEST_ASSERT_MSG((0 == ret), "atiny_set_log_level(...) failed");
}

void TestAgenttiny::test_atiny_reconnect(void)
{
    void *pHandle = NULL;

    int ret = 0;
    ret = atiny_reconnect(pHandle);
    TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "atiny_reconnect(...) failed");

    // through atiny_init(...) initialize pHandle
    stubInfo si_mutex_create;
    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create, &si_mutex_create);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    cleanStub(&si_mutex_create);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");

    ret = atiny_reconnect(pHandle);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_reconnect(...) failed");

    ret = 0;
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");
}

void TestAgenttiny::test_atiny_event_handle(void)
{
    module_type_t module_type = MODULE_LWM2M;
    lwm2m_uri_t uri;
    int code = STATE_REGISTERED;
    char *arg = NULL;
    int arg_len = 0;
    int ret = 0;

    stubInfo stub_info;
    setStub((void *)atiny_fota_manager_repot_result, (void *)stub_atiny_fota_manager_repot_result, &stub_info);
    atiny_event_handle(module_type, code, arg, arg_len);
    TEST_ASSERT_MSG((0 == ret), "atiny_event_handle(...) failed");

    code = STATE_REG_FAILED;
    atiny_event_handle(module_type, code, arg, arg_len);
    TEST_ASSERT_MSG((0 == ret), "atiny_event_handle(...) failed");

    module_type = MODULE_NET;
    atiny_event_handle(module_type, code, arg, arg_len);
    TEST_ASSERT_MSG((0 == ret), "atiny_event_handle(...) failed");

    module_type = MODULE_URI;
    atiny_event_handle(module_type, code, arg, arg_len);
    TEST_ASSERT_MSG((0 == ret), "atiny_event_handle(...) failed");

    memset(&uri, 0, sizeof(lwm2m_uri_t));
    uri.objectId = 19;
    arg = (char *)&uri;
    arg_len = sizeof(lwm2m_uri_t);

    atiny_init_rpt(); // must
    code = OBSERVE_UNSUBSCRIBE;
    atiny_event_handle(module_type, code, arg, arg_len);
    TEST_ASSERT_MSG((0 == ret), "atiny_event_handle(...) failed");

    code = OBSERVE_SUBSCRIBE;
    atiny_event_handle(module_type, code, arg, arg_len);
    TEST_ASSERT_MSG((0 == ret), "atiny_event_handle(...) failed");

    module_type = (module_type_t)0xff;
    atiny_event_handle(module_type, code, arg, arg_len);
    TEST_ASSERT_MSG((0 == ret), "atiny_event_handle(...) failed");
    cleanStub(&stub_info);
}


void TestAgenttiny::test_observe_handle_ack(void)
{
    lwm2m_transaction_t transaction;
    char *message = (char *)"observe_handle_ack";
    int ret = 0;

    memset(&transaction, 0, sizeof(lwm2m_transaction_t));
    transaction.cfg.callback = transaction_callback;

    transaction.ack_received = 1;
    observe_handle_ack(&transaction, message);
    TEST_ASSERT_MSG((0 == ret), "observe_handle_ack(...) failed");

    transaction.ack_received = 0;
    transaction.retrans_counter = COAP_MAX_RETRANSMIT + 1;
    observe_handle_ack(&transaction, message);
    TEST_ASSERT_MSG((0 == ret), "observe_handle_ack(...) failed");

    transaction.retrans_counter = COAP_MAX_RETRANSMIT;
    observe_handle_ack(&transaction, message);
    TEST_ASSERT_MSG((0 == ret), "observe_handle_ack(...) failed");
}


void TestAgenttiny::test_atiny_init_objects(void)
{
    int ret = 0;
    void *pHandle = NULL;
    
    // through atiny_init(...) initialize pHandle
    stubInfo si_mutex_create;
    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create, &si_mutex_create);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");
    //////////

    handle_data_t *pHandleData = (handle_data_t *)pHandle;

    atiny_device_info_t device_info;
    memset(&device_info, 0, sizeof(atiny_device_info_t));
    device_info.endpoint_name = (char *)"44440003";
    device_info.manufacturer = (char *)"Agent_Tiny";

    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init_objects(...) failed");
    
    cleanStub(&si_mutex_create);

    ret = 0;
    atiny_destroy(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy(...) failed");
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");

    ///////////////////////////////////// abnormal test ///////////////////////////////////
    // atiny_init_rpt() failed
    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_objects(...) failed");

    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create_ex, &si_mutex_create);
    s_create_mutex_failed_index = 0xff;

    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");
    pHandleData = (handle_data_t *)pHandle;
    
    stubInfo si_malloc;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &si_malloc);

    // lwm2m_init failed
    s_malloc_failed_index = 0;
    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "atiny_init_objects(...) failed");
    atiny_destroy_rpt();

    // the second atiny_mutex_create() failed
    s_malloc_failed_index = 0xff;
    s_create_mutex_failed_index = 1;
    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_RESOURCE_NOT_ENOUGH == ret), "atiny_init_objects(...) failed");
    atiny_destroy_rpt();
  
    s_create_mutex_failed_index = 0xff;

    cleanStub(&si_malloc);
    cleanStub(&si_mutex_create);
    
    ret = 0;
    atiny_destroy(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy(...) failed");
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");

    //////////////////////////// get object failed   ////////////////////////////////////
    setStub((void *)atiny_mutex_create, (void *)stub_atiny_mutex_create, &si_mutex_create);
    stubInfo si_object;
    
    // get_security_object() failed
    setStub((void *)get_security_object, (void *)stub_get_security_object, &si_object);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");

    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "atiny_init_objects(...) failed");
    
    cleanStub(&si_object);
    
    ret = 0;
    atiny_destroy(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy(...) failed");
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");

    // get_server_object() failed
    setStub((void *)get_server_object, (void *)stub_get_server_object, &si_object);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");

    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "atiny_init_objects(...) failed");
    
    cleanStub(&si_object);
    
    ret = 0;
    atiny_destroy(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy(...) failed");
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");

    // get_object_device() failed
    setStub((void *)get_object_device, (void *)stub_get_object_device, &si_object);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");

    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "atiny_init_objects(...) failed");
    
    cleanStub(&si_object);
    
    ret = 0;
    atiny_destroy(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy(...) failed");
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");

    // get_object_firmware() failed
    setStub((void *)get_object_firmware, (void *)stub_get_object_firmware, &si_object);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");

    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "atiny_init_objects(...) failed");
    
    cleanStub(&si_object);
    
    ret = 0;
    atiny_destroy(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy(...) failed");
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");

    // get_object_conn_m() failed
    setStub((void *)get_object_conn_m, (void *)stub_get_object_conn_m, &si_object);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");

    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "atiny_init_objects(...) failed");
    
    cleanStub(&si_object);
    
    ret = 0;
    atiny_destroy(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy(...) failed");
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");

    // get_binary_app_data_object() failed
    setStub((void *)get_binary_app_data_object, (void *)stub_get_binary_app_data_object, &si_object);
    agent_tiny_fota_init();
    ret = atiny_init(&s_atiny_params, &pHandle);
    TEST_ASSERT_MSG((ATINY_OK == ret), "atiny_init(...) failed");

    ret = atiny_init_objects(&pHandleData->atiny_params, &device_info, pHandleData);
    TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "atiny_init_objects(...) failed");
    
    cleanStub(&si_object);
    
    ret = 0;
    atiny_destroy(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy(...) failed");
    atiny_deinit(pHandle);
    TEST_ASSERT_MSG((0 == ret), "atiny_deinit(...) failed");

    cleanStub(&si_mutex_create);
}

void TestAgenttiny::test_atiny_set_bootstrap_sequence_state(void)
{
    lwm2m_context_t context;
    memset(&context, 0, sizeof(lwm2m_context_t));

    int ret = 0;
    atiny_set_bootstrap_sequence_state(NULL, NULL);
    TEST_ASSERT_MSG((0 == ret), "atiny_set_bootstrap_sequence_state(...) failed");

    atiny_set_bootstrap_sequence_state(&s_atiny_params, &context);
    TEST_ASSERT_MSG((0 == ret), "atiny_set_bootstrap_sequence_state(...) failed");

    s_atiny_params.bootstrap_mode = BOOTSTRAP_CLIENT_INITIATED;
    atiny_set_bootstrap_sequence_state(&s_atiny_params, &context);
    TEST_ASSERT_MSG((0 == ret), "atiny_set_bootstrap_sequence_state(...) failed");

    s_atiny_params.bootstrap_mode = (atiny_bootstrap_type_e)0xff;
    atiny_set_bootstrap_sequence_state(&s_atiny_params, &context);
    TEST_ASSERT_MSG((0 == ret), "atiny_set_bootstrap_sequence_state(...) failed");

    s_atiny_params.bootstrap_mode = BOOTSTRAP_SEQUENCE;
    atiny_set_bootstrap_sequence_state(&s_atiny_params, &context);
    TEST_ASSERT_MSG((0 == ret), "atiny_set_bootstrap_sequence_state(...) failed");

}

void TestAgenttiny::test_atiny_destroy(void)
{
    int ret = 0;
    atiny_destroy(NULL);
    TEST_ASSERT_MSG((0 == ret), "atiny_destroy(...) failed");
    // others like test_atiny_init_objects
}
/* Private functions --------------------------------------------------------*/
void TestAgenttiny::setup()
{
    // init atiny_params
    atiny_security_param_t  *iot_security_param = NULL;
    atiny_security_param_t  *bs_security_param = NULL;
    memset(&s_atiny_params, 0, sizeof(atiny_param_t));
    s_atiny_params.server_params.binding = (char *)"UQ";
    s_atiny_params.server_params.life_time = 20;
    s_atiny_params.server_params.storing_cnt = 0;

    s_atiny_params.bootstrap_mode = BOOTSTRAP_FACTORY;

    iot_security_param = &(s_atiny_params.security_params[0]);
    bs_security_param = &(s_atiny_params.security_params[1]);

    iot_security_param->server_ip = (char *)"192.168.0.106";
    bs_security_param->server_ip = (char *)"192.168.0.106";

    iot_security_param->server_port = (char *)"5683";
    bs_security_param->server_port = (char *)"5683";

    iot_security_param->psk_Id = NULL;
    iot_security_param->psk = NULL;
    iot_security_param->psk_len = 0;

    bs_security_param->psk_Id = NULL;
    bs_security_param->psk = NULL;
    bs_security_param->psk_len = 0;
}

void TestAgenttiny::tear_down()
{
}

