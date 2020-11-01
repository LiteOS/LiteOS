include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := \
        -I $(LITEOSTOPDIR)/kernel/base/include -I $(LITEOSTOPDIR)/kernel/extended/include \
        -I $(LITEOSTOPDIR)/kernel/base/core

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE)

