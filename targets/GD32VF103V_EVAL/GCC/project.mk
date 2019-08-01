HAL_DRIVER_SRC =  \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/system_gd32vf103.c        \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_eclic.c  \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_exmc.c   \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_exti.c   \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_gpio.c   \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_timer.c  \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_usart.c  \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_rcu.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_spi.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/n22/drivers/n22_func.c
        C_SOURCES += $(HAL_DRIVER_SRC)

#        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_i2c.c    \
         $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_fwdgt.c  \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_pmu.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_rtc.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_spi.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_dma.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_adc.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_bkp.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_can.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_crc.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_dbg.c    \
                $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_fmc.c    \
        $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Source/gd32vf103_dac.c    



TARGET_LIB_SRC = \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Lib/close.c      \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Lib/fstat.c      \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Lib/isatty.c     \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Lib/lseek.c      \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Lib/read.c       \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Lib/sbrk.c       \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Lib/write.c      \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Lib/write_hex.c
        C_SOURCES += $(TARGET_LIB_SRC)

        #$(SDK_DIR)/targets/GD32VF103V_EVAL/Lib/_exit.c      \

USER_SRC =  \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Src/gd32v103v_eval.c \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Src/gd32v103v_lcd_eval.c \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Src/picture.c \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Src/lcd_font.c \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Src/systick.c \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Src/touch_panel.c \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Src/gd25qxx.c \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Src/lcd_demo.c \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/Src/main.c
        C_SOURCES += $(USER_SRC)
        
HANDLERS_SRC =  \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/GCC/handlers.c \
        $(SDK_DIR)/targets/GD32VF103V_EVAL/GCC/init.c
        C_SOURCES += $(HANDLERS_SRC)
        
OS_CONFIG_INC = \
        -I $(SDK_DIR)/targets/GD32VF103V_EVAL/OS_CONFIG
        C_INCLUDES += $(OS_CONFIG_INC)       
# C includes
HAL_DRIVER_INC = \
        -I $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral \
        -I $(SDK_DIR)/drivers/third_party/GigaDevice/GD32VF103_standard_peripheral/Include \
        -I $(SDK_DIR)/drivers/third_party/GigaDevice/n22/drivers
        C_INCLUDES += $(HAL_DRIVER_INC)

ifeq ($(CONFIG_AT_ENABLE),y)
    UART_AT_SRC = $(TOP_DIR)/targets/GD32VF103V_EVAL/uart_at/uart_at.c
    C_SOURCES += $(UART_AT_SRC)
endif

        
RISCV_ASM_INC = \
        -I $(SDK_DIR)/drivers/third_party/GigaDevice/n22/drivers
        AS_INCLUDES += $(RISCV_ASM_INC)

USER_INC = \
        -I $(SDK_DIR)/targets/GD32VF103V_EVAL/Inc
        C_INCLUDES += $(USER_INC)

ASM_SOURCES_S += $(SDK_DIR)/targets/GD32VF103V_EVAL/GCC/start.S \
                 $(SDK_DIR)/targets/GD32VF103V_EVAL/GCC/entry.S
        
# C defines
C_DEFS += -D NDEBUG -DUSE_STDPERIPH_DRIVER -DGD32V103V_EVAL


include $(SDK_DIR)/iot_link/iot.mk
include $(SDK_DIR)/targets/GD32VF103V_EVAL/Demos/user_demo.mk                
