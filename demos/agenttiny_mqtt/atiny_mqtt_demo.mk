################################################################################
# this is used for compile the atiny_mqtt_demo
################################################################################
ATINY_MQTT_DEMO_SRC  = \
		${wildcard $(TOP_DIR)/demos/agenttiny_mqtt/*.c} 
C_SOURCES += $(ATINY_MQTT_DEMO_SRC)


ATINY_MQTT_DEMO_INC = \
	    -I $(TOP_DIR)/demos/agenttiny_mqtt 

C_INCLUDES += $(ATINY_MQTT_DEMO_INC)		
		