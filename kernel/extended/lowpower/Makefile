include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := \
        -I $(LITEOSTOPDIR)/kernel/extended/include \
        -I $(LITEOSTOPDIR)/kernel/extended/include/lowpower

LOCAL_FLAGS := $(LOCAL_INCLUDE)

MODULE_$(LOSCFG_KERNEL_POWER_MGR) += powermgr
MODULE_$(LOSCFG_KERNEL_RUNSTOP) += runstop

ifeq ($(LOSCFG_KERNEL_DEEPSLEEP), y)
MODULE_y += tickless
else
MODULE_$(LOSCFG_KERNEL_TICKLESS) += tickless
endif
include $(MODULE)
