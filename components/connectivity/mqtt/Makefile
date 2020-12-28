include $(LITEOSTOPDIR)/config.mk

include $(LITEOSTOPDIR)/components/net/lwip/lwip.mk
include $(LITEOSTOPDIR)/components/security/security.mk

MODULE_NAME := $(notdir $(CURDIR))

ATINY_MQTT_SRCS := \
    $(wildcard  MQTTClient-C/src/*.c) \
    $(wildcard  MQTTClient-C/src/liteOS/*.c) \
    $(wildcard  MQTTPacket/src/*.c)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(ATINY_MQTT_SRCS))

ATINY_MQTT_INC := \
    -I  MQTTClient-C/src \
    -I  MQTTClient-C/src/liteOS \
    -I  MQTTPacket/src \
    -I  $(LITEOSTOPDIR)/include

ATINY_MQTT_INC += $(LWIP_INCLUDE)

ATINY_MQTT_INC += $(COMPONENTS_SECURITY_INCLUDE)

LOCAL_INCLUDE := $(ATINY_MQTT_INC)

LOCAL_FLAGS :=  $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
