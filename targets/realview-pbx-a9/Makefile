include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LITEOS_PLATFORM)

LOCAL_SRCS := $(wildcard *.c)

ifeq ($(LOSCFG_PLATFORM_ADAPT), y)
LOCAL_SRCS += $(wildcard os_adapt/*.c)
endif

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

ALL_ASSRCS := $(wildcard *.S)
ASSRCS := $(subst board.ld.S,,$(ALL_ASSRCS))

LOCAL_SRCS += $(ASSRCS)

LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include/hisoc \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include/asm \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include/pm \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/uart \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM) \
    -I $(LITEOSTOPDIR)/include

BOARD_DEF += $(C_DEFS)

LOCAL_FLAGS := $(BOARD_DEF) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
