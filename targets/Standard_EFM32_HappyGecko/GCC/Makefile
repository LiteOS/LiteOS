##########################################################################################################################
# EFM32_HAPPY_GECKO GCC compiler Makefile
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
# bootloader
#######################################
USE_BOOTLOADER := no

#######################################
# fota
#######################################
USE_FOTA := no

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
        ${wildcard $(TOP_DIR)/kernel/base/mem/membox/*.c} \
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

HAL_DRIVER_SRC =  \
        ${wildcard $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Drivers/efm32hg322f_driver/src/*.c}
        C_SOURCES += $(HAL_DRIVER_SRC)

KIT_DRIVER_SRC =  \
        $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Drivers/kit_drivers/retargetio.c \
		$(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Drivers/kit_drivers/retargetserial.c
        C_SOURCES += $(KIT_DRIVER_SRC)

USER_SRC =  \
        $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Src/main.c \
        $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Src/sys_init.c \
        $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Src/system_efm32hg.c \
        $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Src/usart.c
        C_SOURCES += $(USER_SRC)

# ASM sources
ASM_SOURCES_s =

ASM_SOURCES_S =  \
		${wildcard $(PROJECTBASE)/startup_efm32hg.S} \
        ${wildcard $(TOP_DIR)/arch/arm/arm-m/cortex-m0/gcc/los_dispatch_gcc.S}


######################################
# firmware library
######################################
PERIFLIB_SOURCES =


#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m0
MCU = $(CPU) -mthumb
# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
        -D EFM32HG322F64 \
		-D RETARGET_USART \

# AS includes
AS_INCLUDES =

# C includes
KERNEL_INC = \
        -I $(TOP_DIR)/kernel/base/include  \
        -I $(TOP_DIR)/kernel/extended/include  \
        -I $(TOP_DIR)/kernel/include \
        -I $(TOP_DIR)/kernel/base/core
        C_INCLUDES += $(KERNEL_INC)

ARCH_INC = \
        -I $(TOP_DIR)/arch/arm/arm-m/include \
        -I $(TOP_DIR)/arch/arm/common/cmsis
        C_INCLUDES += $(ARCH_INC)

HAL_DRIVER_INC = \
        -I $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Drivers/efm32hg322f_driver/inc \
        -I $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Drivers/efm32hg322f_config \
		-I $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Drivers/config \
		-I $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Drivers/kit_drivers
        C_INCLUDES += $(HAL_DRIVER_INC)

USER_INC = \
		-I $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/Inc \
		-I $(TOP_DIR)/targets/EFM32_HAPPY_GECKO/OS_CONFIG
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
ifeq ($(USE_BOOTLOADER), yes)
	LD_FILE = STM32F429IGTx_LiteOS_Bootloader.ld
else
	LD_FILE = efm32hg.ld
endif
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

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile $(LD_FILE)
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