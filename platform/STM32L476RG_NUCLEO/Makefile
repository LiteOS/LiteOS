include hal/STM32L4xx/Makefile
DEFINES += -DLOS_STM32L476xx -DSTM32L476xx 
DIRECTORIES += platform/STM32L476RG_NUCLEO

C_SOURCES += platform/STM32L476RG_NUCLEO/los_bsp_adapter.c  \
			 platform/STM32L476RG_NUCLEO/los_bsp_key.c  \
			 platform/STM32L476RG_NUCLEO/los_bsp_led.c  \
			 platform/STM32L476RG_NUCLEO/los_bsp_uart.c \
			 platform/STM32L476RG_NUCLEO/stm32l4xx_nucleo.c 

ASM_SOURCES += platform/STM32L476RG_NUCLEO/los_startup_gcc.s
             
