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
/*******
 * Notice:
 * The parts of this file refer to https://blog.csdn.net/zlk1214/article/details/76177336
 * Originator: ZLK1214
 *******/

/* Includes -----------------------------------------------------------------*/
#include "wifi_eth.h"
#include "wifi.h"
#include "sys_init.h"

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
#define netifINTERFACE_TASK_STACK_SIZE              ( 4096u )
#define netifINTERFACE_TASK_PRIORITY                ( 0u   )
/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT                      ( LOS_WAIT_FOREVER )

//#define TEST_SSID       "TP-LINK_80D8BE"
//#define TEST_PASSWORD   "87654321"
#define TEST_SSID       "TP-LINK_4CEAC8"
#define TEST_PASSWORD   "123456789"


/* Semaphore to signal incoming packets */
static UINT32 s_uwRxSemID;
static UINT32 s_uwWaitSemID;


/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
static struct netif *s_p88w8686 = NULL;

/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/
/* WiFi Authentication Complete Callback */
void WiFi_AuthenticationCompleteHandler(void)
{
    printf("Authenticated!\n");
    LOS_SemPost(s_uwWaitSemID);
}

/* WiFi Event Callback */
void WiFi_EventHandler(const WiFi_Event *event)
{
    printf("[Event %ld] size=%d", event->event_id, event->header.length);
    if (event->header.length >= sizeof(WiFi_Event) - sizeof(event->mac_addr))
        printf(", reason=%d", event->reason_code);
    if (event->header.length >= sizeof(WiFi_Event))
        printf(", MAC: %02X:%02X:%02X:%02X:%02X:%02X", event->mac_addr[0], event->mac_addr[1], event->mac_addr[2], event->mac_addr[3], event->mac_addr[4], event->mac_addr[5]);
    printf("\n");

    switch (event->event_id)
    {
    case 3:
        printf("Beacon Loss/Link Loss\n");
        break;
    case 4:
        printf("The number of stations in this ad hoc newtork has changed!\n");
        break;
    case 8:
        printf("Deauthenticated!\n");
        break;
    case 9:
        printf("Disassociated!\n");
        break;
    case 17:
        printf("All other stations have been away from this ad hoc network!\n");
        break;
    case 30:
        printf("IBSS coalescing process is finished and BSSID has changed!\n");
        break;
    }

    if (event->header.length > sizeof(WiFi_Event))
        WiFi_DumpData(event + 1, event->header.length - sizeof(WiFi_Event));
}

/* Handle receive packet */
void WiFi_PacketHandler(const WiFi_DataRx *data)
{
    ethernetif_input(s_p88w8686); 
}

static void wifi_associate_callback(void *arg, void *data, WiFi_Status status)
{
    switch (status)
    {
    case WIFI_STATUS_OK:
        printf("Associated!\n");
//        LOS_SemPost(s_uwWaitSemID);
        break;
    case WIFI_STATUS_NOTFOUND:
        printf("SSID not found!\n");
        break;
    case WIFI_STATUS_FAIL:
        printf("Association failed!\n");
        break;
    case WIFI_STATUS_INPROGRESS:
        printf("Waiting for authentication!\n");
        break;
    default:
        printf("Unknown error! status=%d\n", status);
    }
}

static void wifi_associate(void)
{
    WiFi_Connection conn;
    
    conn.security = WIFI_SECURITYTYPE_WPA; // this support WPA/WPA2
    conn.ssid = TEST_SSID;
    conn.password = TEST_PASSWORD; 
    WiFi_AssociateEx(&conn, WIFI_AUTH_MODE_OPEN, -1, wifi_associate_callback, NULL); // must use WIFI_AUTH_MODE_OPEN

    LOS_SemPend(s_uwWaitSemID, LOS_WAIT_FOREVER);
}

void wifi_scan_callback(void *arg, void *data, WiFi_Status status)
{
    if (status == WIFI_STATUS_OK)
        printf("Scan finished!\n");
    else
        printf("Scan failed!\n");
    
    LOS_SemPost(s_uwWaitSemID);
}

static void wifi_scan(void)
{
    WiFi_Scan(wifi_scan_callback, NULL); // scan APs
    LOS_SemPend(s_uwWaitSemID, LOS_WAIT_FOREVER);
}

static void wifi_mac_address_callback(void *arg, void *data, WiFi_Status status)
{
    if (status == WIFI_STATUS_OK)
    {
        memcpy(s_p88w8686->hwaddr, data, 6); // fill mac address
        printf("MAC Addr: %02X:%02X:%02X:%02X:%02X:%02X\n", s_p88w8686->hwaddr[0], s_p88w8686->hwaddr[1], 
            s_p88w8686->hwaddr[2], s_p88w8686->hwaddr[3], s_p88w8686->hwaddr[4], s_p88w8686->hwaddr[5]);
        
        LOS_SemPost(s_uwWaitSemID);
    }
    else
        printf("Cannot get MAC address!\n");
}

static void wifi_get_mac_address(void)
{
    WiFi_GetMACAddress(wifi_mac_address_callback, NULL);
    LOS_SemPend(s_uwWaitSemID, LOS_WAIT_FOREVER);
}

void stop_callback(void *arg, void *data, WiFi_Status status)
{
    char *s1 = (char *)arg;
    char *s2 = s1 + strlen(s1) + 1;
    if (status == WIFI_STATUS_OK)
    {
        printf("%s %s!\n", s1, s2);
    }
    else
        printf("%s not %s!\n", s1, s2);
}

/* Public functions ---------------------------------------------------------*/
static void wifi_eth_thread(void *arg)
{
    UINT32 uwRet;
    
    while (1)
    {
        uwRet = LOS_SemPend(s_uwRxSemID, LOS_WAIT_FOREVER);
        if(uwRet == LOS_OK)
        {
            WiFi_Input();
        }
    }
}

void SDIO_IRQHandler(void)
{
    if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_SDIOIT) == SET){
        __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_SDIOIT);
        LOS_SemPost(s_uwRxSemID);
    }
}

static int8_t wifi_eth_init(struct netif* netif)
{
    WiFi_Init();
    
    LOS_SemCreate(0, &s_uwRxSemID);
    LOS_SemCreate(0, &s_uwWaitSemID);
    
    /* create the task that handles the ETH_MAC */
    sys_thread_new((char*)"mrvl_wifi", wifi_eth_thread, netif, netifINTERFACE_TASK_STACK_SIZE, netifINTERFACE_TASK_PRIORITY);

    LOS_HwiCreate(SDIO_IRQn, 0, 3, SDIO_IRQHandler, 0);
    __SDIO_ENABLE_IT(SDIO, SDIO_IT_SDIOIT);

    s_p88w8686 = netif;

    wifi_get_mac_address();
    wifi_scan();
    wifi_associate();

    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set MAC hardware address */

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

#if LWIP_IPV6 && LWIP_IPV6_MLD
    /*
     * For hardware/netifs that implement MAC filtering.
     * All-nodes link-local is handled by default, so we must let the hardware know
     * to allow multicast packets in.
     * Should set mld_mac_filter previously. */
    if (netif->mld_mac_filter != NULL) {
        ip6_addr_t ip6_allnodes_ll;
        ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
        netif->mld_mac_filter(netif, &ip6_allnodes_ll, NETIF_ADD_MAC_FILTER);
    }
#endif /* LWIP_IPV6 && LWIP_IPV6_MLD */
    
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

