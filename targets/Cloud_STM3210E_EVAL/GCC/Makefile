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
# network
#######################################
USE_AT_FRAMEWORK := no
ifeq ($(USE_AT_FRAMEWORK), yes)
#ESP8266   # SIM900A  # NB_NEUL95
	NETWORK_TYPE := NB_NEUL95
endif

#######################################
# bootloader
#######################################
USE_BOOTLOADER := no

#######################################
# fota
#######################################
USE_FOTA := no

#######################################
# sota
#######################################
USE_SOTA := no

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
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c 
        C_SOURCES += $(HAL_DRIVER_SRC)

HARDWARE_SRC =  \
        ${wildcard $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Hardware/Src/*.c}
        C_SOURCES += $(HARDWARE_SRC)

MBEDTLS_SRC = \
        ${wildcard $(TOP_DIR)/components/security/mbedtls/mbedtls-2.6.0/library/*.c}
        C_SOURCES += $(MBEDTLS_SRC)

MBEDTLS_PORT_SRC = \
        ${wildcard $(TOP_DIR)/components/security/mbedtls/mbedtls_port/*.c}
        C_SOURCES += $(MBEDTLS_PORT_SRC)

ER_COAP_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/lwm2m/core/er-coap-13/er-coap-13.c}
        C_SOURCES += $(ER_COAP_SRC)

LWM2M_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/lwm2m/core/*.c} \
		${wildcard $(TOP_DIR)/components/connectivity/lwm2m/examples/shared/*.c}
        C_SOURCES += $(LWM2M_SRC)

OS_ADAPTER_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/agent_tiny/osadapter/*.c}
        C_SOURCES += $(OS_ADAPTER_SRC)
        
ATINY_TINY_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/agent_tiny/lwm2m_client/*.c}
        C_SOURCES += $(ATINY_TINY_SRC)

AGENT_DEMO_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/agent_tiny/examples/*.c}
        C_SOURCES += $(AGENT_DEMO_SRC)


USER_SRC =  \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/main.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/system_stm32f1xx.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/usart.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/dwt.c \
        $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/sys_init.c
        C_SOURCES += $(USER_SRC)

ifeq ($(USE_SOTA), yes)
SOTA_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/at_fota/*.c}
        C_SOURCES += $(SOTA_SRC)
endif

ifeq ($(USE_AT_FRAMEWORK), yes)
ATADAPTOR_SRC = \
    ${wildcard $(TOP_DIR)/components/connectivity/at_frame/*.c} \
     $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/at_hal.c
    C_SOURCES += $(ATADAPTOR_SRC)

ifeq ($(NETWORK_TYPE), ESP8266)
    USER_DRIVERS_SRC = ${wildcard $(TOP_DIR)/drivers/devices/wifi/esp8266/*.c}
else ifeq ($(NETWORK_TYPE), SIM900A)
    USER_DRIVERS_SRC = ${wildcard $(TOP_DIR)/drivers/devices/gprs/*.c}
else ifeq ($(NETWORK_TYPE), NB_NEUL95)
    USER_DRIVERS_SRC = \
	${wildcard $(TOP_DIR)/drivers/devices/nb/*.c} \
	${wildcard $(TOP_DIR)/components/connectivity/nb_iot/*.c}
endif
    C_SOURCES += $(USER_DRIVERS_SRC)
else
W5500_USER_SRC = \
    $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/wiznet.c \
    $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/w5500_conf.c
    C_SOURCES += $(W5500_USER_SRC)
    
W5500_DRIVER_SRC = \
    ${wildcard $(TOP_DIR)/drivers/devices/net/w5500/base/*.c} \
    ${wildcard $(TOP_DIR)/drivers/devices/net/w5500/internet/dhcp.c}
    C_SOURCES += $(W5500_DRIVER_SRC)
endif

ifeq ($(USE_FOTA), yes)
FOTA_SRC = \
        ${wildcard $(TOP_DIR)/components/fota/ota/*.c} \
        ${wildcard $(TOP_DIR)/components/fota/utility/*.c} \
        ${wildcard $(TOP_DIR)/components/connectivity/agent_tiny/lwm2m_client/fota/*.c} 
        C_SOURCES += $(FOTA_SRC)

FIRMWARE_UPDATE_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/agent_tiny/firmware_update/*.c}
        C_SOURCES += $(FIRMWARE_UPDATE_SRC)
		
C_SOURCES += ${wildcard  $(TOP_DIR)/components/fota/fota_package_storage_device/*.c}
C_SOURCES += $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/board.c
C_SOURCES += $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src/fota_port.c
endif

# ASM sources
ASM_SOURCES_s =  \
        ${wildcard $(PROJECTBASE)/los_startup_gcc.s}

ASM_SOURCES_S =  \
        ${wildcard $(TOP_DIR)/arch/arm/arm-m/cortex-m3/gcc/los_dispatch_gcc.S}


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
        -D USE_MBED_TLS \
        -D MBEDTLS_CONFIG_FILE=\"los_mbedtls_config.h\" \
        -D LWM2M_LITTLE_ENDIAN \
        -D LWM2M_CLIENT_MODE \
        -D NDEBUG \
        -D ATINY_DEBUG \
        -D LWM2M_WITH_LOGS \
        -D AT_INTO \
        -D AT_DEBUG \
#		-D LWM2M_BOOTSTRAP \
#       WITH_DTLS change the place with LWIP_TIMEVAL_PRIVATE

ifeq ($(USE_AT_FRAMEWORK), yes)
        C_DEFS += -DWITH_AT_FRAMEWORK -DUSE_$(NETWORK_TYPE)
else
        C_DEFS += -D WITH_WIZNET
endif

ifeq ($(USE_SOTA), yes)
		C_DEFS += -DWITH_SOTA
endif

ifeq ($(USE_FOTA), yes)
		C_DEFS += -DCONFIG_FEATURE_FOTA
endif

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

HAL_DRIVER_INC = \
        -I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Inc \
        -I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
        C_INCLUDES += $(HAL_DRIVER_INC)

HARDWARE_INC = \
        -I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Hardware/Inc
        C_INCLUDES += $(HARDWARE_INC)

MBEDTLS_INC = \
        -I $(TOP_DIR)/components/security/mbedtls/mbedtls-2.6.0/include \
        -I $(TOP_DIR)/components/security/mbedtls/mbedtls-2.6.0/include/mbedtls
        C_INCLUDES += $(MBEDTLS_INC)

MBEDTLS_PORT_INC = \
        -I $(TOP_DIR)/components/security/mbedtls/mbedtls_port
        C_INCLUDES += $(MBEDTLS_PORT_INC)

ER_COAP_INC = \
        -I $(TOP_DIR)/components/connectivity/lwm2m/core/er-coap-13
        C_INCLUDES += $(ER_COAP_INC)

LWM2M_INC = \
        -I $(TOP_DIR)/components/connectivity/lwm2m/core \
		-I $(TOP_DIR)/components/connectivity/lwm2m//examples/shared
        C_INCLUDES += $(LWM2M_INC)

OS_ADAPTER_INC = \
        -I $(TOP_DIR)/components/connectivity/agent_tiny/osadapter
        C_INCLUDES += $(OS_ADAPTER_INC)

ATINY_TINY_INC = \
        -I $(TOP_DIR)/components/connectivity/agent_tiny/lwm2m_client \
        -I $(TOP_DIR)/components/connectivity/agent_tiny/comm/include
        C_INCLUDES += $(ATINY_TINY_INC)

AGENT_DEMO_INC = \
        -I $(TOP_DIR)/components/connectivity/agent_tiny/examples
        C_INCLUDES += $(AGENT_DEMO_INC)

USER_INC = \
	-I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Inc \
	-I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/OS_CONFIG
	C_INCLUDES += $(USER_INC)
		
ifeq ($(USE_FOTA), yes)
FOTA_INC = \
        -I $(TOP_DIR)/components/fota/ota \
        -I $(TOP_DIR)/components/fota/utility \
        -I $(TOP_DIR)/components/connectivity/agent_tiny/lwm2m_client/fota
        C_INCLUDES += $(FOTA_INC)
		
FIRMWARE_UPDATE_INC = \
        -I $(TOP_DIR)/components/connectivity/agent_tiny/firmware_update
        C_INCLUDES += $(FIRMWARE_UPDATE_INC)


C_INCLUDES += -I $(TOP_DIR)/components/fota/fota_package_storage_device
endif

ifeq ($(USE_SOTA), yes)
SOTA_INC = \
        -I $(TOP_DIR)/components/connectivity/at_fota
        C_INCLUDES += $(SOTA_INC)
endif

ifeq ($(USE_AT_FRAMEWORK), yes)
ATADAPTOR_INC = \
        -I $(TOP_DIR)/components/connectivity/at_frame \
        -I $(TOP_DIR)/targets/Cloud_STM3210E_EVAL/Src
        C_INCLUDES += $(ATADAPTOR_INC)
ifeq ($(NETWORK_TYPE), ESP8266)
    USER_DRIVERS_INC = -I $(TOP_DIR)/drivers/devices/wifi
else ifeq ($(NETWORK_TYPE), SIM900A)
    USER_DRIVERS_INC = -I $(TOP_DIR)/drivers/devices/gprs
else ifeq ($(NETWORK_TYPE), NB_NEUL95)
    USER_DRIVERS_INC = \
	-I $(TOP_DIR)/components/connectivity/nb_iot \
	-I $(TOP_DIR)/drivers/devices/nb
endif
    C_INCLUDES += $(USER_DRIVERS_INC)
else
W5500_DRIVER_INC = \
    -I $(TOP_DIR)/drivers/devices/net/w5500/base \
    -I $(TOP_DIR)/drivers/devices/net/w5500/internet
    C_INCLUDES += $(W5500_DRIVER_INC)
endif


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
	LD_FILE = STM32F103ZE_LiteOS_Bootloader.ld
else
	LD_FILE = STM32F103ZE_LiteOS.ld
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