include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/security/security.mk
include $(LITEOSTOPDIR)/components/connectivity/lwm2m/lwm2m.mk

MODULE_NAME := $(notdir $(CURDIR))

ATINY_LWM2M_SRCS := \
    $(wildcard *.c)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(ATINY_LWM2M_SRCS))

ATINY_LWM2M_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/connectivity/agent_tiny/atiny_lwm2m \
    -I $(LITEOSTOPDIR)/components/ota/flag_operate \
    -I $(LITEOSTOPDIR)/include

ATINY_LWM2M_INCLUDE += \
    $(COMPONENTS_LWM2M_INCLUDE) \
    $(COMPONENTS_SECURITY_INCLUDE)

LOCAL_INCLUDE := $(ATINY_LWM2M_INCLUDE)

LOCAL_FLAGS :=  $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) $(COMPONENTS_LWM2M_ADD_DEFS)

include $(MODULE)

