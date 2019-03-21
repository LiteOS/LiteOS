################################################################################
# this is used for compile the atiny_mqtt
# please add the corresponding file to C_SOURCES C_INCLUDES C_DEFS
################################################################################
#NEXT TIME WE SHOULD MOVE THE JSON OUT
ATINY_MQTT_SRC  = \
		${wildcard $(TOP_DIR)/components/connectivity/agent_tiny/atiny_mqtt/*.c} \
		${wildcard $(TOP_DIR)/components/lib/cJSON/cJSON.c}
C_SOURCES += $(ATINY_MQTT_SRC)	

ATINY_MQTT_INC = \
	    -I $(TOP_DIR)/components/connectivity/agent_tiny/atiny_mqtt \
	    -I $(TOP_DIR)/components/lib/cJSON 

C_INCLUDES += $(ATINY_MQTT_INC)	

