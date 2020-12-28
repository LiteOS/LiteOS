include $(LITEOSTOPDIR)/config.mk
MODULE_NAME := lms_demo

LMS_DEMO_DIR = $(LITEOSTOPDIR)/demos/lms

LMS_DEMO_INCLUDE := -I$(LMS_DEMO_DIR)

BOARD_INCLUDES += $(LMS_DEMO_INCLUDE)

LMS_DEMO_SRC = $(wildcard $(LMS_DEMO_DIR)/*.c)

C_SOURCES += $(LMS_DEMO_SRC)

BOARD_SRCS += $(C_SOURCES)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(BOARD_SRCS))
C_DEFS = -fsanitize=kernel-address

BOARD_DEF += $(C_DEFS)

LOCAL_INCLUDE += $(BOARD_INCLUDES)
LOCAL_FLAGS := $(BOARD_DEF) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
