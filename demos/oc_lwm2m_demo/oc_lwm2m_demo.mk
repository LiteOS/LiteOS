################################################################################
# this is used for compile the oc lwm2m demo
################################################################################

#two types for you to compile:"none" "oc_lwm2m_demo_dtls" and "oc_lwm2m_demo_nodtls" "oc_lwm2m_demo_bs_dtls"
	
ifeq ($(CONFIG_OC_LWM2M_DEMO_TYPE), "none")
	oc_lwm2m_demo_src =
	oc_lwm2m_demo_inc =
	oc_lwm2m_demo_defs =
	
else ifeq ($(CONFIG_OC_LWM2M_DEMO_TYPE), "oc_lwm2m_demo_dtls_bs")
	oc_lwm2m_demo_src  = ${wildcard $(TOP_DIR)/demos/oc_lwm2m_demo/oc_lwm2m_demo_dtls_bs/oc_lwm2m_demo_dtls_bs.c}
	oc_lwm2m_demo_inc = -I $(TOP_DIR)/demos/oc_lwm2m_demo
	oc_lwm2m_demo_defs = -D LWM2M_BOOTSTRAP=1 -D CONFIG_OC_LWM2M_DEMO_ENABLE=1

else ifeq ($(CONFIG_OC_LWM2M_DEMO_TYPE), "oc_lwm2m_demo_dtls")
	oc_lwm2m_demo_src  = ${wildcard $(TOP_DIR)/demos/oc_lwm2m_demo/oc_lwm2m_demo_dtls/oc_lwm2m_demo_dtls.c}
	oc_lwm2m_demo_inc = -I $(TOP_DIR)/demos/oc_lwm2m_demo
	oc_lwm2m_demo_defs = -D CONFIG_OC_LWM2M_DEMO_ENABLE=1

else ifeq ($(CONFIG_OC_LWM2M_DEMO_TYPE), "oc_lwm2m_demo_nodtls")	
	oc_lwm2m_demo_src  = ${wildcard $(TOP_DIR)/demos/oc_lwm2m_demo/oc_lwm2m_demo_nodtls/oc_lwm2m_demo_nodtls.c}
	oc_lwm2m_demo_inc = -I $(TOP_DIR)/demos/oc_lwm2m_demo
	oc_lwm2m_demo_defs = -D CONFIG_OC_LWM2M_DEMO_ENABLE=1

else ifeq ($(CONFIG_OC_LWM2M_DEMO_TYPE), "oc_lwm2m_demo_nodtls_bs")	
	oc_lwm2m_demo_src  = ${wildcard $(TOP_DIR)/demos/oc_lwm2m_demo/oc_lwm2m_demo_nodtls_bs/oc_lwm2m_demo_nodtls_bs.c}
	oc_lwm2m_demo_inc = -I $(TOP_DIR)/demos/oc_lwm2m_demo
	oc_lwm2m_demo_defs = -D LWM2M_BOOTSTRAP=1 -D CONFIG_OC_LWM2M_DEMO_ENABLE=1	
	
endif
		 
C_SOURCES += $(oc_lwm2m_demo_src)
C_INCLUDES += $(oc_lwm2m_demo_inc)
C_DEFS += $(oc_lwm2m_demo_defs)

