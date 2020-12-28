include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := c

# musl_libc path
LOCAL_DIRS_LIBC := errno string stdlib internal time misc \
                locale ctype temp fcntl signal network unistd complex termios
LOCAL_DIRS_LIBC := $(addprefix src/, $(LOCAL_DIRS_LIBC))

# huawei_libc path
LOCAL_DIRS_HUAWEI_LIBC := errno string stdlib signal
LOCAL_DIRS_HUAWEI_LIBC := $(addprefix ../huawei_libc/, $(LOCAL_DIRS_HUAWEI_LIBC))

LOCAL_DIRS_y := $(LOCAL_DIRS_LIBC) $(LOCAL_DIRS_HUAWEI_LIBC)

# STDIO configured module path
LOCAL_DIRS_$(LOSCFG_LIB_STDIO) += src/stdio ../huawei_libc/stdio

LOCAL_SRCS_y := $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))

LOCAL_SRCS_y += ../huawei_libc/time/time.c src/prng/rand.c
ifneq ($(LOSCFG_LIB_STDIO), y)
LOCAL_SRCS_y += src/stdio/__uflow.c src/stdio/__toread.c
endif

LOCAL_SRCS    = $(LOCAL_SRCS_y)

LOCAL_SRCS += $(wildcard src/string/$(ARCH_LOCAL)/*.s)
ifneq ($(LOSCFG_ARCH_ARM_CORTEX_M), y)
LOCAL_SRCS += $(wildcard src/string/$(ARCH_LOCAL)/*.S)
endif

ifeq ($(LITEOS_CPU_TYPE), cortex-a7)
FILTER_FILES = $(wildcard src/string/memcpy.c) \
               $(wildcard src/string/memset.c) \
               $(wildcard src/string/strcmp.c) \
               $(wildcard src/string/memcmp.c)
LOCAL_SRCS := $(filter-out $(FILTER_FILES), $(LOCAL_SRCS))
else ifeq ($(LITEOS_CPU_TYPE), cortex-a9)
FILTER_FILES = $(wildcard src/string/memcpy.c) \
               $(wildcard src/string/memset.c) \
               $(wildcard src/string/strcmp.c) \
               $(wildcard src/string/memcmp.c)
LOCAL_SRCS := $(filter-out $(FILTER_FILES), $(LOCAL_SRCS))
else ifeq ($(LITEOS_CPU_TYPE), riscv)
FILTER_FILES = $(wildcard src/string/memset.c) \
               $(wildcard src/string/strcmp.c) \
               $(wildcard src/string/memcpy.c)
LOCAL_SRCS := $(filter-out $(FILTER_FILES), $(LOCAL_SRCS))
endif

LOCAL_INCLUDE := \
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
    -I $(LITEOSTOPDIR)/lib/huawei_libc/pthread \
    -I $(LITEOSTOPDIR)/lib/huawei_libc/tzdst \

ifeq ($(LOSCFG_LLTSER), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/tools/gcov_ser
endif

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE_LIB)
