##########################################################################################################################
# Cloud_STM32F429IGTx_FIRE GCC compiler Makefile
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
# ------------------------------------------------
include config.mk
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
WITH_AT_FRAMEWORK := yes
ifeq ($(WITH_AT_FRAMEWORK), yes)
#ESP8266   # SIM900A  # NB_NEUL95
	NETWORK_TYPE := NB_NEUL95
endif

#######################################
# bootloader
#######################################
USE_BOOTLOADER := no

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
        ${wildcard $(TOP_DIR)/osdepends/liteos/cmsis/*.c}
        C_SOURCES += $(CMSIS_SRC)

ARCH_SRC =  \
        ${wildcard $(TOP_DIR)/arch/arm/arm-m/src/*.c}
        C_SOURCES += $(ARCH_SRC)

HAL_DRIVER_SRC =  \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c
        C_SOURCES += $(HAL_DRIVER_SRC)

HARDWARE_SRC =  \
        ${wildcard $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Hardware/Src/*.c}
        C_SOURCES += $(HARDWARE_SRC)
		
ifeq ($(WITH_LWIP), yes)
LWIP_SRC =  \
        ${wildcard $(TOP_DIR)/components/net/lwip/lwip-2.0.3/src/api/*.c} \
        ${wildcard $(TOP_DIR)/components/net/lwip/lwip-2.0.3/src/core/ipv4/*.c} \
        ${wildcard $(TOP_DIR)/components/net/lwip/lwip-2.0.3/src/core/*.c} \
        ${wildcard $(TOP_DIR)/components/net/lwip/lwip-2.0.3/src/netif/ethernet.c}
        C_SOURCES += $(LWIP_SRC)

LWIP_PORT_SRC = \
        ${wildcard $(TOP_DIR)/components/net/lwip/lwip_port/OS/*.c}
        C_SOURCES += $(LWIP_PORT_SRC)
endif

ifneq ($(USE_FOTA)_$(WITH_DTLS), no_no)
MBEDTLS_SRC = \
        ${wildcard $(TOP_DIR)/components/security/mbedtls/mbedtls-2.6.0/library/*.c}
        C_SOURCES += $(MBEDTLS_SRC)

MBEDTLS_PORT_SRC = \
        ${wildcard $(TOP_DIR)/components/security/mbedtls/mbedtls_port/*.c}
        C_SOURCES += $(MBEDTLS_PORT_SRC)
endif

ER_COAP_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/lwm2m/core/er-coap-13/er-coap-13.c}
        C_SOURCES += $(ER_COAP_SRC)

LWM2M_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/lwm2m/core/*.c} \
		${wildcard $(TOP_DIR)/components/connectivity/lwm2m/examples/shared/*.c}
        C_SOURCES += $(LWM2M_SRC)

OS_DEPENDS_SRC = \
        ${wildcard $(TOP_DIR)/osdepends/liteos/*.c}
        C_SOURCES += $(OS_DEPENDS_SRC)

SOCKET_ADAPTER_SRC = \
        ${wildcard $(TOP_DIR)/components/net/sal/*.c}
        C_SOURCES += $(SOCKET_ADAPTER_SRC)
		
NEWLIB_SRC = \
        ${wildcard $(TOP_DIR)/components/lib/libc/malloc.c}
        C_SOURCES += $(NEWLIB_SRC)

ATINY_TINY_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/agent_tiny/atiny_lwm2m/*.c}
        C_SOURCES += $(ATINY_TINY_SRC)

ATINY_LOG = \
        ${wildcard $(TOP_DIR)/components/log/*.c}
        C_SOURCES += $(ATINY_LOG)

AGENT_DEMO_SRC = \
        ${wildcard $(TOP_DIR)/demos/agenttiny_lwm2m/*.c}
        C_SOURCES += $(AGENT_DEMO_SRC)

FS_SRC = \
        ${wildcard $(TOP_DIR)/components/fs/vfs/*.c}
        C_SOURCES += $(FS_SRC)

FS_SPIFFS_SRC = \
        ${wildcard $(TOP_DIR)/components/fs/spiffs/*.c} \
        ${wildcard $(TOP_DIR)/components/fs/spiffs/spiffs_git/src/*.c}
        C_SOURCES += $(FS_SPIFFS_SRC)

FS_FATFS_SRC = \
        ${wildcard $(TOP_DIR)/components/fs/fatfs/*.c} \
        ${wildcard $(TOP_DIR)/components/fs/fatfs/ff13b/source/*.c}
        C_SOURCES += $(FS_FATFS_SRC)

FS_JFFS2_SRC = \
        ${wildcard $(TOP_DIR)/components/fs/jffs2/*.c} \
        ${wildcard $(TOP_DIR)/components/fs/jffs2/src/*.c} \
        ${wildcard $(TOP_DIR)/components/fs/jffs2/port/*.c} \
        ${wildcard $(TOP_DIR)/components/fs/jffs2/cyg/compress/*.c} \
        ${wildcard $(TOP_DIR)/components/fs/jffs2/cyg/crc/crc32.c}
        C_SOURCES += $(FS_JFFS2_SRC)

FS_DEMO_SRC = \
        ${wildcard $(TOP_DIR)/demos/fs/*.c}
        C_SOURCES += $(FS_DEMO_SRC)


USER_SRC =  \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/main.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/system_stm32f4xx.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/usart.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/dwt.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/eth.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/sys_init.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/spiffs_stm32f4xx.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/fatfs_stm32f4xx.c \
        $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/jffs2_stm32f4xx.c
        C_SOURCES += $(USER_SRC)

ifeq ($(USE_SOTA), yes)
SOTA_SRC = \
        ${wildcard $(TOP_DIR)/components/ota/sota/*.c}
        C_SOURCES += $(SOTA_SRC)
endif

ifeq ($(WITH_AT_FRAMEWORK), yes)
AT_FRAME_SRC = \
    ${wildcard $(TOP_DIR)/components/net/at_frame/*.c} \
     $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/at_hal.c
    C_SOURCES += $(AT_FRAME_SRC)

ifeq ($(NETWORK_TYPE), ESP8266)
    USER_DRIVERS_SRC = ${wildcard $(TOP_DIR)/components/net/at_device/wifi_esp8266/*.c}
else ifeq ($(NETWORK_TYPE), SIM900A)
    USER_DRIVERS_SRC = ${wildcard $(TOP_DIR)/components/net/at_device/gprs_sim900a/*.c}
else ifeq ($(NETWORK_TYPE), NB_NEUL95)
    USER_DRIVERS_SRC = \
	${wildcard $(TOP_DIR)/components/net/at_device/nb_bc95/*.c} \
	${wildcard $(TOP_DIR)/components/connectivity/nb_iot/*.c}
else ifeq ($(NETWORK_TYPE), ALL)
    USER_DRIVERS_SRC = \
	${wildcard $(TOP_DIR)/components/net/at_device/wifi_esp8266/*.c} \
	${wildcard $(TOP_DIR)/components/net/at_device/gprs_sim900a/*.c} \
	${wildcard $(TOP_DIR)/components/net/at_device/nb_bc95/*.c} \
	${wildcard $(TOP_DIR)/components/connectivity/nb_iot/*.c}
endif
    C_SOURCES += $(USER_DRIVERS_SRC)
endif

ifeq ($(USE_FOTA), yes)
FOTA_SRC = \
		${wildcard  $(TOP_DIR)/components/ota/fota/*.c}
        C_SOURCES += $(FOTA_SRC)

FIRMWARE_UPDATE_SRC = \
        ${wildcard $(TOP_DIR)/components/connectivity/agent_tiny/firmware_update/*.c}
        C_SOURCES += $(FIRMWARE_UPDATE_SRC)
		
C_SOURCES += $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/board.c
C_SOURCES += $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/fota_port.c
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
        -D MBEDTLS_CONFIG_FILE=\"los_mbedtls_config.h\" \
        -D LWM2M_LITTLE_ENDIAN \
        -D LWM2M_CLIENT_MODE \
        -D NDEBUG \
        -D LWIP_TIMEVAL_PRIVATE=0 


ifeq ($(WITH_LWIP), yes)
		C_DEFS += -DWITH_LWIP
endif
ifeq ($(WITH_AT_FRAMEWORK), yes)
		C_DEFS += -DWITH_AT_FRAMEWORK
		ifneq ($(NETWORK_TYPE), ALL)
		C_DEFS += -DUSE_$(NETWORK_TYPE)
		else
		C_DEFS += -DUSE_ESP8266 -DSIM900A -DNB_NEUL95
		endif
endif

ifeq ($(WITH_DTLS), yes)
		C_DEFS += -DWITH_DTLS
endif


ifeq ($(USE_FOTA), yes)
		C_DEFS += -DCONFIG_FEATURE_FOTA
endif

ifeq ($(USE_SOTA), yes)
		C_DEFS += -DWITH_SOTA
endif

ifeq ($(LWM2M_BOOTSTRAP), yes)
		C_DEFS += -DLWM2M_BOOTSTRAP
endif

ifeq ($(SUPPORT_DTLS_SRV), yes)
		C_DEFS += -DSUPPORT_DTLS_SRV
endif

ifeq ($(LWM2M_WITH_LOGS), yes)
		C_DEFS += -DLWM2M_WITH_LOGS
endif

ifeq ($(ATINY_DEBUG), yes)
		C_DEFS += -DATINY_DEBUG
endif




# AS includes
AS_INCLUDES =

# C includes
KERNEL_INC = \
        -I $(TOP_DIR)/kernel/include \
		-I $(TOP_DIR)/kernel/base/include \
		-I $(TOP_DIR)/kernel/extended/include
        C_INCLUDES += $(KERNEL_INC)

INCLUDE_INC = \
        -I $(TOP_DIR)/include
        C_INCLUDES += $(INCLUDE_INC)

ARCH_INC = \
        -I $(TOP_DIR)/arch/arm/arm-m/include \
		-I $(TOP_DIR)/arch/arm/common/cmsis
        C_INCLUDES += $(ARCH_INC)

HAL_DRIVER_INC = \
        -I $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Inc \
        -I $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        C_INCLUDES += $(HAL_DRIVER_INC)

HARDWARE_INC = \
        -I $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Hardware/Inc
        C_INCLUDES += $(HARDWARE_INC)
		

LWIP_INC = \
        -I $(TOP_DIR)/components/net/lwip/lwip-2.0.3/src/include
        C_INCLUDES += $(LWIP_INC)

LWIP_PORT_INC = \
        -I $(TOP_DIR)/components/net/lwip/lwip_port \
        -I $(TOP_DIR)/components/net/lwip/lwip_port/OS
        C_INCLUDES += $(LWIP_PORT_INC)


MBEDTLS_INC = \
        -I $(TOP_DIR)/components/security/mbedtls/mbedtls-2.6.0/include
        C_INCLUDES += $(MBEDTLS_INC)

MBEDTLS_PORT_INC = \
        -I $(TOP_DIR)/components/security/mbedtls/mbedtls_port
        C_INCLUDES += $(MBEDTLS_PORT_INC)

ER_COAP_INC = \
        -I $(TOP_DIR)/components/connectivity/lwm2m/core/er-coap-13
        C_INCLUDES += $(ER_COAP_INC)

LWM2M_INC = \
        -I $(TOP_DIR)/components/connectivity/lwm2m/core \
		-I $(TOP_DIR)/components/connectivity/lwm2m/examples/shared
        C_INCLUDES += $(LWM2M_INC)

DEMO_INC = \
        -I $(TOP_DIR)/demos
        C_INCLUDES += $(DEMO_INC)

FS_SPIFFS_INC = \
		-I $(TOP_DIR)/components/fs/spiffs/spiffs_git/src \
		-I $(TOP_DIR)/components/fs/spiffs/spiffs_git/src/default
		C_INCLUDES += $(FS_SPIFFS_INC)

FS_FATFS_INC = \
		-I $(TOP_DIR)/components/fs/fatfs/ff13b/source
		C_INCLUDES += $(FS_FATFS_INC)

FS_JFFS2_INC = \
		-I $(TOP_DIR)/components/fs/jffs2 \
		-I $(TOP_DIR)/components/fs/jffs2/src \
		-I $(TOP_DIR)/components/fs/jffs2/port
		C_INCLUDES += $(FS_JFFS2_INC)

USER_INC = \
		-I $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/Inc \
		-I $(TOP_DIR)/targets/Cloud_STM32F429IGTx_FIRE/OS_CONFIG
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
	LD_FILE = STM32F429IGTx_LiteOS.ld
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