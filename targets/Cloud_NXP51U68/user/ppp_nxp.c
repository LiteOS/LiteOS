
/* 
PPP interface for lwIP

Author: Sylvain Rochet

Table of Contents:
- Supported PPP protocols and features
- Raw API PPP example for all protocols
- PPPoS input path (raw API, IRQ safe API, TCPIP API)
- Thread safe PPP API (PPPAPI)
- Notify phase callback (PPP_NOTIFY_PHASE)
- Upgrading from lwIP <= 1.4.x to lwIP >= 2.0.x
Supported PPP protocols and features
======================================

Supported Low level protocols:
* PPP over serial using HDLC-like framing, such as wired dialup modems
  or mobile telecommunications GPRS/EDGE/UMTS/HSPA+/LTE modems
* PPP over Ethernet, such as xDSL modems
* PPP over L2TP (Layer 2 Tunneling Protocol) LAC (L2TP Access Concentrator),
  IP tunnel over UDP, such as VPN access

Supported auth protocols:
* PAP, Password Authentication Protocol
* CHAP, Challenge-Handshake Authentication Protocol, also known as CHAP-MD5
* MSCHAPv1, Microsoft version of CHAP, version 1
* MSCHAPv2, Microsoft version of CHAP, version 2
* EAP, Extensible Authentication Protocol

Supported address protocols:
* IPCP, IP Control Protocol, IPv4 addresses negotiation
* IP6CP, IPv6 Control Protocol, IPv6 link-local addresses negotiation

Supported encryption protocols:
* MPPE, Microsoft Point-to-Point Encryption

Supported compression or miscellaneous protocols, for serial links only:
* PFC, Protocol Field Compression
* ACFC, Address-and-Control-Field-Compression
* ACCM, Asynchronous-Control-Character-Map
* VJ, Van Jacobson TCP/IP Header Compression

*/


/*  //2 Raw API PPP example for all protocols
=======================================

As usual, raw API for lwIP means the lightweight API which *MUST* only be used
for NO_SYS=1 systems or called inside lwIP core thread for NO_SYS=0 systems.

*/
#include "netif/ppp/ppp.h"
#include "netif/ppp/pppapi.h"
#include "netif/ppp/pppos.h"

/* The PPP control block */
ppp_pcb *ppp;

/* The PPP IP interface */
struct netif ppp_netif;

/* //PPP status callback  ??????
 * PPP status callback  ??????
 * ===================
 *
 * PPP status callback is called on PPP status change (up, down, …) from lwIP
 * core thread
 */
extern const ip_addr_t* dns_getserver(u8_t numdns);
/* PPP status callback example */
static void status_cb(ppp_pcb *pcb, int err_code, void *ctx) 
{
  struct netif *pppif = ppp_netif(pcb);
  LWIP_UNUSED_ARG(ctx);

  switch(err_code) 
  {
    case PPPERR_NONE: 
    {
        #if LWIP_DNS
              const ip_addr_t *ns;
        #endif /* LWIP_DNS */
              printf("status_cb: Connected\n");
        #if PPP_IPV4_SUPPORT
              printf("   our_ipaddr  = %s\n", ipaddr_ntoa(&pppif->ip_addr));
              printf("   his_ipaddr  = %s\n", ipaddr_ntoa(&pppif->gw));
              printf("   netmask     = %s\n", ipaddr_ntoa(&pppif->netmask));
        #if LWIP_DNS
              ns = dns_getserver(0);
              printf("   dns1        = %s\n", ipaddr_ntoa(ns));
              ns = dns_getserver(1);
              printf("   dns2        = %s\n", ipaddr_ntoa(ns));
        #endif /* LWIP_DNS */
        #endif /* PPP_IPV4_SUPPORT */
        #if PPP_IPV6_SUPPORT
              printf("   our6_ipaddr = %s\n", ip6addr_ntoa(netif_ip6_addr(
pppif, 0)));
        #endif /* PPP_IPV6_SUPPORT */
              break;
    }
    case PPPERR_PARAM: 
    {
      printf("status_cb: Invalid parameter\n");
      break;
    }
    case PPPERR_OPEN: 
    {
      printf("status_cb: Unable to open PPP session\n");
      break;
    }
    case PPPERR_DEVICE: 
    {
      printf("status_cb: Invalid I/O device for PPP\n");
      break;
    }
    case PPPERR_ALLOC: 
    {
      printf("status_cb: Unable to allocate resources\n");
      break;
    }
    case PPPERR_USER: 
    {
      printf("status_cb: User interrupt\n");
      break;
    }
    case PPPERR_CONNECT: 
    {
      printf("status_cb: Connection lost\n");
      break;
    }
    case PPPERR_AUTHFAIL: 
    {
      printf("status_cb: Failed authentication challenge\n");
      break;
    }
    case PPPERR_PROTOCOL: 
    {
      printf("status_cb: Failed to meet protocol\n");
      break;
    }
    case PPPERR_PEERDEAD: 
    {
      printf("status_cb: Connection timeout\n");
      break;
    }
    case PPPERR_IDLETIMEOUT: 
    {
      printf("status_cb: Idle Timeout\n");
      break;
    }
    case PPPERR_CONNECTTIME: 
    {
      printf("status_cb: Max connect time reached\n");
      break;
    }
    case PPPERR_LOOPBACK: 
    {
      printf("status_cb: Loopback detected\n");
      break;
    }
    default: 
    {
      printf("status_cb: Unknown error code %d\n", err_code);
      break;
    }
  }

/*
 * This should be in the switch case, this is put outside of the switch
 * case for example readability.
 */

  if (err_code == PPPERR_NONE) {
    return;
  }

  /* ppp_close() was previously called, don't reconnect */
  if (err_code == PPPERR_USER) {
    /* ppp_free(); -- can be called here */
    return;
  }

  /*
   * Try to reconnect in 30 seconds, if you need a modem chatscript you have
   * to do a much better signaling here ;-)
   */
  ppp_connect(pcb, 30);
  /* OR ppp_listen(pcb); */
}
static u32_t output_cb(ppp_pcb *pcb, u8_t *data, u32_t len, void *ctx) 
{
  return uart_write(data, len);
}
VOID *main_ppp(UINT32  args)
{
    ppp = pppos_create(&ppp_netif,output_cb, status_cb, NULL);
    if(NULL != ppp)
    {
        /* Set this interface as default route */
        ppp_set_default(ppp);
        /* Ask the peer for up to 2 DNS server addresses. */
        ppp_set_usepeerdns(ppp, 1);

        /* Auth configuration, this is pretty self-explanatory */
        ppp_set_auth(ppp, PPPAUTHTYPE_ANY, "login", "password");
        u16_t holdoff = 0;
        ppp_connect(ppp, holdoff);
        ppp_free(ppp);

    }
    return NULL;
    
}

