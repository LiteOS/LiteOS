include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := src/sensor_manager/sensor_commu.c src/sensor_manager/sensor_manager.c

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/sensorhub/include/app \
    -I $(LITEOSTOPDIR)/components/sensorhub/include/arch \
    -I $(LITEOSTOPDIR)/components/sensorhub/include/common

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)