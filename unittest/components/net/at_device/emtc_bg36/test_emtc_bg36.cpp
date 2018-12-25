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
#include "test_emtc_bg36.h"

#include <pthread.h>

#include <stdlib.h>
/* Defines ------------------------------------------------------------------*/


/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
	#include "at_frame/at_main.h"
    #include "bg36.h"
	
	extern at_adaptor_api emtc_bg36_interface;
	extern at_oob_t at_oob;
	//extern at_task at;//at_main.h
    // in at adapter.c
    extern int32_t at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf);
    extern int32_t at_write(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len);
    //extern int32_t at_get_unuse_linkid();
	extern void at_init(at_config *config);
	extern void at_deinit();
	extern int32_t at_struct_init(at_task *at);
	extern int32_t at_struct_deinit(at_task *at);
	extern int bg36_cmd(char *cmd, int32_t len, const char *suffix, char *resp_buf, int* resp_len);
	extern int32_t bg36_data_handler(void *arg, int8_t *buf, int32_t len);
	extern int32_t bg36_cmd_match(const char *buf, char* featurestr,int len);
	extern int32_t at_get_unuse_linkid(void);
	extern void *at_malloc(size_t size);
	extern emtc_socket_info sockinfo[11];


}
/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/
void stub_at_init(at_config *config)
{
	//printf("from at_init to stub_at_init\n");
	return;
}
/*
//int sscanf (const char *str,const char * format,........);     
//Specify return value and modify CREG value
int stub_sscanf(const char * inbuf,const char * BBB,int* creg,char* )
{
	static int s=-1;
	//if(s++==-1)return -1;
	*creg=s;
	printf("stub_sscanf creg==%d\n",s);
	//while(1);
	return s++;
}*/

//(void)bg36_cmd(QUERYCFATT, strlen(QUERYCFATT), "+CGATT", inbuf,&rbuflen);
//change inbuf
int stub_bg36_cmd(char *cmd, int32_t len, const char *suffix, char *resp_buf, int* resp_len)
{
	static unsigned char a=0;//1~255
	//printf("\nstub_bg36_cmd a=%d\n",a);
	if(a>=100)return 0;
	a++;
			//1 single order
			//2 single order
			//3 single order
	
			//4 Rough Walk Through the Cycle
				
	if(a==5)//5 error instruction // Want to enter ret=-1 branch
	{
		char test[]="\r\n+CGATT: 0\r\nKshine";
		stpcpy(resp_buf,test);
		//printf("stub_bg36_cmd:%d change inbuf\n",a);
	}
	if(a==6)//Sixth correct instruction // Hope to jump out of the loop
	{
		char test[]="\r\n+CGATT: 1\r\ntmpbuf";
		stpcpy(resp_buf,test);
		//printf("stub_bg36_cmd:%d change inbuf\n",a);
		//a=100;
		sockinfo[0].buf=(char*)at_malloc(10*sizeof(char));//in close ,enter a branch to free space 
	
	}
	
	if(a==33) //5+28
	{
		char test[]="\r\n+CGATT: 0\r\nKshine";
		stpcpy(resp_buf,test);
		//printf("stub_bg36_cmd:%d change inbuf\n",a);
	}
	if(a==28)
	{
		return AT_FAILED;//Enter a branch
	}

	
	return 0;
}

int stub_bg36_cmd_return_1(char *cmd, int32_t len, const char *suffix, char *resp_buf, int* resp_len)
{
	static int cc=0;
	static int ccc=1;
	static int cccc=2;
	cc++;//   0	1    2  3	  4  5   6   7
	    //	  1	1    3	3	  6	 6	 6   10
	if(cc==ccc)
	{
		ccc+=cccc;
		cccc++;
		return 1;
	}
	return 0;
}
int stub_bg36_cmd_qq(char *cmd, int32_t len, const char *suffix, char *resp_buf, int* resp_len)
{
	static int qq=0;
	qq++;
	
	if(qq==1)
	{
		//"AT+QIOPEN=1,id,\"TCP\",\"host\",port,0,1\r+QIOPEN: 0,0,tmpbuf"
		memset(resp_buf,0,64);strcpy(resp_buf,"0,1\r+QIOPEN: 0,0,tmpbuf");
		return AT_OK;
	}
	
	if(qq==3)
	{
	//"AT+QIOPEN=1,id,\"TCP\",\"host\",port,0,1\r+QIOPEN: 1,0,tmpbuf"
		memset(resp_buf,0,64);strcpy(resp_buf,"0,1\r+QIOPEN: 1,0,tmpbuf");
		return AT_OK;
	}
	if(qq==5)
	{
		//"AT+QIOPEN=1,id,\"TCP\",\"host\",port,0,1\r+QIOPEN: 0,1,tmpbuf"
		memset(resp_buf,0,64);strcpy(resp_buf,"0,1\r+QIOPEN: 0,1,tmpbuf");
		return AT_OK;
	}	
	if(qq==7)
	{
		//"AT+QIOPEN=1,id,\"TCP\",\"host\",port,0,1\r+QIOPEN: 0,1,tmpbuf"
		memset(resp_buf,0,64);strcpy(resp_buf,"0,1\r+QIOPEN: 1,1,tmpbuf");
		return AT_OK;
	}
	
	
	if(qq==9)
	{
		//"AT+QIOPEN=1,id,\"TCP\",\"host\",port,0,1\r+QIOPEN: 0,0,tmpbuf"
		memset(resp_buf,0,64);strcpy(resp_buf,"0,1\r+QIOPEN: 0,0,tmpbuf");
		return AT_OK;
	}
	
	
	
	
	return AT_OK;
	
	
}


//LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, 0);
LITE_OS_SEC_TEXT UINT32 stub_LOS_QueueReadCopy(UINT32  uwQueueID,
                    VOID *  pBufferAddr,
                    UINT32 * puwBufferSize,
                    UINT32  uwTimeOut)
{
	//qbuf.addr != NULL ,QUEUE_BUFF  qbuf = {0};
	/*
	typedef struct {
    uint32_t len;
    uint8_t *addr;
    char ipaddr[MAXIPLEN];
    int port;
	}QUEUE_BUFF;
	

int *p;
p = (int*)malloc(sizeof(int) * 128);
	*/
	
	static int ll=0;

	
	if(ll==0)
	{
		ll++;
	QUEUE_BUFF* qbuf=(QUEUE_BUFF*)pBufferAddr;
	if(qbuf->addr==NULL)
	{
		unsigned int num_bytes = sizeof("192.168.111.111");
		uint8_t *ADDR=(uint8_t *)malloc(sizeof(uint8_t)*16);
		//printf("sizeof = %d  should be= %d",num_bytes,sizeof(uint8_t)*16);// 16
		memset(ADDR,0,num_bytes);
		//stpcpy(ADDR,"192.168.111.111");
		qbuf->addr=ADDR;
	}
	
		
		return LOS_OK;
	}
	else
		return LOS_NOK;
}



//LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, 0);
LITE_OS_SEC_TEXT UINT32 stub_LOS_QueueReadCopy_return_False(UINT32  uwQueueID,
                    VOID *  pBufferAddr,
                    UINT32 * puwBufferSize,
                    UINT32  uwTimeOut)
{
	
		return LOS_NOK;
}
LITE_OS_SEC_TEXT UINT32 stub_LOS_QueueReadCopy_return_OK(UINT32  uwQueueID,
                    VOID *  pBufferAddr,
                    UINT32 * puwBufferSize,
                    UINT32  uwTimeOut)
{
		QUEUE_BUFF* qbuf=(QUEUE_BUFF*)pBufferAddr;
		qbuf->len=2;
		char* bb=(char*)at_malloc(2*sizeof(char));
		bb[0]='A';
		bb[1]='B';
		qbuf->addr=(uint8_t*)bb;
		return LOS_OK;
}

UINT32 stub_LOS_QueueDelete(UINT32 uwQueueID)
{
	static int qd=0;
	qd++;
	if(qd==1)return LOS_NOK;
	return LOS_OK;
}

int32_t stub_at_get_11(void)
{
	
	return 11;
}

LITE_OS_SEC_TEXT_INIT UINT32 stub_LOS_QueueCreate(CHAR *pcQueueName,
                                          UINT16 usLen,
                                          UINT32 *puwQueueID,
                                          UINT32 uwFlags,
                                          UINT16 usMaxMsgSize )
{
	
	return LOS_NOK;
}

void *stub_at_malloc_return_NULL(size_t size)
{
	return NULL;
}

UINT32 stub_LOS_QueueWriteCopy( UINT32 uwQueueID,VOID * pBufferAddr,
                             UINT32 uwBufferSize, UINT32 uwTimeOut )
{
    return LOS_NOK;
}


/* Public functions ---------------------------------------------------------*/

// Because there is a conditional dead loop (jump-out conditions are hard to give), stop the thread after passing the thread test and delaying for a moment (to cover the branch)
	void * pthread_bg36_init(void *para)
	{
		stubInfo stub_info;
		setStub((void *)at_init, (void *)stub_at_init, &stub_info);
		emtc_bg36_interface.init();
		cleanStub(&stub_info);
	}


Test_EMTC_BG36::Test_EMTC_BG36()
{

	
	TEST_ADD(Test_EMTC_BG36::test_bg36_init);
	TEST_ADD(Test_EMTC_BG36::test_bg36_bind);
	TEST_ADD(Test_EMTC_BG36::test_bg36_connect);
	TEST_ADD(Test_EMTC_BG36::test_bg36_send);
	TEST_ADD(Test_EMTC_BG36::test_bg36_recv);
	TEST_ADD(Test_EMTC_BG36::test_bg36_data_handler);
	TEST_ADD(Test_EMTC_BG36::test_bg36_cmd_match);
	
	
}

Test_EMTC_BG36::~Test_EMTC_BG36()
{
	//at_struct_deinit(&at);
}


void Test_EMTC_BG36::test_bg36_init()
{
	at_struct_init(&at);
#if 0
//There is a conditional dead loop, using thread testing
	int ret;
	pthread_t first_thread;
	ret = pthread_create(&first_thread, NULL, pthread_bg36_init, NULL);
	TEST_ASSERT_MSG((0 == ret), "bg36_init(...) failed");
	usleep(6000);//Let the function run for a while
    pthread_cancel(first_thread);//Stop thread
    pthread_join(first_thread, NULL);//Waiting for the end of a thread
#else
	//printf("test_bg36_init start\n");
	stubInfo stub_info;
	stubInfo stub_qrc;
	stubInfo stub_cmd;
	stubInfo stub_qd;
	setStub((void *)at_init, (void *)stub_at_init, &stub_info);
	//setStub((void *)sscanf, (void *)stub_sscanf, &stub_scan);
	setStub((void *)bg36_cmd, (void *)stub_bg36_cmd, &stub_cmd);
	setStub((void *)LOS_QueueReadCopy, (void *)stub_LOS_QueueReadCopy, &stub_qrc);//return LOS_OK
	setStub((void *)LOS_QueueDelete, (void *)stub_LOS_QueueDelete,&stub_qd);
	//1

	emtc_bg36_interface.init();
	
	cleanStub(&stub_info);
	cleanStub(&stub_qrc);
	cleanStub(&stub_cmd);
	cleanStub(&stub_qd);
	
	printf("[success]test_bg36_init is ok\n\n");
	//usleep(6000);
	
#endif
	
}

void Test_EMTC_BG36::test_bg36_bind()
{
	//printf("test_bg36_bind start\n");
	const char ipaddr[]="192.168.1.1";
	const char port[]="8181";
	emtc_bg36_interface.bind((const int8_t *)ipaddr, (const int8_t *)port, 1);
	printf("[success]test_bg36_bind is ok\n\n");
}


void Test_EMTC_BG36::test_bg36_connect()
{
	//int32_t bg36_connect(const int8_t * host, const int8_t *port, int32_t proto)
	const char ipaddr[]="192.168.1.1";
	const char port[]="8282";
	//1
	emtc_bg36_interface.connect((const int8_t *)ipaddr, (const int8_t *)port, 1);
	//2
	at.mux_mode = AT_MUXMODE_MULTI;
	emtc_bg36_interface.connect((const int8_t *)ipaddr, (const int8_t *)port, 1);
	//3 at.get_id()
	stubInfo stub_id;
	setStub((void *)at_get_unuse_linkid, (void *)stub_at_get_11, &stub_id);
	at.mux_mode = AT_MUXMODE_MULTI;
	emtc_bg36_interface.connect((const int8_t *)ipaddr, (const int8_t *)port, 1);
	cleanStub(&stub_id);
	//4
	stubInfo stub_cmd;
	setStub((void *)bg36_cmd, (void *)stub_bg36_cmd_qq, &stub_cmd);//change inbuf
	at.mux_mode = AT_MUXMODE_MULTI;//id=0
	
	
	emtc_bg36_interface.connect((const int8_t *)ipaddr, (const int8_t *)port, 1);
	emtc_bg36_interface.connect((const int8_t *)ipaddr, (const int8_t *)port, 1);
	emtc_bg36_interface.connect((const int8_t *)ipaddr, (const int8_t *)port, 1);
	emtc_bg36_interface.connect((const int8_t *)ipaddr, (const int8_t *)port, 1);
	stubInfo stub_lqc;
	setStub((void *)LOS_QueueCreate, (void *)stub_LOS_QueueCreate, &stub_lqc);
	emtc_bg36_interface.connect((const int8_t *)ipaddr, (const int8_t *)port, 1);
	cleanStub(&stub_lqc);
	cleanStub(&stub_cmd);
	printf("[success]test_bg36_connect is ok\n\n");
}

void Test_EMTC_BG36::test_bg36_send()
{
	//int32_t bg36_send(int32_t id , const uint8_t *buf, uint32_t len)
	char buf[]="test_bg36_send test";
	//1
	emtc_bg36_interface.send(1,(uint8_t*)buf,strlen(buf));
	
	//2
	emtc_bg36_interface.send(-1,(uint8_t*)buf,strlen(buf));
	//3
	emtc_bg36_interface.send(11,(uint8_t*)buf,strlen(buf));
	//4
	emtc_bg36_interface.send(1,(uint8_t*)buf,1401);
	//5 stub_bg36_cmd_return_1
	stubInfo stub_cmd;
	setStub((void *)bg36_cmd, (void *)stub_bg36_cmd_return_1, &stub_cmd);
	emtc_bg36_interface.send(1,(uint8_t*)buf,strlen(buf));
	emtc_bg36_interface.send(1,(uint8_t*)buf,strlen(buf));
	cleanStub(&stub_cmd);
	
	
	
	
	
	printf("[success]test_bg36_send is ok\n\n");
}

void Test_EMTC_BG36::test_bg36_recv()
{
	//First, sockinfo has been defined
	/*
	typedef struct emtc_socket_info_t
	{
		int len;
		int offset;
		char *buf;
		bool used_flag;
	}emtc_socket_info;
	emtc_socket_info sockinfo[11];//here define it
	*/
	char buf[]="test_bg36_recv test";
	sockinfo[1].len=strlen(buf);
	sockinfo[1].offset=0;
	//1 if (id  >= MAX_BG36_SOCK_NUM) not in
	printf("[1]");emtc_bg36_interface.recv(1,(uint8_t*)buf,strlen(buf));
	//2 if (id  >= MAX_BG36_SOCK_NUM) in
	printf("[2]");emtc_bg36_interface.recv(11,(uint8_t*)buf,strlen(buf));
	//3 if (sockinfo[id].buf == NULL) not in  and  if (ret != LOS_OK) in
	stubInfo stub_qrc;
	setStub((void *)LOS_QueueReadCopy, (void *)stub_LOS_QueueReadCopy_return_False, &stub_qrc);//return LOS_NOK
	//if (sockinfo[1].buf != NULL)at_free(sockinfo[1].buf);
	 sockinfo[1].buf=NULL;
	printf("[3]");emtc_bg36_interface.recv(1,(uint8_t*)buf,strlen(buf));
	cleanStub(&stub_qrc);
	//4 -------------------------------------------------
	char buf2[64]="test_bg36_recv test";
	sockinfo[1].buf = (char*)at_malloc(strlen(buf2)*sizeof(char));
	memset(sockinfo[1].buf,0,strlen(buf2));
	memcpy(sockinfo[1].buf,buf2,strlen(buf2));
	sockinfo[1].len=strlen(buf2);//strlen(sockinfo[1].buf);//
	sockinfo[1].offset=1;
	printf("[4]");emtc_bg36_interface.recv(1,(uint8_t*)buf2,strlen(buf2));
	
	
	//5------------------------------------------------
	//if(sockinfo[1].buf==NULL)
	sockinfo[1].buf = (char*)at_malloc(strlen(buf2)*sizeof(char));
	memset(sockinfo[1].buf,0,strlen(buf2));
	memcpy(sockinfo[1].buf,buf2,strlen(buf2));
	sockinfo[1].len=strlen(buf2);//strlen(sockinfo[1].buf);//
	sockinfo[1].offset=1;
	setStub((void *)LOS_QueueReadCopy, (void *)stub_LOS_QueueReadCopy_return_False, &stub_qrc);//return LOS_NOK
	printf("[5]");emtc_bg36_interface.recv(1,(uint8_t*)buf2,strlen(buf2));
	cleanStub(&stub_qrc);
	
	//6-------------------------------------------------
	//if(sockinfo[1].buf==NULL)
	char buf3[64]="test is good !";
	sockinfo[1].buf = (char*)at_malloc(strlen(buf3)*sizeof(char));
	memset(sockinfo[1].buf,0,strlen(buf3));
	memcpy(sockinfo[1].buf,buf3,strlen(buf3));
	sockinfo[1].len=strlen(buf3);//strlen(sockinfo[1].buf);//
	sockinfo[1].offset=2;
	
	setStub((void *)LOS_QueueReadCopy, (void *)stub_LOS_QueueReadCopy_return_OK, &stub_qrc);
	printf("[6]");emtc_bg36_interface.recv(1,(uint8_t*)buf3,strlen(buf3));
	cleanStub(&stub_qrc);
	printf("buf3 = %s",buf3);
	printf("[success]test_bg36_recv is ok\n\n");
}


void Test_EMTC_BG36::test_bg36_data_handler()
{
	//int32_t bg36_data_handler(void *arg, int8_t *buf, int32_t len)
	/*
	arg unused
	*/

	char sss[64]="";
	
	memset(sss,0,64);stpcpy(sss,"");		//while(offset < len) not in
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	
	memset(sss,0,64);stpcpy(sss,"ABCD");	// while(offset < len) in
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	
	//if(sockid >= MAX_BG36_SOCK_NUM || sockinfo[sockid].used_flag == false)

	//++
	memset(sss,0,64);stpcpy(sss,"recv01234567");	// while(offset < len ) in,pass “recv”,
	sockinfo[1].used_flag = false;
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	//+-
	memset(sss,0,64);stpcpy(sss,"recv01234567");
	sockinfo[1].used_flag = true;
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	//-+
	memset(sss,0,64);stpcpy(sss,"recv0121");
	sockinfo[1].used_flag = false;
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	//--
	memset(sss,0,64);stpcpy(sss,"recv0121");
	sockinfo[1].used_flag = true;
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	
	// 0< data_len <= AT_DATA_LEN*2  //have \r\n
	memset(sss,0,64);stpcpy(sss,"recv0121,14\r\n");
	sockinfo[1].used_flag = true;
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	// data_len > AT_DATA_LEN*2
	memset(sss,0,64);stpcpy(sss,"recv0121,2400");
	sockinfo[1].used_flag = true;
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	//  data_len <=0
	memset(sss,0,64);stpcpy(sss,"recv0121,0");
	sockinfo[1].used_flag = true;
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	//do not have \r\n 
	memset(sss,0,64);stpcpy(sss,"recv0121,14");
	sockinfo[1].used_flag = true;
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	
	//qbuf.addr == NULL
	stubInfo stub_malloc;
	setStub((void*)at_malloc,(void*)stub_at_malloc_return_NULL,&stub_malloc);
	memset(sss,0,64);stpcpy(sss,"recv0121,14");
	sockinfo[1].used_flag = true;
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	cleanStub(&stub_malloc);
	
	//stub_LOS_QueueWriteCopy
	stubInfo stub_lqwc;
	setStub((void*)LOS_QueueWriteCopy,(void*)stub_LOS_QueueWriteCopy,&stub_lqwc);
	memset(sss,0,64);stpcpy(sss,"recv0121,14\r\n");
	sockinfo[1].used_flag = true;
	bg36_data_handler((void*)NULL,(int8_t*)sss,(int32_t)strlen(sss));
	cleanStub(&stub_lqwc);
}
	
	
void Test_EMTC_BG36::test_bg36_cmd_match()
{
	char featurestr[]="bcd";
	bg36_cmd_match("abcdefg",featurestr,0);
	bg36_cmd_match("1234567",featurestr,0);	
}
	
/* Private functions --------------------------------------------------------*/

