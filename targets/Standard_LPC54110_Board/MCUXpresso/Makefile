##########################################################################################################################
# Standard_LPC54110_Board GCC compiler Makefile
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
#override PROJECTBASE    := $(abspath $(PROJECTBASE))
#TOP_DIR = $(PROJECTBASE)/../../..
TOP_DIR = ../../..

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

FSL_DRIVER_SRC =  \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_clock.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_common.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_ctimer.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_dma.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_dmic.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_dmic_dma.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_flexcomm.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_power.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_reset.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_usart.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers/fsl_gpio.c 
        C_SOURCES += $(FSL_DRIVER_SRC)

UTIL_DRIVER_SRC =  \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/utilities/fsl_debug_console.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/utilities/fsl_assert.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/utilities/log/fsl_log.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/utilities/io/fsl_io.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/utilities/str/fsl_str.c
        C_SOURCES += $(UTIL_DRIVER_SRC)		

USER_SRC =  \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Src/main.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/system_LPC54114_cm4.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Src/sys_init.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Src/board.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Src/clock_config.c \
        $(TOP_DIR)/targets/Standard_LPC54110_Board/Src/pin_mux.c
        C_SOURCES += $(USER_SRC)


# ASM sources
ASM_SOURCES_s = 

ASM_SOURCES_S =  \
        ${wildcard $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/gcc/startup_LPC54114_cm4.S} \
        ${wildcard $(TOP_DIR)/arch/arm/arm-m/cortex-m4/gcc/los_dispatch_gcc.S}


######################################
# firmware library
######################################
PERIFLIB_SOURCES =


#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4
# fpu
FPU = -mfpu=fpv4-sp-d16
# float-abi
FLOAT-ABI = -mfloat-abi=hard
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
        -D CPU_LPC54114J256BD64_cm4 \
        -D LOS_KERNEL_DEBUG_OUT \
        -D NDEBUG

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

NRF_DRIVER_INC = \
        -I $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114 \
		-I $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/drivers \
		-I $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/utilities \
		-I $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/utilities/log \
		-I $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/utilities/io \
		-I $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/utilities/str
        C_INCLUDES += $(NRF_DRIVER_INC)

USER_INC = \
		-I $(TOP_DIR)/targets/Standard_LPC54110_Board/Inc \
		-I $(TOP_DIR)/targets/Standard_LPC54110_Board/OS_CONFIG
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
LD_FILE = LPC54114J256_cm4_flash.ld
LDSCRIPT = $(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/gcc/$(LD_FILE)

# libraries
LIBS = -lc -lm -lnosys
LIBS += -lpower_cm4_hardabi
LIBDIR = -L$(TOP_DIR)/targets/Standard_LPC54110_Board/Drivers/LPC54114/gcc
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