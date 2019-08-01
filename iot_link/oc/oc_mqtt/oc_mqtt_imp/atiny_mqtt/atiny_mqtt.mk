################################################################################
# this is used for compile the oc mqtt implement
# please add the corresponding file to C_SOURCES C_INCLUDES C_DEFS
################################################################################

#uptils now, we only support the atiny_mqtt mode, you could add more implement as you wish

atiny_mqtt_src = ${wildcard $(iot_link_root)/oc/oc_mqtt/oc_mqtt_imp/atiny_mqtt/*.c}
C_SOURCES += $(atiny_mqtt_src)

atiny_mqtt_inc = -I $(iot_link_root)/oc/oc_mqtt/oc_mqtt_imp/atiny_mqtt
C_INCLUDES += $(atiny_mqtt_inc)

atiny_mqtt_defs = -D CONFIG_ATINY_MQTT_ENABLE=1    
C_DEFS += $(atiny_mqtt_defs)

LDFLAGS += -u _printf_float
