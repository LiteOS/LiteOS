ifeq ($(LOSCFG_ARCH_ARM_AARCH32), y)
-include $(LITEOSTOPDIR)/arch/arm/cpu.mk
endif

ifeq ($(LOSCFG_ARCH_ARM_AARCH64), y)
-include $(LITEOSTOPDIR)/arch/arm64/cpu.mk
endif

ifeq ($(LOSCFG_ARCH_XTENSA), y)
-include $(LITEOSTOPDIR)/arch/xtensa/cpu.mk
endif

ifeq ($(LOSCFG_ARCH_RISCV), y)
-include $(LITEOSTOPDIR)/arch/riscv/cpu.mk
endif
