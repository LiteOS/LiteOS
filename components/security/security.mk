COMPONENTS_SECURITY_INCLUDE :=

ifeq ($(LOSCFG_COMPONENTS_SECURITY), y)
COMPONENTS_SECURITY_INCLUDE += \
        -I $(LITEOSTOPDIR)/components/security/mbedtls/mbedtls_port \
        -I $(LITEOSTOPDIR)/components/security/mbedtls/mbedtls-2.16.8/include
endif