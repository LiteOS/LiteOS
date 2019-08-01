################################################################################
# this is used for compile the iotlink
################################################################################

LINUX_IMP_SOURCE  = ${wildcard $(iot_link_root)/os/os_imp/linux/*.c}
C_SOURCES += $(LINUX_IMP_SOURCE)

LINUX_IMP_INC = -I $(iot_link_root)/os/os_imp/linux
C_INCLUDES += $(LINUX_IMP_INC)

C_DEFS += -D CONFIG_LINUX_ENABLE=1
