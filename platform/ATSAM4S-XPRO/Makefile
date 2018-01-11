DIRECTORIES += platform/ATSAM4S-XPRO

DEFINES += -D__SAM4SD32C__ -D__SOFTFP__ -DATSAM4S_XPRO

include hal/ATMEL/SAM4S/Makefile

C_SOURCES += platform/ATSAM4S-XPRO/los_bsp_adapter.c  \
			 platform/ATSAM4S-XPRO/los_bsp_key.c  \
			 platform/ATSAM4S-XPRO/los_bsp_led.c  \
			 platform/ATSAM4S-XPRO/los_bsp_uart.c \
			 platform/ATSAM4S-XPRO/system_SAM4S.c
			 

ASM_SOURCES += platform/ATSAM4S-XPRO/los_startup_gcc.s
             
