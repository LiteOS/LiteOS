################################################################################
# this is used for compile the soft timer module
# make sure that this module depends on the osal module
# make sure the osal module is enabled and some os is selected for the link
################################################################################

ifeq ($(CONFIG_STIMER_ENABLE),y)

    STIMER_MODULE_SRC  = ${wildcard $(iot_link_root)/stimer/*.c} 
    C_SOURCES += $(STIMER_MODULE_SRC)	
    		
    CJSON_MODULE_INC = -I $(iot_link_root)/stimer
    C_INCLUDES += $(CJSON_MODULE_INC)
    
    C_DEFS += -D CONFIG_STIMER_ENABLE=1

endif


