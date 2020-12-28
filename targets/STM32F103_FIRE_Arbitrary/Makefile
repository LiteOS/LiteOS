include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LITEOS_PLATFORM)

ALL_ASSRCS := $(wildcard board/$(LITEOS_PLATFORM)/*.S)
ASSRCS := $(subst board/$(LITEOS_PLATFORM)/board.ld.S,,$(ALL_ASSRCS))

LOCAL_SRCS += $(ASSRCS)

USER_SRC =  \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/os_adapt/os_adapt.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/main.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/system_stm32f1xx.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/usart.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/sys_init.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/tim.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/user_task.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/gpio.c
    C_SOURCES += $(USER_SRC)

# C includes
HAL_DRIVER_INC = \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F1xx_HAL_Driver/Inc \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F1xx_HAL_Driver/Inc/Legacy
    BOARD_INCLUDES += $(HAL_DRIVER_INC)

INCLUDE_INC = \
    -I $(LITEOSTOPDIR)/include
    BOARD_INCLUDES += $(INCLUDE_INC)

BSP_INC = \
    -I $(LITEOSTOPDIR)/targets/bsp/common \
    -I $(LITEOSTOPDIR)/targets/bsp/include \
    -I $(LITEOSTOPDIR)/targets/bsp/hw/include \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/st_timer \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include \
    -I $(LITEOSTOPDIR)/kernel/extended/include
    BOARD_INCLUDES += $(BSP_INC)

USER_INC = \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Inc
    BOARD_INCLUDES += $(USER_INC)

# C defines
C_DEFS +=  \
    -DUSE_HAL_DRIVER \
    -DSTM32F103xE \
    -DNDEBUG \
    -D__LITEOS__ \
    -D_ALL_SOURCE

ASM_SOURCES =  \
        ${wildcard $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/los_startup_gcc.S}

BOARD_SRCS += $(ASM_SOURCES)
BOARD_SRCS += $(C_SOURCES)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(BOARD_SRCS))

BOARD_DEF += $(C_DEFS)
LOCAL_INCLUDE += $(BOARD_INCLUDES)

LOCAL_FLAGS := $(LITEOS_CFLAGS_INTERWORK) $(BOARD_DEF) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
