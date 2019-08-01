################################################################################
# this is used for compile the dtls
################################################################################
#only support "wakaama" now
ifeq ($(CONFIG_LWM2M_TYPE), "wakaama")
	include $(iot_link_root)/network/lwm2m/lwm2m_imp/wakaama/wakaama.mk
endif
