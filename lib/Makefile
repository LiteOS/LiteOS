include $(LITEOSTOPDIR)/config.mk

MODULE_y += libsec

MODULE_$(LOSCFG_LIB_LIBC) += libc huawei_libc
MODULE_$(LOSCFG_LIB_LIBM) += libc/src/math
MODULE_$(LOSCFG_LIB_ZLIB) += zlib

ifeq ($(LITEOS_CPU_TYPE), cortex-a7)
MODULE_$(LOSCFG_LIB_LIBC) += huawei_libc/arch/$(ARCH_LOCAL)/cortex-a7
else ifeq ($(LITEOS_CPU_TYPE), cortex-a9)
MODULE_$(LOSCFG_LIB_LIBC) += huawei_libc/arch/$(ARCH_LOCAL)/cortex-a7
else ifeq ($(LITEOS_CPU_TYPE), riscv)
MODULE_$(LOSCFG_LIB_LIBC) += huawei_libc/arch/$(ARCH_LOCAL)
endif

include $(MODULE)