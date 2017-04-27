PROJECT           := LiteOS_STM32F4

TARGET            := $(PROJECT)
TARGET_ELF        := $(TARGET).elf
TARGET_BIN        := $(TARGET).bin
TARGET_HEX        := $(TARGET).hex

OBJCPFLAGS_ELF_TO_BIN   = -Obinary
OBJCPFLAGS_ELF_TO_HEX   = -Oihex
OBJCPFLAGS_BIN_TO_HEX   = -Ibinary -Oihex
OBJCPFLAGS_ELF_TO_SREC  = -Osrec
OBJCPFLAGS_ELF_TO_LIST  = -S

ifneq  (, $(LITEOSBASE)) 
 PROJECTBASE := $(LITEOSBASE)/projects/STM32F429I_DISCO_GCC
else	
 PROJECTBASE := $(PWD)
endif

#Make the PROJECTBASE absolute.
override PROJECTBASE		:=	$(abspath $(PROJECTBASE))

TOP_DIR := $(PROJECTBASE)/../..
include  $(PROJECTBASE)/env.mk



LDSCRIPT = $(PROJECTBASE)/../../platform/STM32F429I_DISCO/LiteOS.ld

CFLAGS      +=   -Wall -mlittle-endian  -mcpu=cortex-m4 -march=armv7e-m -mthumb -mthumb-interwork  -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D LOS_GCC_COMPILE -D _GCC_WRAP_STDINT_H  -ffreestanding   -D ARM_MATH_CM4 -std=gnu99  --specs=rdimon.specs 
ASFLAGS     +=   -Wall -mlittle-endian  -mcpu=cortex-m4 -march=armv7e-m -mthumb -mthumb-interwork  -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D LOS_GCC_COMPILE -D _GCC_WRAP_STDINT_H  -ffreestanding   -D ARM_MATH_CM4 -std=gnu99 --specs=rdimon.specs 

#CFLAGS		+=	-D LOS_STM32F429ZI  -D STM32F429xx -D USE_STM32F429I_DISCO  -D USE_HAL_DRIVER
#ASFLAGS	+=	-D LOS_STM32F429ZI  -D STM32F429xx -D USE_STM32F429I_DISCO  -D USE_HAL_DRIVER

LDFLAGS     +=  -T $(LDSCRIPT) 




OBJ_DIR := $(TOP_DIR)/obj

SRC := ${wildcard $(TOP_DIR)/kernel/base/core/*.c}
SRC += ${wildcard $(TOP_DIR)/kernel/base/ipc/*.c}
SRC += ${wildcard $(TOP_DIR)/kernel/base/mem/*.c}
SRC += ${wildcard $(TOP_DIR)/kernel/base/misc/*.c}
SRC += ${wildcard $(TOP_DIR)/kernel/config/*.c}
SRC += ${wildcard $(TOP_DIR)/kernel/cmsis/*.c}
SRC += ${wildcard $(TOP_DIR)/kernel/cpu/arm/cortex-m4/*.c}
SRC += ${wildcard $(TOP_DIR)/platform/STM32F429I_DISCO/*.c}
SRC += ${wildcard $(TOP_DIR)/example/api/*.c}
#SRC += ${wildcard $(TOP_DIR)/STM32FXX/stm32f429/Src/*.c}
#SRC += ${wildcard $(TOP_DIR)/STM32FXX/Drivers/STM32F4xx_HAL_Driver/Src/*.c}
#SRC += ${wildcard $(TOP_DIR)/STM32FXX/Drivers/BSP/STM32F429I-Discovery/*.c}
ifneq (, $(LITEOSBASE))
SRC += ${wildcard $(TOP_DIR)/../jerryscript/targets/liteos-stm32f4/source/*.c}
else
SRC += ${wildcard $(TOP_DIR)/user/*.c}
endif

ASM_SRC := ${wildcard $(PROJECTBASE)/../../kernel/cpu/arm/cortex-m4/los_dispatch_gcc.s}
ASM_SRC += ${wildcard $(PROJECTBASE)/../../platform/STM32F429I_DISCO/los_startup_gcc.s}

OBJ := $(SRC:%.c=%.o)
ASMOBJ += $(ASM_SRC:%.s=%.o)

CFLAGS += -I $(TOP_DIR)/kernel/base/include
CFLAGS += -I $(TOP_DIR)/kernel/include
CFLAGS += -I $(TOP_DIR)/kernel/link/gcc
CFLAGS += -I $(TOP_DIR)/kernel/config
CFLAGS += -I $(TOP_DIR)/kernel/cmsis
CFLAGS += -I $(TOP_DIR)/kernel/cpu/arm/cortex-m4
CFLAGS += -I $(TOP_DIR)/platform/STM32F429I_DISCO
CFLAGS += -I $(TOP_DIR)/example/include
#CFLAGS += -I $(TOP_DIR)/STM32FXX/stm32f429/Inc
#CFLAGS += -I $(TOP_DIR)/STM32FXX/Drivers/CMSIS/Include
#CFLAGS += -I $(TOP_DIR)/STM32FXX/Drivers/CMSIS/Device/ST/STM32F4xx/Include
#CFLAGS += -I $(TOP_DIR)/STM32FXX/Drivers/STM32F4xx_HAL_Driver/Inc
#CFLAGS += -I $(TOP_DIR)/STM32FXX/Drivers/BSP/STM32F429I-Discovery


ifneq  (, $(LITEOSBASE)) 
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/ecma/base
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/jrt
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/ecma/operations
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/lit
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/jmem
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/ecma/builtin-objects
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/vm
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/jcontext
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/parser/js
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/debugger
CFLAGS += -I $(TOP_DIR)/../jerryscript/jerry-core/parser/regexp
CFLAGS += -I $(TOP_DIR)/../jerryscript/targets/liteos-stm32f4/source
endif


.PHONY: all clean 



all: $(TARGET_BIN) 


$(TARGET_BIN):$(TARGET_ELF)
	$(OBJCOPY) $(OBJCPFLAGS_ELF_TO_BIN) -S  $(TARGET_ELF)   $(TARGET_BIN) 
	#$(OBJDUMP) -sStD $(TARGET_ELF) > map

$(TARGET_HEX):$(TARGET_ELF)
	$(OBJCOPY) $(OBJCPFLAGS_ELF_TO_HEX) -S  $(TARGET_ELF)   $(TARGET_HEX) 

$(TARGET_ELF):$(OBJ) $(ASMOBJ) 
	echo $(PROJECTBASE)
	echo $(TOP_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@  $(JS_CFLAGS)


%.o:%.c  
	$(CC) $(CFLAGS) -c  -o $@ $^

%.o:%.s 
	$(CC) $(ASFLAGS) -c  -o $@ $^


clean:
	#$(MAKE) -C $(LIB) clean
	rm $(OBJ)
	rm $(ASMOBJ)
	rm $(TARGET_BIN)
	rm $(TARGET_ELF)
	#rm map
	@echo "clean"
