include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/net/lwip/lwip.mk

MODULE_NAME := ipv6_client

IPV6_CLIENT_SRCS = $(wildcard *.c)

IPV6_CLIENT_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/ipv6_client \
    -I $(LITEOSTOPDIR)/include

IPV6_CLIENT_INCLUDE += $(LWIP_INCLUDE)

LOCAL_INCLUDE := $(IPV6_CLIENT_INCLUDE)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(IPV6_CLIENT_SRCS))

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE)
