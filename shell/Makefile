include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

ifeq ($(LOSCFG_SHELL_FULL_CAP), y)
LOCAL_SRCS := $(wildcard full/src/base/*.c) \
              $(wildcard full/src/cmds/*.c) \

ifeq ($(LOSCFG_SHELL_UART), y)
LOCAL_SRCS += full/src/base/porting/port_uart.c
endif
endif

ifeq ($(LOSCFG_SHELL_FULL_CAP), y)
LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/include \
    -I $(LITEOSTOPDIR)/shell/full/include \
    -I $(LITEOSTOPDIR)/components/fs/ramfs \
    -I $(LITEOSTOPDIR)/components/fs/fat \
    -I $(LITEOSTOPDIR)/kernel/base/include
endif


ifndef LOSCFG_PLATFORM_OSAPPINIT
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/test/include
endif

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) $(LITEOS_CFLAGS_INTERWORK)

include $(MODULE)
