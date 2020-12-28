include $(LITEOSTOPDIR)/config.mk

MODULE_NAME   := gui
BOARD_INCLUDES :=


LVGL_SRC = \
    $(wildcard lvgl/src/lv_core/*.c) \
    $(wildcard lvgl/src/lv_draw/*.c) \
    $(wildcard lvgl/src/lv_font/*.c) \
    $(wildcard lvgl/src/lv_gpu/*.c) \
    $(wildcard lvgl/src/lv_hal/*.c) \
    $(wildcard lvgl/src/lv_misc/*.c) \
    $(wildcard lvgl/src/lv_themes/*.c) \
    $(wildcard lvgl/src/lv_widgets/*.c)

LVGL_INC = \
    -I $(LITEOSTOPDIR)components/gui/lvgl/src/lv_core/ \
    -I $(LITEOSTOPDIR)components/gui/lvgl/src/lv_draw/ \
    -I $(LITEOSTOPDIR)components/gui/lvgl/src/lv_font/ \
    -I $(LITEOSTOPDIR)components/gui/lvgl/src/lv_gpu/ \
    -I $(LITEOSTOPDIR)components/gui/lvgl/src/lv_hal/ \
    -I $(LITEOSTOPDIR)components/gui/lvgl/src/lv_misc/ \
    -I $(LITEOSTOPDIR)components/gui/lvgl/src/lv_themes/ \
    -I $(LITEOSTOPDIR)components/gui/lvgl/src/lv_widgets/


LITEGL_INCLUDE = $(LVGL_INC)

C_SOURCES += $(LVGL_SRC)
BOARD_INCLUDES += $(LITEGL_INCLUDE)

C_DEFS := -D LV_CONF_INCLUDE_SIMPLE
ifeq ($(LOSCFG_PLATFORM_STM32F769IDISCOVERY), y)
C_DEFS += -D STM32F7
BOARD_INCLUDES += -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F7xx_HAL_Driver/Inc
endif

BOARD_SRCS += $(C_SOURCES)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(BOARD_SRCS))

BOARD_DEF += $(C_DEFS)
LOCAL_INCLUDE += $(BOARD_INCLUDES)

LOCAL_CFLAGS += -Wno-unused-value

LOCAL_FLAGS := $(BOARD_DEF) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
