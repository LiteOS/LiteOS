# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.

LITEOS_LIBC_INCLUDE := -I $(LITEOSTOPDIR)/lib/libsec/include

# Exporting musl libc APIs
#
# Attention:
# './lib/libc/src/include' and 'lib/libc/src/internal' should not exported,
# while os would compile failed if those are not included which should be fix.
ifeq ($(LOSCFG_LIB_LIBC), y)
    ifeq ($(LOSCFG_ARCH_ARM_AARCH32), y)
        ARCH_LOCAL := arm
    else ifeq ($(LOSCFG_ARCH_ARM_AARCH64), y)
        ARCH_LOCAL := aarch64
    else ifeq ($(LOSCFG_ARCH_RISCV_RV32IM), y)
        ARCH_LOCAL := riscv32
    endif

    LITEOS_LIBC_INCLUDE += \
        -I $(LITEOSTOPDIR)/lib/libc/arch/$(ARCH_LOCAL) \
        -I $(LITEOSTOPDIR)/lib/libc/arch/generic \
        -I $(LITEOSTOPDIR)/lib/huawei_libc/include \
        -I $(LITEOSTOPDIR)/lib/libc/include

endif

ifeq ($(LOSCFG_LIB_ZLIB), y)
    LITEOS_ZLIB_INCLUDE += -I $(LITEOSTOPDIR)/lib/zlib/include
endif

ifeq ($(LOSCFG_KERNEL_CPPSUPPORT), y)
    LITEOS_BASELIB += -lsupc++ -lstdc++
endif

LITEOS_LIB_INCLUDE := $(LITEOS_LIBC_INCLUDE) $(LITEOS_LIBM_INCLUDE) \
                      $(LITEOS_ZLIB_INCLUDE) $(LITEOS_COMPILER_GCC_INCLUDE)