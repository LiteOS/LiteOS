################################################################################
# this is used for compile the iotlink
################################################################################

#now support "liteos" and "linux" "macos"

ifeq ($(CONFIG_OS_TYPE), "liteos")
	include $(iot_link_root)/os/os_imp/liteos/liteos_imp.mk
else ifeq ($(CONFIG_OS_TYPE), "linux")
	include $(iot_link_root)/os/os_imp/linux/linux_imp.mk
else ifeq ($(CONFIG_OS_TYPE), "macos")
	include $(iot_link_root)/os/os_imp/macos/macos_imp.mk	
else
	
endif





