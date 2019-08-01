################################################################################
# this is used for compile the ota storage
################################################################################

ota_storage_src = $(iot_link_root)/ota/ota_storage/*.c
C_SOURCES += $(ota_storage_src)


ota_storage_inc = $(iot_link_root)/ota/ota_storage
C_INCLUDES += $(ota_storage_inc)







