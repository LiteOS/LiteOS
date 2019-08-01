################################################################################
# this is used for compile the mqtt lib
################################################################################

mqtt_paho_src = \
		${wildcard $(TOP_DIR)/iot_link/network/mqtt/mqtt_imp/mqtt_paho/mqtt_port/*.c} \
        ${wildcard $(TOP_DIR)/iot_link/network/mqtt/mqtt_imp/mqtt_paho/mqtt_paho/MQTTPacket/src/*.c} \
        $(TOP_DIR)/iot_link/network/mqtt/mqtt_imp/mqtt_paho/mqtt_paho/MQTTClient-C/src/MQTTClient.c
        C_SOURCES += $(mqtt_paho_src)
        
mqtt_paho_inc = \
        -I $(TOP_DIR)/iot_link/network/mqtt/mqtt_imp/mqtt_paho/mqtt_paho/MQTTClient-C/src \
		-I $(TOP_DIR)/iot_link/network/mqtt/mqtt_imp/mqtt_paho/mqtt_paho/MQTTPacket/src \
		-I $(TOP_DIR)/iot_link/network/mqtt/mqtt_imp/mqtt_paho/mqtt_port 

C_INCLUDES += $(mqtt_paho_inc)

mqtt_paho_defs = -D MQTTCLIENT_PLATFORM_HEADER=paho_osdepends.h
C_DEFS += $(mqtt_paho_defs)
