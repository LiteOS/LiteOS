############### this is a makefile that you can config it ###############
-include $(LITEOSTOPDIR)/build/mk/los_config.mk

TOP_LD_PATH      = $(LITEOSTOPDIR)
SCRIPTS_PATH     = $(LITEOS_SCRIPTPATH)
LITEOS_MK_PATH   = $(MK_PATH)
LITEOS_SUBDIRS   = $(LIB_SUBDIRS)
LIB_BIGODIR      = $(LITEOS_LIB_BIGODIR)

##### liteos tables -u ldflags #####
-include $(LITEOS_MK_PATH)/liteos_tables_ldflags.mk

# Use generated file to import $(LITEOS_LIBDEP)
-include $(LIBDEP_FILE)

LITEOS_COPTS   := $(LITEOS_COPTS_DEBUG) $(LITEOS_COPTS_BASE)  \
                  $(LITEOS_COPTS_EXTRA) $(LITEOS_CORE_COPTS)
LITEOS_CXXOPTS  := $(LITEOS_CXXOPTS_BASE)
LITEOS_INCLUDE  := $(LITEOS_KERNEL_INCLUDE)  $(LITEOS_EXTKERNEL_INCLUDE) \
                   $(LITEOS_COMPAT_INCLUDE)  $(LITEOS_FS_INCLUDE) \
                   $(LITEOS_NET_INCLUDE)     $(LITEOS_LIB_INCLUDE) \
                   $(LITEOS_DRIVERS_INCLUDE) $(LOSCFG_TOOLS_DEBUG_INCLUDE) \
                   $(LITEOS_PLATFORM_INCLUDE)
LITEOS_ASFLAGS  := $(LITEOS_ASOPTS)      $(LITEOS_INCLUDE)
LITEOS_CFLAGS   := $(LITEOS_COPTS)       $(LITEOS_CMACRO) \
                   $(LITEOS_CMACRO_TEST) $(LITEOS_IMAGE_MACRO) \
                   $(LITEOS_INCLUDE)
LITEOS_CXXFLAGS := $(LITEOS_CXXOPTS)     $(LITEOS_CXXMACRO) \
                   $(LITEOS_CMACRO)      $(LITEOS_CXXINCLUDE)
LITEOS_LDFLAGS  := $(LITEOS_LD_OPTS)     $(LITEOS_LD_PATH) \
                   $(LITEOS_LD_SCRIPT)

