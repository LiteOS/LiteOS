DIRECTORIES += kernel/cpu/arm/cortex-m4

C_SOURCES += kernel/cpu/arm/cortex-m4/los_hw.c  \
             kernel/cpu/arm/cortex-m4/los_hw_tick.c  \
			 kernel/cpu/arm/cortex-m4/los_hwi.c
			 

ifneq (${GCOV}, true)
ASM_SOURCES += kernel/cpu/arm/cortex-m4/los_dispatch_gcc.s
endif
             