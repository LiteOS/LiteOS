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