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
#include "stm32f1xx_hal.h"
#include "w5500.h"
#include "w5500_conf.h"
#include "utility.h"
#include "socket.h"
#include "dhcp.h"
#include "tcp_demo.h"
#include "udp_demo.h"
#include "dns.h"
#include "smtp.h"
#include "http_server.h"
#include "http_client.h"
#include "netbios.h"
#include "ntp.h"
#include "ping.h"

/* Defines ------------------------------------------------------------------*/

#define TEST_TCP_CLIENT     0
#define TEST_TCP_SERVER     1
#define TEST_DHCP_CLIENT    2
#define TEST_UDP            3
#define TEST_DNS            4
#define TEST_SMTP           5
#define TEST_HTTP_SERVER    6
#define TEST_HTTP_CLIENT    7
#define TEST_NETBIOS        8
#define TEST_NTP            9
#define TEST_PING           10

#define USE_TEST_TYPE       TEST_HTTP_SERVER
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
uint8 reboot_flag = 0;

/* Private function prototypes ----------------------------------------------*/
#if (USE_TEST_TYPE == TEST_TCP_CLIENT)
void w5500_tcp_client(void)
{
    printf("\r\nw5500 TCP Client Demo V1.0\r\n");
    
    printf(" server IP  : %d.%d.%d.%d\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
	printf(" server port: %d \n",remote_port);
    
    while(1)
    {
        do_tcp_client();
    }
}
#endif

#if (USE_TEST_TYPE == TEST_TCP_SERVER)
void w5500_tcp_server(void)
{
    printf("\r\nw5500 TCP Server Demo V1.0\r\n");
    
    printf(" server IP  : %d.%d.%d.%d\n",local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	printf(" listen port: %d \n",local_port);
    
    while(1)
    {
        do_tcp_server();
    }
}
#endif

#if (USE_TEST_TYPE == TEST_DHCP_CLIENT)
// TEST FAILED
void w5500_dhcp_client(void)
{
    printf("\r\nw5500 DHCP Client Demo V1.0\r\n");
    
    dhcp_timer_init();
    init_dhcp_client();
    
    while(1)
    {
        do_dhcp();
    }
}
#endif

#if (USE_TEST_TYPE == TEST_UDP)
void w5500_udp(void)
{
    printf("\r\nw5500 UDP Demo V1.0\r\n");
        
    printf(" local  port: %d \n",local_port);
    printf(" remote port: %d \n",remote_port);
    
    while(1)
    {
        do_udp();
    }
}
#endif

#if (USE_TEST_TYPE == TEST_DNS)
void w5500_dns(void)
{
    printf("\r\nw5500 DNS Demo V1.0\r\n");

    printf(" parse the IP of %s\r\n",domain_name); 
    
    while(1)
    {
        do_dns();
        wizDelayMs(1000);
    }
}
#endif

#if (USE_TEST_TYPE == TEST_SMTP)
void w5500_smtp(void)
{
    printf("\r\nw5500 DNS Demo V1.0\r\n");
    
	printf(" parse ip of smtp.126.com \n");
	printf(" from mailbox:%s \n",from);
	printf(" to   mailbox:%s \n",to);
  
	mail_message();	
	while(1) 
	{
		do_dns();/* parse dns of smtp.126.com*/
		do_smtp(); /* send mail */
		if(mail_send_ok)
        {
    	    while(1);
        }
	}
}
#endif

#if (USE_TEST_TYPE == TEST_HTTP_SERVER)
void w5500_http_server(void)
{
    printf("\r\nw5500 TCP Server Demo V1.0\r\n");
        
	printf(" input the IP of w5500 in browser: %d.%d.%d.%d to access web Server \n",
          ConfigMsg.lip[0],ConfigMsg.lip[1],ConfigMsg.lip[2],ConfigMsg.lip[3]);
  
    mail_message(); 
    while(1) 
    {
        do_https();
        if(1 == reboot_flag)
        {
            reboot();
        }
    }
}
#endif

#if (USE_TEST_TYPE == TEST_HTTP_CLIENT)
void w5500_http_client(void)
{
    printf("\r\nw5500 HTTP Client Demo V1.0\r\n");
    
    printf(" YEElink server ip  :%d.%d.%d.%d\r\n",yeelink_server_ip[0],yeelink_server_ip[1],yeelink_server_ip[2],yeelink_server_ip[3]);
    printf(" YEElink server port:%d \r\n",yeelink_port);
    printf(" my Yeilink ID      :%s \r\n",yeelink_id);
    printf(" my Yeilink Password:%s \r\n",yeelink_pwd);
  
    while(1)  
    {
        do_http_client();   
    }
}
#endif

#if (USE_TEST_TYPE == TEST_NETBIOS)
void w5500_netbios(void)
{
    printf("\r\nw5500 NetBios Demo V1.0\r\n");

    dhcp_timer_init();
    init_dhcp_client();
    
    while(1)
    {
        do_dhcp(); 
        if(dhcp_ok == 1)
        {
            do_https();
            do_netbios();
        }
    }
}
#endif

#if (USE_TEST_TYPE == TEST_NTP)
void w5500_ntp(void)
{
    printf("\r\nw5500 NetBios Demo V1.0\r\n");
    
    printf(" NTP server ip  :%d.%d.%d.%d\r\n",ntp_server_ip[0],ntp_server_ip[1],ntp_server_ip[2],ntp_server_ip[3]);
    printf(" NTP server port:%d \r\n",ntp_port);
    
    ntp_client_init();
    dhcp_timer_init();
    init_dhcp_client();
    
    while(1)
    {
        do_dhcp();
        if(dhcp_ok == 1)
        {
            do_ntp_client();
        } 
    }
}
#endif

#if (USE_TEST_TYPE == TEST_PING)
void w5500_ping(void)
{
    printf("\r\nw5500 Ping Demo V1.0\r\n");
    
    while(1)
	{
        do_ping();
		if(req >= 4) break;	
	}
}
#endif
/* Public functions ---------------------------------------------------------*/
void w5500_demo(void)
{
    printf("\r\nThis is a w5500 demo...\r\n");

    w5500_init();
    w5500_set_mac();
#if (USE_TEST_TYPE != TEST_DHCP_CLIENT && USE_TEST_TYPE != TEST_NETBIOS)    
    w5500_set_ip();
#endif

    socket_buf_init(txsize, rxsize);

#if (USE_TEST_TYPE == TEST_TCP_CLIENT)
    w5500_tcp_client();
#elif (USE_TEST_TYPE == TEST_TCP_SERVER)
    w5500_tcp_server();
#elif (USE_TEST_TYPE == TEST_DHCP_CLIENT)
    w5500_dhcp_client();
#elif (USE_TEST_TYPE == TEST_UDP)
    w5500_udp();
#elif (USE_TEST_TYPE == TEST_DNS)
    w5500_dns();
#elif (USE_TEST_TYPE == TEST_SMTP)
    w5500_smtp();
#elif (USE_TEST_TYPE == TEST_HTTP_SERVER)
    w5500_http_server();
#elif (USE_TEST_TYPE == TEST_HTTP_CLIENT)
    w5500_http_client();
#elif (USE_TEST_TYPE == TEST_NETBIOS)
    w5500_netbios();
#elif (USE_TEST_TYPE == TEST_NTP)
    w5500_ntp();
#elif (USE_TEST_TYPE == TEST_PING)
    w5500_ping();
#endif
}
/* Private functions --------------------------------------------------------*/

