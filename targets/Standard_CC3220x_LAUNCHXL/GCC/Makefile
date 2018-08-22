##########################################################################################################################
# Standard_CC3220x_LAUNCHXL GCC compiler Makefile
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
        
DEVICES_SRC = \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx/driverlib/cpu.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx/driverlib/flash.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx/driverlib/gpio.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx/driverlib/interrupt.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx/driverlib/pin.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx/driverlib/prcm.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx/driverlib/systick.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx/driverlib/timer.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx/driverlib/uart.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx/driverlib/utils.c
		C_SOURCES += $(DEVICES_SRC)  
        
DRIVERS_SRC = \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/drivers/gpio/GPIOCC32XX.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/drivers/utils/List.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/drivers/utils/RingBuf.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/drivers/power/PowerCC32XX.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/drivers/uart/UARTCC32XX.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/drivers/UARTx.c
		C_SOURCES += $(DRIVERS_SRC)   
        
DPL_SRC = \
		${wildcard $(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/dpl/*.c}
		C_SOURCES += $(DPL_SRC)

USER_SRC =  \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Src/main.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Src/system_cc32xx.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Src/usart.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Src/CC3220SF_LAUNCHXL.c \
		$(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Src/sys_init.c
		C_SOURCES += $(USER_SRC)

STARTUP_SRC = \
		${wildcard $(PROJECTBASE)/startup_cc32xx_gcc.c}
		C_SOURCES += $(STARTUP_SRC)

# ASM sources
ASM_SOURCES_s =

ASM_SOURCES_S =  \
		${wildcard $(TOP_DIR)/arch/arm/arm-m/cortex-m4/gcc/los_dispatch_gcc.S}

ASM_SOURCES_asm = \
        ${wildcard $(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/drivers/power/PowerCC32XX_asm.sm4g}

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
FLOAT-ABI = -mfloat-abi=soft
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
		-D DeviceFamily_CC3220 \
        -D LOS_KERNEL_DEBUG_OUT \
        -D gcc

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

DRIVER_INC = \
        -I $(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers \
		-I $(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Drivers/ti/devices/cc32xx
        C_INCLUDES += $(DRIVER_INC)

USER_INC = \
		-I $(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/Inc \
		-I $(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/dpl \
		-I $(TOP_DIR)/targets/Standard_CC3220x_LAUNCHXL/OS_CONFIG
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
LD_FILE = CC3220SF_LAUNCHXL_LiteOS.lds
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
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES_asm:.sm4g=.o)))
vpath %.sm4g $(sort $(dir $(ASM_SOURCES_asm)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
	
$(BUILD_DIR)/%.o: %.sm4g Makefile | $(BUILD_DIR)
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