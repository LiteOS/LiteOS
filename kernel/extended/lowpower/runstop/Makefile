include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard src/*.c) $(wildcard src/*.s)

LOCAL_INCLUDE := \
	-I $(LITEOSTOPDIR)/kernel/extended/include \
	-I $(LITEOSTOPDIR)/kernel/extended/include/lowpower
LOCAL_FLAGS := $(LOCAL_INCLUDE)

LOCAL_CFLAGS := $(LITEOS_GCOV_OPTS)

include $(MODULE)
