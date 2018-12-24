
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
#include "test_atiny_socket.h"
#include <stdlib.h>

//#include <cstring>
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"
#include "osdepends/atiny_osdep.h"
#include <sys/socket.h>
#include <sys/types.h>
#include "errno.h"
#define WITH_LINUX	1

#define RECV_NUM  25
int g_select,g_send;
typedef struct
{
	int fd;
}atiny_net_context;


extern void *atiny_net_bind(const char *host, const char *port, int proto);
extern void *atiny_net_connect(const char *host, const char *port, int proto);
extern int atiny_net_recv(void *ctx, unsigned char *buf, size_t len);
extern int atiny_net_recv_timeout(void *ctx, unsigned char *buf, size_t len, uint32_t timeout);
extern int atiny_net_send(void *ctx, const unsigned char *buf, size_t len);
extern void atiny_net_close(void *ctx);
extern int getaddrinfo(const char *nodename, const char *servername, const struct addrinfo *hints, struct addrinfo **res);
extern int fcntl(int s, int cmd, int val);
extern int atiny_net_accept( void *bind_ctx, void *client_ctx, void *client_ip, size_t buf_size, size_t *ip_len );
extern int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen);
extern int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
extern int recvfrom(int s, void *mem, size_t len, int flags,struct sockaddr *from, socklen_t *fromlen);
extern int getsockname(int s, struct sockaddr *name, socklen_t *namelen);
extern int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);
extern int atiny_net_send_timeout(void *ctx, const unsigned char *buf, size_t len, uint32_t timeout);


int recv_flag = 0;
int getsockopt_flag = 0;
int accept_flag = 0;
int recvfrom_flag = 0;
int connect_flag = 0;
int setsockopt_flag =0;




}

/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestAtiny_Socket::TestAtiny_Socket()
{
	TEST_ADD(TestAtiny_Socket::test_atiny_net_bind);
    TEST_ADD(TestAtiny_Socket::test_atiny_net_connect);

    TEST_ADD(TestAtiny_Socket::test_atiny_net_recv);
    TEST_ADD(TestAtiny_Socket::test_atiny_net_recv_timeout);
    TEST_ADD(TestAtiny_Socket::test_atiny_net_send);
	TEST_ADD(TestAtiny_Socket::test_atiny_net_accept);
	TEST_ADD(TestAtiny_Socket::test_atiny_net_send_timeout);
    TEST_ADD(TestAtiny_Socket::test_atiny_net_close);


}

int stub_getaddrinfo(const char *nodename, const char *servername, const struct addrinfo *hints, struct addrinfo **res)
{
		return 1;
}       

void* stub_atiny_malloc(size_t size)
{
		return NULL;

}
int stub_fcntl(int s, int cmd, int val)
{
	return -1;
	
}
int stub_recv(int s, void *mem, size_t len, int flags)
{
	extern int errno;
	if(recv_flag == 0)
	return 0;
	else if(recv_flag == 1)
	{
		errno = EAGAIN;
		return -1;
	}
	else if(recv_flag == 2)
	{
		errno = EBADSLT;
		return -2;
	}
	else if(recv_flag == 3)
	{
		errno = 0;
		return RECV_NUM;
	}
}

int stub_connect(int s, const struct sockaddr *name, int namelen)
{
	if(connect_flag < 0)
		return -1;
	else
		return 0;

}

int stub_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
                struct timeval *timeout)
{
    return g_select;
}

int stub_send(int s, const void *data, size_t size, int flags)
{
    return g_send;
}

int stub_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
	if(getsockopt_flag == 0)
		return -1;
	else if(getsockopt_flag == 1)
	{
		*(int*)optval = 1;
		return 0;
	}
	else if(getsockopt_flag == 2)
	{
		*(int*)optval = 2;
		return 0;
	}
	else if(getsockopt_flag == 3)
	{
		*(int*)optval = 3;
		return 0;
	}
}

int stub_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	if(accept_flag < 0)
		return -1;
	else
		addr->sa_family = AF_INET;
		return 0;

}
int stub_recvfrom(int32_t id , uint8_t  *buf, uint32_t len,char* ipaddr,int* port)
{
	if(recvfrom_flag < 0)
		return -1;
	else
		return 0;
}

int stub_getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
	return 0;
}

int stub_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
	if(setsockopt_flag < 0)
		return -1;
	else
		return 0;
}

TestAtiny_Socket::~TestAtiny_Socket(void)
{

}

void TestAtiny_Socket::test_atiny_net_bind(void)
{
	printf("+++++++++++++++++test_atiny_net_bind begin++++++++++++++++++++++++\n");
	void *param_ctx = NULL;
	
	param_ctx = atiny_net_bind(NULL, NULL, 0);
	TEST_ASSERT_MSG((param_ctx == NULL), "atiny_net_bind(...) failed");
	printf("+++++++++++++++++test_atiny_net_bind end++++++++++++++++++++++++\n");
}

void TestAtiny_Socket::test_atiny_net_accept(void)
{
	printf("+++++++++++++++++test_atiny_net_accept begin++++++++++++++++++++++++\n");


    int result = ATINY_NET_ERR;
	atiny_net_context *para_bind_context, test_bind_context;
	atiny_net_context *para_client_context, test_client_context;
	para_bind_context = &test_bind_context;
	test_bind_context.fd = -1;
	para_client_context = &test_client_context;
	test_client_context.fd = -1;
	getsockopt_flag = 0;
	stubInfo si_getsockopt;
	setStub((void *)getsockopt, (void *)stub_getsockopt, &si_getsockopt);
	result = atiny_net_accept(para_bind_context, para_client_context, NULL, 0, 0);
	TEST_ASSERT_MSG((result == ATINY_NET_ACCEPT_FAILED), "atiny_net_accept(...) failed");
	cleanStub(&si_getsockopt);

	getsockopt_flag = 3;
	setStub((void *)getsockopt, (void *)stub_getsockopt, &si_getsockopt);
	result = atiny_net_accept(para_bind_context, para_client_context, NULL, 0, 0);
	TEST_ASSERT_MSG((result == ATINY_NET_ACCEPT_FAILED), "atiny_net_accept(...) failed");
	cleanStub(&si_getsockopt);

	getsockopt_flag = 1;
	accept_flag = -1;
	stubInfo si_accept;
	setStub((void *)accept, (void *)stub_accept, &si_accept);
	setStub((void *)getsockopt, (void *)stub_getsockopt, &si_getsockopt);
	result = atiny_net_accept(para_bind_context, para_client_context, NULL, 0, 0);
	TEST_ASSERT_MSG((result == ATINY_NET_ACCEPT_FAILED), "atiny_net_accept(...) failed");
	cleanStub(&si_accept);
	cleanStub(&si_getsockopt);


	getsockopt_flag = 2;
	accept_flag = -1;
	recvfrom_flag = -1;
	stubInfo si_recvfrom;
	setStub((void *)accept, (void *)stub_accept, &si_accept);
	setStub((void *)getsockopt, (void *)stub_getsockopt, &si_getsockopt);
	setStub((void *)recvfrom, (void *)stub_recvfrom, &si_recvfrom);
	result = atiny_net_accept(para_bind_context, para_client_context, NULL, 0, 0);
	TEST_ASSERT_MSG((result == ATINY_NET_ACCEPT_FAILED), "atiny_net_accept(...) failed");
	cleanStub(&si_accept);
	cleanStub(&si_recvfrom);
	cleanStub(&si_getsockopt);

	char test_client_ip[] = "192.168.1.100";
	char *para_client_ip = test_client_ip;
	size_t *para_ip_len,test_ip_len = 0;
	para_ip_len = &test_ip_len;
	getsockopt_flag = 1;
	accept_flag = 0;
	setStub((void *)accept, (void *)stub_accept, &si_accept);
	setStub((void *)getsockopt, (void *)stub_getsockopt, &si_getsockopt);
	result = atiny_net_accept(para_bind_context, para_client_context, test_client_ip, 0, para_ip_len);
	TEST_ASSERT_MSG((result == ATINY_NET_BUF_SMALL_FAILED), "atiny_net_accept(...) failed");
	cleanStub(&si_accept);
	cleanStub(&si_getsockopt);


	para_ip_len = &test_ip_len;
	getsockopt_flag = 1;
	accept_flag = 0;
	setStub((void *)accept, (void *)stub_accept, &si_accept);
	setStub((void *)getsockopt, (void *)stub_getsockopt, &si_getsockopt);
	result = atiny_net_accept(para_bind_context, para_client_context, test_client_ip, 8, para_ip_len);
	TEST_ASSERT_MSG((result == 0), "atiny_net_accept(...) failed");
	cleanStub(&si_accept);
	cleanStub(&si_getsockopt);

	getsockopt_flag = 2;
	accept_flag = -1;
	recvfrom_flag = 0;
	connect_flag =-1;
	stubInfo si_connect;
    setStub((void *)connect, (void *)stub_connect, &si_connect);
	setStub((void *)accept, (void *)stub_accept, &si_accept);
	setStub((void *)getsockopt, (void *)stub_getsockopt, &si_getsockopt);
	setStub((void *)recvfrom, (void *)stub_recvfrom, &si_recvfrom);
	result = atiny_net_accept(para_bind_context, para_client_context, NULL, 0, 0);
	TEST_ASSERT_MSG((result == ATINY_NET_ACCEPT_FAILED), "atiny_net_accept(...) failed");
	cleanStub(&si_accept);
	cleanStub(&si_recvfrom);
	cleanStub(&si_getsockopt);

	getsockopt_flag = 2;
	accept_flag = -1;
	recvfrom_flag = 0;
	connect_flag = 0;
	setsockopt_flag =-1;
	stubInfo si_setsockopt;
    setStub((void *)connect, (void *)stub_connect, &si_connect);
	setStub((void *)accept, (void *)stub_accept, &si_accept);
	setStub((void *)getsockopt, (void *)stub_getsockopt, &si_getsockopt);
	setStub((void *)recvfrom, (void *)stub_recvfrom, &si_recvfrom);
	setStub((void *)setsockopt, (void *)stub_setsockopt, &si_setsockopt);
	result = atiny_net_accept(para_bind_context, para_client_context, NULL, 0, 0);
	TEST_ASSERT_MSG((result == ATINY_NET_SOCKET_FAILED), "atiny_net_accept(...) failed");
	cleanStub(&si_accept);
	cleanStub(&si_recvfrom);
	cleanStub(&si_getsockopt);
	cleanStub(&si_setsockopt);



	printf("+++++++++++++++++test_atiny_net_accept end++++++++++++++++++++++++\n");

}

void TestAtiny_Socket::test_atiny_net_connect(void)
{	
	printf("+++++++++++++++++test_atiny_net_connect begin++++++++++++++++++++++++\n");
	void *param_ctx;
	param_ctx = atiny_net_connect(NULL, NULL, 2);
	TEST_ASSERT_MSG((param_ctx == NULL), "atiny_net_connect(...) failed");
	
	char *test_host = (char *)"192.168.1.100";
	char *test_port = (char *)"2440";
	
	
    param_ctx = (atiny_net_context *)atiny_net_connect(test_host, test_port, ATINY_PROTO_UDP+1);
	TEST_ASSERT_MSG((param_ctx == NULL), "atiny_net_connect(...) failed");
	
	param_ctx = (atiny_net_context *)atiny_net_connect(test_host, test_port, ATINY_PROTO_UDP);
	TEST_ASSERT_MSG((param_ctx != NULL), "atiny_net_connect(...) failed");
	atiny_net_close(param_ctx);

    stubInfo si_connect;
    setStub((void *)connect, (void *)stub_connect, &si_connect);
	param_ctx = (atiny_net_context *)atiny_net_connect(test_host, test_port, ATINY_PROTO_TCP);
	TEST_ASSERT_MSG((param_ctx != NULL), "atiny_net_connect(...) failed");
	atiny_net_close(param_ctx);
	
	stubInfo si_getaddrinfo;
    setStub((void *)getaddrinfo, (void *)stub_getaddrinfo, &si_getaddrinfo);
	param_ctx = atiny_net_connect(test_host, test_port, ATINY_PROTO_UDP);
	TEST_ASSERT_MSG((param_ctx == NULL), "atiny_net_connect(...) failed");
	cleanStub(&si_getaddrinfo);
	
	stubInfo si_atiny_malloc;
	setStub((void *)atiny_malloc, (void *)stub_atiny_malloc, &si_atiny_malloc);
	param_ctx = atiny_net_connect(test_host, test_port, ATINY_PROTO_UDP);
	TEST_ASSERT_MSG((param_ctx == NULL), "atiny_net_connect(...) failed");
	cleanStub(&si_atiny_malloc);
	
	stubInfo si_fcntl;
	setStub((void *)fcntl, (void *)stub_fcntl, &si_fcntl);
	param_ctx = atiny_net_connect(test_host, test_port, ATINY_PROTO_UDP);
	TEST_ASSERT_MSG((param_ctx == NULL), "atiny_net_connect(...) failed");
	cleanStub(&si_fcntl);
	printf("+++++++++++++++++test_atiny_net_connect end++++++++++++++++++++++++\n");
}
 
void TestAtiny_Socket::test_atiny_net_recv(void)
{
	printf("+++++++++++++++++test_atiny_net_recv begin++++++++++++++++++++++++\n");
	int result;
	atiny_net_context *para_context, test_context;
	memset(&test_context, 0, sizeof(atiny_net_context));
	test_context.fd = 10;
	unsigned char test_buf[100] = {0};
	
	
	stubInfo si_recv;
	setStub((void *)recv, (void *)stub_recv, &si_recv);
	recv_flag = 0;
	result = atiny_net_recv(&test_context, test_buf, 100);

	TEST_ASSERT_MSG((result == -1), "atiny_net_recv(...) failed");
	recv_flag = 1;
	result = atiny_net_recv(&test_context, test_buf, 100);
	TEST_ASSERT_MSG((result == 0), "atiny_net_recv(...) failed");
    recv_flag = 2;
	result = atiny_net_recv(&test_context, test_buf, 100);
	TEST_ASSERT_MSG((result == -1), "atiny_net_recv(...) failed");

	recv_flag = 3;
	result = atiny_net_recv(&test_context, test_buf, 100);
	TEST_ASSERT_MSG((result == RECV_NUM), "atiny_net_recv(...) failed");
	cleanStub(&si_recv);
	printf("+++++++++++++++++test_atiny_net_recv end++++++++++++++++++++++++\n");
}

void TestAtiny_Socket::test_atiny_net_recv_timeout(void)
{
	printf("+++++++++++++++++test_atiny_net_recv_timeout begin++++++++++++++++++++++++\n");

	int result;
	atiny_net_context *para_context, test_context;
	unsigned char buf[100] = {0};
	para_context = &test_context;
	test_context.fd = -1;
	result = atiny_net_recv_timeout(para_context, buf, 100, 0);
	//printf("result is %d\n", result);
	TEST_ASSERT_MSG((result == -1), "atiny_net_recv_timeout(...) failed");

	stubInfo si_select;
	setStub((void *)select, (void *)stub_select, &si_select);
	
	test_context.fd = 1;
	g_select = 0;
	result = atiny_net_recv_timeout(para_context, buf, 100, 1000);
	TEST_ASSERT_MSG((result == -2), "atiny_net_recv_timeout(...) failed");

	g_select = -1;

	result = atiny_net_recv_timeout(para_context, buf, 100, 1000);
    TEST_ASSERT_MSG((result == -1), "atiny_net_recv_timeout(...) failed");

    stubInfo si_recv;
	setStub((void *)recv, (void *)stub_recv, &si_recv);
    g_select = 1;
    recv_flag = 3;

	result = atiny_net_recv_timeout(para_context, buf, 100, 1000);
    TEST_ASSERT_MSG((result == RECV_NUM), "atiny_net_recv_timeout(...) failed");
	

	cleanStub(&si_recv);
	cleanStub(&si_select);

	printf("+++++++++++++++++test_atiny_net_recv_timeout end++++++++++++++++++++++++\n");
}

void TestAtiny_Socket::test_atiny_net_send(void)
{
	printf("+++++++++++++++++test_atiny_net_send begin++++++++++++++++++++++++\n");
	int result;
	atiny_net_context *param_context = NULL, test_context;
	const unsigned char buf[100] = {0};
	memset(&test_context, 0, sizeof(atiny_net_context));
	param_context = &test_context;
	test_context.fd = -1;
	result = atiny_net_send(param_context, buf, 100);
	TEST_ASSERT_MSG((result == -1), "atiny_net_recv_timeout(...) failed");
    
	stubInfo si_send;
	setStub((void *)send, (void *)stub_send, &si_send);
	g_send = -1;
	errno = EAGAIN;
	test_context.fd = 1;
	result = atiny_net_send(param_context, buf, 100);
	TEST_ASSERT_MSG((result == 0), "atiny_net_recv_timeout(...) failed");
	errno = EBADSLT;
	result = atiny_net_send(param_context, buf, 100);  
	TEST_ASSERT_MSG((result == -1), "atiny_net_recv_timeout(...) failed");
	
    g_send = 20;
	result = atiny_net_send(param_context, buf, 20);  
	TEST_ASSERT_MSG((result == 20), "atiny_net_recv_timeout(...) failed");
    cleanStub(&si_send);
	printf("+++++++++++++++++test_atiny_net_send end++++++++++++++++++++++++\n");
	
}



void TestAtiny_Socket::test_atiny_net_send_timeout(void)
{
	int result;
	int test_ctx = 22;
	int *p_test_ctx = &test_ctx;
	unsigned char test_buffer[] = "hello112233";
	int test_len = 12;
	uint32_t test_timeout_ms = 0;
	result = atiny_net_send_timeout((void *)p_test_ctx, test_buffer, test_len, test_timeout_ms);
	printf("result%d",result);
	TEST_ASSERT_MSG((result == -1), "atiny_net_send_timeout(...) failed");

	
}

void TestAtiny_Socket::test_atiny_net_close(void)
{
	printf("+++++++++++++++++test_atiny_net_close begin++++++++++++++++++++++++\n");
	int result = 0;
	atiny_net_context *param_context = NULL;
	param_context = (atiny_net_context *)atiny_malloc(100);
	memset(param_context, 0, sizeof(atiny_net_context));
	param_context->fd = 1;
	atiny_net_close(param_context);
	TEST_ASSERT_MSG((result == 0), "atiny_net_close(...) failed");
	printf("+++++++++++++++++test_atiny_net_close end++++++++++++++++++++++++\n");
}

void TestAtiny_Socket::setup()
{

}

void TestAtiny_Socket::tear_down()
{

}


