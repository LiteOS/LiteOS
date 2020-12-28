include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

NB_IOT_SRCS := $(wildcard *.c)

NB_IOT_INC := \
    -I $(LITEOSTOPDIR)/components/net/at_device/nb_bc95 \
    -I $(LITEOSTOPDIR)/include

LOCAL_SRCS := $(NB_IOT_SRCS)

LOCAL_INCLUDE := $(NB_IOT_INC)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
