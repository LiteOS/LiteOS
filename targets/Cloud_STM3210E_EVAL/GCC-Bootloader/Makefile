##########################################################################################################################
# Cloud_STM3210E_EVAL GCC compiler Makefile
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
DEBUG = 0
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
HAL_DRIVER_SRC =  \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi_ex.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c
        C_SOURCES += $(HAL_DRIVER_SRC)

HARDWARE_SRC =  \
        ${wildcard $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Hardware/Src/*.c}
        C_SOURCES += $(HARDWARE_SRC)

USER_SRC =  \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/bootloader_main.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/system_stm32f1xx.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/usart.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/board.c
        C_SOURCES += $(USER_SRC)

FOTA_SRC = \
        ${wildcard $(TOP_DIR)/components/fota/ota/*.c} \
        ${wildcard $(TOP_DIR)/components/fota/utility/*.c}
        C_SOURCES += $(FOTA_SRC)

# ASM sources
ASM_SOURCES_s =  \
        ${wildcard $(PROJECTBASE)/startup_stm32f103xe.s}

ASM_SOURCES_S = 


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
# FPU =
# float-abi
# FLOAT-ABI =
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
        -D USE_HAL_DRIVER \
        -D STM32F103xE \
        -D NDEBUG \
        -D USE_BOOTLOADER


# AS includes
AS_INCLUDES =

# C includes
ARCH_INC = \
        -I $(TOP_DIR)/arch/arm/arm-m/include \
        -I $(TOP_DIR)/arch/arm/common/cmsis
        C_INCLUDES += $(ARCH_INC)

HAL_DRIVER_INC = \
        -I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Inc \
        -I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
        C_INCLUDES += $(HAL_DRIVER_INC)

HARDWARE_INC = \
        -I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Hardware/Inc
        C_INCLUDES += $(HARDWARE_INC)

USER_INC = \
	-I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Inc \
	-I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/OS_CONFIG
	C_INCLUDES += $(USER_INC)		

FOTA_INC = \
        -I $(TOP_DIR)/components/fota/ota \
        -I $(TOP_DIR)/components/fota/utility
        C_INCLUDES += $(FOTA_INC)

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
LD_FILE = STM32F103XE_FLASH.ld
LDSCRIPT = $(PROJECTBASE)/$(LD_FILE)

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
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES_s:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES_s)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES_S:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES_S)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
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