COMPONENTS_SECURITY_INCLUDE :=

MBEDTLS_VERSION = mbedtls-2.16.8

ifeq ($(LOSCFG_COMPONENTS_SECURITY_MBEDTLS), y)
COMPONENTS_SECURITY_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/security/mbedtls/mbedtls_port \
    -I $(LITEOSTOPDIR)/components/security/mbedtls/$(MBEDTLS_VERSION)/include
endif