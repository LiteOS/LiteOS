include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := sec
ifeq ($(LOSCFG_LIB_LIBCMINI), y)
LOCAL_SRCS := src/memcpy_s.c \
              src/memset_s.c \
              src/strncpy_s.c \
              src/vsnprintf_s.c \
              src/secureprintoutput_a.c

LITEOS_CFLAGS += -DSECUREC_HAVE_WCTOMB=0 -DSECUREC_ENABLE_SPRINTF_FLOAT=0

else
LOCAL_SRCS := $(wildcard src/*.c)
endif

include $(MODULE)
