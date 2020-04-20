############################# SRCs #################################
HWI_SRC     :=
MMU_SRC     :=
NET_SRC     :=
TIMER_SRC   :=
HRTIMER_SRC :=
UART_SRC    :=
USB_SRC     :=

############################# HI3556V200 Options#################################
ifeq ($(LOSCFG_PLATFORM_HI3556V200), y)
    HWI_TYPE     := arm/interrupt/gic
    TIMER_TYPE   := arm/timer/arm_generic
    HRTIMER_TYPE := hisoc/hrtimer
    NET_TYPE     := hieth
    UART_TYPE    := amba-pl011
    USB_TYPE     := usb3.0_hi3556v200
    LITEOS_CMACRO_TEST += -DTEST3556V200

########################## HI3516DV300 Options##############################
else ifeq ($(LOSCFG_PLATFORM_HI3516DV300), y)
    HWI_TYPE     := arm/interrupt/gic
    TIMER_TYPE   := arm/timer/arm_generic
    HRTIMER_TYPE := hisoc/hrtimer
    NET_TYPE     := hieth
    UART_TYPE    := amba-pl011
    USB_TYPE     := usb3.0_hi3516dv300
    LITEOS_CMACRO_TEST += -DTEST3516DV300

########################## hi1980_imu Options##############################
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
    UART_TYPE   := amba-pl011
    LITEOS_CMACRO_TEST += -DTESTVIRTA53

######################### realview pbx a9 Options#############################
else ifeq ($(LOSCFG_PLATFORM_PBX_A9), y)
    HWI_TYPE    := arm/interrupt/gic
    TIMER_TYPE  := arm/timer/arm_private
    UART_TYPE   := amba-pl011
    LITEOS_CMACRO_TEST += -DTESTPBXA9
######################### STM32F769IDISCOVERY Options#############################
else ifeq ($(LOSCFG_PLATFORM_STM32F769IDISCOVERY), y)
    LITEOS_CMACRO_TEST += -DTESTSTM32F769IDISCOVERY
    TIMER_TYPE := arm/timer/arm_cortex_m

######################### STM32F429IGTX Options#############################
else ifeq ($(LOSCFG_PLATFORM_STM32F429IGTX), y)
    LITEOS_CMACRO_TEST += -DTESTSTM32F429IGTX
endif

HWI_SRC     := hw/$(HWI_TYPE)
TIMER_SRC   := hw/$(TIMER_TYPE)
HRTIMER_SRC := hw/$(HRTIMER_TYPE)
NET_SRC     := net/$(NET_TYPE)
UART_SRC    := uart/$(UART_TYPE)
USB_SRC     := usb/$(USB_TYPE)

LITEOS_BASELIB       += -lbsp

LITEOS_PLATFORM      := $(subst $\",,$(LOSCFG_PLATFORM))

PLATFORM_INCLUDE := -I $(LITEOSTOPDIR)/platform/bsp/board/$(LITEOS_PLATFORM)/include \
                    -I $(LITEOSTOPDIR)/platform/bsp/common \
                    -I $(LITEOSTOPDIR)/platform/bsp/common/pm \
                    -I $(LITEOSTOPDIR)/platform/bsp/hw/include \
                    -I $(LITEOSTOPDIR)/platform/bsp/include \
                    -I $(LITEOSTOPDIR)/platform/bsp/$(UART_SRC)

ifeq ($(findstring y, $(LOSCFG_PLATFORM_HI3516DV300)$(LOSCFG_PLATFORM_HI3556V200)), y)
    PLATFORM_INCLUDE += -I $(LITEOSTOPDIR)/platform/bsp/board/$(LITEOS_PLATFORM)/include/hisoc
endif

-include $(LITEOSTOPDIR)/platform/bsp/board/$(LITEOS_PLATFORM)/board.mk

LIB_SUBDIRS             += platform/bsp
LITEOS_PLATFORM_INCLUDE += $(PLATFORM_INCLUDE)
LITEOS_CXXINCLUDE       += $(PLATFORM_INCLUDE)
