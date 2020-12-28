include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := posix

LOCAL_DIRS_y :=

# huawei_libc path
LOCAL_DIRS_HUAWEI_LIBC := pthread mq sched tzdst misc

# musl_libc path
LOCAL_DIRS_LIBC := exit prng multibyte \
                   env linux process legacy thread
LOCAL_DIRS_LIBC := $(addprefix ../libc/src/, $(LOCAL_DIRS_LIBC))

# posix configured module path
LOCAL_DIRS_$(LOSCFG_COMPAT_POSIX) += $(LOCAL_DIRS_HUAWEI_LIBC) $(LOCAL_DIRS_LIBC)

LOCAL_SRCS_$(LOSCFG_COMPAT_POSIX) += time/time64.c time/timer.c

LOCAL_SRCS_y += $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))

LOCAL_SRCS    = $(LOCAL_SRCS_y)

LOCAL_INCLUDE := -I $(LITEOSTOPDIR)/lib/huawei_libc/pthread \
                 -I $(LITEOSTOPDIR)/lib/huawei_libc/errno \
                 -I $(LITEOSTOPDIR)/lib/huawei_libc/tzdst \
                 -I $(LITEOSTOPDIR)/lib/libc/src/include \
                 -I $(LITEOSTOPDIR)/lib/libc/src/internal \
                 -I $(LITEOSTOPDIR)/lib/libc/src/time


LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE_LIB)
