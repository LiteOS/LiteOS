LWIP_VERSION = lwip-2.1.2

LWIP_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/net/lwip/lwip_port \
    -I $(LITEOSTOPDIR)/components/net/lwip/lwip_port/OS \
    -I $(LITEOSTOPDIR)/components/net/lwip/lwip_port/arch \
    -I $(LITEOSTOPDIR)/components/net/lwip/ppp_port/osport \
    -I $(LITEOSTOPDIR)/components/net/lwip/$(LWIP_VERSION)/src/include
