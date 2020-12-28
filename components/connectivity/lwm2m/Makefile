include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/connectivity/lwm2m/add_definitions.mk

MODULE_NAME := $(notdir $(CURDIR))

LWM2M_ADD_DEFS :=

LWM2M_SRC = \
    $(wildcard core/*.c) \
    $(wildcard core/er-coap-13/*.c) \
    $(wildcard examples/shared/*.c)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(LWM2M_SRC))

LWM2M_INC = \
    -I $(LITEOSTOPDIR)/components/connectivity/agent_tiny/atiny_lwm2m \
    -I core \
    -I examples/shared \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE := $(LWM2M_INC)

LWM2M_ADD_DEFS += -DLWM2M_BIG_ENDIAN

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) $(LWM2M_ADD_DEFS) $(ADD_DEFS)

include $(MODULE)
