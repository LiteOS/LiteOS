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

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __W5500_CONF_H__
#define __W5500_CONF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* Includes -----------------------------------------------------------------*/
#include "stdio.h"
#include "types.h"

#include "stm32f1xx_hal.h"

/* Defines ------------------------------------------------------------------*/
#define WIZ_MAX_SOCKET_NUM          8     // (1-8)
#define WIZ_RX_BUFFER_SIZE          2048

/* Socket port 
 * The W5500 has one general register, eight Socket registers, and a read/write cache for each Socket.
 * so w5500 support 8 socket port(0-8)
 */
#define SOCK_TCPS             0
#define SOCK_HUMTEM			  0
#define SOCK_PING			  0
#define SOCK_TCPC             1
#define SOCK_UDPS             2
#define SOCK_WEIBO      	  2
#define SOCK_DHCP             3
#define SOCK_HTTPS            4
#define SOCK_DNS              5
#define SOCK_SMTP             6
#define SOCK_NTP              7
//#define NETBIOS_SOCK    6 //defined in netbios.c

/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
typedef  void (*pFunction)(void);

typedef __PACKED_STRUCT _CONFIG_MSG											
{
    uint8 mac[6];		    /* MAC address      */
    uint8 lip[4];			/* local IP         */
    uint8 sub[4];			/* subnet mask      */
    uint8 gw[4];			/* gateway          */	
    uint8 dns[4];			/* DNS server address */
    uint8 rip[4];			/* remote IP        */
    uint8 sw_ver[2];		/* software version */
}CONFIG_MSG;

typedef __PACKED_STRUCT _EEPROM_MSG	                    
{
    uint8 mac[6];
    uint8 lip[4];
    uint8 sub[4];
    uint8 gw[4];
}EEPROM_MSG_STR;

/* Extern variables ---------------------------------------------------------*/
extern uint8    remote_ip[4];
extern uint16   remote_port;
extern uint8    local_ip[4];
extern uint16   local_port;

extern uint8    use_dhcp;
extern uint8    use_eeprom;

extern uint8	ip_from;

extern EEPROM_MSG_STR EEPROM_MSG;
extern CONFIG_MSG  	ConfigMsg;

extern uint8    dhcp_ok;
extern uint32   dhcp_time;
extern vuint8	ntptimer;

/* Defines ------------------------------------------------------------------*/
#define FW_VER_HIGH  			1               
#define FW_VER_LOW    			0
#define ON	                 	1
#define OFF	                 	0
#define HIGH	           	 	1
#define LOW		             	0

#define MAX_BUF_SIZE		 	1460       		/* Single packet data size */
#define KEEP_ALIVE_TIME	     	30	// 30sec
#define TX_RX_MAX_BUF_SIZE      2048							 
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

#define IP_FROM_DEFINE	        0
#define IP_FROM_DHCP	        1
#define IP_FROM_EEPROM	        2

/* Functions API ------------------------------------------------------------*/
void reboot(void);
void write_config_to_eeprom(void);
void read_config_from_eeprom(void);

/* W5500 SPI */
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);
uint8 IINCHIP_READ(uint32 addrbsb);
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);

/* w5500 config function */
void w5500_init(void);
void w5500_deinit(void);
void w5500_reset(void);
void w5500_set_mac(void);
void w5500_set_ip(void);

/* timer */
void dhcp_timer_init(void);
void ntp_timer_init(void);

/* wiz delay function */
void wizDelayUs(uint32 usec);
void wizDelayMs(uint32 msec);
void wizDelayS(uint32 sec);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __W5500_CONF_H__ */
