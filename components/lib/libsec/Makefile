include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := sec

LOCAL_SRCS := $(wildcard src/*.c)
ifneq ($(LOSCFG_FS_VFS), y)
LOCAL_SRCS := $(filter-out src/scanf_s.c, $(LOCAL_SRCS))
LOCAL_SRCS := $(filter-out src/fscanf_s.c, $(LOCAL_SRCS))
LOCAL_SRCS := $(filter-out src/fwscanf_s.c, $(LOCAL_SRCS))
LOCAL_SRCS := $(filter-out src/vfscanf_s.c, $(LOCAL_SRCS))
LOCAL_SRCS := $(filter-out src/vfwscanf_s.c, $(LOCAL_SRCS))
LOCAL_SRCS := $(filter-out src/vscanf_s.c, $(LOCAL_SRCS))
LOCAL_SRCS := $(filter-out src/vwscanf_s.c, $(LOCAL_SRCS))
LOCAL_SRCS := $(filter-out src/wscanf_s.c, $(LOCAL_SRCS))
endif

include $(MODULE)
