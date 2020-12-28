include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/lib/cjson/cjson.mk

MODULE_NAME := $(notdir $(CURDIR))

MQTT_DEMO_SRC = \
    $(wildcard *.c)

MQTT_DEMO_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/agenttiny_mqtt \
    -I $(LITEOSTOPDIR)/kernel/include \
    -I $(LITEOSTOPDIR)/kernel/base/include \
    -I $(LITEOSTOPDIR)/include/osdepends \
    -I $(LITEOSTOPDIR)/include

MQTT_DEMO_INCLUDE += $(COMPONENTS_CJSON_INCLUDE)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(MQTT_DEMO_SRC))

LOCAL_INCLUDE := $(MQTT_DEMO_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)