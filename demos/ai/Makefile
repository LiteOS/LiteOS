include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := ai_demo
LOCAL_SRCS_y :=

LOCAL_SRCS_y += $(wildcard ai_demo.c) $(wildcard utils/*.c)
LOCAL_SRCS_$(LOSCFG_DEMOS_AI_MNIST) += $(wildcard mnist/*.c)
LOCAL_SRCS_$(LOSCFG_DEMOS_AI_MNIST_QUANT) += $(wildcard mnist_quant/*.c)
LOCAL_SRCS_$(LOSCFG_DEMOS_AI_MOBILENET) += $(wildcard mobilenet/*.c)
LOCAL_SRCS_$(LOSCFG_DEMOS_AI_RESNET) += $(wildcard resnet/*.c)

ifeq ($(LOSCFG_ARCH_CORTEX_A9), y)
LOCAL_SRCS_y += \
    $(wildcard resnet/*.c) \
    $(wildcard mnist/*.c) \
    $(wildcard mobilenet/*.c)
endif

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/ai/mnist/include \
    -I $(LITEOSTOPDIR)/demos/ai/mnist_quant/include \
    -I $(LITEOSTOPDIR)/demos/ai/mobilenet/include \
    -I $(LITEOSTOPDIR)/demos/ai/resnet/include \
    -I $(LITEOSTOPDIR)/demos/ai/utils \
    -I $(LITEOSTOPDIR)/components/ai/include

LOCAL_CFLAGS += -Wno-error
LOCAL_CFLAGS += -Wno-pointer-sign
LOCAL_CFLAGS += -Wno-discarded-qualifiers
LOCAL_CFLAGS += -Wno-unused-variable
LOCAL_CFLAGS += -Wno-incompatible-pointer-types
LOCAL_CFLAGS += -Wno-int-conversion
LOCAL_CFLAGS += -Wno-strict-prototypes
LOCAL_CFLAGS += -Wno-endif-labels

LOCAL_SRCS = $(LOCAL_SRCS_y)
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
