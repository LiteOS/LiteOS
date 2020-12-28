include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/net/lwip/lwip.mk

MODULE_NAME := $(notdir $(CURDIR))

SAL_SRC = \
    $(wildcard *.c)

SAL_INC := \
    -I $(LITEOSTOPDIR)/include

LOCAL_SRCS := $(subst $(CURDIR)/,, $(SAL_SRC))

LOCAL_INCLUDE := $(SAL_INC) $(LWIP_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
