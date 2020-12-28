include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := sensorhub_demo

LOCAL_SRCS   :=
LOCAL_SRCS_y :=

LOCAL_SRCS_$(LOSCFG_DEMOS_SENSORHUB) += gyro/src/gyro.c gyro/src/sensorhub_demo.c

LOCAL_SRCS += $(LOCAL_SRCS_y)

ifeq ($(LOSCFG_DEMOS_SENSORHUB), y)
LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/sensorhub/gyro/include \
    -I $(LITEOSTOPDIR)/components/sensorhub/include/app \
    -I $(LITEOSTOPDIR)/components/sensorhub/include/arch \
    -I $(LITEOSTOPDIR)/components/sensorhub/include/common
endif

ifeq ($(LOSCFG_PLATFORM_STM32F429IGTX), y)
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/i2c \
    -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/mpu6050 \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F4xx_HAL_Driver/Inc
endif

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
