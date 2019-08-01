################################################################################
# this is used for compile the shell module
################################################################################

ifeq ($(CONFIG_SHELL_ENABLE),y)
    shell_src  = ${wildcard $(iot_link_root)/shell/*.c} 
    C_SOURCES += $(shell_src)	
    		
    shell_inc = -I $(iot_link_root)/shell
    C_INCLUDES += $(shell_inc)
    
    C_DEFS += -D CONFIG_SHELL_ENABLE=1
endif

