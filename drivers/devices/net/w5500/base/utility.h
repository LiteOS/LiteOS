/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd> 

 ******************************************************************************
  File Name     : utility.h
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/7/8
  Last Modified :
  Description   : utility.c header file
  Function List :
  History       :
  1.Date        : 2018/7/8
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/

#ifndef __UTILITY_H__
#define __UTILITY_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* Includes -----------------------------------------------------------------*/
#include "stdio.h"
#include "types.h"

/* Defines ------------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/

uint16 atoi16(char* str,uint16 base); 			/* Convert a string to integer number */
uint32 atoi32(char* str,uint16 base); 			/* Convert a string to integer number */
void itoa(uint16 n,uint8* str, uint8 len);
int validatoi(char* str, int base, int* ret); 		/* Verify character string and Convert it to (hexa-)decimal. */
char c2d(u_char c); 					/* Convert a character to HEX */

uint16 swaps(uint16 i);
uint32 swapl(uint32 l);

void replacetochar(char * str, char oldchar, char newchar);

void mid(int8* src, int8* s1, int8* s2, int8* sub);
void inet_addr_(unsigned char* addr,unsigned char *ip);


char* inet_ntoa(unsigned long addr);			/* Convert 32bit Address into Dotted Decimal Format */
char* inet_ntoa_pad(unsigned long addr);

/* Converts a string containing an (Ipv4) Internet Protocol decimal dotted address into a 32bit address */
uint32 inet_addr(unsigned char* addr);		

char verify_ip_address(char* src, uint8 * ip);/* Verify dotted notation IP address string */

/* htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).*/
uint16 htons( unsigned short hostshort);
/* htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian). */
uint32 htonl(unsigned long hostlong);
/* ntohs function converts a unsigned short from TCP/IP network byte order to host byte order 
    (which is little-endian on Intel processors). */
uint32 ntohs(unsigned short netshort);
/* ntohl function converts a u_long from TCP/IP network order to host byte order 
    (which is little-endian on Intel processors). */
uint32 ntohl(unsigned long netlong);

uint16 checksum(unsigned char * src, unsigned int len);		/* Calculate checksum of a stream */

uint8 check_dest_in_local(u_long destip);			/* Check Destination in local or remote */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __UTILITY_H__ */
