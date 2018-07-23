#ifndef _DHCP_H_
#define _DHCP_H_

#include "stdio.h"
#include "types.h"

#define DEVICE_ID "W5500"
typedef struct _DHCP_GET
{
  uint8 mac[6];
  uint8 lip[4];
  uint8 sub[4];
  uint8 gw[4];
  uint8 dns[4];	
}DHCP_Get;

extern uint32  dhcp_time;
extern DHCP_Get DHCP_GET;
#define EXTERN_DHCP_MAC    DHCP_GET.mac
#define EXTERN_DHCP_SN     DHCP_GET.sub
#define EXTERN_DHCP_GW     DHCP_GET.gw
#define EXTERN_DHCP_DNS    DHCP_GET.dns
#define EXTERN_DHCP_NAME   "iweb"//ConfigMsg.domain
#define EXTERN_DHCP_SIP    DHCP_GET.lip

#define DHCP_RET_NONE      0
#define DHCP_RET_ERR       1
#define DHCP_RET_TIMEOUT   2
#define DHCP_RET_UPDATE    3
#define DHCP_RET_CONFLICT  4




/* DHCP state machine. */
#define  STATE_DHCP_READY        0
#define  STATE_DHCP_DISCOVER     1
#define  STATE_DHCP_REQUEST      2
#define  STATE_DHCP_LEASED       3
#define  STATE_DHCP_REREQUEST    4
#define  STATE_DHCP_RELEASE      5
         
#define  MAX_DHCP_RETRY          3
#define  DHCP_WAIT_TIME          5
         
#define  DHCP_FLAGSBROADCAST     0x8000

/* UDP port numbers for DHCP */
#define  DHCP_SERVER_PORT        67	/* from server to client */
#define  DHCP_CLIENT_PORT        68	/* from client to server */

/* DHCP message OP code */
#define  DHCP_BOOTREQUEST        1
#define  DHCP_BOOTREPLY          2

/* DHCP message type */
#define  DHCP_DISCOVER           1
#define  DHCP_OFFER              2
#define  DHCP_REQUEST            3
#define  DHCP_DECLINE            4
#define  DHCP_ACK                5
#define  DHCP_NAK                6
#define  DHCP_RELEASE            7
#define  DHCP_INFORM             8

#define DHCP_HTYPE10MB           1
#define DHCP_HTYPE100MB          2

#define DHCP_HLENETHERNET        6
#define DHCP_HOPS                0
#define DHCP_SECS                0

#define MAGIC_COOKIE		         0x63825363
#define DEFAULT_XID              0x12345678

#define DEFAULT_LEASETIME        0xffffffff	/* infinite lease time */

/* DHCP option and value (cf. RFC1533) */
enum
{
   padOption               = 0,
   subnetMask              = 1,
   timerOffset             = 2,
   routersOnSubnet         = 3,
   timeServer              = 4,
   nameServer              = 5,
   dns                     = 6,
   logServer               = 7,
   cookieServer            = 8,
   lprServer               = 9,
   impressServer           = 10,
   resourceLocationServer  = 11,
   hostName                = 12,
   bootFileSize            = 13,
   meritDumpFile           = 14,
   domainName              = 15,
   swapServer              = 16,
   rootPath                = 17,
   extentionsPath          = 18,
   IPforwarding            = 19,
   nonLocalSourceRouting   = 20,
   policyFilter            = 21,
   maxDgramReasmSize       = 22,
   defaultIPTTL            = 23,
   pathMTUagingTimeout     = 24,
   pathMTUplateauTable     = 25,
   ifMTU                   = 26,
   allSubnetsLocal         = 27,
   broadcastAddr           = 28,
   performMaskDiscovery    = 29,
   maskSupplier            = 30,
   performRouterDiscovery  = 31,
   routerSolicitationAddr  = 32,
   staticRoute             = 33,
   trailerEncapsulation    = 34,
   arpCacheTimeout         = 35,
   ethernetEncapsulation   = 36,
   tcpDefaultTTL           = 37,
   tcpKeepaliveInterval    = 38,
   tcpKeepaliveGarbage     = 39,
   nisDomainName           = 40,
   nisServers              = 41,
   ntpServers              = 42,
   vendorSpecificInfo      = 43,
   netBIOSnameServer       = 44,
   netBIOSdgramDistServer  = 45,
   netBIOSnodeType         = 46,
   netBIOSscope            = 47,
   xFontServer             = 48,
   xDisplayManager         = 49,
   dhcpRequestedIPaddr     = 50,
   dhcpIPaddrLeaseTime     = 51,
   dhcpOptionOverload      = 52,
   dhcpMessageType         = 53,
   dhcpServerIdentifier    = 54,
   dhcpParamRequest        = 55,
   dhcpMsg                 = 56,
   dhcpMaxMsgSize          = 57,
   dhcpT1value             = 58,
   dhcpT2value             = 59,
   dhcpClassIdentifier     = 60,
   dhcpClientIdentifier    = 61,
   endOption               = 255
};

typedef struct _RIP_MSG
{
   uint8  op; 
   uint8  htype; 
   uint8  hlen;
   uint8  hops;
   uint32 xid;
   uint16 secs;
   uint16 flags;
   uint8  ciaddr[4];
   uint8  yiaddr[4];
   uint8  siaddr[4];
   uint8  giaddr[4];
   uint8  chaddr[16];
   uint8  sname[64];
   uint8  file[128];
   uint8  OPT[312];
}RIP_MSG;

#define MAX_DHCP_OPT	16


void init_dhcp_client(void);

uint8 check_DHCP_state(SOCKET s); // Check the DHCP state
void do_dhcp(void);
#endif	/* _DHCP_H_ */
