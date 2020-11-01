include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := c

# musl_libc path
LOCAL_DIRS_LIBC := errno string stdlib internal time misc \
                locale ctype temp fcntl signal network unistd termios
LOCAL_DIRS_LIBC := $(addprefix src/, $(LOCAL_DIRS_LIBC))

# huawei_libc path
LOCAL_DIRS_HUAWEI_LIBC := errno string stdlib signal time
LOCAL_DIRS_HUAWEI_LIBC := $(addprefix ../huawei_libc/, $(LOCAL_DIRS_HUAWEI_LIBC))

LOCAL_DIRS_y := $(LOCAL_DIRS_LIBC) $(LOCAL_DIRS_HUAWEI_LIBC)

# STDIO configured module path
LOCAL_DIRS_$(LOSCFG_LIB_STDIO) += src/stdio ../huawei_libc/stdio

LOCAL_SRCS_y := $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))

LOCAL_SRCS    = $(LOCAL_SRCS_y)

LOCAL_SRCS += $(wildcard src/string/$(ARCH_LOCAL)/*.s)

LOCAL_INCLUDE := -I $(LITEOSTOPDIR)/lib/libc/arch/generic \
    -I $(LITEOSTOPDIR)/lib/libc/src/include \
    -I $(LITEOSTOPDIR)/lib/libc/src/internal \
    -I $(LITEOSTOPDIR)/lib/libc/src/crypt \
    -I $(LITEOSTOPDIR)/lib/libc/src/ctype \
    -I $(LITEOSTOPDIR)/lib/libc/src/dirent \
    -I $(LITEOSTOPDIR)/lib/libc/src/errno \
    -I $(LITEOSTOPDIR)/lib/libc/src/ipc \
    -I $(LITEOSTOPDIR)/lib/libc/src/locale \
    -I $(LITEOSTOPDIR)/lib/libc/src/math \
    -I $(LITEOSTOPDIR)/lib/libc/src/multibyte \
    -I $(LITEOSTOPDIR)/lib/libc/src/networknetwork \
    -I $(LITEOSTOPDIR)/lib/libc/src/passwd \
    -I $(LITEOSTOPDIR)/lib/libc/src/prng \
    -I $(LITEOSTOPDIR)/lib/libc/src/process \
    -I $(LITEOSTOPDIR)/lib/libc/src/regex \
    -I $(LITEOSTOPDIR)/lib/libc/src/search \
    -I $(LITEOSTOPDIR)/lib/libc/src/time \
    -I $(LITEOSTOPDIR)/drivers/random/dev/random \
    -I $(LITEOSTOPDIR)/compat/linux/include \
    -I $(LITEOSTOPDIR)/lib/huawei_libc/pthread \
    -I $(LITEOSTOPDIR)/lib/huawei_libc/tzdst \

ifeq ($(LOSCFG_LLTSER), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/tools/gcov_ser
endif

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE_LIB)
