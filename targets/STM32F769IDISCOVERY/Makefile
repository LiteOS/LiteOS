include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/components.mk
include $(LITEOSTOPDIR)/demos/demos.mk

MODULE_NAME := $(LITEOS_PLATFORM)

-include $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/config.mk

ALL_ASSRCS := $(wildcard board/$(LITEOS_PLATFORM)/*.S)
ASSRCS := $(subst board/$(LITEOS_PLATFORM)/board.ld.S,,$(ALL_ASSRCS))

LOCAL_SRCS += $(ASSRCS)

# C sources
ifeq ($(LOSCFG_COMPONENTS_FS), y)
    ifeq ($(LOSCFG_FATFS_USE_SD_CARD), y)
    FS_SRC += \
        $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Src/fatfs_sd_hal.c \
        $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Src/sd_diskio.c
    endif

C_SOURCES += $(FS_SRC)
endif

BSP_DRIVER_SRC =  \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/Components/ft5336/ft5336.c \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/Components/ft6x06/ft6x06.c \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/Components/otm8009a/otm8009a.c \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/Components/wm8994/wm8994.c \
    ${wildcard $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/STM32F769I-Discovery/*.c}
C_SOURCES += $(BSP_DRIVER_SRC)

USER_SRC =  \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/os_adapt/os_adapt.c \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Src/main.c \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Src/user_task.c \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Src/tim.c \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Src/platform_init.c \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Src/stm32f7xx_it.c \
    $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Src/system_stm32f7xx.c
    C_SOURCES += $(USER_SRC)

# C includes
HAL_DRIVER_INC = \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F7xx_HAL_Driver/Inc \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/STM32F769I-Discovery \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/STM32F769I-Discovery/Utilities/Fonts \
    -I $(LITEOSTOPDIR)/compat/cmsis
    BOARD_INCLUDES += $(HAL_DRIVER_INC)

INCLUDE_INC = \
    -I $(LITEOSTOPDIR)/include
    BOARD_INCLUDES += $(INCLUDE_INC)

BSP_INC = \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Inc \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/include\asm \
    -I $(LITEOSTOPDIR)/targets/bsp/include \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/st_timer \
    -I $(LITEOSTOPDIR)/arch/arm/cortex_m/cmsis \
    -I $(LITEOSTOPDIR)/arch/arm/include \
    -I $(LITEOSTOPDIR)/arch/arm/cortex_m/include \
    -I $(LITEOSTOPDIR)/compat/posix/src \
    -I $(LITEOSTOPDIR)/kernel/extended/include
    BOARD_INCLUDES += $(BSP_INC)

HARDWARE_INC = \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Hardware/Inc
    BOARD_INCLUDES += $(HARDWARE_INC)

LITEOS_CMSIS_INC = \
    -I $(LITEOSTOPDIR)/osdepends/liteos/cmsis
    BOARD_INCLUDES += $(LITEOS_CMSIS_INC)

USER_INC = \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Inc \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/include \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/Components/wm8994 \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F7xx_HAL_Driver/Inc \
    -I $(LITEOSTOPDIR)/osdepends/liteos/cmsis/1.0 \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/OS_CONFIG
    BOARD_INCLUDES += $(USER_INC)

# C defines
C_DEFS =  \
    -D STM32F7 \
    -D USE_HAL_DRIVER \
    -D USE_STM32F769I_DISCO \
    -D USE_STM32769I_DISCOVERY \
    -D MBEDTLS_CONFIG_FILE=\"mbedtls_config.h\" \
    -D HAVE_STDINT_H

ifeq ($(WITH_FILESYSTEM), yes)
        C_DEFS += -DFS_$(FILESYSTEM_TYPE)
endif

###########################################add support for lvgl########################################################
ifeq ($(LOSCFG_GUI_ENABLE), y)
LITEGL_INCLUDE += $(COMPONENTS_INCLUDE)
LITEGL_INCLUDE += $(DEMOS_INCLUDE)
LVGl_GUI_INC = \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers
    LITEGL_INCLUDE += $(LVGl_GUI_INC)

LVGL_DRIVER_SRC =  \
    ${wildcard $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/tft/*.c} \
    ${wildcard $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/touchpad/*.c}
    C_SOURCES += $(LVGL_DRIVER_SRC)

LITEGL_LIB = -Xlinker -no-enum-size-warning

BOARD_INCLUDES += $(LITEGL_INCLUDE) $(LIBSEC_INCLUDE) $(HAL_DRIVER_INC) $(USER_INC) $(CMSIS_INC) $(KERNEL_INC) $(ARCH_INC)
C_DEFS += -DLV_CONF_INCLUDE_SIMPLE
endif
#######################################################################################################################

ASM_SOURCES =  \
    ${wildcard $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/los_startup_gcc.S}

BOARD_SRCS += $(ASM_SOURCES)
BOARD_SRCS += $(C_SOURCES)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(BOARD_SRCS))

BOARD_DEF += $(C_DEFS)
LOCAL_INCLUDE += $(BOARD_INCLUDES)

LOCAL_CFLAGS += -Wno-unused-function

LOCAL_FLAGS := $(BOARD_DEF) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)
include $(MODULE)
