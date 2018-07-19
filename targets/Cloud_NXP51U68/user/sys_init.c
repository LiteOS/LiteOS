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

#include "sys_init.h"
#include "lwip/dhcp.h"

struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

VOID *main_network(UINT32  args)
{
    /* IP addresses initialization */
    IP_ADDRESS[0] = 192;
    IP_ADDRESS[1] = 168;
    IP_ADDRESS[2] = 3;
    IP_ADDRESS[3] = 10;
    NETMASK_ADDRESS[0] = 255;
    NETMASK_ADDRESS[1] = 255;
    NETMASK_ADDRESS[2] = 255;
    NETMASK_ADDRESS[3] = 0;
    GATEWAY_ADDRESS[0] = 192;
    GATEWAY_ADDRESS[1] = 168;
    GATEWAY_ADDRESS[2] = 3;
    GATEWAY_ADDRESS[3] = 1;

    /* Initilialize the LwIP stack without RTOS */
    tcpip_init(NULL, NULL);
    printf("lwip  init ok\n");
    /* IP addresses initialization without DHCP (IPv4) */
    IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
    IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
    IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

    /* add the network interface (IPv4/IPv6) without RTOS */
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    /* Registers the default network interface */
    netif_set_default(&gnetif);
    if (netif_is_link_up(&gnetif))
    {
        /* When the netif is fully configured this function must be called */
        gnetif.flags |= NETIF_FLAG_LINK_UP;
        netif_set_up(&gnetif);
    }
    else
    {
        /* When the netif link is down this function must be called */
        netif_set_down(&gnetif);
    }
   
    struct dhcp *dhcp;
    dhcp_start(&gnetif);
    while (1) {
        if (dhcp_supplied_address(&gnetif))
        {
            dhcp = netif_dhcp_data(&gnetif);
            printf("IP address: %s\n", ip4addr_ntoa(&dhcp->offered_ip_addr));
            printf("Subnet mask: %s\n", ip4addr_ntoa(&dhcp->offered_sn_mask));
            printf("Default gateway: %s\n", ip4addr_ntoa(&dhcp->offered_gw_addr));
            break;
        }
        LOS_TaskDelay(1000);
    }
    
    extern void agent_tiny_entry(void);
    agent_tiny_entry(); //will never come back until quit
    return NULL;
}

/**
 * atiny_adapter user interface 
 */
void atiny_usleep(unsigned long usec)
{
    volatile int i,j;
    for(j =0;j <usec;j++)
    {
        i = 10000;
        while(i >0)
        {
            i--;
        }
    }
    return;
}

int atiny_random(void* output, size_t len)
{
    int ret = -1;
    unsigned char *dst;
    int value;
    dst = output;
    if((NULL == output)||(len <= 0))
    {
        return ret;
    }
    while(len-- > 0)
    {
        value = rand();
        *dst++= (uint8_t)value;
    }

    return 0;
}

void atiny_reboot(void)
{
    while(1);
}

