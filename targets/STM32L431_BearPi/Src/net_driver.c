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

#ifdef WITH_LWIP

#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "netif/etharp.h"
#include "lwip/sockets.h"
#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/timeouts.h"
#include "ethernetif.h"
#include "hal_rng.h"

extern struct ethernet_api g_eth_api;


struct     netif gnetif;
uint8_t    IP_ADDRESS[4];
uint8_t    NETMASK_ADDRESS[4];
uint8_t    GATEWAY_ADDRESS[4];

#if LWIP_IPV4 && LWIP_IPV6
ip_addr_t  ipaddr;
ip_addr_t  netmask;
ip_addr_t  gw;
#elif LWIP_IPV6
#else
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
#endif

static void lwip_impl_register(void)
{
    STlwIPFuncSsp stlwIPSspCbk = {0};
    stlwIPSspCbk.pfRand = hal_rng_generate_number;
    lwIPRegSspCbk(&stlwIPSspCbk);
}

static void net_init(void)
{
    /* IP addresses initialization */
    IP_ADDRESS[0] = 192;
    IP_ADDRESS[1] = 168;
    IP_ADDRESS[2] = 1;
    IP_ADDRESS[3] = 115;
    NETMASK_ADDRESS[0] = 255;
    NETMASK_ADDRESS[1] = 255;
    NETMASK_ADDRESS[2] = 255;
    NETMASK_ADDRESS[3] = 0;
    GATEWAY_ADDRESS[0] = 192;
    GATEWAY_ADDRESS[1] = 168;
    GATEWAY_ADDRESS[2] = 1;
    GATEWAY_ADDRESS[3] = 1;

#if LWIP_IPV4 && LWIP_IPV6
    IP_ADDR4(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
    IP_ADDR4(&netmask,NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
    IP_ADDR4(&gw,     GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
#elif LWIP_IPV6
    //IP_ADDR6(&ipaddr,);
#else 
    /* IP addresses initialization without DHCP (IPv4) */
    IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
    IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
    IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
#endif

    lwip_impl_register();


    (void)ethernetif_api_register(&g_eth_api);/*×¢²áÌØ¶¨Íø¿¨µÄAPI*/
    /* add the network interface (IPv4/IPv6) without RTOS */
#if LWIP_IPV4 && LWIP_IPV6
    (void)netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input);//lint !e546
#elif LWIP_IPV6

    (void)netif_add(&gnetif, NULL, ethernetif_init, tcpip_input);
    netif_create_ip6_linklocal_address(&gnetif, 1);
    {
        ip6_addr_t ip6;
        err_t ret;
        s8_t idx;
        ip6_addr_t ipv6_gw;

        if (inet_pton(AF_INET6, "2000::2", &ip6) <= 0)
        {
            printf("set source ip6 failed \n");
            return;
        }
        ret = netif_add_ip6_address(&gnetif, &ip6, &idx);
        if (ret != 0)
        {
            printf("netif_add_ip6_address failed,ret %d\n", ret);
            return;
        }

        if (inet_pton(AF_INET6, "2000::1", &ipv6_gw) <= 0)
        {
            printf("inet_pton failed\n");
            return;
        }
        set_lwip_ipv6_default_gw(&gnetif, &ipv6_gw);
    }
#else
    (void)netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input);//lint !e546
#endif

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
}

static void hieth_hw_init(void)
{
    extern void ETH_IRQHandler(void);
    (void)LOS_HwiCreate(ETH_IRQn, 1, 0, ETH_IRQHandler, 0);
}

int netdriver_install()
{

    hieth_hw_init();
    net_init();

    printf("%s:install the netdriver\r\n",__FUNCTION__);

    return 0;
}



#endif


