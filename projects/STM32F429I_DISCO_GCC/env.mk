ifeq ($(QUICK),1)
QUICK_CC = ccache
else
QUICK_CC = 
endif

AR :=  arm-none-eabi-ar
CC := $(QUICK_CC) arm-none-eabi-gcc
NM :=  arm-none-eabi-nm
CPP := $(QUICK_CC) arm-none-eabi-cpp
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump

PWD  := $(shell pwd)
LIB  := $(TOP_DIR)/STM32FXX
MAKE := make


CFLAGS += \
	  -g	\
	  -O0
ASFLAGS += \
	  -g	\
	  -O0
