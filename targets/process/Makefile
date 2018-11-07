TARGET = Huawei_LiteOS
DEBUG = 1
OPT= -std=c11 

HEX = objcopy -O ihex
BIN = objcopy -O binary -S

PROJECTBASE = $(PWD)
override PROJECTBASE := $(abspath $(PROJECTBASE))
TOP_DIR = $(PROJECTBASE)/../..

BUILD_DIR =build

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

ARCH_SRC =  \
        ${wildcard $(TOP_DIR)/arch/process/src/*.c}
C_SOURCES += $(ARCH_SRC)

EXAMPLES_SRC =  \
        ${wildcard $(TOP_DIR)/examples/api/*.c}
C_SOURCES += $(EXAMPLES_SRC)

USER_SRC = \
	${wildcard $(TOP_DIR)/targets/process/*.c}
C_SOURCES += $(USER_SRC)

CPU = 
MCU = -m32

AS_DEFS = 
C_DEFS = -D DEBUG

AS_INCLUDS = 
KERNEL_INC = \
	-I $(TOP_DIR)/kernel/include \
	-I $(TOP_DIR)/kernel/base/include \
	-I $(TOP_DIR)/kernel/extended/include \
	-I $(TOP_DIR)/kernel/base/core \

ARCH_INC = \
	-I $(TOP_DIR)/arch/process/include

USER_INC = \
	-I $(TOP_DIR)/targets/process/OS_CONFIG

EXAMPLE_INC = \
	-I $(TOP_DIR)/examples/include

C_INCLUDES = $(AS_INCLUDS) $(KERNEL_INC) $(ARCH_INC) $(USER_INC) $(EXAMPLE_INC)

ASFLAGS = 
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall 

ifeq ($(DEBUG), 1)
CFLAGS += -g 
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

LIBS = -lc -lm 
LDFLAGS = $(MCU) $(LIBDIR) $(LIBS) -Wl,-m,elf_i386 -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref,-e,populateHeap -Wl,--gc-sections

all:$(BUILD_DIR)/$(TARGET)
	
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	size $@

$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR .dep $(BUILD_DIR)
print-%	: ; @echo $* = $($*)
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
