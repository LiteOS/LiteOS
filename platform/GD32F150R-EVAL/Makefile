include hal/GD32F1x0/Makefile
DEFINES += -DGD32F150R8 -DGD32F130_150 -DRAM_SIZE_LEVEL_0
DIRECTORIES += platform/GD32F150R-EVAL

C_SOURCES += platform/GD32F150R-EVAL/los_bsp_adapter.c  \
			 platform/GD32F150R-EVAL/gd32f1x0_eval.c  \
			 platform/GD32F150R-EVAL/los_bsp_key.c  \
			 platform/GD32F150R-EVAL/los_bsp_led.c  \
			 platform/GD32F150R-EVAL/los_bsp_uart.c
			 

ASM_SOURCES += platform/GD32F150R-EVAL/los_startup_gcc.s
             
