/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd> 

 ******************************************************************************
  File Name     : socket.h
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/7/14
  Last Modified :
  Description   : socket.c header file

                  The W5500 has one general register, 
                  eight Socket registers, and a read/write cache for each Socket.
                  so w5500 support 8 socket port(0-8)
  Function List :
  History       :
  1.Date        : 2018/7/14
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __SOCKET_H__
#define __SOCKET_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* Includes -----------------------------------------------------------------*/
#include "types.h"

/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/

uint8  socket(SOCKET s, uint8 protocol, uint16 port, uint8 flag); // Opens a socket(TCP or UDP or IP_RAW mode)
void   close(SOCKET s); // Close socket
uint8  connect(SOCKET s, uint8 * addr, uint16 port); // Establish TCP connection (Active connection)
void   disconnect(SOCKET s); // disconnect the connection
uint8  listen(SOCKET s);	// Establish TCP connection (Passive connection)
uint16 send(SOCKET s, const uint8 * buf, uint16 len); // Send data (TCP)
uint16 recv(SOCKET s, uint8 * buf, uint16 len);	// Receive data (TCP)
uint16 sendto(SOCKET s, const uint8 * buf, uint16 len, uint8 * addr, uint16 port); // Send data (UDP/IP RAW)
uint16 recvfrom(SOCKET s, uint8 * buf, uint16 len, uint8 * addr, uint16  *port); // Receive data (UDP/IP RAW)

#ifdef __MACRAW__
void   macraw_open(void);
uint16 macraw_send( const uint8 * buf, uint16 len ); //Send data (MACRAW)
uint16 macraw_recv( uint8 * buf, uint16 len ); //Recv data (MACRAW)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SOCKET_H__ */
