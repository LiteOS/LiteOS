################################################################################
# this is used for compile the oc_lwm2m_impdemo, if you want to make the oc_lwm2m
# to the framework, you could refer this one
################################################################################

OC_LWM2M_IMPDEMO_SOURCE  = ${wildcard $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m_imp/oc_lwm2m_imp_demo/*.c} 
C_SOURCES += $(OC_LWM2M_IMPDEMO_SOURCE)	
		
OC_LWM2M_IMPDEMO_INC = -I $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m_imp/oc_lwm2m_imp_demo
C_INCLUDES += $(OC_LWM2M_IMPDEMO_INC)


