include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := m

LOCAL_SRCS = $(wildcard ./*.c)

LOCAL_INCLUDE := \
        -I $(LITEOSTOPDIR)/lib/libc/src/include \
        -I $(LITEOSTOPDIR)/lib/libc/src/internal

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE_LIB)

