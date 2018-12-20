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
#include "at_frame/at_main.h"

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

	extern int chartoint(const char* port);
    extern void at_listener_list_add(at_listener * p);
    extern void at_listner_list_del(at_listener * p);
    extern void store_resp_buf(int8_t * resp_buf, int32_t resp_len, int8_t * buf, uint32_t len);
    extern int32_t at_get_unuse_linkid();
    extern int32_t at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf);
    extern int32_t at_write(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len);
	int32_t at_oob_register(char *featurestr, int cmdlen, oob_callback callback, oob_cmd_match cmd_match);

	extern int los_nb_notify(char* featurestr,int cmdlen, oob_callback callback, oob_cmd_match cmd_match);
    extern void at_init_oob();
    extern int32_t at_init(at_config *config);
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
	uint32_t create_at_recv_task();
	int32_t at_struct_deinit(at_task *at);
    static int32_t test_callback_0(void* arg, int8_t* buf, int32_t buflen){
		printf("+-+-+-+-+-+-+-+-+-+- test_callback_0 +-+-+-+-+-+-+-+-+-+-\n");
		return 1;
	}
	
	static int32_t test_oob_cmd_match_0(const char *buf, char* featurestr,int len){
		printf("+-+-+-+-+-+-+-+-+-+- test_oob_cmd_match_0 +-+-+-+-+-+-+-+-+-+-\n");
		return 0;
	}
	
    static int test_by_shensheng(void){
        printf("I am a test\n");
        return 0;
    }
    static UINT32 stub_LOS_SemPend(UINT32 uwSemHandle, UINT32 uwTimeout)	
    {
    	return -1;
    }
	static void test_step_callback_0(void){}
    int32_t obb_callback_handler(const char * arg, char * buf, int len)
    {
    	return BUFLEN;
    }

    int32_t g_oob_callback2_flag = 0;
    int32_t oob_callback2(void * arg, int8_t * buf, int32_t len)
    {
        g_oob_callback2_flag = 1;
        return strlen((char*)buf);
    }
	void at_listner_list_destroy(at_task *at_tsk);

    static void *stub_LOS_MemAlloc (VOID *pPool, UINT32  uwSize)
    {
        void * ret = NULL;
        if(0 == (g_malloc_ctl_flag & 0x01))
            ret = malloc(uwSize); 
        g_malloc_ctl_flag >>= 1;
        return ret;
    }
	 static UINT32 stub_LOS_QueueCreateOK(CHAR *pcQueueName,UINT16 usLen,UINT32 *puwQueueID,
                                          UINT32 uwFlags,UINT16 usMaxMsgSize )
    {
        return 0;
    }
    static UINT32 stub_LOS_QueueCreateFalse(CHAR *pcQueueName,UINT16 usLen,UINT32 *puwQueueID,
                                          UINT32 uwFlags,UINT16 usMaxMsgSize )
    {
		return 1;
	}								  
    static UINT32 stub_LOS_QueueCreate(CHAR *pcQueueName,UINT16 usLen,UINT32 *puwQueueID,
                                          UINT32 uwFlags,UINT16 usMaxMsgSize )
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
	static UINT32 stub_LOS_SemCreateFalse (UINT16 usCount, UINT32 *puwSemHandle){
		return 1;
	}
	static UINT32 stub_LOS_SemCreateOK (UINT16 usCount, UINT32 *puwSemHandle){
		return 0;
	}
	static UINT32 stub_LOS_SemCreateCon (UINT16 usCount, UINT32 *puwSemHandle){
		if(*puwSemHandle==1)return 1;
		return 0;
	}
	static UINT32  stub_LOS_MuxCreateOK (UINT32 *puwMuxHandle){
		return 0;
	}
	static UINT32  stub_LOS_MuxCreateFalse (UINT32 *puwMuxHandle){
		if(*puwMuxHandle==1)return 1;
		return 0;
		return 1;
	}
	static UINT32  stub_LOS_MuxCreateCon (UINT32 *puwMuxHandle){
		if(*puwMuxHandle==1)return 1;
		return 0;
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
	static void stub_write_at_task_msg(at_msg_type_e type){}
	static CHAR* stub_LOS_TaskNameGet(uint32_t uwTaskID){
		static char c[8];
		return c;
	}
	static int32_t stub_at_struct_initFalse(at_task *at){
		return -1;
	}
	static int32_t stub_at_struct_init(at_task *at){
		return 0;
	}
	static int32_t stub_at_usart_initFalse(void){
		return -1;
	}
	static int32_t stub_at_usart_init(void){
		return 0;
	}
	static uint32_t stub_create_at_recv_taskFalse(void){
		return 2;
	}
	static UINT32 stub_LOS_SemDeleteFalse(UINT32 uwSemHandle){
		return 1;
	}
	static UINT32 stub_LOS_MuxDeleteFalse(UINT32 uwMuxHandle){
		return 1;
	}
	static UINT32 stub_LOS_MuxPend(UINT32 uwMuxHandle, UINT32 uwTimeout){
		return 0;
	}
	static UINT32 stub_LOS_MuxPendFalse(UINT32 uwMuxHandle, UINT32 uwTimeout){
		return 1;
	}
	static void stub_at_listner_list_destroy(at_task *at_tsk){	}
	static UINT32 stub_LOS_MuxPost(UINT32 uwMuxHandle){return 0;}
	static int32_t test_char_uint_0(const int8_t *data, uint32_t len){
		return 0;
	}
	static void stub_at_transmit(uint8_t *cmd, int32_t len, int flag){}
	static void stub_at_listener_list_add_private_del(at_listener *p){
		at_free(p);
	}
	static void stub_at_free_private(at_listener * p){
		//free(p);
		p = NULL;
	}
}//extern "C"

void TestAtadapter::test_at_listener_list_add()
{
   int32_t ret  = -1;
   at_listener listener;
   at_listener_list_add(&listener);
   TEST_ASSERT_MSG(( ret == -1), "Test at_listener_list_add() Failed");
   bool at_head_isNULL = (at.head == NULL);
   if(at_head_isNULL){
		at_listener l_s_al;
		at.head = &l_s_al;
   		at_listener_list_add(&listener);
		at.head = NULL;
   }
   at_listener_list_add(&listener);//at.head != NULL
}

void TestAtadapter::test_at_listner_list_del()
{  
   int32_t ret  = -1;
   at_listener listener;
   at.head = &listener;
   at_listner_list_del(&listener);
   TEST_ASSERT_MSG(( ret == -1), "Test at_listener_list_del() Failed");
   
   at_listner_list_del(at.head->next);
   at_task l_s_att_1;
   //at_task l_s_att_2;
   at_listener l_s_atl_1;
   at_listener l_s_atl_2;
   l_s_att_1.head = &l_s_atl_2;
   l_s_att_1.head->handle_data = test_char_uint_0;
   l_s_att_1.head->next = &l_s_atl_1;
   stubInfo si_free;
   setStub((void*)at_free,(void*)stub_at_free_private,&si_free);
   //at_listner_list_destroy(&l_s_att_1);
   cleanStub(&si_free);
   //at_free(l_s_att_1.head);
   
}

void TestAtadapter::test_at_get_unuse_linkid()
{
    int32_t ret  = -1;
	at_config l_s_atConfig;
    at.init(&l_s_atConfig);
    at.mux_mode = 1;
    ret = at_get_unuse_linkid();
    //TEST_ASSERT_MSG(( ret >= 0 && ret < at_user_conf.linkid_num), "Test at_get_unuse_linkid() Failed");
    at.deinit();
}

void TestAtadapter::test_store_resp_buf()
{
   int32_t ret  = -1;
   int8_t resp_buf[100] = {0};
   int8_t buf[] = "OK";
   uint32_t len = 100;
   int8_t src[100] = {0};
   int maxlen = 0;
   store_resp_buf(resp_buf, src, sizeof(src), &maxlen);
   TEST_ASSERT_MSG(( ret == -1), "Test store_resp_buf() Failed");
   //abnormal
   store_resp_buf(resp_buf, src, sizeof(src), &maxlen);
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
   int8_t buf[] = "\r\n+RECEIVE,1,4:\r\ndAbc";
   int8_t buf_str[] = "+RECEIVE,1,4:\r\ndAbc";
   int32_t len = strlen((char*)buf);
   //at_oob_register(featurestr, cmdlen, NULL ,obb_callback_handler);
   
   ret = cloud_cmd_matching(buf, len);
   TEST_ASSERT_MSG(( ret == BUFLEN), "Test1 cloud_cmd_matching() Failed");
   ret = cloud_cmd_matching(buf_str, len);
   TEST_ASSERT_MSG(( ret == 0), "Test2 cloud_cmd_matching() Failed");
   at_init_oob();
   at_oob.oob_num = 1;
   at_oob.oob[0].cmd_match = test_oob_cmd_match_0;
   at_oob.oob[0].callback = test_callback_0;
   ret = cloud_cmd_matching(buf_str, len);
}

void TestAtadapter::test_at_recv_task()
{
    int oldstate = 0,oldtype = 0;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,&oldstate);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&oldtype);
    pthread_t pid = 0;
    at_listener l;
    char buf[2048] = {0};

    stubInfo si_read_resp;
    setStub((void *)read_resp,(void *)stub_read_resp,&si_read_resp);
    char l_char_1[20];
	char l_p_char_1 = &l_char_1;
    l.cmd_info.suffix = l_p_char_1;//(char*)"TEST_SUFFIX";
    l.cmd_info.resp_buf = NULL;
    l.cmd_info.resp_len = 0;
    //3 lines above commented by shensheng
    l.next = NULL;

	at_config l_s_atconfig;
    at_init(&l_s_atconfig);
    at_oob_register("+IPD", strlen("+IPD"), NULL,oob_callback2);
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

    l.cmd_info.resp_buf = (int8_t*)buf;
    l.cmd_info.resp_len = sizeof(buf);
    //2 lines above commented by shensheng
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

    l.cmd_info.suffix = NULL; 
    
    at_listener_list_add(&l);
    gp_resp_ptr = (uint8_t*)TEST_NONLISTENER_STR;
    sleep(1);
    TEST_ASSERT_MSG((strncmp(buf, TEST_NONLISTENER_STR, strlen(TEST_NONLISTENER_STR)) == 0), "test listener got resp failed!");

    at_listner_list_del(&l);
    int rv_cancel = pthread_cancel(pid);
    printf("rv_cancel is %d\n",rv_cancel);
    printf("before pthread_join \n");
    int retval = 42;
    int * p_rv = &retval;
    pthread_join(pid, &p_rv);
    printf("after pthread_join \n");
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
	setStub((void *)LOS_QueueCreate,(void *)stub_LOS_QueueCreateOK,&si_queue_create);
	setStub((void *)LOS_SemCreate,(void *)stub_LOS_SemCreateFalse,&si_sem_create);
	ret = at_struct_init(&at);
	cleanStub(&si_queue_create);
    cleanStub(&si_sem_create);

	setStub((void *)LOS_QueueCreate,(void *)stub_LOS_QueueCreateOK,&si_queue_create);
	setStub((void *)LOS_SemCreate,(void *)stub_LOS_SemCreateCon,&si_sem_create);
	setStub((void *)LOS_MuxCreate,(void *)stub_LOS_MuxCreateCon,&si_mux_create);
	at.recv_sem = 2;
	at.cmd_mux = 1;
	ret = at_struct_init(&at);//ret = LOS_MuxCreate((UINT32 *)&at->cmd_mux);
	at.cmd_mux = 2;
	at.trx_mux = 1;
	ret = at_struct_init(&at);//ret = LOS_MuxCreate((UINT32 *)&at->trx_mux);
	at.trx_mux = 2;
	at.resp_sem = 1;
	ret = at_struct_init(&at);//ret = LOS_SemCreate(0, (UINT32 *)&at->resp_sem);
	
	cleanStub(&si_queue_create);
    cleanStub(&si_sem_create);
    cleanStub(&si_mux_create);
	at_struct_deinit(NULL);

	stubInfo si_semDel;
	stubInfo si_muxDel;
	stubInfo si_muxpend;
	stubInfo si_muxpost;
	stubInfo si_lislist;
	at.trx_mux_flag = false;
	at.rid_flag = false;
	at.recv_buf = NULL;
	at.cmdresp = NULL;
	at.userdata = NULL;
	at.saveddata = NULL;
	at.linkid = NULL;
	setStub((void*)LOS_MuxPend,(void*)stub_LOS_MuxPend,&si_muxpend);
	setStub((void*)at_listner_list_destroy,(void*)stub_at_listner_list_destroy,&si_lislist);
	setStub((void*)LOS_MuxPost,(void*)stub_LOS_MuxPost,&si_muxpost);

	setStub((void*)LOS_SemDelete,(void*)stub_LOS_SemDeleteFalse,&si_semDel);
	setStub((void*)LOS_MuxDelete,(void*)stub_LOS_MuxDeleteFalse,&si_muxDel);
	at_struct_deinit(&at);
	cleanStub(&si_muxpost);
	cleanStub(&si_semDel);
	cleanStub(&si_muxDel);
	cleanStub(&si_muxpend);
	cleanStub(&si_lislist);
	
	
	
}
void TestAtadapter::test_chartoint(){
	char a[4] = {'4','2','4','7'};
	int ret = chartoint(a);
	TEST_ASSERT(4247==ret);
}
void TestAtadapter::test_at_init(){
	at_init(NULL);
	at_config l_s_ac_1;
	
	char l_char_name[20];
	char l_char_cmd_begin[20];
	char l_char_line_end[20];
	l_s_ac_1.name = l_char_name;
	l_s_ac_1.cmd_begin = l_char_cmd_begin;
	l_s_ac_1.line_end = l_char_line_end;
	
	stubInfo si_atStructInit;
	setStub((void*)at_struct_init,(void*)stub_at_struct_initFalse,&si_atStructInit);
	at_init(&l_s_ac_1);
	cleanStub(&si_atStructInit);

	setStub((void*)at_struct_init,(void*)stub_at_struct_init,&si_atStructInit);
	stubInfo si_usartInit;
	setStub((void*)at_usart_init,(void*)stub_at_usart_initFalse,&si_usartInit);
	at_init(&l_s_ac_1);
	cleanStub(&si_usartInit);
	
	setStub((void*)at_usart_init,(void*)stub_at_usart_init,&si_usartInit);
	stubInfo si_recv;
	setStub((void*)create_at_recv_task,(void*)stub_create_at_recv_taskFalse,&si_recv);
	at_init(&l_s_ac_1);
	cleanStub(&si_recv);
	cleanStub(&si_atStructInit);

	
}
void TestAtadapter::test_at_oob_register(){
	char featurestr[20];
	int cmdlen = 12;
	oob_callback l_callback_1 = test_callback_0;
	oob_cmd_match l_cmd_match_1 = test_oob_cmd_match_0;
	printf("at_oob.oob_num = %x\n",at_oob.oob_num);
	at_oob_register(NULL,cmdlen,NULL,NULL);
	//at_oob_register(featurestr,cmdlen,l_callback_1,l_cmd_match_1);
	at_task l_s_att;
	at_reg_step_callback(&l_s_att,test_step_callback_0);
}
void TestAtadapter::test_at_deinit(){
	//at.tsk_hdl = 21;
	stubInfo si_nameget;
	stubInfo si_write;
	setStub((void*)LOS_TaskNameGet,(void*)stub_LOS_TaskNameGet,&si_nameget);
	setStub((void*)write_at_task_msg,(void*)stub_write_at_task_msg,&si_write);
	at_deinit();
	cleanStub(&si_nameget);
	cleanStub(&si_write);
}
void TestAtadapter::test_set_get_config(){
	at_config a = {
    "BG36",
    3,
    115200,
    11,
    4096,
    "\r\n",
    "\r",
    1, //support multi connection mode
    10000,   //  ms
	};
	at_set_config(&a);
	at_get_config();
}

	
	

void TestAtadapter::test_at_cmd_in_callback(){
	const int8_t cmd[10];
	int32_t len = 10;
	uint32_t timeout = 1000;
	stubInfo si_muxpend;
	stubInfo si_transmit;
	setStub((void*)at_transmit,(void*)stub_at_transmit,&si_transmit);
	
	setStub((void*)LOS_MuxPend,(void*)stub_LOS_MuxPendFalse,&si_muxpend);
	at_cmd_in_callback(cmd,len,test_char_uint_0,timeout);
	cleanStub(&si_muxpend);
	
	setStub((void*)LOS_MuxPend,(void*)stub_LOS_MuxPend,&si_muxpend);
	stubInfo si_lislist;
	stubInfo si_muxpost;
	setStub((void*)LOS_MuxPost,(void*)stub_LOS_MuxPost,&si_muxpost);
	setStub((void*)at_listener_list_add,(void*)stub_at_listener_list_add_private_del,&si_lislist);
	at_cmd_in_callback(cmd,len,test_char_uint_0,timeout);
	cleanStub(&si_lislist);
	cleanStub(&si_muxpost);
	cleanStub(&si_muxpend);
	
	cleanStub(&si_transmit);
	
}

TestAtadapter::TestAtadapter()
{
	TEST_ADD(TestAtadapter::test_at_cmd_in_callback);
	TEST_ADD(TestAtadapter::test_set_get_config);
	TEST_ADD(TestAtadapter::test_at_deinit);
	TEST_ADD(TestAtadapter::test_at_oob_register);
	TEST_ADD(TestAtadapter::test_at_init);
	TEST_ADD(TestAtadapter::test_chartoint);
    TEST_ADD(TestAtadapter::test_at_listener_list_add);
    TEST_ADD(TestAtadapter::test_at_listner_list_del);
    TEST_ADD(TestAtadapter::test_at_get_unuse_linkid);
    TEST_ADD(TestAtadapter::test_store_resp_buf);
    TEST_ADD(TestAtadapter::test_at_cmd);
    TEST_ADD(TestAtadapter::test_at_write);

    TEST_ADD(TestAtadapter::test_cloud_cmd_matching);
    TEST_ADD(TestAtadapter::test_create_at_recv_task);
    
    TEST_ADD(TestAtadapter::test_at_struct_init);
    TEST_ADD(TestAtadapter::test_at_init_oob);
	
	////
	//TEST_ADD(TestAtadapter::test_at_recv_task);
}

void TestAtadapter::setup()
{  
}

void TestAtadapter::tear_down()
{
}

