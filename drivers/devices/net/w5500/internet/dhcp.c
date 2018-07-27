/**
******************************************************************************
* @file    			DHCP.c
* @author  			WIZnet Software Team 
* @version 			V1.0
* @date    			2015-02-14
* @brief   			DHCP	Client
* @attention		使用DHCP配置IP地址需要定义
******************************************************************************
*/
#include <stdio.h>
#include <string.h>	

#include "w5500.h"
#include "socket.h"

#include "dhcp.h"
#include "utility.h"
#include "w5500_conf.h"

//#define DHCP_DEBUG

DHCP_Get DHCP_GET;

uint8*   SRC_MAC_ADDR   = EXTERN_DHCP_MAC;				/*本地MAC地址*/
uint8*   GET_SN_MASK    = EXTERN_DHCP_SN;					/*从DHCP服务器获取到的子网掩码*/
uint8*   GET_GW_IP      = EXTERN_DHCP_GW;					/*从DHCP服务器获取到的网关地址*/
uint8*   GET_DNS_IP     = EXTERN_DHCP_DNS;				/*从DHCP服务器获取到的DNS服务器地址*/
uint8*   DHCP_HOST_NAME = EXTERN_DHCP_NAME;   		/*主机名*/
uint8*   GET_SIP        = EXTERN_DHCP_SIP;				/*从DHCP服务器获取到的本机IP*/
uint8    DHCP_SIP[4] = {0,};											/*已发现的DNS服务器地址*/
uint8    DHCP_REAL_SIP[4] = {0,}; 								/*从DHCP列表中选择使用的DHCP服务器*/
uint8    OLD_SIP[4];        											/*最初获取到的IP地址*/

uint8		dhcp_state       = STATE_DHCP_READY;       /*DHCP客户端状态*/
uint8		dhcp_retry_count = 0;                      /*DHCP重试次数*/
uint8		DHCP_timeout     = 0;                      /*DHCP超时标志位*/
uint32  dhcp_lease_time;													 /*租约时间*/
uint32	next_dhcp_time  = 0;											/*DHCP超时时间*/
uint32	dhcp_tick_cnt   = 0;                   	  
uint8		DHCP_timer;

uint8 Conflict_flag = 0;
uint32  DHCP_XID        = DEFAULT_XID;				
uint8 EXTERN_DHCPBUF[1024];
RIP_MSG*  pRIPMSG = (RIP_MSG*)EXTERN_DHCPBUF;			/*DHCP消息指针*/


void  send_DHCP_DISCOVER(void);										/*向DHCP服务器发送发现报文*/
void  send_DHCP_REQUEST(void);										/*向DHCP服务器发送请求报文*/
void  send_DHCP_RELEASE_DECLINE(char msgtype);		/*向DHCP服务器发送释放报文*/

void  reset_DHCP_time(void);        							/*初始化DHCP计时器*/
void  DHCP_timer_handler(void);     							/*DHCP计时器*/
void  check_DHCP_Timeout(void);     							/*检查是否超时*/
uint8 check_leasedIP(void);												/*检查获取的IP是否冲突*/
uint8 parseDHCPMSG(uint16 length);								/*从DHCP服务器接收消息并解析*/

/**
*@brief		DHCP定时初始化
*@param		无
*@return	无
*/
void reset_DHCP_time(void)
{
	dhcp_time = 0;
	dhcp_tick_cnt = 0;
	next_dhcp_time = dhcp_time + DHCP_WAIT_TIME;
	dhcp_retry_count = 0;
}

/**
*@brief		发送DISCOVER信息给DHCP服务器
*@param		无
*@return	无
*/
void send_DHCP_DISCOVER(void)
{
	uint8 ip[4] = {255,255,255,255};
	uint16 i=0;
	//the host name modified
	uint8 host_name[12];
	//*((uint32*)DHCP_SIP)=0;
	//*((uint32*)DHCP_REAL_SIP)=0;
	memset((void*)pRIPMSG,0,sizeof(RIP_MSG));				/*清空pRIPMSG的 sizeof(RIP_MSG)	个空间*/
	
	pRIPMSG->op = DHCP_BOOTREQUEST;
	pRIPMSG->htype = DHCP_HTYPE10MB;
	pRIPMSG->hlen = DHCP_HLENETHERNET;
	pRIPMSG->hops = DHCP_HOPS;
	pRIPMSG->xid = htonl(DHCP_XID);
	pRIPMSG->secs = htons(DHCP_SECS);
	pRIPMSG->flags =htons(DHCP_FLAGSBROADCAST);
	pRIPMSG->chaddr[0] = SRC_MAC_ADDR[0];
	pRIPMSG->chaddr[1] = SRC_MAC_ADDR[1];
	pRIPMSG->chaddr[2] = SRC_MAC_ADDR[2];
	pRIPMSG->chaddr[3] = SRC_MAC_ADDR[3];
	pRIPMSG->chaddr[4] = SRC_MAC_ADDR[4];
	pRIPMSG->chaddr[5] = SRC_MAC_ADDR[5];    
	
	
	/* MAGIC_COOKIE */
	pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 24)& 0xFF);
	pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 16)& 0xFF);
	pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 8)& 0xFF);
	pRIPMSG->OPT[i++] = (uint8)(MAGIC_COOKIE& 0xFF);
	
	/* Option Request Param. */
	pRIPMSG->OPT[i++] = dhcpMessageType;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = DHCP_DISCOVER;
	
	/*Client identifier*/
	pRIPMSG->OPT[i++] = dhcpClientIdentifier;
	pRIPMSG->OPT[i++] = 0x07;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[0];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[1];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[2];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];
	
	// host name
	pRIPMSG->OPT[i++] = hostName;
	// set the host name
	sprintf((char*)host_name,"%.4s-%02X%02X%02X",DEVICE_ID,SRC_MAC_ADDR[3],SRC_MAC_ADDR[4],SRC_MAC_ADDR[5]);
	
	
	pRIPMSG->OPT[i++] = (uint8)strlen((char*)host_name);
	
	strcpy((char*)(&(pRIPMSG->OPT[i])),(char*)host_name);
	
	
	i+=(uint16)strlen((char*)host_name);	
	
	pRIPMSG->OPT[i++] = dhcpParamRequest;
	pRIPMSG->OPT[i++] = 0x06;
	pRIPMSG->OPT[i++] = subnetMask;
	pRIPMSG->OPT[i++] = routersOnSubnet;
	pRIPMSG->OPT[i++] = dns;
	pRIPMSG->OPT[i++] = domainName;
	pRIPMSG->OPT[i++] = dhcpT1value;
	pRIPMSG->OPT[i++] = dhcpT2value;
	pRIPMSG->OPT[i++] = endOption;
	
	/* send broadcasting packet */
	//printf("send dhcp discover %s\r\n",EXTERN_DHCPBUF);
	//for(uint8 i=0; i<3; i++)
	//Delay_ms(800);
	sendto(SOCK_DHCP, (uint8 *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT);	
	
	#ifdef DHCP_DEBUG	
	printf("sent DHCP_DISCOVER\r\n");
	#endif	
}

/**
*@brief		将请求消息发送到DHCP服务器
*@param		无
*@return	无
*/
void send_DHCP_REQUEST(void)
{
	uint8  ip[4];
	uint16 i = 0;
	//uint16 len=0;
	uint8  host_name[12];
	
	memset((void*)pRIPMSG,0,sizeof(RIP_MSG));
	
	pRIPMSG->op = DHCP_BOOTREQUEST;
	pRIPMSG->htype = DHCP_HTYPE10MB;
	pRIPMSG->hlen = DHCP_HLENETHERNET;
	pRIPMSG->hops = DHCP_HOPS;
	pRIPMSG->xid = htonl(DHCP_XID);
	pRIPMSG->secs = htons(DHCP_SECS);
	
	if(dhcp_state < STATE_DHCP_LEASED)
		pRIPMSG->flags = htons(DHCP_FLAGSBROADCAST);
	else
	{
		pRIPMSG->flags = 0;		// For Unicast
		memcpy(pRIPMSG->ciaddr,GET_SIP,4);
	}		
	
	memcpy(pRIPMSG->chaddr,SRC_MAC_ADDR,6);
	
	/* MAGIC_COOKIE */
	pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 24) & 0xFF);
	pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 16) & 0xFF);
	pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 8) & 0xFF);
	pRIPMSG->OPT[i++] = (uint8)(MAGIC_COOKIE & 0xFF);
	
	/* Option Request Param. */
	pRIPMSG->OPT[i++] = dhcpMessageType;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = DHCP_REQUEST;
	
	pRIPMSG->OPT[i++] = dhcpClientIdentifier;
	pRIPMSG->OPT[i++] = 0x07;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[0];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[1];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[2];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];	
	
	
	if(dhcp_state < STATE_DHCP_LEASED)
	{
		pRIPMSG->OPT[i++] = dhcpRequestedIPaddr;
		pRIPMSG->OPT[i++] = 0x04;
		pRIPMSG->OPT[i++] = GET_SIP[0];
		pRIPMSG->OPT[i++] = GET_SIP[1];
		pRIPMSG->OPT[i++] = GET_SIP[2];
		pRIPMSG->OPT[i++] = GET_SIP[3];
		
		pRIPMSG->OPT[i++] = dhcpServerIdentifier;
		pRIPMSG->OPT[i++] = 0x04;
		pRIPMSG->OPT[i++] = DHCP_SIP[0];
		pRIPMSG->OPT[i++] = DHCP_SIP[1];
		pRIPMSG->OPT[i++] = DHCP_SIP[2];
		pRIPMSG->OPT[i++] = DHCP_SIP[3];
	}
	
	// host name
	pRIPMSG->OPT[i++] = hostName;
	//set the host name
	sprintf((char*)host_name,(char*)"%.4s-%02X%02X%02X",DEVICE_ID,SRC_MAC_ADDR[3],SRC_MAC_ADDR[4],SRC_MAC_ADDR[5]);
	pRIPMSG->OPT[i++] = (uint8)strlen((char*)host_name);	
	strcpy((char*)&(pRIPMSG->OPT[i]),(char*)host_name);
	i+=strlen((char*)host_name);
		
	pRIPMSG->OPT[i++] = dhcpParamRequest;
	pRIPMSG->OPT[i++] = 0x08;
	pRIPMSG->OPT[i++] = subnetMask;
	pRIPMSG->OPT[i++] = routersOnSubnet;
	pRIPMSG->OPT[i++] = dns;
	pRIPMSG->OPT[i++] = domainName;
	pRIPMSG->OPT[i++] = dhcpT1value;
	pRIPMSG->OPT[i++] = dhcpT2value;
	pRIPMSG->OPT[i++] = performRouterDiscovery;
	pRIPMSG->OPT[i++] = staticRoute;
	pRIPMSG->OPT[i++] = endOption;
	
	/* send broadcasting packet */
	if(dhcp_state < STATE_DHCP_LEASED) memset(ip,0xFF,4);
	else 
		memcpy(ip,DHCP_SIP,4);
		sendto(SOCK_DHCP, (const uint8 *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT);
	#ifdef DHCP_DEBUG
		printf("sent DHCP_REQUEST\r\n");
	#endif
}

/**
*@brief		发送释放消息
*@param		msgtype:是否为7
*@return	无
*/
void send_DHCP_RELEASE_DECLINE(char msgtype)
{
	uint16 i =0;
	uint8  ip[4];
	
	memset((void*)pRIPMSG,0,sizeof(RIP_MSG));
	
	pRIPMSG->op = DHCP_BOOTREQUEST;
	pRIPMSG->htype = DHCP_HTYPE10MB;
	pRIPMSG->hlen = DHCP_HLENETHERNET;
	pRIPMSG->hops = DHCP_HOPS;
	pRIPMSG->xid = htonl(DHCP_XID);
	pRIPMSG->secs = htons(DHCP_SECS);
	pRIPMSG->flags =0;// DHCP_FLAGSBROADCAST;
	
	memcpy(pRIPMSG->chaddr,SRC_MAC_ADDR,6);
	
	/* MAGIC_COOKIE */
	pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 24) & 0xFF);
	pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 16) & 0xFF);
	pRIPMSG->OPT[i++] = (uint8)((MAGIC_COOKIE >> 8) & 0xFF);
	pRIPMSG->OPT[i++] = (uint8)(MAGIC_COOKIE & 0xFF);
	
	/* Option Request Param. */
	pRIPMSG->OPT[i++] = dhcpMessageType;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = ((!msgtype) ? DHCP_RELEASE : DHCP_DECLINE);
	
	pRIPMSG->OPT[i++] = dhcpClientIdentifier;
	pRIPMSG->OPT[i++] = 0x07;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[0];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[1];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[2];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];	
	
	pRIPMSG->OPT[i++] = dhcpServerIdentifier;
	pRIPMSG->OPT[i++] = 0x04;
	pRIPMSG->OPT[i++] = DHCP_SIP[0];
	pRIPMSG->OPT[i++] = DHCP_SIP[1];
	pRIPMSG->OPT[i++] = DHCP_SIP[2];
	pRIPMSG->OPT[i++] = DHCP_SIP[3];
	
	if(msgtype)
	{
	pRIPMSG->OPT[i++] = dhcpRequestedIPaddr;
	pRIPMSG->OPT[i++] = 0x04;
	pRIPMSG->OPT[i++] = GET_SIP[0];
	pRIPMSG->OPT[i++] = GET_SIP[1];
	pRIPMSG->OPT[i++] = GET_SIP[2];
	pRIPMSG->OPT[i++] = GET_SIP[3];		
	#ifdef DHCP_DEBUG		
	printf("sent DHCP_DECLINE\r\n");
	#endif		
	}
	#ifdef DHCP_DEBUG	
	else
	{
	printf("sent DHCP_RELEASE\r\n");
	}
	#endif	
	
	pRIPMSG->OPT[i++] = endOption;	
	
	
	if(msgtype) memset(ip,0xFF,4);
	else 
	memcpy(ip,DHCP_SIP,4);
	//printf("send dhcp decline\r\n");
	sendto(SOCK_DHCP, (uint8 *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT);
}

/**	 
*@brief	 	解析获得的DHCP消息
*@param		length:解析消息长度
*@return	0：解析失败  其他：解析成功
*/
uint8 parseDHCPMSG(uint16 length)
{
	uint8  svr_addr[6];
	uint16 svr_port;
	uint16 len;
	uint8  * p;
	uint8  * e;
	uint8  type;
	uint8  opt_len = 0;
	
	len = recvfrom(SOCK_DHCP, (uint8 *)pRIPMSG, length, svr_addr, &svr_port);
	
	#ifdef DHCP_DEBUG
		printf("DHCP_SIP:%u.%u.%u.%u\r\n",DHCP_SIP[0],DHCP_SIP[1],DHCP_SIP[2],DHCP_SIP[3]);
		printf("DHCP_RIP:%u.%u.%u.%u\r\n",DHCP_REAL_SIP[0],DHCP_REAL_SIP[1],DHCP_REAL_SIP[2],DHCP_REAL_SIP[3]);
		printf("svr_addr:%u.%u.%u.%u\r\n",svr_addr[0],svr_addr[1],svr_addr[2],svr_addr[3]);
	#endif	
	
	if(pRIPMSG->op != DHCP_BOOTREPLY || svr_port != DHCP_SERVER_PORT)
	{
		#ifdef DHCP_DEBUG	   
		printf("DHCP : NO DHCP MSG\r\n");
		#endif		
		return 0;
	}
	if(memcmp(pRIPMSG->chaddr,SRC_MAC_ADDR,6) != 0 || pRIPMSG->xid != htonl(DHCP_XID))
	{
		#ifdef DHCP_DEBUG
		printf("No My DHCP Message. This message is ignored.\r\n");
		printf("\tSRC_MAC_ADDR(%02X.%02X.%02X.",SRC_MAC_ADDR[0],SRC_MAC_ADDR[1],SRC_MAC_ADDR[2]);
		printf("%02X.%02X.%02X)",SRC_MAC_ADDR[3],SRC_MAC_ADDR[4],SRC_MAC_ADDR[5]);
		printf(", pRIPMSG->chaddr(%02X.%02X.%02X.",(char)pRIPMSG->chaddr[0],(char)pRIPMSG->chaddr[1],(char)pRIPMSG->chaddr[2]);
		printf("%02X.%02X.%02X)",pRIPMSG->chaddr[3],pRIPMSG->chaddr[4],pRIPMSG->chaddr[5]);
		printf("\tpRIPMSG->xid(%08X), DHCP_XID(%08X)",pRIPMSG->xid,(DHCP_XID));
		printf("\tpRIMPMSG->yiaddr:%d.%d.%d.%d\r\n",pRIPMSG->yiaddr[0],pRIPMSG->yiaddr[1],pRIPMSG->yiaddr[2],pRIPMSG->yiaddr[3]);
		#endif				
		return 0;
	}
	
	if( *((uint32*)DHCP_SIP) != 0x00000000 )
	{
		if( *((uint32*)DHCP_REAL_SIP) != *((uint32*)svr_addr) && 
		*((uint32*)DHCP_SIP) != *((uint32*)svr_addr) ) 
		{
			#ifdef DHCP_DEBUG		
			printf("Another DHCP sever send a response message. This is ignored.\r\n");
			printf("\tIP:%u.%u.%u.%u\r\n",svr_addr[0],svr_addr[1],svr_addr[2],svr_addr[3]);
			#endif				
			return 0;								
		}
	}	
	memcpy(GET_SIP,pRIPMSG->yiaddr,4);
	
	#ifdef DHCP_DEBUG	
		printf("DHCP MSG received\r\n");
		printf("yiaddr : %u.%u.%u.%u\r\n",GET_SIP[0],GET_SIP[1],GET_SIP[2],GET_SIP[3]);
	#endif	
	type = 0;
	p = (uint8 *)(&pRIPMSG->op);
	p = p + 240;
	e = p + (len - 240);
	#ifdef DHCP_DEBUG
		printf("p : %08X  e : %08X  len : %d\r\n", (uint32)p, (uint32)e, len);
	#endif
	while ( p < e ) 
	{
		switch ( *p++ ) 
		{
			case endOption :
				return type;
		
			case padOption :
				break;
			
			case dhcpMessageType :
				opt_len = *p++;
				type = *p;
				#ifdef DHCP_DEBUG			
					printf("dhcpMessageType : %02X\r\n", type);
				#endif			
				break;
			
			case subnetMask :
				opt_len =* p++;
				memcpy(GET_SN_MASK,p,4);
				#ifdef DHCP_DEBUG
					printf("subnetMask : ");
					printf("%u.%u.%u.%u\r\n",GET_SN_MASK[0],GET_SN_MASK[1],GET_SN_MASK[2],GET_SN_MASK[3]);
				#endif			
				break;
			
			case routersOnSubnet :
				opt_len = *p++;
				memcpy(GET_GW_IP,p,4);
				#ifdef DHCP_DEBUG
					printf("routersOnSubnet : ");
					printf("%u.%u.%u.%u\r\n",GET_GW_IP[0],GET_GW_IP[1],GET_GW_IP[2],GET_GW_IP[3]);
				#endif			
				break;
			
			case dns :
				opt_len = *p++;
				memcpy(GET_DNS_IP,p,4);
				break;
			
			case dhcpIPaddrLeaseTime :
				opt_len = *p++;
				dhcp_lease_time = ntohl(*((uint32*)p));
				#ifdef DHCP_DEBUG			
					printf("dhcpIPaddrLeaseTime : %d\r\n", dhcp_lease_time);
				#endif			
				break;
			
			case dhcpServerIdentifier :
				opt_len = *p++;
				#ifdef DHCP_DEBUG						
					printf("DHCP_SIP : %u.%u.%u.%u\r\n", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
				#endif			
				if( *((uint32*)DHCP_SIP) == 0 || 
				*((uint32*)DHCP_REAL_SIP) == *((uint32*)svr_addr) || 
				*((uint32*)DHCP_SIP) == *((uint32*)svr_addr) )
				{
					memcpy(DHCP_SIP,p,4);
					memcpy(DHCP_REAL_SIP,svr_addr,4);	// Copy the real ip address of my DHCP server
					#ifdef DHCP_DEBUG						
						printf("My dhcpServerIdentifier : ");
						printf("%u.%u.%u.%u\r\n", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
						printf("My DHCP server real IP address : ");
						printf("%u.%u.%u.%u\r\n", DHCP_REAL_SIP[0], DHCP_REAL_SIP[1], DHCP_REAL_SIP[2], DHCP_REAL_SIP[3]);
					#endif						
				}
				else
				{
				#ifdef DHCP_DEBUG			   
					printf("Another dhcpServerIdentifier : \r\n");
					printf("\tMY(%u.%u.%u.%u) ", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
					printf("Another(%u.%u.%u.%u)\r\n", svr_addr[0], svr_addr[1], svr_addr[2], svr_addr[3]);
				#endif				
				}
				break;
				
			default :
				opt_len = *p++;
				#ifdef DHCP_DEBUG			
					printf("opt_len : %u\r\n", opt_len);
				#endif			
				break;
		} // switch
		p+=opt_len;
	} // while	
	return 0;
}

/**	
*@brief	 	检查DHCP状态
*@param		s: socket数
*@return	返回得到的DHCP状态
					DHCP_RET_NONE: 未获取到IP地址
					DHCP_RET_TIMEOUT:超时
					DHCP_RET_UPDATE: 获取成功
					DHCP_RET_CONFLICT:IP地址冲突
*/
uint8 check_DHCP_state(SOCKET s) 
{
	uint16 len;   										/*定义一个表示接收数据大小变量*/
	uint8  type;										/*定义一个表示接收封包类型变量*/
	
	type = 0;
	if(getSn_SR(s)!=SOCK_CLOSED)                        /*socket处于打开状态*/
	{
		if ((len = getSn_RX_RSR(s)) > 0)				/*接收到数据*/
		{
			type = parseDHCPMSG(len);					/*解析收到的封包类型*/
		}
	}
	else												/*socket处于关闭状态，重新初始化socket*/
	{
		if(dhcp_state == STATE_DHCP_READY)
		{
		//init_dhcp_client();
			#ifdef DHCP_DEBUG			
				printf("state : STATE_DHCP_READY\r\n");
			#endif	   	
		}	
		if(!socket(SOCK_DHCP,Sn_MR_UDP,DHCP_CLIENT_PORT,0x00))/*初始化socket和端口*/
		{
			#ifdef DHCP_DEBUG	   
				printf("Fail to create the DHCPC_SOCK(%u)\r\n",SOCK_DHCP);
			#endif   
			return DHCP_RET_ERR;								/*socket初始化错误*/
		}
	}
	switch ( dhcp_state )
	{
		case STATE_DHCP_READY:									/*DHCP初始化状态*/
			DHCP_timeout = 0;									/*DHCP超时标志设置为1*/
			reset_DHCP_time();									/*复位超时时间*/
			send_DHCP_DISCOVER();								/*发送DISCOVER包*/
	
			DHCP_timer = 0;										/*set_timer0(DHCP_timer_handler);  */ 	
			dhcp_state = STATE_DHCP_DISCOVER;					/*DHCP的DISCOVER状态*/
			break;	 
  
		case STATE_DHCP_DISCOVER:
			if (type == DHCP_OFFER) 
			{
				reset_DHCP_time();								/*复位超时时间*/
				send_DHCP_REQUEST();							/*发送REQUEST包*/
				dhcp_state = STATE_DHCP_REQUEST;
				#ifdef DHCP_DEBUG			
					printf("state : STATE_DHCP_REQUEST\r\n");
				#endif			
			}
			else 
				check_DHCP_Timeout();							/*检查是否超时*/
			break;
		
		case STATE_DHCP_REQUEST :								/*DHCP的REQUEST状态*/
			if (type == DHCP_ACK) 								/*接收到DHCP服务器回应的off封包*/
			{
				reset_DHCP_time();
				if (check_leasedIP()) 
				{
					#ifdef DHCP_DEBUG					
						printf("state : STATE_DHCP_LEASED\r\n");
					#endif		
					dhcp_state = STATE_DHCP_LEASED;
					return DHCP_RET_UPDATE;
				} 
				else 
				{
					#ifdef DHCP_DEBUG					
						printf("state : STATE_DHCP_DISCOVER\r\n");
					#endif				
					dhcp_state = STATE_DHCP_DISCOVER;
					return DHCP_RET_CONFLICT;
				}
			}	
			else if (type == DHCP_NAK) 
			{
				reset_DHCP_time();								/*复位超时时间*/
				dhcp_state = STATE_DHCP_DISCOVER;
				#ifdef DHCP_DEBUG				
					printf("state : STATE_DHCP_DISCOVER\r\n");
				#endif			
			}
			else 
				check_DHCP_Timeout();							/*检查是否超时*/
			break;
			
		case STATE_DHCP_LEASED :
			if ((dhcp_lease_time != 0xffffffff) && (dhcp_time>(dhcp_lease_time/2))) 
			{
				type = 0;
				memcpy(OLD_SIP,GET_SIP,4);
				DHCP_XID++;
				send_DHCP_REQUEST();
				dhcp_state = STATE_DHCP_REREQUEST;
				#ifdef DHCP_DEBUG
					printf("state : STATE_DHCP_REREQUEST\r\n");
				#endif
				reset_DHCP_time();
			}
			break;
		case STATE_DHCP_REREQUEST :
			if (type == DHCP_ACK) 
			{
				if(memcmp(OLD_SIP,GET_SIP,4)!=0)	
				{
					#ifdef DHCP_DEBUG
						printf("The IP address from the DHCP server is updated.\r\n");
						printf("OLD_SIP=%u.%u.%u.%u",OLD_SIP[0],OLD_SIP[1],OLD_SIP[2],OLD_SIP[3]);
						printf("GET_SIP=%u.%u.%u.%u\r\n",GET_SIP[0],GET_SIP[1],GET_SIP[2],GET_SIP[3]);
					#endif
					return DHCP_RET_UPDATE;
				}
				#ifdef DHCP_DEBUG
				else
				{
					printf("state : STATE_DHCP_LEASED : same IP\r\n");
				}
				#endif
				reset_DHCP_time();
				dhcp_state = STATE_DHCP_LEASED;
			} 
			else if (type == DHCP_NAK) 
			{
				reset_DHCP_time();
				dhcp_state = STATE_DHCP_DISCOVER;
				#ifdef DHCP_DEBUG
					printf("state : STATE_DHCP_DISCOVER\r\n");
				#endif
			} 
			else 
				check_DHCP_Timeout();
			break;
			
		default :
			dhcp_state = STATE_DHCP_READY;
			break;
	}
	if (DHCP_timeout == 1)
	{
		//kill_timer0(DHCP_timer);
		dhcp_state = STATE_DHCP_READY;
		return DHCP_RET_TIMEOUT;
	}
	return DHCP_RET_NONE;
}

/**	
*@brief	 	检查DHCP请求释放超时
*@param		无
*@return	无
*/
void check_DHCP_Timeout(void)
{
	if (dhcp_retry_count < MAX_DHCP_RETRY) 
	{
		if (dhcp_time > next_dhcp_time) 
		{
			dhcp_time = 0;
			next_dhcp_time = dhcp_time + DHCP_WAIT_TIME;
			dhcp_retry_count++;
			switch ( dhcp_state ) 
			{
				case STATE_DHCP_DISCOVER :
					#ifdef DHCP_DEBUG			   
						printf("<<timeout>> state : STATE_DHCP_DISCOVER\r\n");
					#endif				
					send_DHCP_DISCOVER();
					break;
				
				case STATE_DHCP_REQUEST :
					#ifdef DHCP_DEBUG			   
						printf("<<timeout>> state : STATE_DHCP_REQUEST\r\n");
					#endif				
					send_DHCP_REQUEST();
					break;		
				
				case STATE_DHCP_REREQUEST :
					#ifdef DHCP_DEBUG			   
						printf("<<timeout>> state : STATE_DHCP_REREQUEST\r\n");
					#endif				
					send_DHCP_REQUEST();
					break;
				
				default :
					break;
			}
		}
	} 
	else 
	{
		reset_DHCP_time();
		DHCP_timeout = 1;
	
		send_DHCP_DISCOVER();
		dhcp_state = STATE_DHCP_DISCOVER;
		#ifdef DHCP_DEBUG		
			printf("timeout : state : STATE_DHCP_DISCOVER\r\n");
		#endif		
	}
}

/**	
*@brief	  检查获取的IP是否冲突
*@param		无
*@return	无
*/
 uint8 check_leasedIP(void)
{
	#ifdef DHCP_DEBUG
		printf("<Check the IP Conflict %d.%d.%d.%d: ",GET_SIP[0],GET_SIP[1],GET_SIP[2],GET_SIP[3]);
	#endif
	// sendto is complete. that means there is a node which has a same IP.

	if ( sendto(SOCK_DHCP, (const uint8*)"CHECK_IP_CONFLICT", 17, GET_SIP, 5000))
	{
		#ifdef DHCP_DEBUG
			printf(" Conflict>\r\n");
		#endif
		send_DHCP_RELEASE_DECLINE(1);
		return 0;
	}
	#ifdef DHCP_DEBUG
		printf(" No Conflict>\r\n");
	#endif
	return 1;
}
	
/**	
*@brief	 	DHCP 定时器操作
*@param		无
*@return	无
*/
void DHCP_timer_handler(void)
{
	if(dhcp_tick_cnt++ > 1000)
	{
		dhcp_tick_cnt = 0;
		dhcp_time++;
	}
}

/**		 
*@brief	  初始化DHCP客户端
*@param		无
*@return	无
*/
void init_dhcp_client(void)
{
	uint8 ip_0[4]={0,};
	DHCP_XID = 0x12345678;
	memset(OLD_SIP,0,sizeof(OLD_SIP));
	memset(DHCP_SIP,0,sizeof(DHCP_SIP));
	memset(DHCP_REAL_SIP,0,sizeof(GET_SN_MASK));
	
	/*clear ip setted flag */
	iinchip_init();
	setSUBR(ip_0);
	setGAR(ip_0);
	setSIPR(ip_0);
	printf("mac=%02x:%02x:%02x:%02x:%02x:%02x\r\n",SRC_MAC_ADDR[0],SRC_MAC_ADDR[1],SRC_MAC_ADDR[2],SRC_MAC_ADDR[3],SRC_MAC_ADDR[4],SRC_MAC_ADDR[5]);  
	
	dhcp_state = STATE_DHCP_READY;
	#ifdef DHCP_DEBUG
		printf("init_dhcp_client:%u\r\n",SOCK_DHCP);
	#endif   
}
/**	
*@brief	 	执行DHCP Client
*@param		无
*@return	无
*/
void do_dhcp(void)
{
    uint8 dhcpret = 0;
    ip_from = IP_FROM_DHCP;	/*IP配置方法选择为DHCP*/
//    dhcp_timer_init();																 /*初始化DHCP定时器*/
    if(Conflict_flag == 1)
    {
        init_dhcp_client();				                       /*初始化DHCP客户端*/ 
        Conflict_flag = 0;
    }

    dhcpret = check_DHCP_state(SOCK_DHCP);             /*获取DHCP服务状态*/

    switch(dhcpret)
    {
    case DHCP_RET_NONE:                              /*IP地址获取不成功*/ 
        break;

    case DHCP_RET_TIMEOUT:                           /*IP地址获取超时*/ 
        break;

    case DHCP_RET_UPDATE:							/*成功获取到IP地址*/ 
        dhcp_ok = 1;                  
        w5500_set_ip();                                /*将获取到的IP地址写入W5500寄存器*/ 
        printf(" 已从DHCP服务器成功获得IP地址\r\n");

    break;

    case DHCP_RET_CONFLICT:                          /*IP地址获取冲突*/ 
        printf(" 从DHCP获取IP地址失败\r\n");
        dhcp_state = STATE_DHCP_READY; 
        printf(" 重试中\r\n");
        dhcp_ok=0; 
        break;     

    default:
        break;
    }

}





