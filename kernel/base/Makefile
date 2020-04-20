include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(shell pwd))

LOCAL_SRCS := 	$(wildcard ipc/*.c) $(wildcard core/*.c) $(wildcard mem/membox/*.c) $(wildcard mem/common/*.c)	\
		$(wildcard om/*.c)\
		$(wildcard misc/*.c)\
		$(wildcard mem/$(MEM_TYPE)/*.c) \
		$(wildcard mp/*.c) \
		$(wildcard sched/$(SCHED_TYPE)/*.c)

ifeq ($(LOSCFG_MEM_RECORDINFO), y)
LOCAL_SRCS += $(wildcard mem/common/memrecord/*.c)
endif

LOCAL_INCLUDE := \
	-I $(LITEOSTOPDIR)/kernel/base/include \
	-I $(LITEOSTOPDIR)/kernel/base/mem

LOCAL_FLAGS := $(LITEOS_CFLAGS_INTERWORK) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
