PROJECT = LiteOS

# output director for build objects
out ?= build/liteos

# output directory for host build targets
out_host ?= build/host

# FIXME: config by KConfig, please
CHIP = stm32f429
LITEOS_LD_FILE = liteos_sram.ld

# toolchain specific configurations; common clfags and ldflags
include mk/toolchain.mk

# Get build configuration from sub-directories
include kernel/base/build.mk

objs_from_dir = $(foreach obj, $($(2)-y), \
		$(out)/$(1)/$(firstword $($(2)-mock-$(PROJECT)-$(obj)) $(obj)))

# Get all sources to build
all-y += $(call objs_from_dir,kernel/base,kernel-base)

dirs = \
	kernel \
	kernel/base \
	kernel/base/core \
	kernel/base/ipc \
	kernel/base/mem \
	kernel/base/misc

include mk/generic.mk
