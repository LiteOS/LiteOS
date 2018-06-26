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
 #ifndef __M8266_WIFI_SPI_IF_H__
 #define __M8266_WIFI_SPI_IF_H__
 
 //#include "stm32f1xx.h"
 //#include "dwt.h"
 #include "stdio.h"
 #include "stdint.h"
 
 /*--------------------data structure----------------------*/
 typedef struct
 { 
	  /* During connecting phase(connect to a server) , fd is needed in atiny_socket.c. Assign one here
       and make it ordered like 0 , 1 , 2 , 3 because multi connection is needed */
		int32_t link_fd;
		/* when a queue is created , this variable is used to store id of the queue */
		uint32_t queue_id;
		/* To indicate whether this link is vailable or not */
		uint8_t busy_flag;
 }link_params;
 
 typedef struct
 {
		uint32_t len;
    uint8_t *addr;
 }queue_element;
 
 typedef struct
 {
		char * name;
		uint32_t max_link_no;
		link_params * p_link;
		int32_t  (*module_init)( void );
		int32_t  (*connect)( const char * host, const char * port , int32_t proto );
		int32_t  (*send)( int32_t id , const uint8_t  *buf, uint32_t len );
		int32_t  (*recv)( int32_t id , uint8_t * buf, uint32_t len);
	  int32_t  (*recv_timeout)( int32_t id , uint8_t * buf, size_t len, uint32_t timeout );
	  int32_t  (*close)( int32_t id );
	  uint8_t  (*check_link_params)(void);
 }wifi_spi_interface_params;


 /*------------------function prototypes------------------*/
 int32_t wifi_spi_if_register(wifi_spi_interface_params * p_wifi_spi_struct);
 int32_t wifi_spi_if_init(void);
 int32_t wifi_spi_if_send( int32_t id , const uint8_t * buf , uint32_t len );
 int32_t wifi_spi_if_recv( int32_t id , uint8_t * buf , size_t len );
 int32_t wifi_spi_if_recv_timeout(int32_t id , uint8_t * buf , size_t len , uint32_t timeout);
 int32_t wifi_spi_if_connect(const char * host , const char * port , int proto);
 int32_t wifi_spi_if_close(int32_t fd);
 uint8_t wifi_spi_if_check_link_params(void);
 
 /* this variable will be initialized in M8266_WIFI_SPI.c */
 extern wifi_spi_interface_params wifi_spi_if_instance;
 #endif
 
 

 