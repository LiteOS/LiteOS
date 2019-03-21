################################################################################
# this is used for compile the mqtt lib
################################################################################

##uptils now , we only support the paho mqtt
WITH_PAHO_MQTT = yes

MQTT_AL_SOURCE  = \
		${wildcard $(TOP_DIR)/components/mqtt/mqtt_al/*.c} 
C_SOURCES += $(MQTT_AL_SOURCE)	
		
MQTT_AL_INC = \
	    -I $(TOP_DIR)/components/mqtt/mqtt_al 

C_INCLUDES += $(MQTT_AL_INC)

C_DEFS += -DWITH_MQTT
		
		
ifeq ($(WITH_PAHO_MQTT), yes)

	MQTT_PAHO_SOURCE = \
    		${wildcard $(TOP_DIR)/components/mqtt/mqtt_paho/mqtt_port/*.c} \
            ${wildcard $(TOP_DIR)/components/mqtt/mqtt_paho/mqtt_paho/MQTTPacket/src/*.c} \
            $(TOP_DIR)/components/mqtt/mqtt_paho/mqtt_paho/MQTTClient-C/src/MQTTClient.c
            C_SOURCES += $(MQTT_PAHO_SOURCE)
            
    MQTT_PAHO_INC = \
            -I $(TOP_DIR)/components/mqtt/mqtt_paho/mqtt_paho/MQTTClient-C/src \
    		-I $(TOP_DIR)/components/mqtt/mqtt_paho/mqtt_paho/MQTTPacket/src \
    		-I $(TOP_DIR)/components/mqtt/mqtt_paho/mqtt_port 
    
    C_INCLUDES += $(MQTT_PAHO_INC)
    
    MQTT_PAHO_DEF = \
     		-D MQTTCLIENT_PLATFORM_HEADER=mqtt_osdep.h
    
    		C_DEFS += $(MQTT_PAHO_DEF)
 	        
endif