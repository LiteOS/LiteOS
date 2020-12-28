DEMOS_INCLUDE :=

ifeq ($(LOSCFG_DEMOS_FS), y)
include $(LITEOSTOPDIR)/demos/fs/fs.mk
DEMOS_INCLUDE += $(DEMOS_FS_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_GUI), y)
include $(LITEOSTOPDIR)/demos/gui/gui.mk
DEMOS_INCLUDE += $(DEMOS_GUI_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_SENSORHUB), y)
include $(LITEOSTOPDIR)/demos/sensorhub/sensorhub.mk
DEMOS_INCLUDE += $(DEMOS_SENSORHUB_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_KERNEL), y)
include $(LITEOSTOPDIR)/demos/kernel/kernel.mk
DEMOS_INCLUDE += $(DEMOS_KERNEL_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_AGENT_TINY_MQTT), y)
include $(LITEOSTOPDIR)/demos/agenttiny_mqtt/agenttiny_mqtt.mk
DEMOS_INCLUDE += $(DEMOS_AGENTTINY_MQTT_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_AGENT_TINY_LWM2M), y)
include $(LITEOSTOPDIR)/demos/agenttiny_lwm2m/agenttiny_lwm2m.mk
DEMOS_INCLUDE += $(DEMOS_AGENTTINY_LWM2M_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_DTLS_SERVER), y)
include $(LITEOSTOPDIR)/demos/dtls_server/dtls_server.mk
DEMOS_INCLUDE += $(DEMOS_DTLS_SERVER_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY), y)
include $(LITEOSTOPDIR)/demos/nbiot_without_atiny/nbiot.mk
DEMOS_INCLUDE += $(DEMOS_NBIOT_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_LMS), y)
include $(LITEOSTOPDIR)/demos/lms/lms.mk
DEMOS_INCLUDE += $(DEMOS_LMS_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_TRACE), y)
include $(LITEOSTOPDIR)/demos/trace/trace.mk
DEMOS_INCLUDE += $(DEMOS_TRACE_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_IPV6_CLIENT), y)
include $(LITEOSTOPDIR)/demos/ipv6_client/ipv6_client.mk
DEMOS_INCLUDE += $(DEMOS_IPV6_CLIENT_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_AI), y)
include $(LITEOSTOPDIR)/demos/ai/ai.mk
DEMOS_INCLUDE += $(DEMOS_AI_INCLUDE)
endif
