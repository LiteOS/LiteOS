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
#include "test_sim900a.h"

extern "C" 
{

#include "sim900a.h"
    
    extern at_task at;
    extern at_config at_user_conf;

    int QUEUE_WRITE = 0;
    int CMD_TIMES = 0;

    extern int32_t sim900a_echo_off(void);
    extern int32_t sim900a_ini();
    extern int32_t sim900a_connect(const int8_t * host, const int8_t * port, int proto);
    extern int32_t sim900a_recv(int32_t id, int8_t * buf, uint32_t len);
    extern int32_t sim900a_recv_timeout(int32_t id, int8_t * buf, uint32_t len, int32_t timeout);
    extern int32_t sim900a_send(int32_t id , const uint8_t  *buf, uint32_t len);
    extern void sim900a_check(void);
    extern int32_t sim900a_recv_cb(int32_t id);
    extern int32_t sim900a_deinit(void);
    extern int32_t sim900a_close(int32_t id);
    extern int32_t sim900a_data_handler(void * arg, int8_t * buf, int32_t len);
    extern int32_t at_get_unuse_linkid();
    extern int32_t at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf);
    
    extern void* at_malloc(size_t size);
    extern void at_free(void* ptr);

    extern UINT32 LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
                            UINT32 uwFlags,UINT16 usMaxMsgSize );
    extern UINT32 LOS_QueueWriteCopy( UINT32 uwQueueID,
                                         VOID * pBufferAddr,
                                         UINT32 uwBufferSize,
                                         UINT32 uwTimeOut );
    extern UINT32 LOS_QueueReadCopy(UINT32 uwQueueID,
                                    VOID *pBufferAddr,
                                    UINT32 *puwBufferSize,
                                    UINT32 uwTimeOut);				    

    static UINT32 stub_LOS_QueueWriteCopy( UINT32 uwQueueID,
                                     VOID * pBufferAddr,
                                     UINT32 uwBufferSize,
                                     UINT32 uwTimeOut )
    {
        if(-1 == QUEUE_WRITE){
            return 1;
        }
    	if(pBufferAddr != NULL)
    	{
    		QUEUE_BUFF *queuebuf = (QUEUE_BUFF*)pBufferAddr;
    		if(queuebuf->addr != NULL)
    		   at_free(queuebuf->addr);
    	}
    	return 0;
    }

    static UINT32 stub_LOS_QueueReadCopy(UINT32  uwQueueID,
                        VOID *  pBufferAddr,
                        UINT32 * puwBufferSize,
                        UINT32  uwTimeOut)
    {
    	uint8_t str[] = "abcd";
    	QUEUE_BUFF queue_buf = {0,NULL};
    	queue_buf.addr = (uint8_t*)at_malloc(sizeof(str));
    	if(NULL == queue_buf.addr) return -1;
    	memcpy(queue_buf.addr, str,sizeof(str));
    	queue_buf.len = strlen((char *)str);
    	memcpy(queue_buf.addr, str,sizeof(str));
    	memcpy(pBufferAddr,&queue_buf,sizeof(QUEUE_BUFF));
    	return 0;
    }

    static UINT32 stub_LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
            UINT32 uwFlags,UINT16 usMaxMsgSize )
    {
        return 1;
    }                        

    static int32_t stub_at_get_unuse_linkid()
    {
        return -1;
    }
    static int32_t stub_at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf)
    {
        CMD_TIMES++;
        if(CMD_TIMES > 2)
            return 1;
        else
            return -1;

    }
    static void* stub_at_malloc(size_t size)
    {
        return NULL;
    }

}						

void TestSim900a::test_sim900a_echo_off()
{
   int32_t ret  = -1;
   ret = sim900a_echo_off();
   TEST_ASSERT_MSG(( ret != -1), "Test sim900a_echo_off() Failed");
}

void TestSim900a::test_sim900a_ini()
{  
   //single connect
   int ret = -1;
   at_user_conf.mux_mode = 0;
   sim900a_deinit();
   ret = sim900a_ini();
   TEST_ASSERT_MSG(( ret == AT_OK), "Test sim900a_ini() Failed");
   at_user_conf.mux_mode = 1;
}

void TestSim900a::test_sim900a_connect()
{
   int32_t ret  = -1;
   const int8_t host[20] = "192.168.1.101";
   const int8_t port[10] = "5683";
   int proto = 0;
   ret = sim900a_connect(host,port,proto);
   TEST_ASSERT_MSG(( ret != -1), "Test sim900a_connect() Failed");
   
   stubInfo stub_info;
   setStub((void *)LOS_QueueCreate,(void *)stub_LOS_QueueCreate,&stub_info);
   ret = sim900a_connect(host,port,proto);
   TEST_ASSERT_MSG(( ret == -1), "Test sim900a_connect() Failed");
   cleanStub(&stub_info);
   
   setStub((void *)at_get_unuse_linkid,(void *)stub_at_get_unuse_linkid,&stub_info);
   ret = sim900a_connect(host,port,proto);
   TEST_ASSERT_MSG(( ret == -1), "Test sim900a_connect() Failed");
   cleanStub(&stub_info);
   
   //single connect
   at.mux_mode = 1;
   ret = sim900a_connect(host,port,proto);
   TEST_ASSERT_MSG(( ret != -1), "Test sim900a_connect() Failed");
}

void TestSim900a::test_sim900a_recv()
{
   int32_t ret  = -1;
   int32_t id = 0;
   int8_t  buf[100] = {0};
   uint32_t len = sizeof((const char *)buf);
   stubInfo stu_queue;
   setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy,&stu_queue);
   ret = sim900a_recv(id,buf,len);
   cleanStub(&stu_queue);
   TEST_ASSERT_MSG(( ret != 0), "Test sim900a_recv() Failed");
}

void TestSim900a::test_sim900a_recv_timeout()
{
   int32_t ret  = -1;
   int32_t id = 0;
   int8_t  buf[100] = {0};
   uint32_t len = sizeof((const char *)buf);
   stubInfo stu_queue;
   setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy,&stu_queue);
   ret = sim900a_recv_timeout(id,buf,len,0xFF);
   cleanStub(&stu_queue);
   TEST_ASSERT_MSG(( ret != 0), "Test sim900a_recv_timeout() Failed");
}

void TestSim900a::test_sim900a_send()
{
   int32_t ret  = -1;
   int32_t id = 0;
   const uint8_t  buf[100] = "helloworld";
   uint32_t len = strlen((const char *)buf);
   ret = sim900a_send(id, buf, len);
   TEST_ASSERT_MSG(( ret == len), "Test sim900a_send() Failed");
   
   at.mux_mode = 0;
   ret = sim900a_send(id, buf, len);
   TEST_ASSERT_MSG(( ret == len), "Test sim900a_send() Failed");
   at.mux_mode = 1;
}

void TestSim900a::test_sim900a_check()
{
   //abnormal
   stubInfo stub_cmd;
   setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_cmd);
   sim900a_check();
   TEST_ASSERT_MSG(1, "Test sim900a_sim900a_check() Failed");
   cleanStub(&stub_cmd);
}

void TestSim900a::test_sim900a_recv_cb()
{
   int32_t ret  = -1;
   int32_t id = 0;
   ret = sim900a_recv_cb(id);
   TEST_ASSERT_MSG(( ret == -1), "Test sim900a_recv_cb() Failed");
}

void TestSim900a::test_sim900a_deinit()
{
   int32_t ret  = -1;
   ret = sim900a_deinit();
   TEST_ASSERT_MSG(( ret == AT_OK), "Test sim900a_deinit) Failed");
}

void TestSim900a::test_sim900a_close()
{
   int32_t ret  = -1;
   int32_t id = 0;
   ret = sim900a_close(id);
   TEST_ASSERT_MSG(( ret != -1), "Test sim900a_close() Failed");
   
   at.mux_mode = 0;
   ret = sim900a_close(id);
   TEST_ASSERT_MSG(( ret != -1), "Test sim900a_close() Failed");
   at.mux_mode = 1;
}

void TestSim900a::test_sim900a_data_handler()
{
   int32_t ret  = -1;
   int32_t id = 0;
   int8_t  buf[100] = "\r\n+RECEIVE,1,13:\r\ndAbcdefghabcd";
   uint32_t len = strlen((char *)buf);
   ret = sim900a_data_handler(NULL, NULL, len);
   TEST_ASSERT_MSG(( ret == -1), "Test1 sim900a_data_handler() Failed");
   
   ret = sim900a_data_handler(NULL, buf, 0);
   TEST_ASSERT_MSG(( ret == -1), "Test2 sim900a_data_handler() Failed");

   sim900a_deinit();
   sim900a_ini();
   stubInfo stub_queue;
   setStub((void *)LOS_QueueWriteCopy,(void *)stub_LOS_QueueWriteCopy,&stub_queue);
   ret = sim900a_data_handler(NULL, buf, len);
   TEST_ASSERT_MSG(( ret == len), "Test3 sim900a_data_handler() Failed");
   
   //test unexpected branch
   QUEUE_WRITE = -1;
   ret = sim900a_data_handler(NULL, buf, len);
   TEST_ASSERT_MSG(( ret == 1), "Test4 sim900a_data_handler() Failed");
   QUEUE_WRITE = 0;
   cleanStub(&stub_queue);
   
//   //abnormal
   stubInfo stub_malloc;
   setStub((void *)at_malloc,(void *)stub_at_malloc,&stub_malloc);
   ret = sim900a_data_handler(NULL, buf, len);
   TEST_ASSERT_MSG(ret == 0, "Test5 sim900a_data_handler() Failed");
   cleanStub(&stub_malloc);
   
   int8_t buf_err[100] = "\r\n+RECEIVE:1:13:\r\ndAbcdefghabcd";
   ret = sim900a_data_handler(NULL, buf_err, strlen((char *)buf_err));
   TEST_ASSERT_MSG(( ret == 0), "Test6 sim900a_data_handler() Failed");
}

TestSim900a::TestSim900a()
{
   TEST_ADD(TestSim900a::test_sim900a_echo_off);
   TEST_ADD(TestSim900a::test_sim900a_ini);
   TEST_ADD(TestSim900a::test_sim900a_connect);
   TEST_ADD(TestSim900a::test_sim900a_recv);
   TEST_ADD(TestSim900a::test_sim900a_recv_timeout);
   TEST_ADD(TestSim900a::test_sim900a_send);
   TEST_ADD(TestSim900a::test_sim900a_check);//enter while(1) cycle forever
   TEST_ADD(TestSim900a::test_sim900a_recv_cb);
   TEST_ADD(TestSim900a::test_sim900a_deinit);
   TEST_ADD(TestSim900a::test_sim900a_close);
   TEST_ADD(TestSim900a::test_sim900a_data_handler);
}

TestSim900a::~TestSim900a()
{
    test_sim900a_deinit();
}


//protected:
void TestSim900a::setup()
{
}

void TestSim900a::tear_down()
{
}

