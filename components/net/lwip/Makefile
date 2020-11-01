include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LWIP_VERION = lwip-2.1.2
LWIP_DIR = $(LITEOSTOPDIR)/components/net/lwip
LWIP_INC_DIR = $(LITEOSTOPDIR)/components/net
LWIP_SRC_DIR = $(LWIP_DIR)/$(LWIP_VERION)/src

LWIP_SRCS = \
    $(wildcard $(LWIP_VERION)/src/api/*.c)                \
    $(wildcard $(LWIP_VERION)/src/apps/mqtt/*.c)          \
    $(wildcard $(LWIP_VERION)/src/apps/sntp/*.c)          \
    $(wildcard $(LWIP_VERION)/src/apps/mdns/*.c)          \
    $(wildcard $(LWIP_VERION)/src/apps/netbiosns/*.c)     \
    $(wildcard $(LWIP_VERION)/src/core/*.c)               \
    $(wildcard $(LWIP_VERION)/src/core/ipv4/*.c)          \
    $(wildcard $(LWIP_VERION)/src/core/ipv6/*.c)          \
    $(wildcard $(LWIP_VERION)/src/netif/*.c)              \
    $(wildcard $(LWIP_VERION)/src/netif/ppp/*.c)          \
    $(wildcard $(LWIP_VERION)/src/netif/ppp/polarssl/*.c) \
    $(wildcard lwip_port/OS/*.c)

LWIP_INC := \

LOCAL_CFLAGS += -Wno-maybe-uninitialized

LOCAL_SRCS :=  $(LWIP_SRCS)

LOCAL_INCLUDE := $(LWIP_INC)
LOCAL_FLAGS :=  $(LITEOS_CFLAGS_INTERWORK) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
