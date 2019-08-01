################################################################################
# this is used for compile the loader
################################################################################
#make sure that you need the loader

ifeq ($(CONFIG_LOADER_ENABLE),y)

    loader_src = ${wildcard $(iot_link_root)/loader/*.c}
    C_SOURCES += $(loader_src)
    
    loader_inc = -I $(iot_link_root)/loader
    C_INCLUDES += $(loader_inc)
    
    C_DEFS += -D CONFIG_LOADER_ENABLE=1

endif
