ifeq ($(LOSCFG_ARCH_ARM_CORTEX_M), y)
-include $(LITEOSTOPDIR)/arch/arm/cortex_m/cpu.mk
else
-include $(LITEOSTOPDIR)/arch/arm/cortex_a_r/cpu.mk
endif
