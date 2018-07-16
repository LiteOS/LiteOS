/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>

 ******************************************************************************
  File Name     : wifi_eth.c
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/7/5
  Last Modified :
  Description   : marvell sdio wifi ethernetif
  Function List :
  History       :
  1.Date        : 2018/7/5
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
#include "wifi_eth.h"
#include "wifi.h"

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "lwip/err.h"
#include <string.h>

#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "lwip/etharp.h"
//#include "netif/ppp/pppoe.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
static int8_t wifi_eth_init(struct netif* netif)
{
    return 0;
}

static int8_t wifi_eth_output(struct netif *netif, struct pbuf* p)
{
    uint8_t *buffer; 

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    buffer = WiFi_GetPacketBuffer(); // Get the send buffer (wait for the previous frame to be sent)
    pbuf_copy_partial(p, buffer, p->tot_len, 0); // Copy the data frame to be sent to the send buffer
#ifdef WIFI_DISPLAY_PACKET_SIZE
    printf("[Send] len=%hd\n", p->tot_len);
#endif
#ifdef WIFI_DISPLAY_PACKET_TX
    WiFi_OutputBuffer(buffer, p->tot_len);
#endif

    //signal that packet should be sent();
    WiFi_SendPacket(buffer, p->tot_len, NULL, NULL, WIFI_DEFAULT_TIMEOUT_DATAACK);

    MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
    if (((u8_t*)p->payload)[0] & 1) {
        /* broadcast or multicast packet*/
        MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
    } else {
        /* unicast packet */
        MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
    }
    /* increase ifoutdiscards or ifouterrors on error */

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);
    return 0;
}

static struct pbuf* wifi_eth_input(struct netif* netif)
{
    struct pbuf *p = NULL;
    u16_t len = 0;
    const uint8_t *data;

    data = WiFi_GetReceivedPacket(&len); // Gets the data frame content and size
    if(len == 0){
        return p;
    }
#ifdef WIFI_DISPLAY_PACKET_SIZE
    printf("[Recv] len=%hd\n", len);
#endif
#ifdef WIFI_DISPLAY_PACKET_RX
    WiFi_OutputBuffer(data, len);
#endif

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif


    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    if (p != NULL) 
    {
#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

        /* We iterate over the pbuf chain until we have read the entire
        * packet into the pbuf. */
        //for (q = p; q != NULL; q = q->next) {
        /* Read enough bytes to fill this pbuf in the chain. The
         * available data in the pbuf is given by the q->len
         * variable.
         * This does not necessarily have to be a memcpy, you can also preallocate
         * pbufs for a DMA-enabled MAC and after receiving truncate it to the
         * actually received size. In this case, ensure the tot_len member of the
         * pbuf is the sum of the chained pbuf len members.
         */
        //read data into(q->payload, q->len);
        //}
        pbuf_take(p, data, len); // Copy the data frame content to pbuf
        //acknowledge that packet has been read();

        MIB2_STATS_NETIF_ADD(netif, ifinoctets, p->tot_len);
        if (((u8_t*)p->payload)[0] & 1) {
            /* broadcast or multicast packet*/
            MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
        } else {
            /* unicast packet*/
            MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
        }
#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        LINK_STATS_INC(link.recv);
    } else {
        //drop packet(); // 
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
        MIB2_STATS_NETIF_INC(netif, ifindiscards);
    }

    return p;
}

struct ethernet_api g_wifi_eth_api = {
    .init     = wifi_eth_init,
    .output   = wifi_eth_output,
    .input    = wifi_eth_input
};

/* Private functions --------------------------------------------------------*/

