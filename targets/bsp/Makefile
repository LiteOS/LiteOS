include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := bsp

LOCAL_SRCS = $(wildcard board/$(LITEOS_PLATFORM)/*.c) \
            $(wildcard $(HWI_SRC)/*.c) \
            $(wildcard $(TIMER_SRC)/*.c) \
            $(wildcard $(HRTIMER_SRC)/*.c) \
            $(wildcard $(UART_SRC)/*.c)

LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/lib/libc/stdio \
                 -I $(LITEOSTOPDIR)/compat/posix/src \
                 -I $(LITEOSTOPDIR)/drivers/random/dev/random

ifeq ($(LOSCFG_PLATFORM_DVFS), y)
LOCAL_SRCS += $(wildcard board/$(LITEOS_PLATFORM)/dvfs/*.c)
endif

ifeq ($(LOSCFG_SHELL_FULL_CAP), y)
LOCAL_SRCS += $(wildcard board/$(LITEOS_PLATFORM)/extcmd/*.c)
endif

ifeq ($(LOSCFG_DRIVERS_MMC), y)
LOCAL_SRCS += $(wildcard board/$(LITEOS_PLATFORM)/mmc/*.c)
MMC_INCLUDE += -I $(LITEOSTOPDIR)/drivers/mmc/host/$(MMC_HOST_DIR)
LOCAL_INCLUDE += $(MMC_INCLUDE)
endif

LOCAL_SRCS += $(wildcard board/$(LITEOS_PLATFORM)/drivers/*.c)
ifeq ($(findstring y, $(LOSCFG_PLATFORM_HI3516DV300)$(LOSCFG_PLATFORM_HI3556V200)), y)
LOCAL_SRCS += $(wildcard common/*.c)
ifneq ($(LOSCFG_FS_VFS), y)
LOCAL_SRCS := $(filter-out common/console.c common/virtual_serial.c, $(LOCAL_SRCS))
endif
else
LOCAL_SRCS += $(wildcard common/los_config.c)
LOCAL_SRCS += $(wildcard common/los_printf.c)
endif

ifeq ($(LOSCFG_DRIVERS_NETDEV), y)
LOCAL_SRCS += $(wildcard $(NET_SRC)/*.c)
endif

ifeq ($(LOSCFG_DRIVERS_RANDOM), y)
LOCAL_SRCS += $(wildcard board/$(LITEOS_PLATFORM)/random/*.c)
endif

ifeq ($(LOSCFG_DRIVERS_USB), y)
LOCAL_SRCS += $(wildcard $(USB_SRC)/*.c)
endif

ifeq ($(LOSCFG_PLATFORM_ADAPT), y)
LOCAL_SRCS += $(wildcard board/$(LITEOS_PLATFORM)/os_adapt/*.c)
endif

LOCAL_INCLUDE += -I $(wildcard board)/$(LITEOS_PLATFORM)/include/pm
LOCAL_SRCS += $(wildcard board/$(LITEOS_PLATFORM)/pm/*.c)

ifeq ($(LOSCFG_KERNEL_TICKLESS), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifeq ($(LOSCFG_KERNEL_TRACE), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifeq ($(LOSCFG_KERNEL_CPUP), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifeq ($(LOSCFG_KERNEL_RUNSTOP), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifndef LOSCFG_PLATFORM_OSAPPINIT
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/test/include
endif

ALL_ASSRCS := $(wildcard board/$(LITEOS_PLATFORM)/*.S)
ASSRCS := $(subst board/$(LITEOS_PLATFORM)/board.ld.S,,$(ALL_ASSRCS))

LOCAL_SRCS += $(ASSRCS)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
