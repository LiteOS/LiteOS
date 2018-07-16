/******************************************************************************

  Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>

 ******************************************************************************
  File Name     : wifi_sdio_demo.c
  Version       : Initial Draft
  Author        : MAMINJIE
  Created       : 2018/5/26
  Last Modified :
  Description   : marvell 88w8686 sdio wifi demo
  Function List :
  History       :
  1.Date        : 2018/5/26
    Author      : MAMINJIE
    Modification: Created file

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
#include "lwip/netif.h"
#include "lwip/dhcp.h" 
#include "lwip/dns.h" 
#include "lwip/init.h" 
#include "lwip/timeouts.h" 
#include "netif/ethernet.h"
#include "wifi_eth.h"
#include "wifi_sdio_demo.h"
#include "sys_init.h"
#include "agent_tiny_demo.h"

/* Defines ------------------------------------------------------------------*/
//#define TEST_SSID       "TP-LINK_80D8BE"
//#define TEST_PASSWORD   "87654321"
//#define TEST_SSID       "mimax"
#define TEST_SSID       "TP-LINK_4CEAC8"
#define TEST_PASSWORD   "123456789"


#if LWIP_DHCP
#define USE_DHCP    0 // support failuer
#endif

#define DHCP_OFF                      0
#define DHCP_START                    1
#define DHCP_WAIT_ADDRESS             2
#define DHCP_ADDRESS_ASSIGNED         3
#define DHCP_TIMEOUT                  4
#define DHCP_LINK_DOWN                5

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0                    192
#define IP_ADDR1                    168
#define IP_ADDR2                    1
#define IP_ADDR3                    250

/*NETMASK*/
#define NETMASK_ADDR0               255
#define NETMASK_ADDR1               255
#define NETMASK_ADDR2               255
#define NETMASK_ADDR3               0

/*Gateway Address*/
#define GW_ADDR0                    192
#define GW_ADDR1                    168
#define GW_ADDR2                    1
#define GW_ADDR3                    1

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
static struct netif wifi_88w8686;

static UINT32 g_usSemID;

/* Extern variables ---------------------------------------------------------*/

/* Global variables ---------------------------------------------------------*/
UINT32 g_lwm2mHandle = 0xFFFF;

/* Private function prototypes ----------------------------------------------*/


/* Public functions ---------------------------------------------------------*/
static void net_init(void)
{
#if !USE_DHCP
    struct ip4_addr ipaddr, netmask, gw;
#endif
    /* Initilialize the LwIP stack without RTOS */
    tcpip_init(NULL, NULL);

    (void)ethernetif_api_register(&g_wifi_eth_api);

#if USE_DHCP
    netif_add(&wifi_88w8686, IP_ADDR_ANY, IP_ADDR_ANY, IP_ADDR_ANY, NULL, ethernetif_init, ethernet_input);
#else

    /* IP addresses initialization without DHCP (IPv4) */
    IP_ADDR4(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
    IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
    
    netif_add(&wifi_88w8686, &ipaddr, &netmask, &gw, NULL, ethernetif_init, ethernet_input); 

#if LWIP_DNS
    IP4_ADDR(&ipaddr, 8, 8, 8, 8); // first dns server
    dns_setserver(0, &ipaddr);
    IP4_ADDR(&ipaddr, 8, 8, 4, 4); // second dns server
    dns_setserver(1, &ipaddr);
#endif
#endif
    netif_set_default(&wifi_88w8686);
}


void get_address_info(void)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;
    
    __IO uint8_t dhcp_state = DHCP_OFF;
    struct dhcp *dhcp;
    dhcp_state = DHCP_START;
    struct netif *netif = &wifi_88w8686;
    while(1)
    {   
        if(dhcp_state == DHCP_ADDRESS_ASSIGNED){
            dhcp = netif_dhcp_data(netif);
            printf("IP address: %s\n", ip4addr_ntoa(&dhcp->offered_ip_addr));
            printf("Subnet mask: %s\n", ip4addr_ntoa(&dhcp->offered_sn_mask));
            printf("Default gateway: %s\n", ip4addr_ntoa(&dhcp->offered_gw_addr));
            break;
        }else if(dhcp_state == DHCP_TIMEOUT){
            break;
        }
        switch (dhcp_state)
        {
        case DHCP_START:
        {
            ip_addr_set_zero_ip4(&netif->ip_addr);
            ip_addr_set_zero_ip4(&netif->netmask);
            ip_addr_set_zero_ip4(&netif->gw);       
            dhcp_start(netif);
            dhcp_state = DHCP_WAIT_ADDRESS;
        }
        break;
        case DHCP_WAIT_ADDRESS:
        {                
            if (dhcp_supplied_address(netif)) {
                dhcp_state = DHCP_ADDRESS_ASSIGNED; 
            }else{
                dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

                /* DHCP timeout */
                if (dhcp->tries++ > 10)
                {
                    printf("dhcp timeout!\n");
                    dhcp_state = DHCP_TIMEOUT;

                    /* Stop DHCP */
                    dhcp_stop(netif);

                    /* Static address used */
                    IP_ADDR4(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
                    IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
                    IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
                    netif_set_addr(netif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
                }
            }
        }
        break;
        case DHCP_LINK_DOWN:
        {
            /* Stop DHCP */
            dhcp_stop(netif);
            dhcp_state = DHCP_OFF; 
        }
        break;
        default: break;
        }

        LOS_TaskDelay(1000);
        printf("wait for ip!!!\n");
    }

}
void lwm2m_task(void)
{
#if USE_DHCP 
    get_address_info();
#endif
    agent_tiny_entry();
}


UINT32 lwm2m_test(void)
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "lwm2m_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)lwm2m_task;
    task_init_param.uwStackSize = 0x1000;

    if(0xFFFF != g_lwm2mHandle){
        uwRet = LOS_TaskDelete(g_lwm2mHandle);
        if(LOS_OK != uwRet)
        {
            WIFI_LOG("delete task failed!, ret=%d", uwRet);
            return uwRet;
        }
        g_lwm2mHandle = 0xFFFF;
    }
    uwRet = LOS_TaskCreate(&g_lwm2mHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        WIFI_LOG("create task failed!, ret = %d", uwRet);
        return uwRet;
    }
    return uwRet;
        
}

static void set_netif(struct netif *netif, uint8_t up)
{
    if (up)
    {
        net_init();
        if (!netif_is_up(netif))
        {
            netif_set_up(netif);
            lwm2m_test();
        }
    }
    else
    {
        netif_set_down(netif);
#if USE_DHCP
        dhcp_release(netif);
        dhcp_stop(netif);
#endif
    }
}


/* WiFi Authentication Complete Callback */
void WiFi_AuthenticationCompleteHandler(void)
{
    printf("Authenticated!\n");
    set_netif(&wifi_88w8686, 1); // start network card
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
        set_netif(&wifi_88w8686, 0);
        break;
    case 4:
        printf("The number of stations in this ad hoc newtork has changed!\n");
        set_netif(&wifi_88w8686, 1);
        break;
    case 8:
        printf("Deauthenticated!\n");
        set_netif(&wifi_88w8686, 0);
        break;
    case 9:
        printf("Disassociated!\n");
        set_netif(&wifi_88w8686, 0);
        break;
    case 17:
        printf("All other stations have been away from this ad hoc network!\n");
        set_netif(&wifi_88w8686, 0);
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
    ethernetif_input(&wifi_88w8686); 
}

void associate_callback(void *arg, void *data, WiFi_Status status)
{
    switch (status)
    {
    case WIFI_STATUS_OK:
        printf("Associated!\n");
        set_netif(&wifi_88w8686, 1); 
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

void associate_example(void)
{
    WiFi_Connection conn;
    
    conn.security = WIFI_SECURITYTYPE_WPA2; // this support WPA/WPA2
    conn.ssid = TEST_SSID;
    conn.password = TEST_PASSWORD; 
    WiFi_AssociateEx(&conn, WIFI_AUTH_MODE_OPEN, -1, associate_callback, NULL); // must use WIFI_AUTH_MODE_OPEN
}

void scan_callback(void *arg, void *data, WiFi_Status status)
{
    if (status == WIFI_STATUS_OK)
        printf("Scan finished!\n");
    else
        printf("Scan failed!\n");

    associate_example();
}

void mac_address_callback(void *arg, void *data, WiFi_Status status)
{
    if (status == WIFI_STATUS_OK)
    {
        WiFi_Scan(scan_callback, NULL); // scan APs

        memcpy(wifi_88w8686.hwaddr, data, 6); // fill mac address
        printf("MAC Addr: %02X:%02X:%02X:%02X:%02X:%02X\n", wifi_88w8686.hwaddr[0], wifi_88w8686.hwaddr[1], 
            wifi_88w8686.hwaddr[2], wifi_88w8686.hwaddr[3], wifi_88w8686.hwaddr[4], wifi_88w8686.hwaddr[5]);
    }
    else
        printf("Cannot get MAC address!\n");
}

void stop_callback(void *arg, void *data, WiFi_Status status)
{
    char *s1 = (char *)arg;
    char *s2 = s1 + strlen(s1) + 1;
    if (status == WIFI_STATUS_OK)
    {
        set_netif(&wifi_88w8686, 0);
        printf("%s %s!\n", s1, s2);
    }
    else
        printf("%s not %s!\n", s1, s2);
}

void SDIO_IRQHandler(void)
{
    if (__SDIO_GET_FLAG(SDIO, SDIO_FLAG_SDIOIT) == SET){
        __SDIO_CLEAR_FLAG(SDIO, SDIO_FLAG_SDIOIT);
        LOS_SemPost(g_usSemID);
    }
}

void wifi_sdio_demo(void)
{
    UINT32 uwRet;
    
    printf("NUCLEO-429ZI SDIO WIFI DEMO....\n");
    
    LOS_SemCreate(0,&g_usSemID);

    WiFi_Init();

    LOS_HwiCreate(SDIO_IRQn, 0, 3, SDIO_IRQHandler, 0);
    __SDIO_ENABLE_IT(SDIO, SDIO_IT_SDIOIT);

    WiFi_GetMACAddress(mac_address_callback, NULL);
    
    printf("WIFI init successfully!\n");

    while(1)
    {
        uwRet = LOS_SemPend(g_usSemID, LOS_WAIT_FOREVER); 
        if(LOS_OK == uwRet){
            WiFi_Input();
        }
    }
}

/* Private functions --------------------------------------------------------*/


