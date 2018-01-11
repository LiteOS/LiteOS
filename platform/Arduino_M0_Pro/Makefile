DIRECTORIES += platform/Arduino_M0_Pro

DEFINES += -D__SAMD21G18A__  -DRAM_SIZE_LEVEL_0

include hal/ATMEL/SAMD21/Makefile

C_SOURCES += platform/Arduino_M0_Pro/los_bsp_adapter.c  \
			 platform/Arduino_M0_Pro/los_bsp_key.c  \
			 platform/Arduino_M0_Pro/los_bsp_led.c  \
			 platform/Arduino_M0_Pro/los_bsp_uart.c \
			 platform/Arduino_M0_Pro/system_samd21.c
			 

ASM_SOURCES += platform/Arduino_M0_Pro/los_startup_gcc.s
             
