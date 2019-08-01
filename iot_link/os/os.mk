################################################################################
# this is used for compile the iotlink
################################################################################

osal_src  = ${wildcard $(iot_link_root)/os/osal/*.c}
C_SOURCES += $(osal_src)
		
osal_inc = -I $(iot_link_root)/os/osal
C_INCLUDES += $(osal_inc)

#add the implement of the os
include $(iot_link_root)/os/os_imp/os_imp.mk


