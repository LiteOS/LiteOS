include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/security/security.mk

MODULE_NAME := dtls_server
MBEDTLS_VERSION = mbedtls-2.16.8

LOCAL_SRCS :=
LOCAL_SRCS_y :=
LOCAL_INCLUDE :=

LOCAL_SRCS_y += test_dtls_server.c

LOCAL_INCLUDE += $(COMPONENTS_SECURITY_INCLUDE)
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/security/mbedtls/mbedtls_port \
    -I $(LITEOSTOPDIR)/components/security/mbedtls/$(MBEDTLS_VERSION)/include \
    -I $(LITEOSTOPDIR)/demos/dtls_server \
    -I $(LITEOSTOPDIR)/include

LOCAL_SRCS = $(LOCAL_SRCS_y)
LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE)
