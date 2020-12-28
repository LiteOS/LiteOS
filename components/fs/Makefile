include $(LITEOSTOPDIR)/config.mk

MODULE_NAME     := $(notdir $(CURDIR))
LOCAL_SRCS_y    :=
LOCAL_DIRS_y    :=

# VFs
LOCAL_DIRS_$(LOSCFG_COMPONENTS_FS_VFS) += vfs

ifeq ($(LOSCFG_COMPONENTS_FS_VFS), y)
LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/include \
    -I $(LITEOSTOPDIR)/include/fs \
    -I $(LITEOSTOPDIR)/lib/libc/include
endif

# FatFs
LOCAL_DIRS_$(LOSCFG_COMPONENTS_FS_FATFS) += fatfs
LOCAL_DIRS_$(LOSCFG_COMPONENTS_FS_FATFS) += fatfs/ff13b/source
LOCAL_DIRS_$(LOSCFG_COMPONENTS_FS_FATFS) += fatfs/ff13b/source/default

ifeq ($(LOSCFG_COMPONENTS_FS_FATFS), y)
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/fs/fatfs/ff13b/source \
    -I $(LITEOSTOPDIR)/components/fs/fatfs/ff13b/source/default
endif

# DevFs
LOCAL_DIRS_$(LOSCFG_COMPONENTS_FS_DEVFS) += devfs

ifeq ($(LOSCFG_COMPONENTS_FS_DEVFS), y)
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/include/fs
endif

# KiFs
LOCAL_DIRS_$(LOSCFG_COMPONENTS_FS_KIFS) += kifs

ifeq ($(LOSCFG_COMPONENTS_FS_KIFS), y)
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/include/fs
endif

# RamFs
LOCAL_DIRS_$(LOSCFG_COMPONENTS_FS_RAMFS) += ramfs

ifeq ($(LOSCFG_COMPONENTS_FS_RAMFS), y)
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/include/fs
endif

# SpifFs
LOCAL_DIRS_$(LOSCFG_COMPONENTS_FS_SPIFFS) += spiffs
LOCAL_DIRS_$(LOSCFG_COMPONENTS_FS_SPIFFS) += spiffs/spiffs_git/src

ifeq ($(LOSCFG_COMPONENTS_FS_SPIFFS), y)
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/fs/spiffs/spiffs_git/src \
    -I $(LITEOSTOPDIR)/components/fs/spiffs/spiffs_git/src/default
endif

LOCAL_SRCS_y += $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))
LOCAL_SRCS   = $(LOCAL_SRCS_y)

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE)
