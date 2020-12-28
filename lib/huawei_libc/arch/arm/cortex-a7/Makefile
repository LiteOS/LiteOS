include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := csysdeps

LOCAL_INCLUDE :=

LOCAL_SRCS := memset.S memcmp.S strcmp.S

LOCAL_ASFLAGS := -Wa,-mno-warn-deprecated $(AS_OBJS_LIBC_FLAGS)

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE_LIB)
