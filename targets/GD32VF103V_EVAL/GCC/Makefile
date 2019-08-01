##########################################################################################################################
# Sifive_Hifive1 GCC compiler Makefile
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
OPT = -msmall-data-limit=8 -mno-save-restore -fmessage-length=0 -Os -g



#######################################
# binaries
#######################################
CROSS_COMPILE = riscv-none-embed
ifeq ($(RISCV_PATH),)
RISCV_GCC     := $(CROSS_COMPILE)-gcc
RISCV_GXX     := $(CROSS_COMPILE)-g++
RISCV_OBJDUMP := $(CROSS_COMPILE)-objdump
RISCV_OBJCOPY := $(CROSS_COMPILE)-objcopy
RISCV_GDB     := $(CROSS_COMPILE)-gdb
RISCV_AR      := $(CROSS_COMPILE)-ar
RISCV_AS      := $(CROSS_COMPILE)-as
RISCV_SIZE    := $(CROSS_COMPILE)-size
RISCV_GDB     := $(CROSS_COMPILE)-gdb
else
RISCV_GCC     := $(abspath $(RISCV_PATH)/bin/$(CROSS_COMPILE)-gcc)
RISCV_GXX     := $(abspath $(RISCV_PATH)/bin/$(CROSS_COMPILE)-g++)
RISCV_OBJDUMP := $(abspath $(RISCV_PATH)/bin/$(CROSS_COMPILE)-objdump)
RISCV_OBJCOPY := $(abspath $(RISCV_PATH)/bin/$(CROSS_COMPILE)-objcopy)
RISCV_GDB     := $(abspath $(RISCV_PATH)/bin/$(CROSS_COMPILE)-gdb)
RISCV_AR      := $(abspath $(RISCV_PATH)/bin/$(CROSS_COMPILE)-ar)
RISCV_AS      := $(abspath $(RISCV_PATH)/bin/$(CROSS_COMPILE)-as)
RISCV_SIZE    := $(abspath $(RISCV_PATH)/bin/$(CROSS_COMPILE)-size)
RISCV_GDB     := $(abspath $(RISCV_PATH)/bin/$(CROSS_COMPILE)-gdb)
PATH          := $(abspath $(RISCV_PATH)/bin):$(PATH)
endif

CC         = $(RISCV_GCC)
GXX        = $(RISCV_GXX)
OBJCOPY    = $(RISCV_OBJCOPY)
OBJDUMP    = $(RISCV_OBJDUMP)
AR         = $(RISCV_AR)
AS         = $(RISCV_AS)
SZ         = $(RISCV_SIZE)
BIN        = $(RISCV_OBJCOPY) -O binary -S


PROJECTBASE = $(abspath $(CURDIR))
TOP_DIR     = $(abspath $(PROJECTBASE)/../../..)

#this is for IoT Studio automatic generating kconfig compatibility
ifndef SDK_DIR
    SDK_DIR=$(abspath $(PROJECTBASE)/../../..)
endif

#######################################
# paths
#######################################
# firmware library path
PERIFLIB_PATH =

# Build path
BUILD_DIR = build


################################################################################
#common variables for other module or components
C_SOURCES =
C_DEFS =
C_INCLUDES =
LDFLAGS =
ASM_SOURCES_S =
AS_DEFS =
AS_INCLUDES =
PERIFLIB_SOURCES =
LIBS =
LIBDIR =
LDFLAGS =
ASFLAGS =
CFLAGS =


##########################LOAD THE SOURCES INCLUDES AND DEFINES#################
include $(PROJECTBASE)/config.mk
include $(PROJECTBASE)/project.mk
######################################
# firmware library
######################################
PERIFLIB_SOURCES =


#######################################
# CFLAGS
#######################################


# Set the arch, ABI, and code model
RISCV_CFLAGS   += -march=rv32imac -mabi=ilp32 -mcmodel=medlow -fsigned-char $(OPT) -std=gnu11
RISCV_CXXFLAGS += -march=rv32imac -mabi=ilp32 -mcmodel=medlow -fsigned-char $(OPT) -std=gnu11
# Prune unused functions and data
RISCV_CFLAGS   += -ffunction-sections -fdata-sections
RISCV_CXXFLAGS += -ffunction-sections -fdata-sections
# Include the Metal headers
RISCV_CFLAGS   += $(C_INCLUDES)
RISCV_CXXFLAGS += $(C_INCLUDES)

# Turn on garbage collection for unused sections
RISCV_LDFLAGS += -march=rv32imac -mabi=ilp32 -mcmodel=medlow -ffunction-sections -fdata-sections -Wl,--gc-sections
# Turn off the C standard library
RISCV_LDFLAGS += -nostartfiles -Xlinker --gc-sections -Wl,-Map,"GD32V103V_EVAL.map" 
#-nostdlib  --specs=nano.specs
# Link to the relevant libraries
#RISCV_LDLIBS += -Wl,--start-group -lc -lgcc -Wl,--end-group
#-lnosys

# compile gcc flags
ASFLAGS = -march=rv32imac -mabi=ilp32 -mcmodel=medlow -ffunction-sections -fdata-sections $(OPT) -O0 $(AS_INCLUDES) -x assembler-with-cpp

CFLAGS = $(RISCV_CFLAGS) $(C_DEFS)
LDFLAGS = $(RISCV_LDFLAGS) $(C_DEFS)

ifeq ($(DEBUG), 1)
CFLAGS += -g
ASFLAGS += -g
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$@"

# excluded unnecessary warnings
CFLAGS += -Wno-missing-braces


#######################################
# LDFLAGS
#######################################
# link script
LD_FILE = GD32VF103xB.lds

LDSCRIPT = $(PROJECTBASE)/$(LD_FILE)

# libraries
LIBS = -lnosys
ifeq ($(USE_OTA), yes)
LIBS += -lhwpatch
endif
LIBDIR = -L$(PROJECTBASE)/../Lib
LDFLAGS += $(RISCV_LDLIBS)
LDFLAGS +=   -T $(LDSCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

ifneq ($(RISCV_OPENOCD_PATH),)
RISCV_OPENOCD=$(RISCV_OPENOCD_PATH)/bin/openocd
#RISCV_OPENOCD=/home/baikal/workspace/RISCV/gnu-mcu-eclipse/openocd/0.10.0-10-20181020-0522/bin/openocd
else
#if RISCV_OPENOCD_PATH is not set, just look on the PATH
RISCV_OPENOCD=openocd
endif


# default action: build all
all:
	@echo "JUST FOR CI"
	@echo "To build the elf, make GD"

GD: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

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
	$(CC) -c $(CFLAGS)  $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(CC) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
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
#-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
