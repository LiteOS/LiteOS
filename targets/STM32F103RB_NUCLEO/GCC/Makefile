##########################################################################################################################
# STM32F103RB_NUCLEO GCC compiler Makefile
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
# ------------------------------------------------

######################################
# target
######################################
TARGET = Huawei_LiteOS
######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# binaries
#######################################
PREFIX    = arm-none-eabi-
CC        = $(PREFIX)gcc
AS        = $(PREFIX)gcc -x assembler-with-cpp
OBJCOPY   = $(PREFIX)objcopy
OBJDUMP   = $(PREFIX)objdump
AR        = $(PREFIX)ar
SZ        = $(PREFIX)size
LD        = $(PREFIX)ld
HEX       = $(OBJCOPY) -O ihex
BIN       = $(OBJCOPY) -O binary -S


PROJECTBASE = $(PWD)
override PROJECTBASE    := $(abspath $(PROJECTBASE))
TOP_DIR = $(PROJECTBASE)/../../..


#######################################
# paths
#######################################
# firmware library path
PERIFLIB_PATH =

# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
KERNEL_SRC =  \
        ${wildcard $(TOP_DIR)/kernel/*.c} \
        ${wildcard $(TOP_DIR)/kernel/base/core/*.c} \
        ${wildcard $(TOP_DIR)/kernel/base/ipc/*.c} \
        ${wildcard $(TOP_DIR)/kernel/base/mem/bestfit_little/*.c} \
        ${wildcard $(TOP_DIR)/kernel/base/mem/common/*.c} \
        ${wildcard $(TOP_DIR)/kernel/base/misc/*.c} \
        ${wildcard $(TOP_DIR)/kernel/base/om/*.c} \
        ${wildcard $(TOP_DIR)/kernel/extended/tickless/*.c}
        C_SOURCES += $(KERNEL_SRC)

CMSIS_SRC =  \
        ${wildcard $(TOP_DIR)/components/cmsis/*.c}
        C_SOURCES += $(CMSIS_SRC)

ARCH_SRC =  \
        ${wildcard $(TOP_DIR)/arch/arm/arm-m/src/*.c}
        C_SOURCES += $(ARCH_SRC)

EXAMPLES_SRC =  \
        ${wildcard $(TOP_DIR)/examples/api/*.c}
        C_SOURCES += $(EXAMPLES_SRC)

HAL_DRIVER_SRC =  \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
        C_SOURCES += $(HAL_DRIVER_SRC)

HAL_CMSIS_SRC =  \
        $(PROJECTBASE)/../Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
C_SOURCES += $(HAL_CMSIS_SRC)

USER_SRC =  \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Src/main.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Src/usart.c \
        $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Src/sys_init.c
        C_SOURCES += $(USER_SRC)


# ASM sources
ASM_SOURCES =  \
        ${wildcard $(PROJECTBASE)/../Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s}  \
        ${wildcard $(PROJECTBASE)/los_dispatch_gcc.s}


######################################
# firmware library
######################################
PERIFLIB_SOURCES =


#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3
# fpu
# None for cortex-m3
# float-abi
# None for cortex-m3
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
        -D USE_HAL_DRIVER \
        -D STM32F103xB \
        -D LOS_KERNEL_DEBUG_OUT


# AS includes
AS_INCLUDES =

# C includes
KERNEL_INC = \
        -I $(TOP_DIR)/kernel/base/include  \
        -I $(TOP_DIR)/kernel/extended/include  \
        -I $(TOP_DIR)/kernel/include \
        -I $(TOP_DIR)/kernel/base/core
        C_INCLUDES += $(KERNEL_INC)

CMSIS_INC = \
        -I $(TOP_DIR)/components/cmsis  \
        -I $(TOP_DIR)/components/cmsis/2.0
        C_INCLUDES += $(CMSIS_INC)

ARCH_INC = \
        -I $(TOP_DIR)/arch/arm/arm-m/include \
        -I $(TOP_DIR)/arch/arm/common/cmsis
        C_INCLUDES += $(ARCH_INC)

EXAMPLES_INC = \
        -I $(TOP_DIR)/examples/include
        C_INCLUDES += $(EXAMPLES_INC)

HAL_DRIVER_INC = \
        -I $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Inc \
        -I $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
        C_INCLUDES += $(HAL_DRIVER_INC)

HAL_CMSIS_INC = \
        -I $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Drivers/CMSIS/Device/ST/STM32F1xx/Include
        C_INCLUDES += $(HAL_CMSIS_INC)

USER_INC = \
        -I $(TOP_DIR)/targets/STM32F103RB_NUCLEO/Inc \
        -I $(TOP_DIR)/targets/STM32F103RB_NUCLEO/OS_CONFIG
        C_INCLUDES += $(USER_INC)


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LD_FILE = STM32F103RBTx_FLASH.ld
LDSCRIPT = $(PROJECTBASE)/../Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/linker/$(LD_FILE)

# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***