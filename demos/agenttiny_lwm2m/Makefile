include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/lib/cjson/cjson.mk
include $(LITEOSTOPDIR)/components/connectivity/lwm2m/lwm2m.mk

MODULE_NAME := $(notdir $(CURDIR))

LWM2M_DEMO_SRC = \
    $(wildcard *.c)

LWM2M_DEMO_INCLUDE = \
    -I$(LITEOSTIPDIR)/components/connectivity/lwm2m/core \
    -I $(LITEOSTOPDIR)/demos/agenttiny_lwm2m \
    -I $(LITEOSTOPDIR)/kernel/include \
    -I $(LITEOSTOPDIR)/kernel/base/include \
    -I $(LITEOSTOPDIR)/include/osdepends \
    -I $(LITEOSTOPDIR)/include/log \
    -I $(LITEOSTOPDIR)/include

LWM2M_DEMO_INCLUDE += $(COMPONENTS_LWM2M_INCLUDE)

LWM2M_DEMO_INCLUDE += $(COMPONENTS_CJSON_INCLUDE)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(LWM2M_DEMO_SRC))
LOCAL_INCLUDE := $(LWM2M_DEMO_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
