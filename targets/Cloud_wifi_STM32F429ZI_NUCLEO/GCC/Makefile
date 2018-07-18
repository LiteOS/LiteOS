##########################################################################################################################
# Cloud_wifi_STM32F429ZI_NUCLEO GCC compiler Makefile
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
# network
#######################################
USE_AT_FRAMEWORK := no
ifeq ($(USE_AT_FRAMEWORK), yes)
#ESP8266   # SIM900A  # NB_NEUL95
	NETWORK_TYPE := ESP8266
else
	NETWORK_TYPE = MRVL_SDIO_WIFI
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

HAL_DRIVER_SRC =  \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c 

ifeq ($(NETWORK_TYPE), MRVL_SDIO_WIFI)
HAL_DRIVER_SRC += \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c 
endif
		C_SOURCES += $(HAL_DRIVER_SRC)

HARDWARE_SRC =  \
        ${wildcard $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Hardware/Src/*.c}
        C_SOURCES += $(HARDWARE_SRC)

LWIP_SRC =  \
        ${wildcard $(TOP_DIR)/components/net/lwip-2.0.3/src/api/*.c} \
        ${wildcard $(TOP_DIR)/components/net/lwip-2.0.3/src/core/ipv4/*.c} \
        ${wildcard $(TOP_DIR)/components/net/lwip-2.0.3/src/core/*.c} \
        ${wildcard $(TOP_DIR)/components/net/lwip-2.0.3/src/netif/ethernet.c}
        C_SOURCES += $(LWIP_SRC)

LWIP_PORT_SRC = \
        ${wildcard $(TOP_DIR)/components/net/lwip_port/OS/*.c}
        C_SOURCES += $(LWIP_PORT_SRC)

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
        ${wildcard $(TOP_DIR)/components/connectivity/lwm2m/core/*.c}
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
        
ifeq ($(NETWORK_TYPE), MRVL_SDIO_WIFI)
WIFI_SRC = \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/wifi_eth.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/wifi_lowlevel.c 
    	C_SOURCES += $(WIFI_SRC)
endif

USER_SRC =  \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/main.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/system_stm32f4xx.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/usart.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/dwt.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/eth.c \
        $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/sys_init.c 
        C_SOURCES += $(USER_SRC)

ifeq ($(USE_AT_FRAMEWORK), yes)
ATADAPTOR_SRC = \
    ${wildcard $(TOP_DIR)/components/connectivity/at_frame/*.c} \
     $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/at_hal.c
    C_SOURCES += $(ATADAPTOR_SRC)

ifeq ($(NETWORK_TYPE), ESP8266)
    USER_DRIVERS_SRC = ${wildcard $(TOP_DIR)/drivers/devices/wifi/*.c}
else ifeq ($(NETWORK_TYPE), SIM900A)
    USER_DRIVERS_SRC = ${wildcard $(TOP_DIR)/drivers/devices/gprs/*.c}
else ifeq ($(NETWORK_TYPE), NB_NEUL95)
    USER_DRIVERS_SRC = \
	${wildcard $(TOP_DIR)/drivers/devices/nb/*.c} \
	${wildcard $(TOP_DIR)/components/connectivity/nb_iot/*.c}
endif
    C_SOURCES += $(USER_DRIVERS_SRC)
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
C_SOURCES += $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/board.c
C_SOURCES += $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Src/fota_port.c
endif


ifeq ($(NETWORK_TYPE), MRVL_SDIO_WIFI)
MRVL_DRIVERS_SRC = \
	$(TOP_DIR)/drivers/devices/wifi/marvell/wifi.c \
	$(TOP_DIR)/drivers/devices/wifi/marvell/tiny_aes.c \
	$(TOP_DIR)/drivers/devices/wifi/marvell/wpa.c
	C_SOURCES += $(MRVL_DRIVERS_SRC)
endif

# ASM sources
ASM_SOURCES_s =  \
        ${wildcard $(PROJECTBASE)/los_startup_gcc.s}

ASM_SOURCES_S =  \
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
        -D USE_HAL_DRIVER \
        -D STM32F429xx \
        -D USE_MBED_TLS \
        -D MBEDTLS_CONFIG_FILE=\"los_mbedtls_config.h\" \
        -D LWM2M_LITTLE_ENDIAN \
        -D LWM2M_CLIENT_MODE \
        -D NDEBUG \
		-D LWM2M_BOOTSTRAP \
        -D LWIP_TIMEVAL_PRIVATE=0 
#       -D WITH_DTLS \   
#       WITH_DTLS change the place with LWIP_TIMEVAL_PRIVATE
        
ifeq ($(USE_AT_FRAMEWORK), yes)
        C_DEFS += -D WITH_AT_FRAMEWORK -D USE_$(NETWORK_TYPE)
else
        C_DEFS += -D WITH_LWIP
endif

ifeq ($(USE_FOTA), yes)
	C_DEFS += -DCONFIG_FEATURE_FOTA
endif

ifeq ($(NETWORK_TYPE), MRVL_SDIO_WIFI)
	C_DEFS += -D USE_$(NETWORK_TYPE)
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
        -I $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Inc \
        -I $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        C_INCLUDES += $(HAL_DRIVER_INC)

HARDWARE_INC = \
        -I $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Hardware/Inc
        C_INCLUDES += $(HARDWARE_INC)

LWIP_INC = \
        -I $(TOP_DIR)/components/net/lwip-2.0.3/src/include
        C_INCLUDES += $(LWIP_INC)

LWIP_PORT_INC = \
        -I $(TOP_DIR)/components/net/lwip_port \
        -I $(TOP_DIR)/components/net/lwip_port/OS
        C_INCLUDES += $(LWIP_PORT_INC)

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
        -I $(TOP_DIR)/components/connectivity/lwm2m/core 
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
        -I $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/Inc \
        -I $(TOP_DIR)/targets/Cloud_wifi_STM32F429ZI_NUCLEO/OS_CONFIG
        C_INCLUDES += $(USER_INC)
        
ifeq ($(USE_FOTA), yes)
FOTA_INC = \
        -I $(TOP_DIR)/components/fota/ota \
        -I $(TOP_DIR)/components/fota/utility \
	-I $(TOP_DIR)/components/fota/fota_package_storage_device \
        -I $(TOP_DIR)/components/connectivity/agent_tiny/lwm2m_client/fota
        C_INCLUDES += $(FOTA_INC)

FIRMWARE_UPDATE_INC = \
        -I $(TOP_DIR)/components/connectivity/agent_tiny/firmware_update
        C_INCLUDES += $(FIRMWARE_UPDATE_INC)

endif

ATADAPTOR_INC = \
        -I $(TOP_DIR)/components/connectivity/at_frame
        C_INCLUDES += $(ATADAPTOR_INC)
        
ifeq ($(USE_AT_FRAMEWORK), yes)
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
endif

ifeq ($(NETWORK_TYPE), MRVL_SDIO_WIFI)
MRVL_DRIVERS_INC = \
        -I $(TOP_DIR)/drivers/devices/wifi/marvell
        C_INCLUDES += $(MRVL_DRIVERS_INC)
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
LD_FILE = STM32F429ZITx_LiteOS.ld
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
