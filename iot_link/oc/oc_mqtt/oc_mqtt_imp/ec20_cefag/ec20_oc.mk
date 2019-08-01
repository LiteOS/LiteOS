EC20_SOURCE = \
                ${wildcard $(iot_link_root)/oc/oc_mqtt/oc_mqtt_imp/ec20_cefag/*.c}
C_SOURCES += $(EC20_SOURCE)

EC20_INC = \
                -I $(iot_link_root)/oc/oc_mqtt/oc_mqtt_imp/ec20_cefag

C_INCLUDES += $(EC20_INC)
C_DEFS += -D CONFIG_OC_MQTT_EC20_ENABLE=1

