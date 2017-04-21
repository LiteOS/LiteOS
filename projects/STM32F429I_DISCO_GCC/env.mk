
AR := arm-none-eabi-ar
CC := arm-none-eabi-gcc
NM := arm-none-eabi-nm
CPP := arm-none-eabi-cpp
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump

PWD  := $(shell pwd)
LIB  := $(TOP_DIR)/STM32FXX
MAKE := make


CFLAGS += \
	  -g	\
	  -O0
