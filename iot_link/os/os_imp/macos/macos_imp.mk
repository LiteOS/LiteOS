################################################################################
# this is used for compile the iotlink
################################################################################

macos_imp_src  = ${wildcard $(iot_link_root)/os/os_imp/macos/*.c}
C_SOURCES += $(macos_imp_src)

macos_imp_inc = -I $(iot_link_root)/os/os_imp/macos
C_INCLUDES += $(macos_imp_inc)

C_DEFS += -D CONFIG_MACOS_ENABLE=1
