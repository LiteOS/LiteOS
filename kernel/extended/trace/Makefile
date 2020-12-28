include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard *.c)

ifeq ($(LOSCFG_TRACE_CLIENT_INTERACT), y)
LOCAL_SRCS += $(wildcard pipeline/*.c)
endif

ifeq ($(LOSCFG_TRACE_PIPELINE_SERIAL), y)
LOCAL_SRCS += $(wildcard pipeline/serial/*.c)
endif

LOCAL_INCLUDE := \
        -I $(LITEOSTOPDIR)/kernel/base/include \
        -I $(LITEOSTOPDIR)/kernel/extended/include \
        -I $(LITEOSTOPDIR)/kernel/extended/trace/pipeline

ifeq ($(LOSCFG_TRACE_PIPELINE_SERIAL), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/trace/pipeline/serial
endif

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE)

