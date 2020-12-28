############################# SRCs #################################
HWI_SRC     :=
MMU_SRC     :=
NET_SRC     :=
TIMER_SRC   :=
HRTIMER_SRC :=
UART_SRC    :=
USB_SRC     :=
HAL_DRIVER_TYPE :=

########################## qemu virt a53 Options##############################
ifeq ($(LOSCFG_PLATFORM_QEMU_VIRT_A53), y)
    HWI_TYPE    := arm/interrupt/gic
    TIMER_TYPE  := arm/timer/arm_generic
    UART_TYPE   := amba_pl011
    LITEOS_CMACRO_TEST += -DTESTVIRTA53

######################### realview pbx a9 Options#############################
else ifeq ($(LOSCFG_PLATFORM_PBX_A9), y)
    HWI_TYPE    := arm/interrupt/gic
    TIMER_TYPE  := arm/timer/arm_private
    UART_TYPE   := amba_pl011
    LITEOS_CMACRO_TEST += -DTESTPBXA9
######################### STM32F769IDISCOVERY Options#########################
else ifeq ($(LOSCFG_PLATFORM_STM32F769IDISCOVERY), y)
    TIMER_TYPE := arm/timer/arm_cortex_m
    LITEOS_CMACRO_TEST += -DTESTSTM32F769IDISCOVERY
    LITEOS_CMACRO_TEST += -DSTM32F769xx
    HAL_DRIVER_TYPE := STM32F7xx_HAL_Driver
######################### STM32F429IGTX Options###############################
else ifeq ($(LOSCFG_PLATFORM_STM32F429IGTX), y)
    TIMER_TYPE := arm/timer/arm_cortex_m
    LITEOS_CMACRO_TEST += -DTESTSTM32F429IGTX
    LITEOS_CMACRO_TEST += -DSTM32F429xx
    HAL_DRIVER_TYPE := STM32F4xx_HAL_Driver
######################### STM32F407ZGTX Options###############################
else ifeq ($(LOSCFG_PLATFORM_STM32F407_ATK_EXPLORER), y)
    TIMER_TYPE := arm/timer/arm_cortex_m
    LITEOS_CMACRO_TEST += -DSTM32F407xx
    HAL_DRIVER_TYPE := STM32F4xx_HAL_Driver
######################### STM32L431_BearPi Options############################
else ifeq ($(LOSCFG_PLATFORM_STM32L431_BearPi), y)
    TIMER_TYPE := arm/timer/arm_cortex_m
    LITEOS_CMACRO_TEST += -DTESTSTM32L431_BearPi
    LITEOS_CMACRO_TEST += -DSTM32L431xx
    HAL_DRIVER_TYPE := STM32L4xx_HAL_Driver
######################### STM32F103VETX Options###############################
else ifeq ($(LOSCFG_PLATFORM_STM32F103_FIRE_ARBITRARY), y)
    TIMER_TYPE := arm/timer/arm_cortex_m
    LITEOS_CMACRO_TEST += -D__FPU_PRESENT -DSTM32F103xE
    HAL_DRIVER_TYPE := STM32F1xx_HAL_Driver
############################# realview-pbx-a9 Options#############################
else ifeq ($(LOSCFG_PLATFORM_PBX_A9), y)
    HWI_TYPE     := arm/interrupt/gic
    TIMER_TYPE   := hw/arm/timer/arm_private
######################### STM32F072_Nucleo Options###############################
else ifeq ($(LOSCFG_PLATFORM_STM32F072_Nucleo), y)
    TIMER_TYPE := arm/timer/arm_cortex_m
    LITEOS_CMACRO_TEST += -DSTM32F072xB
    HAL_DRIVER_TYPE := STM32F0xx_HAL_Driver
endif

HWI_SRC     := hw/$(HWI_TYPE)
TIMER_SRC   := hw/$(TIMER_TYPE)
HRTIMER_SRC := hw/$(HRTIMER_TYPE)
NET_SRC     := bsp/net/$(NET_TYPE)
UART_SRC    := drivers/uart/$(UART_TYPE)
USB_SRC     := bsp/usb/$(USB_TYPE)
HAL_DRIVER_SRC := drivers/$(HAL_DRIVER_TYPE)

LITEOS_PLATFORM  := $(subst $\",,$(LOSCFG_PLATFORM))

PLATFORM_INCLUDE := -I $(LITEOSTOPDIR)/targets/bsp/common \
                    -I $(LITEOSTOPDIR)/targets/bsp/common/pm \
                    -I $(LITEOSTOPDIR)/targets/bsp/hw/include \
                    -I $(LITEOSTOPDIR)/targets/bsp/include \
                    -I $(LITEOSTOPDIR)/targets/bsp/$(UART_SRC) \
                    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include \
                    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Inc \
                    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include/asm \
                    -I $(LITEOSTOPDIR)/arch/arm/cortex_m/cmsis \
                    -I $(LITEOSTOPDIR)/lib/huawei_libc/time

ifeq ($(LITEOS_PLATFORM), hi3516ev200)
    PLATFORM_INCLUDE += -I $(LITEOSTOPDIR)/platform/bsp/board/$(LITEOS_PLATFORM)/include/hisoc
endif

LIB_SUBDIRS += targets/bsp
LIB_SUBDIRS += targets/$(LITEOS_PLATFORM)

#determine libs to link for windows
ifneq ($(OS), Linux)
    LITEOS_BASELIB += -l$(LITEOS_PLATFORM) -lsec -lbase -linit -lbsp -lc -lm -losdepends

    ifeq ($(LITEOS_CPU_TYPE), $(findstring $(LITEOS_CPU_TYPE), cortex-a7 cortex-a9))
        LITEOS_BASELIB += -lcsysdeps
    endif

    ifeq ($(LOSCFG_SHELL), y)
        LITEOS_BASELIB += -lshell
    endif
    ifeq ($(LOSCFG_COMPAT_CMSIS), y)
        LITEOS_BASELIB += -lcmsis
    endif
    ifeq ($(LOSCFG_COMPAT_POSIX), y)
        LITEOS_BASELIB += -lposix
    endif
    ifeq ($(LOSCFG_KERNEL_TICKLESS), y)
        LITEOS_BASELIB += -ltickless
    endif
    ifeq ($(LOSCFG_KERNEL_CPUP), y)
        LITEOS_BASELIB += -lcpup
    endif
    ifeq ($(LOSCFG_KERNEL_TRACE), y)
        LITEOS_BASELIB += -ltrace
    endif
    ifeq ($(LOSCFG_KERNEL_CPPSUPPORT), y)
        LITEOS_BASELIB += -lcppsupport
    endif
    ifeq ($(LOSCFG_COMPONENTS_FS), y)
        LITEOS_BASELIB += -lfs
    endif
    ifeq ($(LOSCFG_COMPONENTS_GUI), y)
        LITEOS_BASELIB += -lgui
    endif
    ifeq ($(LOSCFG_COMPONNETS_NET_AT), y)
        LITEOS_BASELIB += -lat_device
        LITEOS_BASELIB += -lat_frame
    endif
    ifeq ($(LOSCFG_COMPONENTS_CONNECTIVITY_NB_IOT), y)
        LITEOS_BASELIB += -lnb_iot
    endif
    ifeq ($(LOSCFG_COMPONENTS_SENSORHUB), y)
        LITEOS_BASELIB += -lsensorhub
    endif
    ifeq ($(LOSCFG_DEMOS_SENSORHUB), y)
        LITEOS_BASELIB += -lsensorhub_demo
    endif
    ifeq ($(LOSCFG_COMPONENTS_NET_LWIP), y)
        LITEOS_BASELIB += -llwip
    endif
    ifeq ($(LOSCFG_COMPONENTS_SECURITY_MBEDTLS), y)
        LITEOS_BASELIB += -lmbedtls
    endif
    ifeq ($(LOSCFG_COMPONENTS_LIB_CJSON), y)
        LITEOS_BASELIB += -lcjson
    endif
    ifeq ($(LOSCFG_COMPONENTS_LOG), y)
        LITEOS_BASELIB += -llog
    endif
    ifeq ($(LOSCFG_COMPONENTS_CONNECTIVITY_MQTT), y)
        LITEOS_BASELIB += -lmqtt
    endif
    ifeq ($(LOSCFG_COMPONENTS_CONNECTIVITY_LWM2M), y)
        LITEOS_BASELIB += -llwm2m
    endif
    ifeq ($(LOSCFG_COMPONENTS_CONNECTIVITY_ATINY_MQTT), y)
        LITEOS_BASELIB += -latiny_mqtt
    endif
    ifeq ($(LOSCFG_COMPONENTS_CONNECTIVITY_ATINY_LWM2M), y)
        LITEOS_BASELIB += -latiny_lwm2m
    endif
    ifeq ($(LOSCFG_COMPONENTS_NET_SAL), y)
        LITEOS_BASELIB += -lsal
    endif

    ifeq ($(LOSCFG_DEMOS_KERNEL), y)
        LITEOS_BASELIB += -lkernel_demo
    endif
    ifeq ($(LOSCFG_DEMOS_FS), y)
        LITEOS_BASELIB += -lfs_demo
    endif
    ifeq ($(LOSCFG_DEMOS_GUI), y)
        LITEOS_BASELIB += -lgui_demo
    endif
    ifeq ($(LOSCFG_DEMOS_AGENT_TINY_MQTT), y)
        LITEOS_BASELIB += -lagenttiny_mqtt
    endif
    ifeq ($(LOSCFG_DEMOS_AGENT_TINY_LWM2M), y)
        LITEOS_BASELIB += -lagenttiny_lwm2m
    endif
    ifeq ($(LOSCFG_DEMOS_DTLS_SERVER), y)
        LITEOS_BASELIB += -ldtls_server
    endif
    ifeq ($(LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY), y)
        LITEOS_BASELIB += -lnbiot_without_atiny
    endif
    ifeq ($(LOSCFG_DEMOS_IPV6_CLIENT), y)
        LITEOS_BASELIB += -lipv6_client
    endif
    ifeq ($(LOSCFG_DEMOS_LMS), y)
        LITEOS_BASELIB += -llms_demo
    endif
    ifeq ($(LOSCFG_DEMOS_TRACE), y)
        LITEOS_BASELIB += -ltrace_demo
    endif
    ifeq ($(LOSCFG_DEMOS_AI), y)
        LITEOS_BASELIB += -lai_demo
    endif
endif

ifeq ($(LOSCFG_DEMOS_AI), y)
    ifeq ($(LOSCFG_ARCH_ARM_CORTEX_A), y)
        LITEOS_BASELIB += -lcortex-a-nnacl
    else ifeq ($(LOSCFG_ARCH_ARM_CORTEX_M), y)
        LITEOS_BASELIB += -lcortex-m-nnacl
endif
endif

ifeq ($(LOSCFG_KERNEL_LMS), y)
    LITEOS_BASELIB += -llms
endif

LITEOS_PLATFORM_INCLUDE += $(PLATFORM_INCLUDE)
LITEOS_CXXINCLUDE       += $(PLATFORM_INCLUDE)
