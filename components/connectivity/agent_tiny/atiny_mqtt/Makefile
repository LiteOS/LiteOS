include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/security/security.mk
include $(LITEOSTOPDIR)/components/lib/cjson/cjson.mk
include $(LITEOSTOPDIR)/components/connectivity/mqtt/mqtt.mk

MODULE_NAME := $(notdir $(CURDIR))

AGENT_TINY_SRCS := \
    $(wildcard *.c)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(AGENT_TINY_SRCS))

AGENT_TINY_INC := \
    -I $(LITEOSTOPDIR)/components/components/connectivity/agent_tiny/atiny_mqtt \
    -I $(LITEOSTOPDIR)/components/ota/flag_operate\
    -I $(LITEOSTOPDIR)/include

AGENT_TINY_INC += \
    $(COMPONENTS_MQTT_INCLUDE) \
    $(COMPONENTS_CJSON_INCLUDE) \
    $(COMPONENTS_SECURITY_INCLUDE)

LOCAL_INCLUDE := $(AGENT_TINY_INC)

LOCAL_EXT_FLAG := -Wno-unused-variable

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
