################################################################################
# this is used for compile the iotlink
################################################################################

ifneq ($(CONFIG_TCPIP_TYPE),"none")

    SAL_SOURCE  = ${wildcard $(iot_link_root)/network/tcpip/sal/*.c}
    C_SOURCES += $(SAL_SOURCE)
    		
    SAL_INC = -I $(iot_link_root)/network/tcpip/sal
    C_INCLUDES += $(SAL_INC)
    
    #now suport "lwip" and "linux"  "macos" socket
    
    ifeq ($(CONFIG_TCPIP_TYPE), "lwip")
    	include $(iot_link_root)/network/tcpip/lwip/lwip_imp.mk
    else ifeq ($(CONFIG_TCPIP_TYPE), "linux_socket")
    	include $(iot_link_root)/network/tcpip/linux_socket/linux_socket_imp.mk
    else ifeq ($(CONFIG_TCPIP_TYPE), "macos_socket")
    	include $(iot_link_root)/network/tcpip/macos_socket/macos_socket_imp.mk
    else ifeq ($(CONFIG_TCPIP_TYPE), "at_esp8266")
    	include $(iot_link_root)/network/tcpip/at_device/wifi_esp8266/esp8266_imp.mk	
    else
    	
    endif

    C_DEFS += -D CONFIG_TCPIP_ENABLE=1
    
endif
