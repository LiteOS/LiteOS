include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

OSDEP_SRC = \
    $(wildcard liteos/*.c)

OSDEP_INCLUDE := \
    -I $(LITEOSTOPDIR)/include \
    -I $(LITEOSTOPDIR)/kernel/include

LOCAL_SRCS += $(subst $(CURDIR)/,, $(OSDEP_SRC))

LOCAL_INCLUDE += $(OSDEP_INCLUDE)

LOCAL_FLAGS +=  $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)
include $(MODULE)