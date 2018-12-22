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
#include "test_bc95.h"
/* Defines ------------------------------------------------------------------*/
#define TEST_STATE_OK   0
#define TEST_STATE_ERR  1
#define MAX_SOCK_NUM    5

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
char g_state = TEST_STATE_OK;
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
extern "C"
{

    #include "sal/atiny_socket.h"
    #include "osdepends/atiny_osdep.h"
	//  #include "atadapter.h"
    #include "at_device/bc95.h"
	

    extern at_task at;
	extern socket_info sockinfo[MAX_SOCK_NUM];
	extern at_adaptor_api bc95_interface;
    // in atadapter.c
    extern int32_t at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf);
    extern int32_t at_write(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len);
    extern int32_t at_get_unuse_linkid();
	extern void at_init();
	extern void at_deinit();

    extern char *strnstr(const char *s1, const char *s2, size_t len);
	extern void HexStrToStr(char *source, unsigned char *dest, int sourceLen);
	extern int32_t nb_check_csq(void);
    extern int32_t nb_create_udpsock(const int8_t * host, int port, int32_t proto);
	extern int32_t nb_create_sock(int port,int proto);
	extern int nb_decompose_str(char* str, int *readleft, int *out_sockid);
	extern void nb_reattach(void);
	extern int32_t nb_bind(int8_t * host, int8_t *port, int32_t proto);
    extern int32_t nb_connect(const int8_t * host, const int8_t *port, int32_t proto);
    extern int32_t nb_send(int32_t id , const uint8_t  *buf, uint32_t len);
	extern int32_t nb_set_no_encrypt(void);
    extern int32_t nb_recv(int32_t id , uint8_t  *buf, uint32_t len);
	extern int32_t nb_recvfrom(int32_t id , uint8_t  *buf, uint32_t len,char* ipaddr,int* port);
    extern int32_t nb_recv_cb(int32_t id);
    extern int32_t nb_deinit(void);
    extern int32_t nb_close(int32_t socket);
	extern int32_t at_cmd_multi_suffix(const int8_t *cmd, int  len, at_cmd_info_s *cmd_info);
	extern void nb_step(void);
   // extern int32_t nb_send_str(const char* buf, int len);
    extern int32_t nb_data_ioctl(void* arg,int8_t * buf, int32_t len);
    //extern int32_t nb_cmd_match(const char *buf, char* featurestr,int len);
    
    // in liteos queue
    extern UINT32 LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
                            UINT32 uwFlags,UINT16 usMaxMsgSize );
//    extern UINT32 LOS_QueueWriteCopy( UINT32 uwQueueID, VOID * pBufferAddr,
//                            UINT32 uwBufferSize, UINT32 uwTimeOut );
    extern UINT32 LOS_QueueReadCopy(UINT32  uwQueueID, VOID *  pBufferAddr,
                            UINT32 * puwBufferSize, UINT32  uwTimeOut);
    // in atadapter.c
    extern void* at_malloc(size_t size);
    
    // stubs
    UINT32 stub_LOS_QueueCreate_fail(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
                            UINT32 uwFlags,UINT16 usMaxMsgSize )
    {
		return 1;
    }
	
    char *g_buf = (char*)"\r14,22,+abc , 44";
    uint32_t g_len  = strlen(g_buf)+1;
    UINT32 stub_LOS_QueueReadCopy(UINT32  uwQueueID, VOID *  pBufferAddr,
                            UINT32 * puwBufferSize, UINT32  uwTimeOut)
    {
		QUEUE_BUFF	*qbuf = (QUEUE_BUFF *)pBufferAddr;
		qbuf->len  = g_len;
		qbuf->addr = (uint8_t *)at_malloc(g_len);
		if(qbuf->addr != NULL){
		    memcpy(qbuf->addr,(uint8_t *)g_buf,g_len);
		    return 0;
		}
		else
		{
			return 1;
		}
		
    }
	UINT32 stub_LOS_QueueReadCopy1(UINT32  uwQueueID, VOID *  pBufferAddr,
                            UINT32 * puwBufferSize, UINT32  uwTimeOut)
    {
		QUEUE_BUFF	*qbuf = (QUEUE_BUFF *)pBufferAddr;
		qbuf->len  = g_len;

		qbuf->addr = (uint8_t *)at_malloc(g_len);
		if(qbuf->addr != NULL){
		    memcpy(qbuf->addr,(uint8_t *)g_buf,g_len);
		}

		//qbuf->ipaddr = (char *)at_malloc(g_len);
		if(qbuf->ipaddr != NULL){
		    memcpy(qbuf->ipaddr,"192.168.0.000",g_len);
		    return 0;
		}
		else
		{
			return 1;
		}
		
    }						
	
	UINT32 stub_LOS_QueueReadCopy_fail(UINT32  uwQueueID, VOID *  pBufferAddr,
                            UINT32 * puwBufferSize, UINT32  uwTimeOut)
    {
		return 1;
    }
	
	int32_t stub_nb_create_udpsock_max(const int8_t * host, int port, int32_t proto)
	{
		return MAX_SOCK_NUM;
	}

    int32_t stub_nb_create_sock(int port,int proto)
	{
        return 1;
    }

	int stub_nb_sock_to_idx(int socket)
    {
        return 5;
    }
    static int32_t stub_at_cmd(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf)
    {
        char *tmp = NULL;

        if(g_state == TEST_STATE_ERR){
            return AT_FAILED;
        }
        if(rep_buf != NULL)
			memcpy(rep_buf,"SENT=2,\"1 2 3\"",sizeof("SENT=2,\"1 2 3\""));
        return AT_OK;
    }
	
	static int32_t stub_at_cmd2(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf)
    {
        char *tmp = NULL;
        
        if(g_state == TEST_STATE_ERR){
            return AT_FAILED;
        }
        if(rep_buf != NULL)
        {
			memcpy(rep_buf,"1\r 1 2 3 OK",sizeof("1\r 1 2 3 OK"));
        }
        return AT_OK;
    }
	
    static int32_t stub_at_cmd3(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf)
    {
        char *tmp = NULL;

        if(g_state == TEST_STATE_ERR){
            return AT_FAILED;
        }
        if(rep_buf != NULL)
			memcpy(rep_buf,"-1\r 1 2 3 OK",sizeof("-1\r 1 2 3 OK"));
        return AT_OK;
    }
	
    static int32_t stub_at_write(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len)
    {
        return AT_FAILED;
    }

    static int32_t stub_at_get_unuse_linkid()
    {
        return -1;
    }
	static int32_t stub_at_cmd_in_callback1(const int8_t *cmd, int32_t len,
                    int32_t (*handle_data)(const int8_t *data, uint32_t len),  uint32_t timeout)
    {
        return 1;
	}
    static int32_t stub_at_cmd_in_callback2(const int8_t *cmd, int32_t len,
                    int32_t (*handle_data)(const int8_t *data, uint32_t len),  uint32_t timeout)
    {
        return 0;
	}
	static	int32_t	stub_at_cmd_multi_suffix(const int8_t *cmd, int  len, at_cmd_info_s *cmd_info)
    {
        cmd_info->match_idx=1;
		memcpy(cmd_info->resp_buf,"1\rOK",strlen("1\rOK")+1);
		
		return AT_OK;
	}

	static	int32_t stub_at_cmd_multi_suffix1(const int8_t *cmd, int  len, at_cmd_info_s *cmd_info)
	{
        cmd_info->match_idx=1;
		
        return AT_OK;
	}

	int stub_nb_decompose_str(const char* str, int *readleft, int *out_sockid)
	{
	    return 1;
	}

	int stub_nb_decompose_strok(const char* str, int *readleft, int *out_sockid)
	{
		return 0;
	}

	static int32_t stub_at_cmd_in_callback(const int8_t *cmd, int32_t len,
                    int32_t (*handle_data)(const int8_t *data, uint32_t len),  uint32_t timeout)
	{
		int8_t datatest[20]="ERROR";
		int8_t datatest2[20]="\r\n123";
		uint32_t length=3;
		
		stubInfo stub_decom;
		stubInfo stub_decomok;

		handle_data(datatest,length);

		setStub((void *)nb_decompose_str,(void *)stub_nb_decompose_str,&stub_decom);
		handle_data(datatest2,length);

		cleanStub(&stub_decom);
		
		setStub((void *)nb_decompose_str,(void *)stub_nb_decompose_strok,&stub_decomok);
		handle_data(datatest2,length);

		cleanStub(&stub_decomok);

		return 0;
	}

    static UINT32 stub_LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID,
            UINT32 uwFlags,UINT16 usMaxMsgSize )
    {
        return 1;
    }

    static UINT32 stub_LOS_QueueWriteCopy( UINT32 uwQueueID, VOID * pBufferAddr,
                     UINT32 uwBufferSize, UINT32 uwTimeOut )
    {
        if(g_state == TEST_STATE_ERR){
            return 1;
        }
        // simulate memory release
        QUEUE_BUFF *pqbuf = (QUEUE_BUFF *)pBufferAddr;
        if(pqbuf && pqbuf->addr){
            at_free(pqbuf->addr);
        }
        return 0;
    }
	static UINT32 stub_LOS_QueueWriteCopy1( UINT32 uwQueueID, VOID * pBufferAddr,
                     UINT32 uwBufferSize, UINT32 uwTimeOut )
    {
        return 1;
    }			 

    static void* stub_at_malloc(size_t size)
    {
        return NULL;
    }

	static int nb_send_str(const char* buf, int len)
    {
        return 0;
    }
#if 0
    static UINT32 stub_LOS_QueueReadCopy(UINT32  uwQueueID, VOID *  pBufferAddr,
                    UINT32 * puwBufferSize, UINT32  uwTimeOut)
    {
        
        QUEUE_BUFF qbuf = {0, NULL};
        char buf[] = { 0x1,0x2,0x3,0x4,0x5 };

        if(g_state == TEST_STATE_ERR){
            return 1;
        }
        qbuf.addr = (uint8_t*)atiny_malloc(sizeof(buf));
        if (NULL == qbuf.addr)
        {
            printf("malloc for qbuf failed!");
            goto END;
        }

        qbuf.len = sizeof(buf);
        memcpy(qbuf.addr, buf, sizeof(buf));
        memcpy(pBufferAddr, &qbuf, sizeof(QUEUE_BUFF));
        *puwBufferSize = qbuf.len;
        
        END:
        return 0;
    }
#endif
}

TestBC95::TestBC95()
{
    at_init();
    TEST_ADD(TestBC95::test_str_to_hex);
	//TEST_ADD(TestBC95::test_strnstr);
	TEST_ADD(TestBC95::test_HexStrToStr);
	TEST_ADD(TestBC95::test_nb_check_csq);
	
    TEST_ADD(TestBC95::test_nb_reboot);
    TEST_ADD(TestBC95::test_nb_hw_detect);
    TEST_ADD(TestBC95::test_nb_set_cdpserver);
    TEST_ADD(TestBC95::test_nb_send_psk);
	TEST_ADD(TestBC95::test_nb_set_no_encrypt);
    TEST_ADD(TestBC95::test_nb_send_payload);
    TEST_ADD(TestBC95::test_nb_query_ip);
	TEST_ADD(TestBC95::test_nb_create_sock);
	TEST_ADD(TestBC95::test_nb_decompose_str);
	TEST_ADD(TestBC95::test_nb_reattach);
	TEST_ADD(TestBC95::test_nb_bind);
    TEST_ADD(TestBC95::test_nb_get_netstat);
    TEST_ADD(TestBC95::test_nb_create_udpsock);
	
	TEST_ADD(TestBC95::test_nb_connect);
	TEST_ADD(TestBC95::test_nb_send);
	TEST_ADD(TestBC95::test_nb_recv);
	TEST_ADD(TestBC95::test_nb_recvfrom);
	TEST_ADD(TestBC95::test_nb_recv_timeout);
	TEST_ADD(TestBC95::test_nb_close);
	TEST_ADD(TestBC95::test_nb_recv_cb);
	TEST_ADD(TestBC95::test_nb_deinit);
    TEST_ADD(TestBC95::test_nb_send_str);
    TEST_ADD(TestBC95::test_nb_data_ioctl);
	TEST_ADD(TestBC95::test_nb_cmd_match);
	TEST_ADD(TestBC95::test_nb_handle_sock_data);
	TEST_ADD(TestBC95::test_nb_int);
	//TEST_ADD(TestBC95::test_nb_step);
	
    
}

TestBC95::~TestBC95()
{
    at_deinit();
}

void TestBC95::test_str_to_hex(void)
{
    int32_t ret = 0;
	
	char bufin[3] = "22";
	char bufout[10];
    
    ret = str_to_hex(NULL,0,NULL);
    TEST_ASSERT_MSG((ret == -1), "test_str_to_hex null failed");
    ret = str_to_hex(bufin,2,bufout);
    TEST_ASSERT_MSG((ret == AT_OK), "test_str_to_hex normal failed");
    
}
#if 0
void TestBC95::test_strnstr(void)
{
    char *ret;

	const char *s1 ="habc";
	const char *s2="";
	size_t len=3;
	ret = strnstr(s1,s2,len);
	TEST_ASSERT_MSG((ret != NULL), "test_strstr normal failed");
	s2="ab";
	ret = strnstr(s1,s2,len);
	TEST_ASSERT_MSG((ret != NULL), "test_strstr normal failed");
	s2="zz";
	ret = strnstr(s1,s2,len);
	TEST_ASSERT_MSG((ret == NULL), "test_strstr normal failed");
}
#endif

void TestBC95::test_HexStrToStr(void)
{   
	char *source="ab";
	char *sources="12";
	unsigned char dest[5];
	int sourceLen=2;
	HexStrToStr(source, dest, sourceLen);
	HexStrToStr(sources, dest, sourceLen);
}

void TestBC95::test_nb_check_csq(void)
{
	int ret;

	stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	ret = nb_check_csq();
	cleanStub(&stub_info);
	TEST_ASSERT_MSG((ret == AT_OK), "test_nb_check_csq failed");
}

/*
void TestBC95::test_nb_alloc_sock(void)
{   
    int ret;
	
    int socket=1;
	sockinfo[0].used_flag=1;
	sockinfo[0].socket=1;
	ret=nb_alloc_sock(socket);
	TEST_ASSERT_MSG((ret == 0), "test_nb_alloc_sock null failed");
}
*/

void TestBC95::test_nb_reboot(void)
{
    int32_t ret = AT_OK;
    stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	ret = nb_reboot();
	cleanStub(&stub_info);
    TEST_ASSERT_MSG((ret == AT_OK), "test_nb_reboot failed");
	
}
void TestBC95::test_nb_hw_detect(void)
{
    int32_t ret = AT_OK;
	
    stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	ret = nb_hw_detect();
	cleanStub(&stub_info);
    TEST_ASSERT_MSG((ret == AT_OK), "test_nb_hw_detect failed");
}

	

void TestBC95::test_nb_set_cdpserver(void)
{
    int32_t ret = AT_OK;

    stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);

	g_state = TEST_STATE_ERR;
	ret = nb_set_cdpserver((char*)"10.0.0.1",(char*)"123456789123456789123");
	TEST_ASSERT_MSG((ret != AT_OK), "test_nb_set_cdpserver failed");
	
	g_state = TEST_STATE_ERR;
	ret = nb_set_cdpserver((char*)"10.0.0.1",(char*)"5683");
	TEST_ASSERT_MSG((ret == AT_FAILED), "test_nb_set_cdpserver failed");
	g_state = TEST_STATE_OK;
	ret = nb_set_cdpserver((char*)"10.0.0.1",(char*)"5683");
	TEST_ASSERT_MSG((ret == AT_OK), "test_nb_set_cdpserver failed");

	cleanStub(&stub_info);
    
}
void TestBC95::test_nb_send_psk(void)
{
	int32_t ret = AT_OK;
    stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	ret = nb_send_psk((char*)"123456",(char*)"654321");
	cleanStub(&stub_info);
    TEST_ASSERT_MSG((ret == AT_OK), "test_nb_send_psk failed");
}
void TestBC95::test_nb_set_no_encrypt(void)
{
	int32_t ret = AT_OK;
	stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	g_state = TEST_STATE_ERR;
	ret = nb_set_no_encrypt();
	cleanStub(&stub_info);
	TEST_ASSERT_MSG((ret != AT_OK), "test_nb_set_no_encrypt failed");
}
void TestBC95::test_nb_send_payload(void)
{
    int32_t ret = AT_OK;
	char buf[5]={'1','2','3','4'};
	ret=nb_send_payload(buf,550);
	
	g_state = TEST_STATE_ERR;
	stubInfo stub_info;
    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	ret = nb_send_payload(buf,4);
	
	TEST_ASSERT_MSG((ret == -1), "test_nb_send_payload failed");
	g_state = TEST_STATE_OK;
	ret = nb_send_payload(buf,4);
	
	cleanStub(&stub_info);
    TEST_ASSERT_MSG((ret == AT_OK), "test_nb_send_payload failed");
	
}
void TestBC95::test_nb_query_ip(void)
{
    int32_t ret = AT_OK;
    stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	ret = nb_query_ip();
	cleanStub(&stub_info);
    TEST_ASSERT_MSG((ret == AT_OK), "test_nb_query_ip failed");
}
void TestBC95::test_nb_create_sock(void)
{
	int ret;
    stubInfo stub_info;
	
	ret=nb_create_sock(5683,5);
	 TEST_ASSERT_MSG((ret == -1), "test_nb_create_sock normal failed");
	ret=nb_create_sock(5683,6);
	 TEST_ASSERT_MSG((ret == -1), "test_nb_create_sock normal failed");
    
	setStub((void *)at_cmd_multi_suffix,(void *)stub_at_cmd_multi_suffix,&stub_info);
	ret = nb_create_sock(5683,17);
	cleanStub(&stub_info);
	TEST_ASSERT_MSG((ret >=1), "test_nb_create_sock normal failed");
}	
void TestBC95::test_nb_decompose_str(void)
{
    int ret;

	//char *str;
	int readleft; 
	int out_sockid;
	stubInfo stub_info;
    stubInfo stub_info1;
	stubInfo stub_info2;
	char str4[]="4,4.5.6.7,89,1,2,3";
	
	char str[]="1";
	nb_decompose_str(str, &readleft, &out_sockid);
    TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");
	char str1[]="123,45";
	nb_decompose_str(str1, &readleft, &out_sockid);
	TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");
	char str2[]="123,45,67";
	nb_decompose_str(str2, &readleft, &out_sockid);
	TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");
	char str3[]="123,4.5.6.7,89";
	nb_decompose_str(str3, &readleft, &out_sockid);
	TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");
	char str5[]="123,4.5.6.7,89,1024";
	nb_decompose_str(str5, &readleft, &out_sockid);
	TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");
	char str6[]="123,4.5.6.7,89,1";
	nb_decompose_str(str6, &readleft, &out_sockid);
	TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");
    char str7[]="123,4.5.6.7,89,1,2";
	nb_decompose_str(str7, &readleft, &out_sockid);
	TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");

	
	
	sockinfo[4].used_flag=1;
	sockinfo[4].socket=4;


	setStub((void *)at_malloc,(void *)stub_at_malloc,&stub_info1);
	setStub((void *)LOS_QueueWriteCopy,(void *)stub_LOS_QueueWriteCopy,&stub_info);
	ret = nb_decompose_str(str4, &readleft, &out_sockid);
	sockinfo[4].used_flag=0;
	sockinfo[4].socket=0;
	TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");

    ret = nb_decompose_str(str4, &readleft, &out_sockid);
	TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");

	sockinfo[4].used_flag=1;
	sockinfo[4].socket=4;
	//setStub((void *)at_malloc,(void *)stub_at_malloc,&stub_info1);
	ret = nb_decompose_str(str4, &readleft, &out_sockid);
	sockinfo[4].used_flag=0;
	sockinfo[4].socket=0;
	
    cleanStub(&stub_info1);	
	cleanStub(&stub_info);	
    TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");
    
	sockinfo[4].used_flag=1;
	sockinfo[4].socket=4;
	setStub((void *)LOS_QueueWriteCopy,(void *)stub_LOS_QueueWriteCopy1,&stub_info2);
	ret = nb_decompose_str(str4, &readleft, &out_sockid);
	sockinfo[4].used_flag=0;
	sockinfo[4].socket=0;
	//cleanStub(&stub_info1);
	cleanStub(&stub_info2);
	TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");
}

void TestBC95::test_nb_bind(void)
{  int ret;
   nb_bind((int8_t *)"192.168.1.116",(int8_t *)"5683",1);
   TEST_ASSERT_MSG((ret =AT_FAILED), "test_nb_decompose_str normal failed");
}

void TestBC95::test_nb_reattach(void)
{
	nb_reattach();
}


void TestBC95::test_nb_get_netstat(void)
{
    int32_t ret = AT_OK;
    stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	ret = nb_get_netstat();
	cleanStub(&stub_info);
    TEST_ASSERT_MSG((ret == AT_OK), "test_nb_get_netstat failed");
}
void TestBC95::test_nb_create_udpsock(void)
{
//    int32_t ret = AT_OK;
//    stubInfo stub_info;
//	setStub((void *)at_cmd,(void *)stub_at_cmd2,&stub_info);
//	ret = nb_create_udpsock((int8_t *)"192.168.1.116",5683,1);
//	cleanStub(&stub_info);
//    TEST_ASSERT_MSG((ret >= 0), "test_nb_create_udpsock failed");
//	
//	setStub((void *)at_cmd,(void *)stub_at_cmd3,&stub_info);
//	ret = nb_create_udpsock((int8_t *)"192.168.1.116",5683,1);
//	cleanStub(&stub_info);
//    TEST_ASSERT_MSG((ret == -1), "test_nb_create_udpsock failed");
	
}
void TestBC95::test_nb_connect(void)
{
      int32_t ret = AT_OK;

	

    stubInfo stub_info;
	stubInfo stub_info2;
	stubInfo stub_info3;
	stubInfo stub_info4;
	stubInfo stub_info5;

	setStub((void *)at_cmd,(void *)stub_at_cmd2,&stub_info);
	ret = nb_connect((int8_t *)"192.168.1.116",(int8_t *)"5683",1);
	TEST_ASSERT_MSG((ret ==-1), "test_nb_connect failed");
	
   
	setStub((void *)LOS_QueueCreate,(void *)stub_LOS_QueueCreate_fail,&stub_info2);
	ret = nb_connect((int8_t *)"192.168.1.116",(int8_t *)"5683",1);
	TEST_ASSERT_MSG((ret == -1), "test_nb_connect failed");

	setStub((void*)nb_create_sock,(void*)stub_nb_create_sock,&stub_info4);
	ret = nb_connect((int8_t *)"192.168.1.116",(int8_t *)"5683",1);
	TEST_ASSERT_MSG((ret == -1), "test_nb_connect failed");
	cleanStub(&stub_info4);

	
    sockinfo[0].used_flag=1;
	sockinfo[0].socket=1;
	sockinfo[1].used_flag=0;
	
	setStub((void*)nb_create_sock,(void*)stub_nb_create_sock,&stub_info4);
	ret = nb_connect((int8_t *)"192.168.1.116",(int8_t *)"5683",1);
	TEST_ASSERT_MSG((ret ==-1), "test_nb_connect failed");
	cleanStub(&stub_info4);

	sockinfo[0].used_flag=0;
	sockinfo[0].socket=0;
	sockinfo[1].used_flag=0;

	sockinfo[1].used_flag=1;
	setStub((void*)nb_create_sock,(void*)stub_nb_create_sock,&stub_info4);
	ret = nb_connect((int8_t *)"192.168.1.116",(int8_t *)"5683",1);
	TEST_ASSERT_MSG((ret ==-1), "test_nb_connect failed");
	cleanStub(&stub_info4);
	sockinfo[1].used_flag=0;

    sockinfo[0].used_flag=1;
	sockinfo[1].used_flag=1;
	sockinfo[2].used_flag=1;
	sockinfo[3].used_flag=1;
	sockinfo[4].used_flag=1;
	setStub((void*)nb_create_sock,(void*)stub_nb_create_sock,&stub_info4);
	ret = nb_connect((int8_t *)"192.168.1.116",(int8_t *)"5683",1);
	TEST_ASSERT_MSG((ret ==-1), "test_nb_connect failed");
	cleanStub(&stub_info4);
	sockinfo[0].used_flag=0;
	sockinfo[1].used_flag=0;
	sockinfo[2].used_flag=0;
	sockinfo[3].used_flag=0;
	sockinfo[4].used_flag=0;
	

	cleanStub(&stub_info2);

    sockinfo[0].used_flag=1;
	sockinfo[0].socket=1;
	sockinfo[1].used_flag=0;
	
	setStub((void*)nb_create_sock,(void*)stub_nb_create_sock,&stub_info4);
	ret = nb_connect((int8_t *)"192.168.1.116",(int8_t *)"5683",1);
	TEST_ASSERT_MSG((ret >= 0), "test_nb_connect failed");
	cleanStub(&stub_info4);

	sockinfo[0].used_flag=0;
	sockinfo[0].socket=0;
	sockinfo[1].used_flag=0;
   

	
	cleanStub(&stub_info);
}
void TestBC95::test_nb_send(void)
{
    int32_t ret = AT_OK;
	uint8_t buf[5]={0,1,2,3};
    stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	ret = nb_send(0,buf,4);
	cleanStub(&stub_info);
    TEST_ASSERT_MSG((ret >= 0), "test_nb_send failed");
	ret=nb_send(0,NULL,4);
	ret=nb_send(5,NULL,4);
}
void TestBC95::test_nb_recv(void)
{
	int32_t ret = AT_OK;
	uint8_t buf[100];
	printf("come into test_nb_recv\n");
    stubInfo stub_info;
	setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy,&stub_info);
	ret = nb_recv(0,buf,100);
	TEST_ASSERT_MSG((ret>=0), "test_nb_recv failed");
	cleanStub(&stub_info);
	
	setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy_fail,&stub_info);
	ret = nb_recv(0,buf,100);
	TEST_ASSERT_MSG((ret <0), "test_nb_recv failed");
	cleanStub(&stub_info);
}
void TestBC95::test_nb_recvfrom(void)
{
	//int32_t id , uint8_t  *buf, uint32_t len,char* ipaddr,int* port
	int ret;
	int32_t id=6;
    uint8_t buft[]="123";
	uint32_t len=3;
	char ipaddrr[]="123";
	int port;
	

	ret=nb_recvfrom(id,buft,len,ipaddrr,&port);\
	TEST_ASSERT_MSG((ret == -1), "test_nb_recvfrom failed");
  
}
void TestBC95::test_nb_recv_timeout(void)
{
    int ret;
    int32_t id=6;
    uint8_t buft[20]="0";
	uint32_t len=3;
	char ipaddrr[20]="0";
	int port;
	int32_t timeout=10;

	nb_recv_timeout(id,buft,len,ipaddrr,&port,timeout);
	
	stubInfo stub_info;
	setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy1,&stub_info);
	ret=nb_recv_timeout(1,buft,len,ipaddrr,&port,timeout);//this line
	cleanStub(&stub_info);
	
	TEST_ASSERT_MSG((ret >=0), "test_nb_recv_timeout failed");


  /*

   int32_t ret = AT_OK;
   uint8_t buf[100];
	
    stubInfo stub_info;
	setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy,&stub_info);
	ret = nb_recv_timeout(0,buf,100,5);
	TEST_ASSERT_MSG((ret >= 0), "test_nb_recv_timeout failed");
	cleanStub(&stub_info);
	
	setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy_fail,&stub_info);
	ret = nb_recv_timeout(0,buf,100,5);
	TEST_ASSERT_MSG((ret == -1), "test_nb_recv_timeout failed");
	cleanStub(&stub_info);
  */
}

void TestBC95::test_nb_close(void)
{
    int32_t ret = AT_OK;
    stubInfo stub_info;
	stubInfo stub_info1;
	stubInfo stub_info2;
	
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	setStub((void *)at_cmd_multi_suffix,(void *)stub_at_cmd_multi_suffix1,&stub_info1);
	setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy_fail,&stub_info2);
	sockinfo[1].used_flag=1;
	sockinfo[1].socket;
	ret = nb_close(1);
	cleanStub(&stub_info2);
	cleanStub(&stub_info1);
	cleanStub(&stub_info);
    TEST_ASSERT_MSG((ret == 0), "nb_close failed");
	sockinfo[1].used_flag=0;

    nb_close(6);
    
/*
	stubInfo stub_info;
		setStub((void *)LOS_QueueReadCopy,(void *)stub_LOS_QueueReadCopy,&stub_info);
		ret = nb_recv(0,buf,100);
		TEST_ASSERT_MSG((ret >= 0), "test_nb_recv failed");
		cleanStub(&stub_info);
*/

}

void TestBC95::test_nb_recv_cb(void)
{
    int32_t ret = AT_OK;
	
	ret = nb_recv_cb(1);
	TEST_ASSERT_MSG((ret == -1), "test_nb_recv_cb failed");
}

void TestBC95::test_nb_deinit(void)
{
    int32_t ret = AT_OK;
    stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	ret = nb_deinit();
	cleanStub(&stub_info);
    TEST_ASSERT_MSG((ret == 0), "test_nb_deinit failed");
}
void TestBC95::test_nb_send_str(void)
{
    char* buf = (char *)"send ok"; 
    int len = strlen(buf);
    int ret = 0;
        
    stubInfo stub_info;
	setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
	ret = nb_send_str(buf, len);
	cleanStub(&stub_info);
    TEST_ASSERT_MSG((ret == 0), "test_nb_send_str failed");
}

void TestBC95::test_nb_cmd_match(void)
{
	int ret;
	char buff[]="+NSONMI:4,5";
	char *buff1=NULL;
	char buff2[]="123";
	char buff3[]="+NSONMI:1";
	char featurestr[]="12";
	int len=3;

	nb_cmd_match(buff1,featurestr,len);
	nb_cmd_match(buff2,featurestr,len);
	nb_cmd_match(buff3,featurestr,len);
	nb_cmd_match(buff,featurestr,len);

	sockinfo[4].used_flag=1;
	sockinfo[4].socket=4;
	stubInfo stub_info1;
	setStub((void *)at_cmd_in_callback,(void *)stub_at_cmd_in_callback1,&stub_info1);
	ret=nb_cmd_match(buff,featurestr,len);
	cleanStub(&stub_info1);
	TEST_ASSERT_MSG((ret != 0), "test_nb_cmd_match failed");

    sockinfo[4].used_flag=1;
	sockinfo[4].socket=4;
	stubInfo stub_info2;
	setStub((void *)at_cmd_in_callback,(void *)stub_at_cmd_in_callback2,&stub_info2);
	ret=nb_cmd_match(buff,featurestr,len);
	cleanStub(&stub_info2);
	sockinfo[4].used_flag=0;
	sockinfo[4].socket=0;
    TEST_ASSERT_MSG((ret != 0), "test_nb_cmd_match failed");



/*
	sockinfo[4].used_flag=1;
	sockinfo[4].socket=4;
	nb_cmd_match(buff,featurestr,len);
	sockinfo[4].used_flag=0;
	sockinfo[4].socket=0;
*/


}

void TestBC95::test_nb_handle_sock_data(void)
{
    char buffer[50]="+NSONMI:1,3,";
	
	char* featurestr="abc";
	int len =3;
	sockinfo[1].used_flag=1;
	sockinfo[1].socket=1;

	stubInfo stub_cmd;
	setStub((void *)at_cmd_in_callback,(void *)stub_at_cmd_in_callback,&stub_cmd);
	nb_cmd_match(buffer,featurestr,len);
		
	cleanStub(&stub_cmd);

	sockinfo[1].used_flag=0;
	sockinfo[1].socket=0;	
}

void TestBC95::test_nb_int(void)
{
	bc95_interface.init();
	at.step_callback();
}


/*
void TestBC95::test_nb_step(void)
{
	char buff[]="+NSONMI:4,5";
	
	char featurestr[]="12";
	int len=3;
	sockinfo[4].used_flag=1;
	sockinfo[4].socket=4;
	stubInfo stub_info1;
	setStub((void *)at_cmd_in_callback,(void *)stub_at_cmd_in_callback1,&stub_info1);
	nb_cmd_match(buff,featurestr,len);
	cleanStub(&stub_info1);
	nb_step();
}
*/

/*
void TestBC95::test_nb_step(void)
{
	nb_step();
	g_data_ind_info.valid_flag=1;
	nb_step();
	g_data_ind_info.valid_flag=0;
}
*/



void TestBC95::test_nb_data_ioctl(void)
{
//    void* arg = NULL;
//    int8_t *buf = NULL;
//    int32_t len = 0;
//    char *para = (char *)"+NSONMI,";
//    int ret = -1;
//    

//    ret = nb_data_ioctl(arg, buf, len);
//    TEST_ASSERT(ret == -1);

//    buf = (int8_t *)para;
//    len = 100;
//    
//    stubInfo stub_info;
//    stubInfo si;
//    setStub((void *)at_cmd,(void *)stub_at_cmd,&stub_info);
//    setStub((void *)LOS_QueueWriteCopy,(void *)stub_LOS_QueueWriteCopy,&si);
//    ret = nb_data_ioctl(arg, buf, len);
//    TEST_ASSERT(ret == 0);

//    para = (char *)"+NSONMI,12,120";
//    g_state = TEST_STATE_ERR;
//    ret = nb_data_ioctl(arg, buf, len);
//    TEST_ASSERT(ret == 1);
//    cleanStub(&stub_info);
//    cleanStub(&si);
}




/* Private functions --------------------------------------------------------*/

