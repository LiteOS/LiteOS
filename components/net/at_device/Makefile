include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

AT_SRCS :=
AT_INC :=
ifeq ($(LOSCFG_COMPONENTS_NET_AT_ESP8266), y)
AT_SRCS := $(wildcard wifi_esp8266/*.c)
AT_INC := -I $(LITEOSTOPDIR)/components/net/at_device/wifi_esp8266
endif

ifeq ($(LOSCFG_COMPONENTS_NET_AT_BG36), y)
AT_SRCS := $(wildcard emtc_bg36/*.c)
AT_INC := -I $(LITEOSTOPDIR)/components/net/at_device/emtc_bg36
endif

ifeq ($(LOSCFG_COMPONENTS_NET_AT_SIM900A), y)
AT_SRCS := $(wildcard gprs_sim900a/*.c)
AT_INC := -I $(LITEOSTOPDIR)/components/net/at_device/gprs_sim900a
endif

ifeq ($(LOSCFG_COMPONENTS_NET_AT_BC95), y)
AT_SRCS := $(wildcard nb_bc95/*.c)
AT_INC := -I $(LITEOSTOPDIR)/components/net/at_device/nb_bc95
endif

AT_INC := -I $(LITEOSTOPDIR)/include

LOCAL_SRCS := $(AT_SRCS)

LOCAL_INCLUDE := $(AT_INC)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
