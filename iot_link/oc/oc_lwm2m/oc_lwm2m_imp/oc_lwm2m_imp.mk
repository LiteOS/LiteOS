
#OC_LWM2M_IMP_DEMO is for the demo; and OC_LWM2M_IMP_AGENT
				
ifeq ($(CONFIG_OC_LWM2M_TYPE),"demo")
	include $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m_imp/oc_lwm2m_imp_demo/oc_lwm2m_impdemo.mk
else ifeq ($(CONFIG_OC_LWM2M_TYPE),"soft")
	include $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m_imp/atiny_lwm2m/atiny_lwm2m.mk
else ifeq ($(CONFIG_OC_LWM2M_TYPE),"boudica150")
	include $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m_imp/boudica150/boudica150_oc.mk
else
        
endif
