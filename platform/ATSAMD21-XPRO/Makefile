DIRECTORIES += platform/ATSAMD21-XPRO

DEFINES += -D__SAMD21J18A__  -DRAM_SIZE_LEVEL_0

include hal/ATMEL/SAMD21/Makefile

C_SOURCES += platform/ATSAMD21-XPRO/los_bsp_adapter.c  \
			 platform/ATSAMD21-XPRO/los_bsp_clock.c  \
			 platform/ATSAMD21-XPRO/los_bsp_key.c  \
			 platform/ATSAMD21-XPRO/los_bsp_led.c  \
			 platform/ATSAMD21-XPRO/los_bsp_uart.c \
             platform/ATSAMD21-XPRO/uart.c
			 

ASM_SOURCES += platform/ATSAMD21-XPRO/los_startup_gcc.s
             
