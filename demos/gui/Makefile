include $(LITEOSTOPDIR)/config.mk
MODULE_NAME := gui_demo

LVGL_DIR = $(LITEOSTOPDIR)/components/gui/lvgl
LVGL_DEMO_DIR = $(LITEOSTOPDIR)/demos/gui

BOARD_INCLUDES += \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F7xx_HAL_Driver/Inc \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers

LVGL_DEMO_INCLUDE := \
    -I$(LVGL_DEMO_DIR) \
    -I$(LVGL_DEMO_DIR)/widgets \
    -I$(LVGL_DIR)
BOARD_INCLUDES += $(LVGL_DEMO_INCLUDE)

# C includes
HAL_DRIVER_INC := \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/STM32F769I-Discovery \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F7xx_HAL_Driver/Inc \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/STM32F769I-Discovery/Utilities/Fonts \
    -I $(LITEOSTOPDIR)/compat/cmsis
BOARD_INCLUDES += $(HAL_DRIVER_INC)

LVGL_DEMO_SRC = \
    $(wildcard $(LVGL_DEMO_DIR)/*.c) \
    $(wildcard $(LVGL_DEMO_DIR)/widgets/*.c)

C_SOURCES += $(LVGL_DEMO_SRC)

BOARD_SRCS += $(C_SOURCES)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(BOARD_SRCS))
C_DEFS = \
    -D STM32F7 \
    -D STM32F769xx \
    -D LV_CONF_INCLUDE_SIMPLE

BOARD_DEF += $(C_DEFS)

LOCAL_INCLUDE += $(BOARD_INCLUDES)
LOCAL_FLAGS := $(BOARD_DEF) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
