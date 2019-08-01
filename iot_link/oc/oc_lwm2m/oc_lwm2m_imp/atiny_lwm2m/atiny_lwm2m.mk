OC_LWM2M_AGENT_SRC = \
        ${wildcard $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m_imp/atiny_lwm2m/*.c}

ifeq ($(USE_FOTA), y)
        C_SOURCES += $(OC_LWM2M_AGENT_SRC)
else
        OC_LWM2M_AGENT_SRC_NO_FOTA = \
        $(filter-out $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m_imp/atiny_lwm2m/atiny_fota_manager.c \
        $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m_imp/atiny_lwm2m/atiny_fota_state.c \
        $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m_imp/atiny_lwm2m/firmware_update.c, $(OC_LWM2M_AGENT_SRC))
        C_SOURCES += $(OC_LWM2M_AGENT_SRC_NO_FOTA)
endif

OC_LWM2M_ATINY_INC = -I $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m_imp/atiny_lwm2m        
C_INCLUDES += $(OC_LWM2M_ATINY_INC)
C_DEFS += -D CONFIG_OC_LWM2M_AGENT_ENABLE=1