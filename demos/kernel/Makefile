include $(LITEOSTOPDIR)/config.mk
MODULE_NAME := kernel_demo

KERNEL_DEMO_SRC_y :=

BOARD_INCLUDES += -I $(LITEOSTOPDIR)/demos/kernel/include
KERNEL_DEMO_SRC_y += $(wildcard api/*.c)

HAL_DRIVER_INC := \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F4xx_HAL_Driver/Inc \
    -I $(LITEOSTOPDIR)/compat/cmsis
BOARD_INCLUDES += $(HAL_DRIVER_INC)

USER_INCLUDE += \
    -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Inc \
    -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Hardware/Inc
BOARD_INCLUDES += $(USER_INCLUDE)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(KERNEL_DEMO_SRC_y))

LOCAL_INCLUDE += $(BOARD_INCLUDES)
LOCAL_FLAGS := $(BOARD_DEF) $(LOCAL_INCLUDE)

include $(MODULE)
