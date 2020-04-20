include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LOSCFG_ARCH_CPU)

LOCAL_SRCS := $(wildcard src/*.c) $(wildcard ../../common/*.c) $(wildcard src/*.S)
LOCAL_INCLUDE := \
	-I $(LITEOSTOPDIR)/kernel/base/include \
	-I $(LITEOSTOPDIR)/kernel/extended/include

ifeq ($(LITEOS_ARM_ARCH), -march=armv7-a)
LOCAL_SRCS += $(wildcard src/armv7a/*.S)
endif

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

ifeq ($(LOSCFG_GDB), y)
LOCAL_FLAGS += $(AS_OBJS_LIBC_FLAGS)
endif
include $(MODULE)
