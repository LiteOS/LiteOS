################################################################################
# this is used for compile the iotlink
################################################################################

LINK_MISC_SOURCE  = ${wildcard $(iot_link_root)/link_misc/*.c} 
C_SOURCES += $(LINK_MISC_SOURCE)	
		
LINK_MISC_INC = -I $(iot_link_root)/link_misc
C_INCLUDES += $(LINK_MISC_INC)


