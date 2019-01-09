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
#include "test_at_main.h"
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
    extern void at_recv_task(void);
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
    extern void at_listner_list_destroy(at_task *at_tsk);
	extern uint32_t create_at_recv_task();
	extern int32_t at_struct_deinit(at_task *at);
    extern at_config *at_get_config(void);
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
	

    static void *stub_LOS_MemAlloc (VOID *pPool, UINT32  uwSize)
    {
        void * ret = NULL;
        if(0 == (g_malloc_ctl_flag & 0x01))
            ret = malloc(uwSize);
        //printf("[stub_LOS_MemAlloc:]g_malloc_ctl_flag is %d,ret is %p\n",g_malloc_ctl_flag,ret);
        g_malloc_ctl_flag >>= 1;
        return ret;
    }
	 
    static UINT32 stub_LOS_QueueCreateFalse(CHAR *pcQueueName,UINT16 usLen,UINT32 *puwQueueID,
                                          UINT32 uwFlags,UINT16 usMaxMsgSize )
    {
		return 1;
	}								  
    static UINT32 stub_LOS_QueueCreate(CHAR *pcQueueName,UINT16 usLen,UINT32 *puwQueueID,
                                          UINT32 uwFlags,UINT16 usMaxMsgSize )
    {
        return 1;
    }

    static UINT32 stub_LOS_SemCreate (UINT16 usCount, UINT32 *puwSemHandle)
    {
        int ret = LOS_OK;
        if (g_semcreate_ctl_flag & 0x01)
            ret = LOS_NOK;
        g_semcreate_ctl_flag >>= 1;
        return ret;
    }
    static UINT32 stub_LOS_SemDelete (UINT32 uwSemHandle)
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

    
	static void stub_write_at_task_msg(at_msg_type_e type){}
	static CHAR* stub_LOS_TaskNameGet(uint32_t uwTaskID){
		return (char *)100;
	}
    static UINT32 stub_LOS_TaskDeleteFalse(UINT32 uwTaskID){
		return 1;
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
    static uint32_t stub_create_at_recv_taskTrue(void){
		return 0;
	}
    
	static UINT32 stub_LOS_SemDeleteFalse(UINT32 uwSemHandle){
		return 1;
	}
	static UINT32 stub_LOS_MuxDeleteFalse(UINT32 uwMuxHandle){
		return 1;
	}
	static UINT32 stub_LOS_MuxPendFalse(UINT32 uwMuxHandle, UINT32 uwTimeout){
		return 1;
	}
	static void stub_at_listner_list_destroy(at_task *at_tsk){	}
	static int32_t test_char_uint_0(const int8_t *data, uint32_t len){
		return 0;
	}
	static void stub_at_transmit(uint8_t *cmd, int32_t len, int flag){}

    static void *stub_at_malloc_failed(size_t size)
    {
        return NULL;
    }
    static int32_t handle_data_demo(const int8_t *data, uint32_t len)
    {
        printf("+++++++++++++++handle_data_demo return 0\n");
        return 0;
    }

    static UINT32 stub_LOS_TaskCreateFalse(UINT32 *puwTaskID, TSK_INIT_PARAM_S *pstInitParam)
    {
        return 1;
    }

    static void stub_step_callback(void)
    {
    
    }

    #define TEST_AT_READ_RESP_START 20
    #define TEST_AT_READ_RESP_END   80
     UINT32 task_start = 0;
    static int stub_read_resp(uint8_t *buf, recv_buff* recv_buf)
    {
        if(task_start == 3)return 0;
        else return (TEST_AT_READ_RESP_END - TEST_AT_READ_RESP_START);
    }
    
    UINT32 stub_LOS_QueueReadCopy(UINT32  uwQueueID,VOID *  pBufferAddr,
                            UINT32 * puwBufferSize, UINT32  uwTimeOut)
    {
        recv_buff *rbuff = (recv_buff *)pBufferAddr;
        
        task_start++;
        sleep(1);
        rbuff->end=TEST_AT_READ_RESP_END;
        rbuff->ori=TEST_AT_READ_RESP_START;
        
        printf("task_start is %d,at.head is %p\n",task_start,at.head);
        if(task_start == 4)
        {
            rbuff->msg_type =AT_USART_RX;
            printf("task_start is %d, return NOK!!\n",task_start);
            return 1;
        }
        if(task_start >= 8)
        {
             
            rbuff->msg_type =AT_TASK_QUIT; 
            printf("task_start is %d, return QUIT MSG!!\n",task_start);
            return 0;
        }
        
        if(task_start == 1)
        {
            printf("task_start is %d, return AT_SENT_DONE!!\n",task_start);
            rbuff->msg_type =AT_SENT_DONE;
        }
        else
        {
            printf("task_start is %d, return AT_USART_RX!!\n",task_start);
            rbuff->msg_type =AT_USART_RX;
            if(task_start == 6)
            {
                printf("+++++++++++++++call at_init_oob now ++++++++++++++++++++++!!\n");
                at_init_oob();
            }
        }

        return 0;

        
    }

        
}//extern "C"

void TestAtadapter::test_at_init(){
    int32_t ret; 
    
	ret = at_init(NULL);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_init failed!");
    
	at_config at_user_conf;

    at_user_conf.name = "SIM900A";
    at_user_conf.usart_port = 3;
    at_user_conf.buardrate = 115200;
    at_user_conf.linkid_num = 4;
    at_user_conf.user_buf_len = 1024*2;
    at_user_conf.cmd_begin = "\r\n";
    at_user_conf.line_end = "\r";
    at_user_conf.mux_mode = 1;
    at_user_conf.timeout = 5000;

	at_set_config(&at_user_conf);
	
	stubInfo si_atStructInit;
	setStub((void*)at_struct_init,(void*)stub_at_struct_initFalse,&si_atStructInit);
	ret = at_init(&at_user_conf);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_init failed!");
    cleanStub(&si_atStructInit);
    
	
	stubInfo si_usartInit;
	setStub((void*)at_usart_init,(void*)stub_at_usart_initFalse,&si_usartInit);
	ret = at_init(&at_user_conf);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_init failed!");
    cleanStub(&si_usartInit);
    
   
	stubInfo si_recv;
	setStub((void*)create_at_recv_task,(void*)stub_create_at_recv_taskFalse,&si_recv);
    ret = at_init(&at_user_conf);
    TEST_ASSERT_MSG((ret == AT_FAILED), "test at_init failed!");
    cleanStub(&si_recv);
    
  
    stubInfo si_recv2;
    setStub((void*)create_at_recv_task,(void*)stub_create_at_recv_taskTrue,&si_recv2);
    ret = at_init(&at_user_conf);
	TEST_ASSERT_MSG((ret == AT_OK), "test at_init failed!");
    at_deinit();

    ret = at_init(&at_user_conf);
	TEST_ASSERT_MSG((ret == AT_OK), "test at_init failed!");
    
    cleanStub(&si_recv2);

    

    


}

void TestAtadapter::test_at_get_time()
{
    uint32_t curtime;
    curtime = at_get_time();
    TEST_ASSERT_MSG((curtime == 0), "test test_at_get_time failed!");
    
}

void TestAtadapter::test_chartoint(){
	char a[4] = {'4','2','4','7'};
	int ret = chartoint(a);
	TEST_ASSERT(4247==ret);
}


at_listener listener;
at_listener listener2;
at_listener listener3;

void TestAtadapter::test_at_listener_list_add()
{
   
    memset(&listener, 0 ,sizeof(listener));
    listener.handle_data = handle_data_demo;
    listener.expire_time = at_get_time() + 1000;
    at_listener_list_add(&listener);
    TEST_ASSERT_MSG(( at.head != NULL), "Test at_listener_list_add() Failed");

   
    memset(&listener2, 0 ,sizeof(listener2));
    listener2.handle_data = handle_data_demo;
    listener2.expire_time = at_get_time() + 1000;
    at_listener_list_add(&listener2);

    memset(&listener3, 0 ,sizeof(listener3));
    listener3.handle_data = handle_data_demo;
    listener3.expire_time = at_get_time() + 1000;
    at_listener_list_add(&listener3);
   
}

void TestAtadapter::test_at_listner_list_del()
{  
    at_listner_list_del(&listener);
    at_listner_list_del(&listener3);
    at_listner_list_del(&listener2);
 
    TEST_ASSERT_MSG(( at.head == NULL), "Test at_listener_list_add() Failed");
   
}

void TestAtadapter::test_at_listner_list_destroy()
{
    at_listener *ln1;
    at_listener *ln2;

    ln1 = malloc(sizeof(at_listener));
    memset(ln1, 0 ,sizeof(*ln1));
    ln1->handle_data = handle_data_demo;
    ln1->expire_time = at_get_time() + 1000;
    at_listener_list_add(ln1);
    TEST_ASSERT_MSG(( at.head != NULL), "Test test_at_listner_list_destroy() Failed");
    ln2 = malloc(sizeof(at_listener));
    memset(ln2, 0 ,sizeof(*ln2));
    ln2->handle_data = handle_data_demo;
    ln2->expire_time = at_get_time() + 1000;
    at_listener_list_add(ln2);

    at_listner_list_destroy(&at);
    TEST_ASSERT_MSG(( at.head == NULL), "Test test_at_listner_list_destroy() Failed");
    
    
}


void TestAtadapter::test_at_get_unuse_linkid()
{
    int32_t ret  = -1;
    at_config *config = at_get_config();
    ret = at_get_unuse_linkid();
    TEST_ASSERT_MSG(( ret >= 0 && ret < config->linkid_num), "Test at_get_unuse_linkid() Failed");
}

void TestAtadapter::test_store_resp_buf()
{
   int32_t ret  = -1;
   int8_t resp_buf[100] = {0};
   uint32_t len = 100;
   int8_t src[] = "hello world";
   int maxlen = 100;
   store_resp_buf(resp_buf, src, sizeof(src), &maxlen);
   TEST_ASSERT_MSG(( maxlen == sizeof(src)), "Test store_resp_buf() Failed");
}

void TestAtadapter::test_at_cmd_in_callback(){
    int32_t ret;
	const int8_t cmd[10];
	int32_t len = 10;
	uint32_t timeout = 1000;
	stubInfo si_muxpend;
	stubInfo si_transmit;
	setStub((void*)at_transmit,(void*)stub_at_transmit,&si_transmit);
	
	setStub((void*)LOS_MuxPend,(void*)stub_LOS_MuxPendFalse,&si_muxpend);
	ret = at_cmd_in_callback(cmd,len,test_char_uint_0,timeout);
    TEST_ASSERT_MSG(( ret == AT_FAILED), "test_at_cmd_in_callback() Failed");
	cleanStub(&si_muxpend);
    
    stubInfo si_malloc;
    setStub((void*)at_malloc,(void*)stub_at_malloc_failed,&si_malloc);
    ret = at_cmd_in_callback(cmd,len,test_char_uint_0,timeout);
    TEST_ASSERT_MSG(( ret == AT_FAILED), "test_at_cmd_in_callback() Failed");
    cleanStub(&si_malloc);
	
	ret = at_cmd_in_callback(cmd,len,test_char_uint_0,timeout);
    TEST_ASSERT_MSG(( ret == AT_OK), "test_at_cmd_in_callback() Failed");

	cleanStub(&si_transmit);

    at_listner_list_destroy(&at);
	
}

void TestAtadapter::test_at_cmd_multi_suffix()
{
    
}


void TestAtadapter::test_at_cmd()
{
   int32_t ret  = -1;
   int8_t cmd[] = "AT\r";
   const char suffix[] = "OK";
   
   //abnormal
   ret = at_cmd(NULL, 0,suffix,NULL);
   TEST_ASSERT_MSG(( ret == AT_FAILED), "Test test_at_cmd() Failed");
   
   stubInfo stub_sem;
   setStub((void *)LOS_SemPend,(void *)stub_LOS_SemPend,&stub_sem); 
   ret = at_cmd(cmd, strlen((char*)cmd),suffix,NULL);
   TEST_ASSERT_MSG(( ret == AT_FAILED), "Test test_at_cmd() Failed");
   cleanStub(&stub_sem);

   ret = at_cmd(cmd, strlen((char*)cmd),suffix,NULL);
   TEST_ASSERT_MSG(( ret == AT_OK), "Test test_at_cmd() Failed");
}

void TestAtadapter::test_at_write()
{
   int32_t ret  = -1;
   int8_t cmd[] = "AT+CIPSEND";
   int8_t suffix[] = "SEND OK";
   int8_t buf[] = "helloworld";
   uint32_t buflen = strlen((char *)buf);
   
   
   stubInfo stub_sem;
   setStub((void *)LOS_SemPend,(void *)stub_LOS_SemPend,&stub_sem); 
   ret = at_write(cmd,suffix,buf,buflen);
   TEST_ASSERT_MSG(( ret == -1), "Test test_at_write() Failed");
   cleanStub(&stub_sem);

   ret = at_write(cmd,suffix,buf,buflen);
   TEST_ASSERT_MSG(( ret == buflen), "Test test_at_write() Failed");
}


void TestAtadapter::test_at_init_oob()
{
   
}

void TestAtadapter::test_at_oob_register(){
    
    int32_t ret  = -1;
    
	char featurestr[20];
	int cmdlen = 12;
	oob_callback l_callback_1 = test_callback_0;
	oob_cmd_match l_cmd_match_1 = test_oob_cmd_match_0;
    at_init_oob();
	printf("at_oob.oob_num = %x\n",at_oob.oob_num);
    
	ret = at_oob_register(NULL,cmdlen,NULL,NULL);
    TEST_ASSERT_MSG(( ret == -1), "Test test_at_write() Failed");
    
	ret = at_oob_register("+Receive",cmdlen,l_callback_1,l_cmd_match_1);

    printf("++++++++++++++++++++at_oob.oob_num = %x\n",at_oob.oob_num);
    TEST_ASSERT_MSG(( ret == 0), "Test test_at_write() Failed");
	at_task l_s_att;
	at_reg_step_callback(&l_s_att,test_step_callback_0);
    printf("+++++++++exit+++++++++++at_oob.oob_num = %x\n",at_oob.oob_num);
}



void TestAtadapter::test_cloud_cmd_matching()
{
   int32_t ret  = -1;

   char featurestr[] = "\r\n+RECEIVE";
   int cmdlen = strlen(featurestr);
   int8_t buf[] = "\r\n+RECEIVE,1,4:\r\ndAbc";
   int8_t buf_str[] = "+RECEIVE,1,4:\r\ndAbc";
   int32_t len = strlen((char*)buf);
   
   ret = cloud_cmd_matching(buf, len);
   TEST_ASSERT_MSG(( ret == len), "Test1 cloud_cmd_matching() Failed");

   at_init_oob();
   
   ret = cloud_cmd_matching(buf_str, len);
   TEST_ASSERT_MSG(( ret == 0), "Test2 cloud_cmd_matching() Failed");
   
   ret = at_oob_register("+Receive",cmdlen,test_callback_0,test_oob_cmd_match_0);
}




void TestAtadapter::test_at_struct_init()
{
    int32_t ret  = -1;

    at_task att;

    stubInfo si_mem_alloc;
    stubInfo si_queue_create;
    stubInfo si_sem_create;
    stubInfo si_mux_create;
    memset(&att,0,sizeof(att));

    setStub((void *)LOS_MemAlloc,(void *)stub_LOS_MemAlloc,&si_mem_alloc); 
    
    setStub((void *)LOS_SemCreate,(void *)stub_LOS_SemCreate,&si_sem_create);
    setStub((void *)LOS_MuxCreate,(void *)stub_LOS_MuxCreate,&si_mux_create);

    ret = at_struct_init(NULL);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for NULL arg failed!");

    setStub((void *)LOS_QueueCreate,(void *)stub_LOS_QueueCreate,&si_queue_create);
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the first sem_create failed!");
    cleanStub(&si_queue_create);
    

    g_semcreate_ctl_flag = 1;
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the first sem_create failed!");
    g_semcreate_ctl_flag = 0;

    g_muxcreate_ctl_flag = 1;
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the first mux_create failed!");
    g_muxcreate_ctl_flag = 0;

    g_muxcreate_ctl_flag = 2;
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the second mux_create failed!");
    g_muxcreate_ctl_flag = 0;


    g_semcreate_ctl_flag = 2;
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the second sem_create failed!");
    g_semcreate_ctl_flag = 0;
    



    g_malloc_ctl_flag = 1;
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the first atiny_malloc failed!");

    g_malloc_ctl_flag = 2;
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the second atiny_malloc failed!");


    g_malloc_ctl_flag = 4;
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the third atiny_malloc failed!");


    g_malloc_ctl_flag = 8;
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the fourth atiny_malloc failed!");


    g_malloc_ctl_flag = 16;
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_init for the fifth atiny_malloc failed!");
    g_malloc_ctl_flag = 0;

    int32_t id = 0;

    cleanStub(&si_mem_alloc);
    cleanStub(&si_sem_create);
    cleanStub(&si_mux_create);

    ret = at_struct_deinit(NULL);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_deinit failed!");


    #if 1
    
    ret = at_struct_init(&att);
    TEST_ASSERT_MSG((ret == AT_OK), "Test at_struct_deinit failed!");
    
    stubInfo sem_del;
    
    setStub((void *)LOS_SemDelete,(void *)stub_LOS_SemDelete,&sem_del);
    g_semcreate_ctl_flag = 1;
    printf("---------------------------------++++++++++++=att.head is %p\n",att.head);
    
    ret = at_struct_deinit(&att);
    printf("22 exit from test_at_struct_init++++++++++++++++++++++++++++++++++++++\n");
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_deinit failed!");
    g_semcreate_ctl_flag = 0;
   
    stubInfo mux_del;
    setStub((void *)LOS_MuxDelete,(void *)stub_LOS_SemCreateFalse,&mux_del);
    ret = at_struct_deinit(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_deinit failed!");
    cleanStub(&mux_del);

    g_semcreate_ctl_flag = 2;
    ret = at_struct_deinit(&att);
    TEST_ASSERT_MSG((ret == -1), "Test at_struct_deinit failed!");
    g_semcreate_ctl_flag = 0;
    
    cleanStub(&sem_del);

    printf("exit from test_at_struct_init++++++++++++++++++++++++++++++++++++++\n");
    #endif

}


void TestAtadapter::test_create_at_recv_task()
{
    int32_t ret  = -1;
    
    ret = create_at_recv_task();
    TEST_ASSERT_MSG((ret == 0), "Test test_create_at_recv_task failed!");

    stubInfo tsk_create;
    setStub((void *)LOS_TaskCreate,(void *)stub_LOS_TaskCreateFalse,&tsk_create);
    ret = create_at_recv_task();
    TEST_ASSERT_MSG((ret == 1), "Test test_create_at_recv_task failed!");
    cleanStub(&tsk_create);
    
   
}
void TestAtadapter::test_at_recv_task()
{
    at_listener *ln1;
    at_listener *ln2;

    printf("$$$$$$$$$$$$$$$---at.head is %p\n",at.head);

    ln1 = malloc(sizeof(at_listener));
    memset(ln1, 0 ,sizeof(*ln1));
    ln1->handle_data = handle_data_demo;
    ln1->expire_time = at_get_time() + 1000;
    at_listener_list_add(ln1);
    printf("$$$$$$$$$$$$$$$------ln1 is %p, at.head  is %p, handle_data is %p\n",ln1,at.head,ln1->handle_data);
    
    ln2 = malloc(sizeof(at_listener));
    memset(ln2, 0 ,sizeof(*ln2));
    ln2->handle_data = NULL;
    ln2->expire_time = at_get_time() + 1000;
    at_listener_list_add(ln2);
    
    printf("$$$$$$$$$$$$$$$------ln2 is %p, at.head  is %p, handle_data is %p\n",ln2,at.head,ln2->handle_data);
    
    TEST_ASSERT_MSG(( at.head != NULL), "Test test_at_listner_list_destroy() Failed");

    stubInfo queue_recv;
    stubInfo recv_resp;
    at.step_callback = stub_step_callback;
    setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy,&queue_recv);
    setStub((void *)read_resp,(void *)stub_read_resp,&queue_recv);
    at_recv_task();
    
    at_listner_list_del(ln2);
    free(ln2);
    
    cleanStub(&queue_recv);

    //free(ln2);
    
}


void TestAtadapter::test_at_deinit(){

    stubInfo tsk1;
    stubInfo del1;
    
    setStub((void *)LOS_TaskNameGet,(void *)stub_LOS_TaskNameGet,&tsk1);
    setStub((void *)LOS_TaskDelete,(void *)stub_LOS_TaskDeleteFalse,&del1);

    stubInfo sem_del;
    
    setStub((void *)LOS_SemDelete,(void *)stub_LOS_SemDelete,&sem_del);
    g_semcreate_ctl_flag = 1;
    at_deinit();
    g_semcreate_ctl_flag = 0;
    cleanStub(&sem_del);
    
    cleanStub(&tsk1);
    cleanStub(&del1);
    
	
}



TestAtadapter::TestAtadapter()
{
    TEST_ADD(TestAtadapter::test_at_init);
    TEST_ADD(TestAtadapter::test_at_get_time);
    TEST_ADD(TestAtadapter::test_chartoint);
    TEST_ADD(TestAtadapter::test_at_listener_list_add);
    TEST_ADD(TestAtadapter::test_at_listner_list_del);
    TEST_ADD(TestAtadapter::test_at_listner_list_destroy);
    TEST_ADD(TestAtadapter::test_at_get_unuse_linkid);
    TEST_ADD(TestAtadapter::test_store_resp_buf);
    TEST_ADD(TestAtadapter::test_at_cmd_in_callback);
    TEST_ADD(TestAtadapter::test_at_cmd_multi_suffix);
    TEST_ADD(TestAtadapter::test_at_cmd);
    TEST_ADD(TestAtadapter::test_at_write);
    TEST_ADD(TestAtadapter::test_at_init_oob);
    TEST_ADD(TestAtadapter::test_at_oob_register);
    TEST_ADD(TestAtadapter::test_cloud_cmd_matching);
    
    TEST_ADD(TestAtadapter::test_at_struct_init);
    TEST_ADD(TestAtadapter::test_create_at_recv_task);
    
	////
	TEST_ADD(TestAtadapter::test_at_recv_task);
	TEST_ADD(TestAtadapter::test_at_deinit);
}

void TestAtadapter::setup()
{  
}

void TestAtadapter::tear_down()
{
}

