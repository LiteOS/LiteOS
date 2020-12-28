include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LOSCFG_ARCH_CPU)

LOCAL_SRCS_y := $(wildcard src/canary.c) $(wildcard src/cpu.c) \
                $(wildcard src/fault.c) $(wildcard src/task.c) \
                $(wildcard src/mmu.c)
                
LOCAL_INCLUDE := \
        -I $(LITEOSTOPDIR)/kernel/extended/include \

ASSRCS = $(wildcard src/*.S)
LOCAL_SRCS_y += $(ASSRCS)

LOCAL_SRCS = $(LOCAL_SRCS_y)
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
