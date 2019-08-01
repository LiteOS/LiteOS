################################################################################
# this is used for compile the mqtt lib
################################################################################
#you should choose one mqtt implement, uptils now, we  only adapter the "paho" mqtt				
ifeq ($(CONFIG_MQTT_TYPE), "paho")

	include $(iot_link_root)/network/mqtt/mqtt_imp/mqtt_paho/mqtt_paho.mk
 	        
endif