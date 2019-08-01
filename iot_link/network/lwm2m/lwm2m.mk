################################################################################
# this is used for compile the dtls
################################################################################
ifneq ($(CONFIG_LWM2M_TYPE),"none")

    LWM2M_MODULE_SOURCE  = ${wildcard $(iot_link_root)/network/lwm2m/lwm2m_al/*.c}
    C_SOURCES += $(LWM2M_MODULE_SOURCE)
    		
    LWM2M_MODULE_INC = -I $(iot_link_root)/network/lwm2m/lwm2m_al
    C_INCLUDES += $(LWM2M_MODULE_INC)
    
    #you should configure the lwm2m implement method
    include $(iot_link_root)/network/lwm2m/lwm2m_imp/lwm2m_imp.mk

endif
