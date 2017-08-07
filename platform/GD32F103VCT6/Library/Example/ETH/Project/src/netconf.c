/**
  ******************************************************************************
  * @file    ETH/Project/src/netconf.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Net configuration.  
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "platform_cfg.h"
#include "gd32f10x.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
#define LCD_TIMER_MSECS       250
#define MAX_DHCP_TRIES        4
#define SELECTED              1
#define NOT_SELECTED         (!SELECTED)
#define CLIENTMAC6            2

/* Private variables ---------------------------------------------------------*/
struct netif netif;
__IO uint32_t TCPTimer = 0;
__IO uint32_t ARPTimer = 0;

#ifdef LWIP_DHCP
__IO uint32_t DHCPfineTimer = 0;
__IO uint32_t DHCPcoarseTimer = 0;
static uint32_t IPaddress = 0;
#endif

__IO uint32_t DisplayTimer = 0;

/* Private function prototypes -----------------------------------------------*/
void Delay_Count(__IO uint32_t nCount);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize the lwIP stack
  * @param  None
  * @retval None
  */
void LwIP_Init(void)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
 
    uint8_t macaddress[6]={0xcc,0xbb,0xaa,0x99,0x88,0x1};

    /* Initialize the dynamic memory heap defined by MEM_SIZE */
    mem_init();

    /* Initialize the memory pools defined by MEMP_NUM_x */
    memp_init();

    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, MASK_0, MASK_1, MASK_2, MASK_3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

    Set_MAC_Address(macaddress);

    /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
 		 struct ip_addr *netmask, struct ip_addr *gw,
         void *state, err_t (* init)(struct netif *netif),
         err_t (* input)(struct pbuf *p, struct netif *netif))
    
    Adds your network interface to the netif_list. Allocate a struct
    netif and pass a pointer to this structure as the first argument.
    Give pointers to cleared ip_addr structures when using DHCP,
    or fill them with sane numbers otherwise. The state pointer may be NULL.

    The init function pointer must point to a initialization function for
    your ethernet netif interface. The following code illustrates it's use */
    netif_add(&netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    /*  Registers the default network interface */
    netif_set_default(&netif);


#if LWIP_DHCP
    /*  Create a new DHCP client for this interface on the first call.
    Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
    the predefined regular intervals after starting the client.
    You can peek in the netif->dhcp struct for the actual DHCP status */
    dhcp_start(&netif);
#endif

    /* When the netif is fully configured this function must be called */
    netif_set_up(&netif);
}

/**
  * @brief  Called when a frame is received
  * @param  None
  * @retval None
  */
void LwIP_Pkt_Handle(void)
{
    /* Read a received packet from the Ethernet buffers 
       and send it to the lwIP for handling */
    ethernetif_input(&netif);
}

/**
  * @brief  LwIP periodic tasks
  * @param  localtime the current LocalTime value
  * @retval None
  */
void LwIP_Periodic_Handle(__IO uint32_t localtime)
{

    /* TCP periodic process every 250 ms */
    if ((localtime - TCPTimer) >= TCP_TMR_INTERVAL)
    {
        TCPTimer =  localtime;
        tcp_tmr();
    }
    /* ARP periodic process every 5s */
    if ((localtime - ARPTimer) >= ARP_TMR_INTERVAL)
    {
        ARPTimer =  localtime;
        etharp_tmr();
    }

#if LWIP_DHCP
    /* Fine DHCP periodic process every 500ms */
    if (localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
    {
        DHCPfineTimer =  localtime;
        dhcp_fine_tmr();
    }
    /* DHCP Coarse periodic process every 60s */
    if (localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
    {
        DHCPcoarseTimer =  localtime;
        dhcp_coarse_tmr();
    }
#endif
}

/**
  * @brief  Show message periodic handling
  * @param  localtime: the current LocalTime value
  * @retval None
  */
void Display_Periodic_Handle(__IO uint32_t localtime)
{ 
    uint8_t macaddress[6];
    u16 PHYRegData;
    /* 250 ms */
    if (localtime - DisplayTimer >= LCD_TIMER_MSECS)
    {
        DisplayTimer = localtime;
        /* We have got a new IP address so update the display */
        if (IPaddress != netif.ip_addr.addr)
        {
            __IO uint8_t iptab[4];
            uint8_t iptxt[20];
    
            /* Read the new IP address */
            IPaddress = netif.ip_addr.addr;

            iptab[0] = (uint8_t)(IPaddress >> 24);
            iptab[1] = (uint8_t)(IPaddress >> 16);
            iptab[2] = (uint8_t)(IPaddress >> 8);
            iptab[3] = (uint8_t)(IPaddress);

            sprintf((char*)iptxt, "   %d.%d.%d.%d    ", iptab[3], iptab[2], iptab[1], iptab[0]);

            /* Display the new IP address */
#if LWIP_DHCP
            if (netif.flags & NETIF_FLAG_DHCP)
            {            
                /* Display the IP address */
                printf("IP address assigned \r\n");
                printf("  by a DHCP server  \r\n");
                printf("%s\r\n",iptxt);         
                Delay_Count(800 * KEY_DELAY);
            }
            else
#endif /* LWIP_DHCP */
            {        
                /* Display the IP address */
                printf("  Static IP address\r\n");
                printf("%s\r\n", iptxt); 
                ETH_GetMACAddress(0, macaddress);
                printf("\n\r Your MAC are configured: %X:%X:%X:%X:%X:%X", macaddress[0], macaddress[1], macaddress[2], macaddress[3], macaddress[4], macaddress[5]);
                printf("\n\r Static IP address: %s", iptxt);
            }           
            PHYRegData = ETH_GetPHYRegisterValue(PHY_ADDRESS,16);
            if(PHYRegData & 0x0002)
            {  
                /* Set Ethernet speed to 10M following the autonegotiation */    
                printf("\n\r==>ETH_Speed_10M!");         
            }
            else
            {   
                /* Set Ethernet speed to 100M following the autonegotiation */ 
                printf("\n\r==>ETH_Speed_100M!");                 
            } 
            /* Configure the MAC with the Duplex Mode fixed by the autonegotiation process */
            if((PHYRegData & 0x0004) != (uint32_t)RESET)
            {
                /* Set Ethernet duplex mode to FullDuplex following the autonegotiation */
                printf("\n\r==>ETH_Mode_FullDuplex!");        
            }
            else
            {
                /* Set Ethernet duplex mode to HalfDuplex following the autonegotiation */
                printf("\n\r==>ETH_Mode_HalfDuplex!");
            }

            iptab[0] = (uint8_t)(IPaddress >> 24);
            iptab[1] = (uint8_t)(IPaddress >> 16);
            iptab[2] = (uint8_t)(IPaddress >> 8);
            iptab[3] = (uint8_t)(IPaddress);
            sprintf((char*)iptxt, "ip: %d.%d.%d.%d ", iptab[3], iptab[2], iptab[1], iptab[0]);
            ETH_GetMACAddress(0, macaddress);
            printf("\n\r Your MAC are configured: %X:%X:%X:%X:%X:%X", macaddress[0], macaddress[1], macaddress[2], macaddress[3], macaddress[4], macaddress[5]);
            printf("\n\r Your Ip are configured: %s\n\r", &iptxt[3]);
        
            /* Read the new gw address Gigadevice */
            IPaddress = netif.gw.addr;
            iptab[0] = (uint8_t)(IPaddress >> 24);
            iptab[1] = (uint8_t)(IPaddress >> 16);
            iptab[2] = (uint8_t)(IPaddress >> 8);
            iptab[3] = (uint8_t)(IPaddress);
            sprintf((char*)iptxt, "gw: %d.%d.%d.%d  ", iptab[3], iptab[2], iptab[1], iptab[0]);
            printf(" Your gw are configured: %s\n\r", &iptxt[3]);
            /* The new IP address */
            IPaddress = netif.ip_addr.addr;      
        }

#if LWIP_DHCP
    
        else if (IPaddress == 0)
        {  
            printf("     Looking for    "); 
            printf("     DHCP server    ");
            printf("     please wait... ");      
            /* If no response from a DHCP server for MAX_DHCP_TRIES times */
            /* Stop the dhcp client and set a static IP address */
            if (netif.dhcp->tries > MAX_DHCP_TRIES)
            {
                struct ip_addr ipaddr;
                struct ip_addr netmask;
                struct ip_addr gw;         
                printf("    DHCP timeout    ");   
                dhcp_stop(&netif);

                IP4_ADDR(&ipaddr, 192, 168, 16,100 );
                IP4_ADDR(&netmask, 255, 255, 255, 0);
                IP4_ADDR(&gw, 192, 168, 16, 1);

                netif_set_addr(&netif, &ipaddr , &netmask, &gw);
            }
        }
#endif
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
