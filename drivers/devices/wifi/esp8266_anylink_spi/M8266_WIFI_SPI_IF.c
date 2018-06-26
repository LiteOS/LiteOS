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
 
 #include "M8266_WIFI_SPI_IF.h"
 #include "M8266_WIFI_SPI_PRINT.h"
 
 /* this static variable is a pointer that points to wifi_spi_if_instance */
 static wifi_spi_interface_params * p_global_wifi_spi_if_instance = NULL;
 
 /**
	 *@brief this function registers wifi_spi interface(make p_wifi_struct points to a variable containing all stuff 
	         needed to control wifi module using spi)
   *@params p_wifi_spi_struct---> a pointer that points to wifi_spi_interface_params variable
   *@ret 1--->OK 0--->fail	 
 */
 int32_t wifi_spi_if_register(wifi_spi_interface_params * p_wifi_spi_struct)
 {
	  /* param check */
		if( p_wifi_spi_struct == NULL )
		{
			print_log(" can not register a empty module interface ");
			return 0;
		}
	 
		/* register only when p_global_wifi_spi_if_variable has never been registered before */
		if( p_global_wifi_spi_if_instance == NULL )
		{
			p_global_wifi_spi_if_instance = p_wifi_spi_struct;
			print_log("wifi module registered OK");
		}
		
		return 1;
 }
 
 
 /**
	 *@brief this function is exported to user to init wifi module
	 *@params none
   *@ret 1--->OK 0/-1--->fail 
 */
 int32_t wifi_spi_if_init(void)
 {
		if( p_global_wifi_spi_if_instance != NULL && p_global_wifi_spi_if_instance->module_init != NULL )
		{
			/* 1:wifi module init OK 0:wifi module init fail */
			return p_global_wifi_spi_if_instance->module_init(); 
		}
		else
		{
			/* -1:wifi module init function is not executed */
			print_log(" wifi module init failed ");
			return -1;
		}
 }
 

 /**
	 *@brief this function is exported to user to send data to wifi module
   *@params id---> this is a uint32_t value to mark socket and the id will be assigned when connecting to server
            buf---> a pointer that points to data buffer
            len---> length of data to be sent
   *@ret    bytes of data sent(not 0) 0--->errors may happened
 */
 int32_t wifi_spi_if_send( int32_t id , const uint8_t  *buf, uint32_t len )
 {
		int32_t ret;

		if( p_global_wifi_spi_if_instance != NULL && p_global_wifi_spi_if_instance->send != NULL)
		{
			ret = p_global_wifi_spi_if_instance->send(id , buf , len);
			
			/* send 0 byte: should return */
			if(ret == 0)
			{
				print_log(" num of data sent is 0!! ");
			}
			else if(ret == -1)
			{
				print_log(" num of data required to send is illegal!! ");
			}
			else
			{
				print_log(" num of data sent is %d " , ret);
			}
		}
		
		return ret;
 }
 
 
 /**
	 *@brief this function is used to receive data from server(blocked func---> blocked for unlimited time)
   *@params id---> assigned during connection phase
            buf---> points to space to store data
            len---> length of data required to be retrieved
   *@ret    authentic bytes of data received
 */
 int32_t wifi_spi_if_recv( int32_t id , uint8_t * buf, size_t len )
 {
		int32_t ret;
	 
		if( p_global_wifi_spi_if_instance != NULL && p_global_wifi_spi_if_instance->recv != NULL)
		{
			ret = p_global_wifi_spi_if_instance->recv( id , buf , len  );
			
			/* receive 0 byte: should return*/
			if(ret == 0)
			{
				print_log(" num of data received is 0!!");
				return ret;
			}
			else if( ret == -1 )
			{
				print_log("receive without timeout failed");
			}
			else
			{
				print_log(" num of data received is %d " , ret);
			}
		}
		
		return ret;
 }
 
 
 /**
   *@brief this function is another version of WIFI_SPI_If_Recv(blocked for limited time)
   *@params timeout---> blocked for timeout ticks
   *@ret num of bytes received or 0---> fail
 */
 int32_t wifi_spi_if_recv_timeout(int32_t id, unsigned char * buf, size_t len, uint32_t timeout)
 {
		int32_t ret;
	 
		if( p_global_wifi_spi_if_instance != NULL && p_global_wifi_spi_if_instance->recv_timeout != NULL)
		{
			ret = p_global_wifi_spi_if_instance->recv_timeout(id , buf , len , timeout);
			
			if( ret == 0 )
			{
				print_log(" timeout version:num of data received is 0!! ");
				return ret;
			}
			else if(ret == -1)
			{
				print_log("reveive with timeout failed");
			}
			else
			{
				print_log(" timeout verison:num of data received is %d " , ret);
			}
		}
		
		return ret;
 }
 
 
 /**
	 *@brief  this function is used to connect to a server
	 *@breif  host--->ip address or DNS port--->remote port proto---> protocal
   *@ret    return link number as fd
 */
 int32_t wifi_spi_if_connect(const char* host, const char* port, int proto)
 {
		int ret = -1;
	 
		if( p_global_wifi_spi_if_instance != NULL && p_global_wifi_spi_if_instance->connect != NULL)
		{
			ret = p_global_wifi_spi_if_instance->connect( host , port , proto );
			/* -1 indicates error bacause link no is between 0~3 */
			if(ret == -1)
			{
				print_log(" connect fail:mostly because connection has reached its climax(multi connection) ");
			}
			else
			{
				print_log("connect OK:link no(fd) is %d" , ret);
			}
		}
		
		return ret;
 }
 
 
 /**
	 *@brief close a connection according to fd(link_no assigned in connection phase)
   *@params fd---> link_no
   *@ret 1--->OK 0--->fail
 */
 int32_t wifi_spi_if_close(int32_t fd)
 {
	  int ret;
	 
		if( p_global_wifi_spi_if_instance != NULL && p_global_wifi_spi_if_instance->close != NULL)
		{
			ret = p_global_wifi_spi_if_instance->close(fd);
			
			if( ret == 0 )
			{
				print_log("close connection %d fail " , fd);
			}
			else
			{
				print_log("close connecton %d OK" , fd);
			}
		}
		
		return ret;
 }
 
 
 /**
	 *@brief check current link status
   *@params none
   *@ret 1--->OK 0--->fail
 */
 uint8_t wifi_spi_if_check_link_params()
 {
		int ret;
	 
		if( p_global_wifi_spi_if_instance != NULL && p_global_wifi_spi_if_instance->check_link_params != NULL )
		{
			ret = p_global_wifi_spi_if_instance->check_link_params();
		}
		else
		{
			print_log(" function is not found");
			ret = 0;
		}
		
		return ret;
 }
 
 
 
 
 
 
 