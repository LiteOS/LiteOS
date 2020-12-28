# strip quotation mark in configuration

LOSCFG_ARCH_CPU_STRIP   := $(subst $\",,$(LOSCFG_ARCH_CPU))
LOSCFG_ARCH_CPU          = $(LOSCFG_ARCH_CPU_STRIP)

LOSCFG_ARCH_FPU_STRIP   := $(subst $\",,$(LOSCFG_ARCH_FPU))
LOSCFG_ARCH_FPU          = $(LOSCFG_ARCH_FPU_STRIP)

LITEOS_BASELIB          += -l$(LOSCFG_ARCH_CPU)
LIB_SUBDIRS             += arch/arm64/

# CPU compile options
# AArch64 has no specific FPU compile options like AArch32 does.
ifeq ($(LOSCFG_ARCH_FPU_DISABLE), y)
EXTENSION               := +nofp
endif
LITEOS_CPU_OPTS         := -mcpu=$(LOSCFG_ARCH_CPU)$(EXTENSION)

LITEOS_CORE_COPTS        = $(LITEOS_CPU_OPTS) $(LITEOS_FLOAT_OPTS) $(LITEOS_FPU_OPTS)
LITEOS_INTERWORK        += $(LITEOS_CORE_COPTS)
LITEOS_NODEBUG          += $(LITEOS_CORE_COPTS)
LITEOS_ASOPTS           += $(LITEOS_CPU_OPTS)
LITEOS_CXXOPTS_BASE     += $(LITEOS_CORE_COPTS)

ARCH_INCLUDE            := -I $(LITEOSTOPDIR)/arch/arm64/include \
                           -I $(LITEOSTOPDIR)/arch/arm64/include/arch \
                           -I $(LITEOSTOPDIR)/arch/arm64/src/include

LITEOS_PLATFORM_INCLUDE += $(ARCH_INCLUDE)
LITEOS_CXXINCLUDE       += $(ARCH_INCLUDE)

# expose FPU info to assembly code
ifeq ($(LOSCFG_ARCH_FPU_DISABLE), y)
    LITEOS_CMACRO       += -DLOSCFG_ARCH_FPU_DISABLE
else
    LITEOS_CMACRO       += -DLOSCFG_AARCH64_FPU
endif

# extra definition for other module
LITEOS_CPU_TYPE          = $(LOSCFG_ARCH_CPU)
LITEOS_ARM_ARCH         := -march=$(subst $\",,$(LOSCFG_ARCH_ARM_VER))

# linux style macros
LINUX_ARCH_$(LOSCFG_ARCH_ARM_V8A) = -D__LINUX_ARM_ARCH__=8

AS_OBJS_LIBC_FLAGS      += $(LINUX_ARCH_y)
