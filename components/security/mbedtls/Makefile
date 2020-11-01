include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))
LOCAL_SRCS_y :=
LOCAL_DIRS_y :=
C_DEFS :=

MBEDTLS_VERSION = mbedtls-2.16.8
ifeq ($(LOSCFG_COMPONENTS_SECURITY_MBEDTLS), y)
LOCAL_DIRS_y += mbedtls_port
LOCAL_DIRS_y += $(MBEDTLS_VERSION)/library

LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/include \
    -I $(LITEOSTOPDIR)/components/security/mbedtls/mbedtls_port \
    -I $(LITEOSTOPDIR)/components/security/mbedtls/$(MBEDTLS_VERSION)/include \
    -I $(LITEOSTOPDIR)/components/security/mbedtls/$(MBEDTLS_VERSION)/include/mbedtls

ifeq ($(LOSCFG_DTLS_PCK_ENCRYPT), y)
C_DEFS += -DMBEDTLS_CONFIG_FILE=\"los_mbedtls_config_psk.h\"
elif ($(LOSCFG_DTLS_CRET_ENCRYPT), y)
C_DEFS += -DMBEDTLS_CONFIG_FILE=\"los_mbedtls_config_cert.h\"
else
C_DEFS += -DMBEDTLS_CONFIG_FILE=\"los_mbedtls_config.h\"
endif
endif

LOCAL_SRCS_y += $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))
LOCAL_SRCS    = $(LOCAL_SRCS_y)

LOCAL_FLAGS := $(C_DEFS) $(LOCAL_INCLUDE)

include $(MODULE)