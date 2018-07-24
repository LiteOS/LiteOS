/**
******************************************************************************
* @file    netbios.c
* @author  WIZnet Software Team 
* @version V1.0
* @date    2015-02-14
* @brief   nebios 名字解析服务器功能    通过Ping	NETBIOS_W5500_NAME 可获得其IP地址
* @attention  
******************************************************************************
*/
#include <string.h>
#include <stdio.h>

#include "w5500.h"
#include "socket.h"
#include "utility.h"
#include "netbios.h"

#define NETBIOS_W5500_NAME "W5500"					/* 定义NetBIOS名字*/

#define NETBIOS_SOCK     6							/* 定义NetBIOS采用的socket*/
#define NETBIOS_PORT     137						/* "NetBIOS Name service"的默认端口 */


#define NETBIOS_NAME_LEN 16							/*NetBIOS名字最大长度*/

#define NETBIOS_MSG_MAX_LEN 512						/*NetBIOS报文的最大长度 */

#define NETBIOS_NAME_TTL 10//300000					/*NetBIOS响应时间*/

/** NetBIOS header flags */
#define NETB_HFLAG_RESPONSE           0x8000U
#define NETB_HFLAG_OPCODE             0x7800U
#define NETB_HFLAG_OPCODE_NAME_QUERY  0x0000U
#define NETB_HFLAG_AUTHORATIVE        0x0400U
#define NETB_HFLAG_TRUNCATED          0x0200U
#define NETB_HFLAG_RECURS_DESIRED     0x0100U
#define NETB_HFLAG_RECURS_AVAILABLE   0x0080U
#define NETB_HFLAG_BROADCAST          0x0010U
#define NETB_HFLAG_REPLYCODE          0x0008U
#define NETB_HFLAG_REPLYCODE_NOERROR  0x0000U

/** NetBIOS name flags */
#define NETB_NFLAG_UNIQUE             0x8000U
#define NETB_NFLAG_NODETYPE           0x6000U
#define NETB_NFLAG_NODETYPE_HNODE     0x6000U
#define NETB_NFLAG_NODETYPE_MNODE     0x4000U
#define NETB_NFLAG_NODETYPE_PNODE     0x2000U
#define NETB_NFLAG_NODETYPE_BNODE     0x0000U

/** NetBIOS message header */
#pragma pack(1)
typedef struct _NETBIOS_HDR {
  uint16 trans_id;
  uint16 flags;
  uint16 questions;
  uint16 answerRRs;
  uint16 authorityRRs;
  uint16 additionalRRs;
}NETBIOS_HDR;


/** NetBIOS message name part */
typedef struct _NETBIOS_NAME_HDR {
  uint8  nametype;
  uint8  encname[(NETBIOS_NAME_LEN*2)+1];
  uint16 type;
  uint16 cls;
  uint32 ttl;
  uint16 datalen;
  uint16 flags;
  //ip_addr_p_t addr;
  uint8 addr[4];
}NETBIOS_NAME_HDR;


/** NetBIOS 报文结构体 */
typedef struct _NETBIOS_RESP
{
  NETBIOS_HDR      resp_hdr;
  NETBIOS_NAME_HDR resp_name;
}NETBIOS_RESP;
#pragma pack()

 /**
*@brief		  NetBIOS解码
*@param		  *name_enc			解码前的NetBios名字
*@param		  *name_dec			解码后的NetBios名字
*@param		  name_dec_len
*@return	  无
*/
static int netbios_name_decoding( char *name_enc, char *name_dec, int name_dec_len)
{
  char *pname;
  char  cname;
  char  cnbname;
  int   index = 0;

  /* 启动NetBios解码 */
  pname  = name_enc;
  for (;;) {
    /* Every two characters of the first level-encoded name
     * turn into one character in the decoded name. */
    cname = *pname;
    if (cname == '\0')
      break;    /* no more characters */
    if (cname == '.')
      break;    /* scope ID follows */
    if (cname < 'A' || cname > 'Z') {
      /* Not legal. */
      return -1;
    }
    cname -= 'A';
    cnbname = cname << 4;
    pname++;

    cname = *pname;
    if (cname == '\0' || cname == '.') {
      /* No more characters in the name - but we're in
       * the middle of a pair.  Not legal. */
      return -1;
    }
    if (cname < 'A' || cname > 'Z') {
      /* Not legal. */
      return -1;
    }
    cname -= 'A';
    cnbname |= cname;
    pname++;

    /* Do we have room to store the character? */
    if (index < NETBIOS_NAME_LEN) {
      /* Yes - store the character. */
      name_dec[index++] = (cnbname!=' '?cnbname:'\0');
    }
  }

  return 0;
}
char netbios_rx_buf[NETBIOS_MSG_MAX_LEN];
char netbios_tx_buf[NETBIOS_MSG_MAX_LEN];
 /**
*@brief		  执行NetBIOS name解析程序 
*@param		  无
*@return	  无
*/
void do_netbios(void)
{
    unsigned char state;
    unsigned int len;
    state = getSn_SR(NETBIOS_SOCK);
    switch(state)
    {
	case SOCK_UDP:
		if((len=getSn_RX_RSR(NETBIOS_SOCK))>0)
		{
			unsigned char rem_ip_addr[4];
			uint16 rem_udp_port;
			char netbios_name[NETBIOS_NAME_LEN+1];
			NETBIOS_HDR* netbios_hdr;
			NETBIOS_NAME_HDR* netbios_name_hdr;
			len=recvfrom(NETBIOS_SOCK,(unsigned char*)&netbios_rx_buf,len,rem_ip_addr,&rem_udp_port);
			printf("rem_ip_addr=%d.%d.%d.%d:%d\r\n",rem_ip_addr[0],rem_ip_addr[1],rem_ip_addr[2],rem_ip_addr[3],rem_udp_port);
			netbios_hdr = (NETBIOS_HDR*)netbios_rx_buf;
			netbios_name_hdr = (NETBIOS_NAME_HDR*)(netbios_hdr+1);
			/* 如果数据包是NetBIOS查询包*/
			if (((netbios_hdr->flags & ntohs(NETB_HFLAG_OPCODE)) == ntohs(NETB_HFLAG_OPCODE_NAME_QUERY)) &&
					((netbios_hdr->flags & ntohs(NETB_HFLAG_RESPONSE)) == 0) &&
					 (netbios_hdr->questions == ntohs(1))) 
			{
				printf("netbios name query question\r\n");
				/* 对NetBIOS包进行解码 */
				netbios_name_decoding( (char*)(netbios_name_hdr->encname), netbios_name, sizeof(netbios_name));
				printf("name is %s\r\n",netbios_name);
				/* 如果是针对本机的NetBIOS查询 */
				if (strcmp(netbios_name, NETBIOS_W5500_NAME) == 0) 
				{
					uint8 ip_addr[4];
					NETBIOS_RESP *resp = (NETBIOS_RESP*)netbios_tx_buf;
					/* 处理NetBIOS响应包的header*/
					resp->resp_hdr.trans_id      = netbios_hdr->trans_id;
					resp->resp_hdr.flags         = htons(NETB_HFLAG_RESPONSE |
														NETB_HFLAG_OPCODE_NAME_QUERY |
														NETB_HFLAG_AUTHORATIVE |
														NETB_HFLAG_RECURS_DESIRED);
					resp->resp_hdr.questions     = 0;
					resp->resp_hdr.answerRRs     = htons(1);
					resp->resp_hdr.authorityRRs  = 0;
					resp->resp_hdr.additionalRRs = 0;

					/* 处理NetBIOS响应包的header数据*/
					memcpy( resp->resp_name.encname, netbios_name_hdr->encname, sizeof(netbios_name_hdr->encname));
					resp->resp_name.nametype     = netbios_name_hdr->nametype;
					resp->resp_name.type         = netbios_name_hdr->type;
					resp->resp_name.cls          = netbios_name_hdr->cls;
					resp->resp_name.ttl          = htonl(NETBIOS_NAME_TTL);
					resp->resp_name.datalen      = htons(sizeof(resp->resp_name.flags)+sizeof(resp->resp_name.addr));
					resp->resp_name.flags        = htons(NETB_NFLAG_NODETYPE_BNODE);
					getSIPR(ip_addr);
					memcpy(resp->resp_name.addr,ip_addr,4);
											
					/* 发送NetBIOS响应 */
					sendto(NETBIOS_SOCK, (unsigned char*)resp, sizeof(NETBIOS_RESP), rem_ip_addr, rem_udp_port);
					printf("send response\r\n");
				}
			}
		}
		break;
		
	case SOCK_CLOSED:
		close(NETBIOS_SOCK);
		socket(NETBIOS_SOCK,Sn_MR_UDP,NETBIOS_PORT,0);
		break;
	
	default:
		break;
    }
}

