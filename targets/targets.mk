############################# SRCs #################################
HWI_SRC     :=
MMU_SRC     :=
NET_SRC     :=
TIMER_SRC   :=
HRTIMER_SRC :=
UART_SRC    :=
USB_SRC     :=
HAL_DRIVER_TYPE :=

############################# HI3556V200 Options##############################
ifeq ($(LOSCFG_PLATFORM_HI3556V200), y)
    HWI_TYPE     := arm/interrupt/gic
    TIMER_TYPE   := arm/timer/arm_generic
    HRTIMER_TYPE := hisoc/hrtimer
    NET_TYPE     := hieth
    UART_TYPE    := amba_pl011
    USB_TYPE     := usb3.0_hi3556v200
    LITEOS_CMACRO_TEST += -DTEST3556V200

########################## hi1980_imu Options################################
else ifeq ($(LOSCFG_PLATFORM_HI1980_IMU), y)
    HWI_TYPE    := arm/interrupt/gic
    TIMER_TYPE  := arm/timer/arm_generic
    UART_TYPE   := amba-pl011
    WARNING_AS_ERROR   :=
    LITEOS_CMACRO_TEST += -DTESTHI1980IMU

########################## qemu virt a53 Options##############################
else ifeq ($(LOSCFG_PLATFORM_QEMU_VIRT_A53), y)
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
######################### STM32L431_BearPi Options############################
else ifeq ($(LOSCFG_PLATFORM_STM32L431_BearPi), y)
    TIMER_TYPE := arm/timer/arm_cortex_m
    LITEOS_CMACRO_TEST += -DTESTSTM32L431_BearPi
    LITEOS_CMACRO_TEST += -DSTM32L431xx
    HAL_DRIVER_TYPE := STM32L4xx_HAL_Driver
endif

HWI_SRC     := bsp/hw/$(HWI_TYPE)
TIMER_SRC   := bsp/hw/$(TIMER_TYPE)
HRTIMER_SRC := bsp/hw/$(HRTIMER_TYPE)
NET_SRC     := bsp/net/$(NET_TYPE)
UART_SRC    := bsp/uart/$(UART_TYPE)
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
                    -I $(LITEOSTOPDIR)/arch/arm/cortex_m/cmsis

ifeq ($(LITEOS_PLATFORM), hi3556v200)
    PLATFORM_INCLUDE += -I $(LITEOSTOPDIR)/platform/bsp/board/$(LITEOS_PLATFORM)/include/hisoc
endif


LIB_SUBDIRS += targets/bsp
LIB_SUBDIRS += targets/$(LITEOS_PLATFORM)

#determin libs to link for windows
ifneq ($(OS), Linux)
    LITEOS_BASELIB += -l$(LITEOS_PLATFORM) -lsec -lbase -lbsp -lc -lm

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
    ifeq ($(LOSCFG_KERNEL_CPPSUPPORT), y)
        LITEOS_BASELIB += -lcppsupport
    endif
    ifeq ($(LOSCFG_COMPONENTS_FS), y)
        LITEOS_BASELIB += -lfs
    endif
    ifeq ($(LOSCFG_COMPONENTS_GUI), y)
        LITEOS_BASELIB += -lgui
    endif
    ifeq ($(LOSCFG_COMPONENTS_SENSORHUB), y)
        LITEOS_BASELIB += -lsensorhub
    endif
    ifeq ($(LOSCFG_DEMOS_FS), y)
        LITEOS_BASELIB += -lfs_demo
    endif
    ifeq ($(LOSCFG_DEMOS_GUI), y)
        LITEOS_BASELIB += -lgui_demo
    endif
    ifeq ($(LOSCFG_DEMOS_SENSORHUB), y)
        LITEOS_BASELIB += -lsensorhub_demo
    endif
    ifeq ($(LOSCFG_COMPONENTS_NET_LWIP), y)
        LITEOS_BASELIB += -llwip
    endif
endif

LITEOS_PLATFORM_INCLUDE += $(PLATFORM_INCLUDE)
LITEOS_CXXINCLUDE       += $(PLATFORM_INCLUDE)
