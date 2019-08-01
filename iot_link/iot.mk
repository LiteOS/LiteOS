################################################################################
# this is used for compile the iotlink
################################################################################

iot_link_root = $(SDK_DIR)/iot_link

#configure the common includes
include $(iot_link_root)/inc/inc.mk

#configure the misc
include $(iot_link_root)/link_misc/link_misc.mk

#configure the log
include $(iot_link_root)/link_log/link_log.mk

#configure the operation system for the iot_link
include $(iot_link_root)/os/os.mk

#configure the shell for the iot_link
include $(iot_link_root)/shell/shell.mk

#configure the soft timer for the iot_link
include $(iot_link_root)/stimer/stimer.mk

#configure the driver famework for the iot_link
include $(iot_link_root)/driver/driver.mk

#configure the AT famework for the iot_link
include $(iot_link_root)/at/at.mk

#configure the libc
include $(iot_link_root)/libc/libc.mk

#configure the cJSON 
include $(iot_link_root)/cJSON/cJSON.mk 

#configure the sal for the iot_link
include $(iot_link_root)/network/tcpip/tcpip.mk

#configure the dtls for the iot link
include $(iot_link_root)/network/dtls/dtls.mk

#configure the mqtt
include $(iot_link_root)/network/mqtt/mqtt.mk
include $(iot_link_root)/oc/oc_mqtt/oc_mqtt.mk

#configure the lwm2m
include $(iot_link_root)/network/lwm2m/lwm2m.mk
include $(iot_link_root)/oc/oc_lwm2m/oc_lwm2m.mk

#configure the loader	
include $(iot_link_root)/loader/loader.mk

iot_link_src  = ${wildcard $(iot_link_root)/*.c} 
C_SOURCES += $(iot_link_src)	
		
iot_link_inc = -I $(iot_link_root)
C_INCLUDES += $(iot_link_inc)

