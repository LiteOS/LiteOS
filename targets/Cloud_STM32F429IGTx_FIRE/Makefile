include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LITEOS_PLATFORM)

-include $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/config.mk

ALL_ASSRCS := $(wildcard board/$(LITEOS_PLATFORM)/*.S)
ASSRCS := $(subst board/$(LITEOS_PLATFORM)/board.ld.S,,$(ALL_ASSRCS))

LOCAL_SRCS += $(ASSRCS)

HARDWARE_SRC =  \
    ${wildcard Hardware/Src/*.c}
    C_SOURCES += $(HARDWARE_SRC)

USER_SRC =  \
    os_adapt/os_adapt.c \
    Src/main.c \
    Src/system_stm32f4xx.c \
    Src/usart.c \
    Src/eth.c \
    Src/sys_init.c \
    Src/flash_adaptor.c \
    Src/user_task.c \
    Src/tim.c
    C_SOURCES += $(USER_SRC)

ifeq ($(LOSCFG_COMPONENTS_FS), y)
    ifeq ($(LOSCFG_FATFS_USE_SD_CARD), y)
        FS_SRC += \
            Src/fatfs_sd_hal.c \
            Src/stm324x9i_eval_sd.c \
            Src/sd_diskio.c
    endif
    ifeq ($(LOSCFG_FATFS_USE_SPI_FLASH), y)
        FS_SRC += \
            Src/fatfs_spi_hal.c
    endif
    ifeq ($(LOSCFG_COMPONENTS_FS_SPIFFS), y)
        FS_SRC += \
            Src/spiffs_hal.c
    endif
    C_SOURCES += $(FS_SRC)
endif

ifeq ($(LOSCFG_DEMOS_SENSORHUB), y)
BSP_DRIVER_SRC = \
    Drivers/i2c/i2c.c \
    Drivers/mpu6050/mpu6050.c
    C_SOURCES += $(BSP_DRIVER_SRC)
endif

# C includes
HAL_DRIVER_INC = \
   -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F4xx_HAL_Driver/Inc \
   -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    BOARD_INCLUDES += $(HAL_DRIVER_INC)

INCLUDE_INC = \
    -I $(LITEOSTOPDIR)/include
    BOARD_INCLUDES += $(INCLUDE_INC)

BSP_INC = \
    -I $(LITEOSTOPDIR)/targets/bsp/common \
    -I $(LITEOSTOPDIR)/targets/bsp/include \
    -I $(LITEOSTOPDIR)/targets/bsp/hw/include \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/st_timer \
    -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/include \
    -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/BSP/STM324x9I_EVAL \
    -I $(LITEOSTOPDIR)/kernel/extended/include \
    -I $(LITEOSTOPDIR)/kernel/include
    BOARD_INCLUDES += $(BSP_INC)

HARDWARE_INC = \
    -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Hardware/Inc
    BOARD_INCLUDES += $(HARDWARE_INC)

USER_INC = \
    -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Inc
    BOARD_INCLUDES += $(USER_INC)

BSP_DRIVER_INC = \
    -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/i2c \
    -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/mpu6050
    BOARD_INCLUDES += $(BSP_DRIVER_INC)

# C defines
C_DEFS +=  \
    -DUSE_HAL_DRIVER \
    -DSTM32F429xx \
    -DNDEBUG \
    -D__LITEOS__ \
    -D_ALL_SOURCE

ASM_SOURCES =  \
    ${wildcard los_startup_gcc.S}

BOARD_SRCS += $(ASM_SOURCES)
BOARD_SRCS += $(C_SOURCES)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(BOARD_SRCS))

BOARD_DEF += $(C_DEFS)
LOCAL_INCLUDE += $(BOARD_INCLUDES)

LOCAL_FLAGS := $(LITEOS_CFLAGS_INTERWORK) $(BOARD_DEF) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
