include $(LITEOSTOPDIR)/config.mk

MODULE_NAME     := $(notdir $(CURDIR))

LOCAL_SRCS_y    := los_init.c \

LOCAL_SRCS_y += $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))
LOCAL_SRCS    = $(LOCAL_SRCS_y)

LOCAL_INCLUDE   := \
                -I $(LITEOSTOPDIR)/kernel/extended/include \
                -I $(LITEOSTOPDIR)/kernel/extended/dynload/include

ifndef LOSCFG_PLATFORM_OSAPPINIT
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/test/include
endif

ifeq ($(LOSCFG_SHELL_MINI), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/shell/mini/include
endif

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
