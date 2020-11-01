COMPONENTS_INCLUDE :=

ifeq ($(LOSCFG_COMPONENTS_CONNECTIVITY), y)
include $(LITEOSTOPDIR)/components/connectivity/connectivity.mk
COMPONENTS_INCLUDE += $()
endif

ifeq ($(LOSCFG_COMPONENTS_FS), y)
include $(LITEOSTOPDIR)/components/fs/fs.mk
COMPONENTS_INCLUDE += $(COMPONENTS_FS_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_GUI), y)
include $(LITEOSTOPDIR)/components/gui/gui.mk
COMPONENTS_INCLUDE += $(COMPONENTS_GUI_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_SENSORHUB), y)
include $(LITEOSTOPDIR)/components/sensorhub/sensorhub.mk
COMPONENTS_INCLUDE += $(COMPONENTS_SENSORHUB_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_NETWORK), y)
include $(LITEOSTOPDIR)/components/net/lwip/lwip.mk
COMPONENTS_INCLUDE += $(LWIP_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_SECURITY), y)
include $(LITEOSTOPDIR)/components/security/security.mk
COMPONENTS_INCLUDE += $(COMPONENTS_SECURITY_INCLUDE)
endif