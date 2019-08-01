################################################################################
# this is used for compile the at modules
# make sure that this module depends on the driver module
# make sure driver module is enabled
################################################################################

ifeq ($(CONFIG_DRIVER_ENABLE),y)
    DRIVER_MODULE_SRC  = ${wildcard $(iot_link_root)/driver/driver.c}    #n fs related source here
    C_SOURCES += $(DRIVER_MODULE_SRC)	
		
    DRIVER_MODULE_INC = -I $(iot_link_root)/driver
    C_INCLUDES += $(DRIVER_MODULE_INC)
    
    DRIVER_MODULE_DEF = -D CONFIG_DRIVER_ENABLE=1
    C_DEFS += $(DRIVER_MODULE_DEF)
    
endif


