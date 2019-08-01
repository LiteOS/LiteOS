################################################################################
# this is used for compile the at modules
# make sure that this module depends on the driver module
# make sure driver module is enabled
################################################################################

ifeq ($(CONFIG_CJSON_ENABLE),y)

    CJSON_MODULE_SRC  = ${wildcard $(iot_link_root)/cJSON/*.c} 
    C_SOURCES += $(CJSON_MODULE_SRC)	
    		
    CJSON_MODULE_INC = -I $(iot_link_root)/cJSON
    C_INCLUDES += $(CJSON_MODULE_INC)
    
    C_DEFS += -D CONFIG_JSON_ENABLE=1

endif


