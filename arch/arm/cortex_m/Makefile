include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LOSCFG_ARCH_CPU)

LOCAL_SRCS := $(wildcard src/*.c) $(wildcard src/*.S)

ifneq ($(LOSCFG_FPB_ENABLE), y)
LOCAL_SRCS := $(filter-out src/fpb.c, $(LOCAL_SRCS))
endif

ifneq ($(LOSCFG_APC_ENABLE), y)
LOCAL_SRCS := $(filter-out src/mpu.c, $(LOCAL_SRCS))
endif

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/kernel/base/include \
    -I $(LITEOSTOPDIR)/kernel/extended/include \

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

ifeq ($(LOSCFG_GDB), y)
LOCAL_FLAGS += $(AS_OBJS_LIBC_FLAGS)
endif
include $(MODULE)
