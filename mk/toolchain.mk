# Toolchain

CROSS_COMPILE ?= arm-none-eabi-
HOST_COMPILE ?=

CC = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)cpp
LD = $(CROSS_COMPILE)ld
STRIP = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
NM = $(CROSS_COMPILE)nm
SIZE = $(CROSS_COMPILE)size

BUILDCC = $(HOST_COMPILE)gcc

CFLAGS_WARN = \
	-Wall -Werror -Wundef -Wno-trigraphs	\
	-fno-strict-aliasing -fno-common				\
	-Werror-implicit-function-declaration -Wno-format-security	\
	-fno-delete-null-pointer-checks -Wno-pointer-sign 		\
	-fno-strict-overflow -fconserve-stack
CFLAGS_OPT = \
	-O0 -fno-toplevel-reorder \
	-fdata-sections -ffunction-sections
CFLAGS_DEBUG = -g3

# Platform
CFLAGS_CPU += -mthumb -mthumb-interwork -Xassembler -mimplicit-it=thumb
CFLAGS_CPU += -mno-sched-prolog -mno-unaligned-access
CFLAGS_CPU += -Wdouble-promotion -fsingle-precision-constant

CFLAGS_INCLUDE = \
	-I. \
	-Ikernel/base/include \
	-Ikernel/include \
	-Iplatform/include/keil \
	-Iplatform/bsp/sample/config \
	-Iplatform/cpu/arm/cortex-m4

CFLAGS = \
	-std=gnu99 -isystem \
	-nostdlib -ffreestanding \
	$(CFLAGS_INCLUDE) $(CFLAGS_CPU) $(CFLAGS_OPT) $(CFLAGS_DEBUG) $(CFLAGS_WARN) $(CFLAGS_y) $(CFLAGS_MISC_DEFINE)
