include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := bsp

STM32F769IDISCOVERY_HAL_SRC = $(wildcard $(HAL_DRIVER_SRC)/Src/*.c) 
ifeq ($(LOSCFG_PLATFORM_STM32F769IDISCOVERY), y)  
STM32F769IDISCOVERY_HAL_OUT = \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usb.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_utils.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_mmc.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_smbus.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_timebase_rtc_alarm_template.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_timebase_rtc_wakeup_template.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_timebase_tim_template.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dac.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dma.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dma2d.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_exti.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_gpio.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_i2c.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_lptim.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_pwr.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rcc.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rng.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rtc.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_spi.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_tim.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usart.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_utils.c
    STM32F769IDISCOVERY_HAL_SRC := $(filter-out $(STM32F769IDISCOVERY_HAL_OUT), $(STM32F769IDISCOVERY_HAL_SRC))
    LOCAL_SRCS = $(STM32F769IDISCOVERY_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32F429IGTX), y)
STM32F429IGTX_HAL_SRC = \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c
    LOCAL_SRCS = $(STM32F429IGTX_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32F407_ATK_EXPLORER), y)
STM32F407ZGTX_HAL_SRC = \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c
    LOCAL_SRCS = $(STM32F407ZGTX_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32L431_BearPi), y)
STM32L431_BearPi_HAL_SRC = \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_iwdg.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c
    LOCAL_SRCS = $(STM32L431_BearPi_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32F103_FIRE_ARBITRARY), y)
STM32F103VETX_HAL_SRC = \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_eth.c
    LOCAL_SRCS = $(STM32F103VETX_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32F072_Nucleo), y)
STM32F072RBTX_HAL_SRC = \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_iwdg.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c
    LOCAL_SRCS = $(STM32F072RBTX_HAL_SRC)
endif

LOCAL_SRCS += \
    $(wildcard $(HWI_SRC)/*.c) \
    $(wildcard $(TIMER_SRC)/*.c) \
    $(wildcard $(HRTIMER_SRC)/*.c) \
    $(wildcard $(UART_SRC)/*.c)

LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/lib/libc/stdio \
    -I $(LITEOSTOPDIR)/compat/posix/src \
    -I $(LITEOSTOPDIR)/drivers/random/dev/random \
    -I $(LITEOSTOPDIR)/targets/bsp/common \
    -I $(LITEOSTOPDIR)/targets/bsp/include \
    -I $(LITEOSTOPDIR)/targets/bsp/hw/include \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/st_timer \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Inc \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include \
    -I $(LITEOSTOPDIR)/targets/bsp/$(LITEOS_PLATFORM)/include \
    -I $(LITEOSTOPDIR)/targets/bsp/$(HAL_DRIVER_SRC)/Inc \
    -I $(LITEOSTOPDIR)/targets/bsp/$(HAL_DRIVER_SRC)/Inc/Legacy \
    -I $(LITEOSTOPDIR)/compat/cmsis

ifeq ($(findstring $(LOSCFG_SHELL)$(LOSCFG_SHELL_LK)$(LOSCFG_SHELL_DMESG), y y y),)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/shell/full/include
endif

ifeq ($(LOSCFG_ARCH_ARM_CORTEX_M), y)
BSP_HWI_SRC = \
    ${wildcard  hw/arm/interrupt/nvic/*.c} \
    $(wildcard common/dwt.c)
    LOCAL_SRCS += $(BSP_HWI_SRC)

BSP_TIMER_SRC = \
    ${wildcard hw/arm/timer/arm_cortex_m/*.c}
    LOCAL_SRCS += $(BSP_TIMER_SRC)

LOCAL_SRCS += $(wildcard drivers/uart/uart_debug.c)
LOCAL_SRCS += $(wildcard drivers/st_timer/timer.c)
endif

LOCAL_SRCS += $(wildcard common/demo_entry.c)

LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/include

ifeq ($(LOSCFG_COMPONNETS_NET_AT), y)
LOCAL_SRCS += $(wildcard drivers/uart/at_hal.c)
endif

ifeq ($(LOSCFG_DRIVERS_NETDEV), y)
LOCAL_SRCS += $(wildcard $(NET_SRC)/*.c)
endif

ifeq ($(LOSCFG_DRIVERS_RANDOM), y)
LOCAL_SRCS += $(wildcard board/$(LITEOS_PLATFORM)/random/*.c)
endif

ifeq ($(LOSCFG_DRIVERS_USB), y)
LOCAL_SRCS += $(wildcard $(USB_SRC)/*.c)
endif

ifeq ($(LOSCFG_PLATFORM_ADAPT), y)
LOCAL_SRCS += $(wildcard board/$(LITEOS_PLATFORM)/os_adapt/*.c)
endif

LOCAL_INCLUDE += -I $(wildcard board)/$(LITEOS_PLATFORM)/include/pm
LOCAL_SRCS += $(wildcard board/$(LITEOS_PLATFORM)/pm/*.c)

ifeq ($(LOSCFG_KERNEL_TICKLESS), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifeq ($(LOSCFG_KERNEL_TRACE), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifeq ($(LOSCFG_KERNEL_CPUP), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifeq ($(LOSCFG_KERNEL_RUNSTOP), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifndef LOSCFG_PLATFORM_OSAPPINIT
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/test/include
endif

ALL_ASSRCS := $(wildcard board/$(LITEOS_PLATFORM)/*.S)
ASSRCS := $(subst board/$(LITEOS_PLATFORM)/board.ld.S,,$(ALL_ASSRCS))

LOCAL_SRCS += $(ASSRCS)
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) $(LITEOS_CFLAGS_INTERWORK)

include $(MODULE)
