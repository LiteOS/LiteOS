################################################################################
# this is used for compile the linux socket
################################################################################

#configure the linux socket itself
LINUX_SOCKET_IMP_SOURCE  = ${wildcard $(TOP_DIR)/iot_link/network/tcpip/linux_socket/linux_socket_imp/*.c}
C_SOURCES += $(LINUX_SOCKET_IMP_SOURCE)
		
LINUX_SOCKET_IMP_INC = -I $(TOP_DIR)/iot_link/network/tcpip/linux_socket/linux_socket_imp
C_INCLUDES += $(LINUX_SOCKET_IMP_INC)

C_DEFS += -D CONFIG_LINUX_SOCKET_ENABLE=1
