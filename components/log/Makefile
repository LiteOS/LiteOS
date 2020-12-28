include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOG_SRC = \
    $(wildcard *.c)

LOCAL_SRCS += $(subst $(CURDIR)/,, $(LOG_SRC))

LOG_INCLUDE := \
    -I $(LITEOSTOPDIR)/include \
    -I $(LITEOSTOPDIR)/kernel/include

LOCAL_INCLUDE += $(LOG_INCLUDE)
LOCAL_FLAGS +=  $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
