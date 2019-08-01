################################################################################
# this is used for compile the ota storage
################################################################################

boot_flag_src = $(iot_link_root)/ota/boot_flag/*.c
C_SOURCES += $(boot_flag_src)


boot_flag_inc = $(iot_link_root)/ota/boot_flag
C_INCLUDES += $(boot_flag_inc)







