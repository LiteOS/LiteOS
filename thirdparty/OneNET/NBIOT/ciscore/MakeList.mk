LOCAL_PATH := $(SRC_PATH)/ciscore

LOCAL_FILES := \
er-coap-13/er-coap-13.c \
std_object/std_object.c \
std_object/std_object_security.c \
cis_block1.c \
cis_bootstrap.c \
cis_config.c \
cis_core.c \
cis_data.c \
cis_json.c \
cis_list.c \
cis_log.c \
cis_management.c \
cis_memtrace.c \
cis_objects.c \
cis_observe.c \
cis_packet.c \
cis_registration.c \
cis_tlv.c \
cis_transaction.c \
cis_uri.c \
cis_utils.c

BUILD_FILES += $(addprefix $(LOCAL_PATH)/,$(LOCAL_FILES))