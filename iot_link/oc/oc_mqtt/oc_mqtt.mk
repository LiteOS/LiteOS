################################################################################
# this is used for compile the OC_MQTT
# please add the corresponding file to C_SOURCES C_INCLUDES C_DEFS
################################################################################
#NEXT TIME WE SHOULD MOVE THE JSON OUT

ifneq ($(CONFIG_OC_MQTT_TYPE),"none")

    OC_MQTT_AL_SRC  = ${wildcard $(iot_link_root)/oc/oc_mqtt/oc_mqtt_al/*.c}
    C_SOURCES += $(OC_MQTT_AL_SRC)	
    
    OC_MQTT_AL_INC = -I $(iot_link_root)/oc/oc_mqtt/oc_mqtt_al
    C_INCLUDES += $(OC_MQTT_AL_INC)	
    
    oc_mqtt_defs = -D CONFIG_OC_MQTT_ENABLE=1
    C_DEFS += $(oc_mqtt_defs)
    
    
    #you must choose one of the oc mqtt implement
    include $(iot_link_root)/oc/oc_mqtt/oc_mqtt_imp/oc_mqtt_imp.mk
    
    
    #with mqtt demo
    include $(SDK_DIR)/demos/oc_mqtt_demo/oc_mqtt_demo.mk

endif