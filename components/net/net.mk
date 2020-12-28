COMPONENTS_NET_INCLUDE :=

ifeq ($(LOSCFG_COMPONENTS_NET_LWIP), y)
include $(LITEOSTOPDIR)/components/net/lwip/lwip.mk
COMPONENTS_INCLUDE += $(LWIP_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_NET_SAL), y)
COMPONENTS_NET_INCLUDE += \
    -I $(LITEOSTOPDIR)/include/sal
endif

ifeq ($(LOSCFG_COMPONNETS_NET_AT), y)
include $(LITEOSTOPDIR)/components/net/at_device/at.mk
COMPONENTS_INCLUDE += $(COMPONENTS_AT_INCLUDE)
endif