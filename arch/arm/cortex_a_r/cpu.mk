# strip quotation mark in configuration

LOSCFG_ARCH_CPU_STRIP   := $(subst $\",,$(LOSCFG_ARCH_CPU))
LOSCFG_ARCH_CPU          = $(LOSCFG_ARCH_CPU_STRIP)

LOSCFG_ARCH_FPU_STRIP   := $(subst $\",,$(LOSCFG_ARCH_FPU))
LOSCFG_ARCH_FPU          = $(LOSCFG_ARCH_FPU_STRIP)

LITEOS_BASELIB          += -l$(LOSCFG_ARCH_CPU)
LIB_SUBDIRS             += arch/arm/cortex_a_r

# CPU compile options
LITEOS_CPU_OPTS         := -mcpu=$(LOSCFG_ARCH_CPU)$(EXTENSION)

# FPU compile options
LITEOS_FLOAT_OPTS       := -mfloat-abi=softfp
LITEOS_FPU_OPTS         := -mfpu=$(LOSCFG_ARCH_FPU)
# gcc libc folder style is combine with core and fpu
# for example, cortex-a7 with softfp abi and neon vfp4 is: a7_softfp_neon_vfp4
LITEOS_GCCLIB           := $(subst cortex-,,$(LOSCFG_ARCH_CPU))_softfp_$(LOSCFG_ARCH_FPU)

LITEOS_CORE_COPTS        = $(LITEOS_CPU_OPTS) $(LITEOS_FLOAT_OPTS) $(LITEOS_FPU_OPTS)
LITEOS_INTERWORK        += $(LITEOS_CORE_COPTS)
LITEOS_NODEBUG          += $(LITEOS_CORE_COPTS)
LITEOS_ASOPTS           += $(LITEOS_CPU_OPTS)
LITEOS_CXXOPTS_BASE     += $(LITEOS_CORE_COPTS)

LITEOS_COPTS_BASE       += -mno-unaligned-access -mthumb-interwork

ifeq ($(LOSCFG_THUMB), y)
LITEOS_COPTS_EXTRA_INTERWORK += -mthumb
LITEOS_CMACRO           += -DLOSCFG_INTERWORK_THUMB
endif

ARCH_INCLUDE            := -I $(LITEOSTOPDIR)/arch/arm/cortex_a_r/include \
                           -I $(LITEOSTOPDIR)/arch/arm/cortex_a_r/include/arch \
                           -I $(LITEOSTOPDIR)/arch/arm/cortex_a_r/src/include

LITEOS_PLATFORM_INCLUDE += $(ARCH_INCLUDE)
LITEOS_CXXINCLUDE       += $(ARCH_INCLUDE)

# expose FPU info to assembly code
ifeq ($(LOSCFG_ARCH_FPU_DISABLE), y)
    LITEOS_CMACRO       += -DLOSCFG_ARCH_FPU_DISABLE
else ifeq ($(LOSCFG_ARCH_FPU_VFP_D16), y)
    LITEOS_CMACRO       += -DLOSCFG_ARCH_FPU_VFP_D16
else ifeq ($(LOSCFG_ARCH_FPU_VFP_D32), y)
    LITEOS_CMACRO       += -DLOSCFG_ARCH_FPU_VFP_D32
endif

# extra definition for other module
LITEOS_CPU_TYPE          = $(LOSCFG_ARCH_CPU)
LITEOS_ARM_ARCH         := -march=$(subst $\",,$(LOSCFG_ARCH_ARM_VER))

# linux style macros
LINUX_ARCH_$(LOSCFG_ARCH_ARM_V7A) = -D__LINUX_ARM_ARCH__=7
LINUX_ARCH_$(LOSCFG_ARCH_ARM_V7R) = -D__LINUX_ARM_ARCH__=7
LINUX_ARCH_$(LOSCFG_ARCH_ARM_V8A) = -D__LINUX_ARM_ARCH__=8
LINUX_ARCH_$(LOSCFG_ARCH_ARM_V8R) = -D__LINUX_ARM_ARCH__=8

AS_OBJS_LIBC_FLAGS      += $(LINUX_ARCH_y)
