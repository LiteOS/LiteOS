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
#include "test_MQTTliteos.h"
#include "ssl.h"
#include "ctr_drbg.h"
#include "net_sockets.h"
#include "sys/socket.h"
#include "unistd.h"
#include "MQTTClient.h"
#include "dtls_interface.h"
static int i;
extern "C"
{   
	
    #include "MQTTliteos.h"
    typedef mqtt_security_info_s *(*t_test_get_security_info)(void);
    static Timer t;
    //extern int mbedtls_mqtt_connect( mbedtls_net_context *ctx, const char *host,const char *port, int proto );
    static uint64_t stub_atiny_gettime_ms(void)
    {
        static int timer = 1;
        return timer++;
    }
    static int stub_mbedtls_ssl_read( mbedtls_ssl_context *ssl, unsigned char *buf, size_t len )
    {
        return 1;
    }
    static int stub_mbedtls_ssl_write( mbedtls_ssl_context *ssl, const unsigned char *buf, size_t len )
    {
        return 1;
    }
    static int stub_mbedtls_ctr_drbg_seed( mbedtls_ctr_drbg_context *ctx,
                       int (*f_entropy)(void *, unsigned char *, size_t),
                       void *p_entropy,
                       const unsigned char *custom,
                       size_t len )
    {
        ctx->p_entropy = p_entropy;
        return 0;
    }
    static int stub_mbedtls_mqtt_connect( mbedtls_net_context *ctx, const char *host,
                            const char *port, int proto )
    {
        return 0;
    }
                            
    static int stub_mbedtls_net_set_block( mbedtls_net_context *ctx )
                                {
                                    return 0;
                                }
    
    static int stub_mbedtls_ssl_handshake( mbedtls_ssl_context *ssl )
    {
        return 0;
    }
    static int stub_mbedtls_ssl_handshake_step( mbedtls_ssl_context *ssl )
    {
        return MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE;
    }
    
    int stub_connect(int s, void *name, int namelen)
    {
        return 1;
    }
    int stub_recv(int s, void *mem, size_t len, int flags)
    {
        return 0;
    }
    int stub_recv_err(int s, void *mem, size_t len, int flags)
    {
        return -1;
    }
    int stub_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
                struct timeval *timeout)
    {
        return 1;
    }
    int stub_write(int s, const void *data, size_t size, int flags)
    {
        return sizeof(data);
    }
	static void *stub_atiny_net_connect(const char *host, const char *port, int proto){
		return NULL;
	}
	 int atiny_task_mutex_delete(atiny_task_mutex_s *mutex);
	
	 mbedtls_ssl_context *dtls_ssl_new(dtls_establish_info_s *info, char plat_type);
	
	 int dtls_shakehand(mbedtls_ssl_context *ssl, const dtls_shakehand_info_s *info);
	
	void dtls_ssl_destroy(mbedtls_ssl_context *ssl);
	static void print_ss(const char * p){
		printf("===========================================================\n");
		printf("+-+-+-+-+-+-+-    %s    +-+-+-+-+-+-\n",p);
		printf("===========================================================\n");
	}
	static int stub_atiny_task_mutex_delete(atiny_task_mutex_s *mutex){
		return 0;
	}
	static int mbedtls_net_set_block( mbedtls_net_context *ctx )
	{		
		return 0;
	}

	static int mbedtls_net_set_nonblock( mbedtls_net_context *ctx )
	{		
		return 0;
	}
	static int stub_MQTTDeserialize_ack(unsigned char* packettype, unsigned char* dup, unsigned short* packetid, unsigned char* buf, int buflen)
	{
		return 0;
	}
	static int stub_MQTTDeserialize_ack_ret1(unsigned char* packettype, unsigned char* dup, unsigned short* packetid, unsigned char* buf, int buflen)
	{
		return 0;
	}
	
	static int stub_MQTTDeserialize_ack_ret2(unsigned char* packettype, unsigned char* dup, unsigned short* packetid, unsigned char* buf, int buflen)
	{
		//printf("+-+-+-+-+-+-	%p	+-+-+-+-+-+-\n",stub_MQTTDeserialize_ack_ret2);
		return 2;
	}
	
	static int stub_MQTTSerialize_ack(unsigned char* buf, int buflen, unsigned char packettype, unsigned char dup, unsigned short packetid)
	{
		return 0;
	}
	static int stub_MQTTSerialize_ack_ret(unsigned char* buf, int buflen, unsigned char packettype, unsigned char dup, unsigned short packetid)
	{
		return 10;
	}
	
		static void test_fp_0(MessageData*){}
		static void test_fp_1(MessageData*){}
		static void test_fp_2(MessageData*){}
		static void test_fp_3(MessageData*){}
		static void test_fp_4(MessageData*){}
		static void test_msg_handler(MessageData*){}
		static int static_Ret_of_test_mqttread = 0;
		static int test_mqttread(Network*v_network, unsigned char*v_char, int v_int1, int v_int2){
			return static_Ret_of_test_mqttread;
		}
		static int static_mqttread_count = 0;
		static int test_mqttread_con(Network*v_network, unsigned char*v_char, int v_int1, int v_int2){
			if(static_mqttread_count<6){
				static_mqttread_count++;
				*v_char = 0x1111;
				return 1;
			}
			return 0;
		}
		
		static int test_mqttwrite(Network*, unsigned char*, int, int){
			//printf("+-+-+-+-+-+-	0  +-+-+-+-+-+-\n");
			return 0;
		}
		static int test_mqttwrite_ret10(Network*, unsigned char*, int, int){
			//printf("+-+-+-+-+-+-	ret10  +-+-+-+-+-+-\n");
			return 10;
		}
		static void test_disconnect(Network*){}
		static mqtt_security_info_s static_l_s_msinfo_1;
		static int static_count_get_secuInfo = 0;
		static int static_secu_value_for_ifelse = 1;
		static mqtt_security_info_s * test_get_security_info(void){
			if(0==static_count_get_secuInfo){
				static_count_get_secuInfo++;
				static_l_s_msinfo_1.security_type = 0;
			}
			else if(1==static_count_get_secuInfo){
				static_count_get_secuInfo++;
				static_l_s_msinfo_1.security_type = 2;
			}
			else if(2==static_count_get_secuInfo){
				static_count_get_secuInfo++;
				static_l_s_msinfo_1.security_type = static_secu_value_for_ifelse;
			}
			else if(10==static_count_get_secuInfo){
				static_l_s_msinfo_1.security_type = 10;
			}
			return &static_l_s_msinfo_1;
		}
		static int stub_MutexLock(Mutex *mutex){return 0;}
		static int stub_MutexUnlock(Mutex *mutex){return 0;}
		static int stub_MQTTSerialize_disconnect(unsigned char *buf, int buflen){return 0;}
		static int stub_MQTTSerialize_publish_ret10(unsigned char *buf, int buflen, unsigned char dup, int qos, unsigned char retained, unsigned short packetid,
							  MQTTString topicName, unsigned char *payload, int payloadlen){
			return 10;
		}
		static void stub_TimerCountdown(Timer *timer, unsigned int timeout){
			//printf("+-+-+-+-+-+-	timer count down  +-+-+-+-+-+-\n");
		}
		static int static_count_for_thread = 0;
		static void stub_TimerCountdownMS(Timer *timer, unsigned int timeout){
			static_count_for_thread++;
		}
		static bool stub_TimerIsExpiredFalse(Timer *timer){return false;}
		static int static_count_timer_expire = 0;
		static bool stub_TimerIsExpired_con(Timer *timer){
			if(static_count_timer_expire==0){
				static_count_timer_expire++;
				return false;
			}
			else if(static_count_timer_expire==2){
				//static_count_timer_expire++;
				return true;
			}
			
			return false;
		}
		//stub_TimerIsExpired_keepalive
		static int static_count_expire_keepalive;
		static bool stub_TimerIsExpired_keepalive(Timer *timer){
			if(static_count_expire_keepalive==0){
				static_count_expire_keepalive++;
				return true;
				
			}
			else if(static_count_expire_keepalive==1){
				static_count_expire_keepalive++;
				return true;
			}
			return false;
		}
		static int static_count_waitfor=0;
		static int stub_waitfor_ret(MQTTClient* c, int packet_type, Timer* timer){
			static_count_waitfor++;
			if(1==static_count_waitfor)return 4;
			else if(2==static_count_waitfor)return 7;
			else if(11 == static_count_waitfor) return 11;
			else if(9 == static_count_waitfor) return 9;
			else if(102 == static_count_waitfor) return 2;
			return 0;
			//static_count_waitfor++;
			
		}
		static int stub_MQTTSerialize_unsubscribe_ret10(unsigned char *buf, int buflen, unsigned char dup, unsigned short packetid,
								  int count, MQTTString topicFilters[]){return 10;}
		static int stub_MQTTSerialize_subscribe_ret10(unsigned char *buf, int buflen, unsigned char dup, unsigned short packetid, int count,
								MQTTString topicFilters[], int requestedQoSs[]){return 10;}
		static int stub_MQTTDeserialize_suback(unsigned short *packetid, int maxcount, int *count, int grantedQoSs[], unsigned char *buf, int buflen){
			*grantedQoSs = 0xa0;
			return 1;
		}
		static int stub_MQTTSerialize_connect_ret10(unsigned char *buf, int buflen, MQTTPacket_connectData *options){
			return 10;
		}
		static int static_ret_of_cycle = 0;
		static int stub_cycle(MQTTClient* c, Timer* timer){return static_ret_of_cycle;}
		static int stub_ThreadStart(Thread *thread, void (*fn)(void *), void *arg){return 0;}
		static int stub_MQTTSerialize_pingreq(unsigned char *buf, int buflen){
			return 1;
		}
		static int stub_TimerLeftMS(Timer *timer){
			return 0;
		}
		static mbedtls_ssl_context static_mbedtls_ssl_context_1;
	static mbedtls_ssl_context *stub_dtls_ssl_new(dtls_establish_info_s *info, char plat_type){
		return &static_mbedtls_ssl_context_1;
	}
	static int stub_atiny_snprintf(char *buf, unsigned int size, const char *format, ...){
		return 0;
	}
	static int stub_dtls_shakehand(mbedtls_ssl_context *ssl, const dtls_shakehand_info_s *info){
		return 0;
	}
	static int stub_dtls_shakehandFalse(mbedtls_ssl_context *ssl, const dtls_shakehand_info_s *info){
		return -1;
	}
	static void stub_dtls_ssl_destroy(mbedtls_ssl_context *ssl){}
	static void stub_atiny_net_close(void *ctx){}
	static int stub_atiny_net_recv_timeout_retneg2(void *ctx, unsigned char *buf, size_t len,uint32_t timeout){
		return -2;
	}
	static void stub_mbedtls_ssl_conf_read_timeout( mbedtls_ssl_config *conf, uint32_t timeout ){}
	static int stub_mbedtls_ssl_read_retneg0x6900( mbedtls_ssl_context *ssl, unsigned char *buf, size_t len ){
		return -0x6900;
	}
	static int stub_mbedtls_ssl_read_retneg0x6880( mbedtls_ssl_context *ssl, unsigned char *buf, size_t len ){
		return -0x6880;
	}
	static int stub_mbedtls_ssl_read_retneg0x6800( mbedtls_ssl_context *ssl, unsigned char *buf, size_t len ){
		return -0x6800;
	}
	static int stub_atiny_net_send_timeout(void *ctx, const unsigned char *buf, size_t len,uint32_t timeout){
		return 0;
	}
	static  int stub_dtls_write(mbedtls_ssl_context *ssl, const unsigned char *buf, size_t len){
		return 0;
	}
	static int stub_atiny_task_mutex_create_OK(atiny_task_mutex_s *mutex){
		return 0;
	}
	static int stub_atiny_task_mutex_lock_OK(atiny_task_mutex_s *mutex){
		return 0;
	}
	static int stub_atiny_task_mutex_unlock_OK(atiny_task_mutex_s *mutex){
		return 0;
	}
}//extern "C"

void TestMQTTLiteos::test_TimerInit()
{
    stubInfo si;
    setStub((void *)atiny_gettime_ms, (void *)stub_atiny_gettime_ms, &si);
    TimerInit(&t);
    TEST_ASSERT(t.end_time > 0);
    cleanStub(&si);
}
void TestMQTTLiteos::test_TimerIsExpired()
{
    char result = -1;
    
    stubInfo si;
    setStub((void *)atiny_gettime_ms, (void *)stub_atiny_gettime_ms, &si);
    result = TimerIsExpired(&t);
    TEST_ASSERT(result = TRUE);
    cleanStub(&si);
}
void TestMQTTLiteos::test_TimerCountdownMS()
{
    unsigned int timeout = 1;
    
    stubInfo si;
    setStub((void *)atiny_gettime_ms, (void *)stub_atiny_gettime_ms, &si);
    TimerCountdownMS(&t, timeout);
    TEST_ASSERT(t.end_time > timeout);
    cleanStub(&si);
}
void TestMQTTLiteos::test_TimerCountdown()
{
    unsigned int timeout = 1;
    
    stubInfo si;
    setStub((void *)atiny_gettime_ms, (void *)stub_atiny_gettime_ms, &si);
    TimerCountdown(&t, timeout);
    TEST_ASSERT(t.end_time > timeout * 1000);
    cleanStub(&si);
}
void TestMQTTLiteos::test_TimerLeftMS()
{
    int result = -1;
    
    stubInfo si;
    setStub((void *)atiny_gettime_ms, (void *)stub_atiny_gettime_ms, &si);
    result = TimerLeftMS(&t);
    TEST_ASSERT(result > 0);
    cleanStub(&si);
}
void TestMQTTLiteos::test_NetworkInit()
{
    Network n;
    
    memset(&n, sizeof(Network), 0);
    NetworkInit(NULL,NULL);

    NetworkInit(&n,NULL);
	t_test_get_security_info p_func;
	NetworkInit(&n,p_func);
    TEST_ASSERT(&n != NULL);
}
void TestMQTTLiteos::test_los_read()
{
	Network l_s_network_1;
	l_s_network_1.mqttread = test_mqttread;
	l_s_network_1.mqttwrite = test_mqttwrite;
	l_s_network_1.get_security_info = test_get_security_info;
	l_s_network_1.ctx = NULL;
	
	static_count_get_secuInfo = 0;
	int a;
	t_test_get_security_info p_func = (t_test_get_security_info)&a;
	NetworkInit(&l_s_network_1,p_func);
	///
	Network l_s_network_2;
	l_s_network_2.mqttread = test_mqttread;
	l_s_network_2.mqttwrite = test_mqttwrite;
	l_s_network_2.get_security_info = test_get_security_info;
	static_count_get_secuInfo = 0;
	l_s_network_2.ctx = NULL;
	char l_buffer_1[20];
	int l_len_1 = 10;
	///
	l_s_network_1.mqttread(NULL,NULL,0,0);

	stubInfo si_atiny_net_recv_timeout;
	setStub((void*)atiny_net_recv_timeout,(void*)stub_atiny_net_recv_timeout_retneg2,&si_atiny_net_recv_timeout);
	static_count_get_secuInfo = 0;
	(&l_s_network_1)->mqttread(&l_s_network_2,l_buffer_1,0,0);
	static_count_get_secuInfo = 1;
	
	(&l_s_network_1)->mqttread(&l_s_network_2,l_buffer_1,0,0);
	static_count_get_secuInfo = 1;

	mbedtls_ssl_context l_s_mbedtls_ssl_context_1;
	
	l_s_network_2.ctx = &l_s_mbedtls_ssl_context_1;
	stubInfo si_mbedtls_ssl_conf_read_timeout;
	stubInfo si_mbedtls_ssl_read;
	setStub((void*)mbedtls_ssl_read,(void*)stub_mbedtls_ssl_read_retneg0x6900,&si_mbedtls_ssl_read);
	setStub((void*)mbedtls_ssl_conf_read_timeout,(void*)stub_mbedtls_ssl_conf_read_timeout,&si_mbedtls_ssl_conf_read_timeout);
	
	(&l_s_network_1)->mqttread(&l_s_network_2,l_buffer_1,0,0);
	
	cleanStub(&si_mbedtls_ssl_read);
	static_count_get_secuInfo = 1;
	setStub((void*)mbedtls_ssl_read,(void*)stub_mbedtls_ssl_read_retneg0x6880,&si_mbedtls_ssl_read);
	(&l_s_network_1)->mqttread(&l_s_network_2,l_buffer_1,0,0);

	cleanStub(&si_mbedtls_ssl_read);
	static_count_get_secuInfo = 1;
	setStub((void*)mbedtls_ssl_read,(void*)stub_mbedtls_ssl_read_retneg0x6800,&si_mbedtls_ssl_read);
	(&l_s_network_1)->mqttread(&l_s_network_2,l_buffer_1,0,0);

	cleanStub(&si_mbedtls_ssl_read);


	
	cleanStub(&si_mbedtls_ssl_conf_read_timeout);
	static_count_get_secuInfo = 10;
	(&l_s_network_1)->mqttread(&l_s_network_2,l_buffer_1,0,0);
	cleanStub(&si_atiny_net_recv_timeout);
	
    
}
void TestMQTTLiteos::test_los_write()
{
	Network l_s_network_1;
	l_s_network_1.mqttread = test_mqttread;
	l_s_network_1.mqttwrite = test_mqttwrite;
	l_s_network_1.get_security_info = test_get_security_info;
	l_s_network_1.ctx = NULL;
	
	static_count_get_secuInfo = 0;
	int a;
	t_test_get_security_info p_func = (t_test_get_security_info)&a;
	NetworkInit(&l_s_network_1,p_func);
	///
	Network l_s_network_2;
	l_s_network_2.mqttread = test_mqttread;
	l_s_network_2.mqttwrite = test_mqttwrite;
	l_s_network_2.get_security_info = test_get_security_info;
	static_count_get_secuInfo = 0;
	l_s_network_2.ctx = NULL;
	char l_buffer_1[20];
	int l_len_1 = 10;
	l_s_network_1.mqttwrite(NULL,NULL,0,0);
	stubInfo si_atiny_net_send_timeout;
	stubInfo si_dtls_write;
	setStub((void*)atiny_net_send_timeout,(void*)stub_atiny_net_send_timeout,&si_atiny_net_send_timeout);
	setStub((void*)dtls_write,(void*)stub_dtls_write,&si_dtls_write);
	static_count_get_secuInfo = 0;
	l_s_network_1.mqttwrite(&l_s_network_2,l_buffer_1,0,0);
	static_count_get_secuInfo = 1;
	l_s_network_1.mqttwrite(&l_s_network_2,l_buffer_1,0,0);
	static_count_get_secuInfo = 10;
	l_s_network_1.mqttwrite(&l_s_network_2,l_buffer_1,0,0);
	cleanStub(&si_atiny_net_send_timeout);
	cleanStub(&si_dtls_write);
}
void TestMQTTLiteos::test_NetworkConnect()
{

    Network n;
    char* addr = "192.168.1.189";
    int port = 8000;
    int ret = 0;
    mqtt_context_t *ctx;
    
    memset(&n,  0, sizeof(Network));
    ctx = (mqtt_context_t *)malloc(sizeof(mqtt_context_t));
    NetworkInit(&n,NULL);


    ret = NetworkConnect(NULL, addr, port);
    TEST_ASSERT(ret == -1);

	Network l_s_network_1;
	l_s_network_1.mqttread = test_mqttread;
	l_s_network_1.mqttwrite = test_mqttwrite;
	l_s_network_1.get_security_info = test_get_security_info;
	static_l_s_msinfo_1.security_type = 0;
	stubInfo si_atiny_net_connect;
	setStub((void*)atiny_net_connect,(void*)stub_atiny_net_connect,&si_atiny_net_connect);// NULL
	ret = NetworkConnect(&l_s_network_1,addr,port);
	cleanStub(&si_atiny_net_connect);

	static_l_s_msinfo_1.security_type = 3;
	ret = NetworkConnect(&l_s_network_1,addr,port);
	
	static_l_s_msinfo_1.security_type = 2;
	uint8_t l_char_1[20];
	uint8_t l_char_2[20];
	uint8_t l_char_3[20];
	static_l_s_msinfo_1.u.psk.psk_id = l_char_1;
	static_l_s_msinfo_1.u.psk.psk = l_char_2;
	static_l_s_msinfo_1.u.ca.ca_crt = l_char_3;
	stubInfo	si_dtls_ssl_new;
	stubInfo si_atiny_snprintf;
	stubInfo si_dtls_shakehand;
	stubInfo si_dtls_ssl_destroy;
	setStub((void*)dtls_ssl_new,(void*)stub_dtls_ssl_new,&si_dtls_ssl_new);
	setStub((void*)atiny_snprintf,(void*)stub_atiny_snprintf,&si_atiny_snprintf);
	setStub((void*)dtls_shakehand,(void*)stub_dtls_shakehand,&si_dtls_shakehand);
	setStub((void*)dtls_ssl_destroy,(void*)stub_dtls_ssl_destroy,&si_dtls_ssl_destroy);
	static_count_get_secuInfo = 1;
	static_secu_value_for_ifelse = 1;
	ret = NetworkConnect(&l_s_network_1,addr,port);
	static_count_get_secuInfo = 1;
	static_secu_value_for_ifelse = 2;
	ret = NetworkConnect(&l_s_network_1,addr,port);

	cleanStub(&si_dtls_shakehand);
	setStub((void*)dtls_shakehand,(void*)stub_dtls_shakehandFalse,&si_dtls_shakehand);
	static_count_get_secuInfo = 1;
	static_secu_value_for_ifelse = 2;
	ret = NetworkConnect(&l_s_network_1,addr,port);
	cleanStub(&si_dtls_ssl_destroy);
	cleanStub(&si_dtls_ssl_new);
	cleanStub(&si_atiny_snprintf);
	cleanStub(&si_dtls_shakehand);	
}
void TestMQTTLiteos::test_mbedtls_net_set_block()
{
    mbedtls_net_context ctx;
    int ret = 0;

    memset(&ctx,  0, sizeof(mbedtls_net_context));
    ctx.fd = 3;
    ret = mbedtls_net_set_block(&ctx);
    TEST_ASSERT(ret == 0);
}
void TestMQTTLiteos:: test_mbedtls_net_set_nonblock()
{
    mbedtls_net_context ctx;
    int ret = 0;

    memset(&ctx,  0, sizeof(mbedtls_net_context));
    ctx.fd = 3;
    ret = mbedtls_net_set_nonblock(&ctx);
    TEST_ASSERT(ret == 0);
}
void TestMQTTLiteos::test_NetworkDisconnect()
{
    Network n;
    int ret = 0;
    memset(&n, 0, sizeof(Network));
    NetworkDisconnect(NULL);
	
	Network l_s_network_1;
	l_s_network_1.mqttread = test_mqttread;
	l_s_network_1.mqttwrite = test_mqttwrite;
	l_s_network_1.get_security_info = test_get_security_info;
	l_s_network_1.ctx = NULL;
	
	static_count_get_secuInfo = 0;
	stubInfo si_atiny_net_close;//void atiny_net_close(void *ctx)
	setStub((void*)atiny_net_close,(void*)stub_atiny_net_close,&si_atiny_net_close);
	NetworkDisconnect(&l_s_network_1);

	static_count_get_secuInfo = 1;
	
	stubInfo si_dtls_ssl_destroy;
	setStub((void*)dtls_ssl_destroy,(void*)stub_dtls_ssl_destroy,&si_dtls_ssl_destroy);
	NetworkDisconnect(&l_s_network_1);
	cleanStub(&si_atiny_net_close);
	cleanStub(&si_dtls_ssl_destroy);

}
void TestMQTTLiteos::test_MutexDestory(){
	Mutex a;
	stubInfo si_atiny_task_mutex_delete;
	setStub((void*)atiny_task_mutex_delete,(void*)stub_atiny_task_mutex_delete,&si_atiny_task_mutex_delete);
		//
	MutexDestory(&a);
	cleanStub(&si_atiny_task_mutex_delete);
	ThreadStart(NULL,NULL,NULL);
}
void TestMQTTLiteos::test_mutexInit(){
	stubInfo si_mutex_create;
	setStub((void*)atiny_task_mutex_create,(void*)stub_atiny_task_mutex_create_OK,&si_mutex_create);
	MutexInit(NULL);
	cleanStub(&si_mutex_create);
}
void TestMQTTLiteos::test_mutexLock(){
	stubInfo si_atiny_task_mutex_lock;
	setStub((void*)atiny_task_mutex_lock,(void*)stub_atiny_task_mutex_lock_OK,&si_atiny_task_mutex_lock);
	MutexLock(NULL);
	cleanStub(&si_atiny_task_mutex_lock);
}
void TestMQTTLiteos::test_mutexunlock(){
	stubInfo si_atiny_task_mutex_unlock;
	setStub((void*)atiny_task_mutex_unlock,(void*)stub_atiny_task_mutex_unlock_OK,&si_atiny_task_mutex_unlock);
	MutexUnlock(NULL);
	cleanStub(&si_atiny_task_mutex_unlock);
}



TestMQTTLiteos::TestMQTTLiteos()
{
    TEST_ADD(TestMQTTLiteos::test_TimerInit);
    TEST_ADD(TestMQTTLiteos::test_TimerIsExpired);
    TEST_ADD(TestMQTTLiteos::test_TimerCountdownMS);
    TEST_ADD(TestMQTTLiteos::test_TimerCountdown);
    TEST_ADD(TestMQTTLiteos::test_TimerLeftMS);
    TEST_ADD(TestMQTTLiteos::test_NetworkInit);
   // 
    TEST_ADD(TestMQTTLiteos::test_NetworkConnect);
    TEST_ADD(TestMQTTLiteos::test_mbedtls_net_set_block);
    TEST_ADD(TestMQTTLiteos::test_mbedtls_net_set_nonblock);
 
     TEST_ADD(TestMQTTLiteos::test_MutexDestory);
    TEST_ADD(TestMQTTLiteos::test_NetworkDisconnect);
	TEST_ADD(TestMQTTLiteos::test_los_read);
	TEST_ADD(TestMQTTLiteos::test_los_write);
	TEST_ADD(TestMQTTLiteos::test_mutexInit);
	TEST_ADD(TestMQTTLiteos::test_mutexLock);
	TEST_ADD(TestMQTTLiteos::test_mutexunlock);
}

void TestMQTTLiteos::setup()
{
    //printf("TestMQTTLiteos:come into test func %d\n",++i);
    return;
}
void TestMQTTLiteos::tear_down()
{
    //printf("TestMQTTLiteos:exit from test func %d\n",i);
    return ;
}

