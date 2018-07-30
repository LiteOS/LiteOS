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
 
#include <cpptest.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <pthread.h>
#include <unistd.h>

#include "test_atadapter.h"
#include "atadapter.h"
#include "stub.h"

int32_t BUFLEN = 10;
extern at_task at;
extern at_config at_user_conf;
extern at_oob_t at_oob;
extern "C" {

    int QUEUE_WRITE = 0;
    int QUEUE_CREATE = 0;
    int CMD_TIMES = 0;

    uint32_t g_semcreate_ctl_flag = 0;
    uint32_t g_muxcreate_ctl_flag = 0;
    uint32_t g_quecreate_ctl_flag = 0;
    uint32_t g_malloc_ctl_flag = 0;  //for malloc function
    uint8_t * gp_resp_ptr = NULL;


    extern void at_listener_list_add(at_listener * p);
    extern void at_listner_list_del(at_listener * p);
    extern void store_resp_buf(int8_t * resp_buf, int32_t resp_len, int8_t * buf, uint32_t len);
    extern int32_t at_get_unuse_linkid();
    extern int32_t at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf);
    extern int32_t at_write(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len);
    extern int32_t at_oob_register(char* featurestr,int cmdlen, oob_callback callback);
    extern void at_init_oob(void);
    extern int32_t at_init();
    extern int32_t at_struct_init(at_task * at);
    extern void at_deinit();
    extern void at_recv_task(uint32_t p);
    extern int cloud_cmd_matching(int8_t * buf, int32_t len);
    extern void* atiny_malloc(size_t size);
    extern void atiny_free(void* ptr);

    extern void *LOS_MemAlloc (VOID *pPool, UINT32  uwSize);
    extern UINT32 LOS_QueueCreate(CHAR *pcQueueName,
                                          UINT16 usLen,
                                          UINT32 *puwQueueID,
                                          UINT32 uwFlags,
                                          UINT16 usMaxMsgSize );

    extern UINT32 LOS_QueueWriteCopy( UINT32 uwQueueID,
                                         VOID * pBufferAddr,
                                         UINT32 uwBufferSize,
                                         UINT32 uwTimeOut );
    extern UINT32 LOS_QueueReadCopy(UINT32 uwQueueID,
                                    VOID *pBufferAddr,
                                    UINT32 *puwBufferSize,
                                    UINT32 uwTimeOut);	
    extern UINT32 LOS_SemPend(UINT32 uwSemHandle, UINT32 uwTimeout);
    extern UINT32 LOS_SemCreate (UINT16 usCount, UINT32 *puwSemHandle);
    extern UINT32  LOS_MuxCreate (UINT32 *puwMuxHandle);
    extern int read_resp(uint8_t * buf);
    
    static UINT32 stub_LOS_SemPend(UINT32 uwSemHandle, UINT32 uwTimeout)	
    {
    	return -1;
    }
    int32_t obb_callback_handler(void * arg, int8_t * buf, int32_t len)
    {
    	return BUFLEN;
    }

    int32_t g_oob_callback2_flag = 0;
    int32_t oob_callback2(void * arg, int8_t * buf, int32_t len)
    {
        g_oob_callback2_flag = 1;
        return strlen((char*)buf);
    }

    static void *stub_LOS_MemAlloc (VOID *pPool, UINT32  uwSize)
    {
        void * ret = NULL;
        if(0 == (g_malloc_ctl_flag & 0x01))
            ret = malloc(uwSize); 
        g_malloc_ctl_flag >>= 1;
        return ret;
    }

    static UINT32 stub_LOS_QueueCreate(CHAR *pcQueueName,
                                          UINT16 usLen,
                                          UINT32 *puwQueueID,
                                          UINT32 uwFlags,
                                          UINT16 usMaxMsgSize )
    {
        UINT32 ret = LOS_NOK;
        if (g_quecreate_ctl_flag & 0x01)
            ret = QUEUE_CREATE;
        g_quecreate_ctl_flag >>= 1;
    	return ret;
    }

    static UINT32 stub_LOS_SemCreate (UINT16 usCount, UINT32 *puwSemHandle)
    {
        int ret = LOS_OK;
        if (g_semcreate_ctl_flag & 0x01)
            ret = LOS_NOK;
        g_semcreate_ctl_flag >>= 1;
        return ret;
    }

    static UINT32  stub_LOS_MuxCreate (UINT32 *puwMuxHandle)
    {
    	int ret = LOS_OK;
        if (g_muxcreate_ctl_flag & 0x01)
            ret = LOS_NOK;
        g_muxcreate_ctl_flag >>= 1;
        return ret;
    }

    static int stub_read_resp(uint8_t * buf)
    {
        if(NULL == gp_resp_ptr){
            return 0;
        }
        usleep(500000);
        strncpy((char*)buf, (char*)gp_resp_ptr, strlen((const char*)gp_resp_ptr));
    	return strlen((const char*)gp_resp_ptr);
    }
}
							
void TestAtadapter::test_at_listener_list_add()
{
   int32_t ret  = -1;
   at_listener listener;
   listener.suffix = (int8_t *)"OK";
   listener.resp = (int8_t*)"\r\n";
   at_listener_list_add(&listener);
   TEST_ASSERT_MSG(( ret == -1), "Test at_listener_list_add() Failed");
}

void TestAtadapter::test_at_listner_list_del()
{  
   int32_t ret  = -1;
   at_listener listener;
   listener.suffix = (int8_t *)"OK";
   listener.resp = (int8_t*)"\r\n";
   at.head = &listener;
   at_listner_list_del(&listener);
   TEST_ASSERT_MSG(( ret == -1), "Test at_listener_list_del() Failed");
}

void TestAtadapter::test_at_get_unuse_linkid()
{
    int32_t ret  = -1;
    at.init();
    at.mux_mode = 1;
    ret = at_get_unuse_linkid();
    TEST_ASSERT_MSG(( ret >= 0 && ret < at_user_conf.linkid_num), "Test at_get_unuse_linkid() Failed");
    at.deinit();
}

void TestAtadapter::test_store_resp_buf()
{
   int32_t ret  = -1;
   int8_t resp_buf[100] = {0};
   int8_t buf[] = "OK";
   uint32_t len = 100;
   store_resp_buf(resp_buf, sizeof(resp_buf), NULL, len);
   TEST_ASSERT_MSG(( ret == -1), "Test store_resp_buf() Failed");
   //abnormal
   store_resp_buf(resp_buf, sizeof(resp_buf), buf, len);
   TEST_ASSERT_MSG(( ret == -1), "Test store_resp_buf() Failed");
}

void TestAtadapter::test_at_cmd()
{
   int32_t ret  = -1;
   int8_t cmd[] = "AT\r";
   const char suffix[] = "OK";
   ret = at_cmd(cmd, strlen((char*)cmd),suffix,NULL);
   TEST_ASSERT_MSG(( ret == AT_OK), "Test test_at_cmd() Failed");
   
   //abnormal
   stubInfo stub_sem;
   setStub((void *)LOS_SemPend,(void *)stub_LOS_SemPend,&stub_sem); 
   ret = at_cmd(cmd, strlen((char*)cmd),suffix,NULL);
   TEST_ASSERT_MSG(( ret == -1), "Test test_at_cmd() Failed");
   cleanStub(&stub_sem);
}

void TestAtadapter::test_at_write()
{
   int32_t ret  = -1;
   int8_t cmd[] = "AT+CIPSEND";
   int8_t suffix[] = "SEND OK";
   int8_t buf[] = "helloworld";
   uint32_t buflen = strlen((char *)buf);
   ret = at_write(cmd,suffix,buf,buflen);
   TEST_ASSERT_MSG(( ret == buflen), "Test test_at_write() Failed");
   
   //abnormal
   stubInfo stub_sem;
   setStub((void *)LOS_SemPend,(void *)stub_LOS_SemPend,&stub_sem); 
   ret = at_write(cmd,suffix,buf,buflen);
   TEST_ASSERT_MSG(( ret == -1), "Test test_at_write() Failed");
   cleanStub(&stub_sem);
}

void TestAtadapter::test_cloud_cmd_matching()
{
   int32_t ret  = -1;
   at_init_oob();
   char featurestr[] = "\r\n+RECEIVE";
   int cmdlen = strlen(featurestr);
   ret = at_oob_register(featurestr, cmdlen, obb_callback_handler);
   int8_t buf[] = "\r\n+RECEIVE,1,4:\r\ndAbc";
   int8_t buf_str[] = "+RECEIVE,1,4:\r\ndAbc";
   int32_t len = strlen((char*)buf);
   ret = cloud_cmd_matching(buf, len);
   TEST_ASSERT_MSG(( ret == BUFLEN), "Test1 cloud_cmd_matching() Failed");
   //abnormal
   ret = cloud_cmd_matching(buf_str, len);
   TEST_ASSERT_MSG(( ret == 0), "Test2 cloud_cmd_matching() Failed");
}

void TestAtadapter::test_at_recv_task()
{
    pthread_t pid = 0;
    at_listener l;
    char buf[2048] = {0};

    stubInfo si_read_resp;
    setStub((void *)read_resp,(void *)stub_read_resp,&si_read_resp);
        
    l.suffix = (int8_t*)"TEST_SUFFIX";
    l.resp = NULL;
    l.resp_len = 0;
    l.next = NULL;
    at_init();
    at_oob_register("+IPD", strlen("+IPD"), oob_callback2);
    pthread_create(&pid, NULL, (void* (*)(void*))at_recv_task, NULL);

    #define NULL_STR ""
    #define TEST_OOB_STR "+IPD:xxxxxxxxxxxxxxxxxx"
    #define TEST_LISTENER_STR "TEST_SUFFIXxxxxxxxxxxxxxxxxxx"
    #define TEST_NONLISTENER_STR "TEST_NONLISTENER_STR"

    gp_resp_ptr = (uint8_t*)NULL_STR;
    sleep(1);
    TEST_ASSERT_MSG((g_oob_callback2_flag == 0), "test read_resp return 0 failed!");

    gp_resp_ptr = (uint8_t*)TEST_OOB_STR;
    sleep(1);
    TEST_ASSERT_MSG((g_oob_callback2_flag == 1), "test oob_callback get called failed!");
    g_oob_callback2_flag = 0;

    gp_resp_ptr = (uint8_t*)TEST_LISTENER_STR;
    sleep(1);
    TEST_ASSERT_MSG((strlen(buf) == 0), "test listener got resp failed, should be 0!");

    at_listener_list_add(&l);
    gp_resp_ptr = (uint8_t*)TEST_LISTENER_STR;
    sleep(1);
    TEST_ASSERT_MSG((strlen(buf) == 0), "test listener got resp failed, should be 0!");
    at_listner_list_del(&l);

    l.resp = (int8_t*)buf;
    l.resp_len = sizeof(buf);
    at_listener_list_add(&l);
    gp_resp_ptr = (uint8_t*)TEST_LISTENER_STR;
    sleep(1);
    printf("%d:buf = %s\r\n", __LINE__, buf);
    TEST_ASSERT_MSG((strncmp(buf, TEST_LISTENER_STR,strlen(TEST_LISTENER_STR)) == 0), "test listener got resp failed!");
    at_listner_list_del(&l);
    
    memset(buf, 0, sizeof(buf));
    at_listener_list_add(&l);
    gp_resp_ptr = (uint8_t*)TEST_NONLISTENER_STR;
    sleep(1);
    TEST_ASSERT_MSG((strncmp(buf, TEST_NONLISTENER_STR, strlen(TEST_NONLISTENER_STR)) == 0), "test listener got resp failed!");
    at_listner_list_del(&l);

    l.suffix = NULL;
    at_listener_list_add(&l);
    gp_resp_ptr = (uint8_t*)TEST_NONLISTENER_STR;
    sleep(1);
    TEST_ASSERT_MSG((strncmp(buf, TEST_NONLISTENER_STR, strlen(TEST_NONLISTENER_STR)) == 0), "test listener got resp failed!");

    at_listner_list_del(&l);
    pthread_cancel(pid);
    
    pthread_join(pid, NULL);
    cleanStub(&si_read_resp);
    at_deinit();
}

void TestAtadapter::test_create_at_recv_task()
{
   int32_t ret  = -1;
  
}

void TestAtadapter::test_at_init_oob()
{
   int32_t ret  = -1;
 
}

void TestAtadapter::test_at_struct_init()
{
    int32_t ret  = -1;
    at_task at;

    stubInfo si_mem_alloc;
    stubInfo si_queue_create;
    stubInfo si_sem_create;
    stubInfo si_mux_create;

    setStub((void *)LOS_MemAlloc,(void *)stub_LOS_MemAlloc,&si_mem_alloc); 
    setStub((void *)LOS_QueueCreate,(void *)stub_LOS_QueueCreate,&si_queue_create);
    setStub((void *)LOS_SemCreate,(void *)stub_LOS_SemCreate,&si_sem_create);
    setStub((void *)LOS_MuxCreate,(void *)stub_LOS_MuxCreate,&si_mux_create);

    ret = at_struct_init(NULL);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for NULL arg failed!");

    g_semcreate_ctl_flag = 1;
    ret = at_struct_init(&at);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the first sem_create failed!");

    g_semcreate_ctl_flag = 2;
    ret = at_struct_init(&at);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the second sem_create failed!");
    g_semcreate_ctl_flag = 0;

    g_muxcreate_ctl_flag = 1;
    ret = at_struct_init(&at);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the first mux_create failed!");
    g_muxcreate_ctl_flag = 0;


    g_malloc_ctl_flag = 1;
    ret = at_struct_init(&at);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the first atiny_malloc failed!");

    g_malloc_ctl_flag = 2;
    ret = at_struct_init(&at);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the second atiny_malloc failed!");

    g_malloc_ctl_flag = 4;
    ret = at_struct_init(&at);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the third atiny_malloc failed!");

    g_malloc_ctl_flag = 8;
    ret = at_struct_init(&at);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the fourth atiny_malloc failed!");

    int32_t id = 0;

    cleanStub(&si_mem_alloc);
    cleanStub(&si_queue_create);
    cleanStub(&si_sem_create);
    cleanStub(&si_mux_create);
}

TestAtadapter::TestAtadapter()
{
   TEST_ADD(TestAtadapter::test_at_listener_list_add);
   TEST_ADD(TestAtadapter::test_at_listner_list_del);
   TEST_ADD(TestAtadapter::test_at_get_unuse_linkid);
   TEST_ADD(TestAtadapter::test_store_resp_buf);
   TEST_ADD(TestAtadapter::test_at_cmd);
   TEST_ADD(TestAtadapter::test_at_write);
   TEST_ADD(TestAtadapter::test_cloud_cmd_matching);
   //TEST_ADD(TestAtadapter::test_at_init);
   TEST_ADD(TestAtadapter::test_at_recv_task);
   TEST_ADD(TestAtadapter::test_at_struct_init);
   
   
   
   #if 0
   TEST_ADD(TestAtadapter::test_create_at_recv_task);
   TEST_ADD(TestAtadapter::test_at_init_oob);
   TEST_ADD(TestAtadapter::test_at_init);
   TEST_ADD(TestAtadapter::test_at_oob_register);
   #endif
}

//protected:
void TestAtadapter::setup()
{  
}

void TestAtadapter::tear_down()
{
}

