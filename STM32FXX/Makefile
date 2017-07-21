
include $(shell pwd)/../projects/STM32F429I_DISCO_GCC/env.mk

CFLAGS      +=   -Wall -mlittle-endian  -mcpu=cortex-m4 -march=armv7e-m -mthumb -mthumb-interwork  -mfloat-abi=hard -mfpu=fpv4-sp-d16  -D _GCC_WRAP_STDINT_H -D LOS_STM32F429ZI  -D STM32F429xx -D USE_STM32F429I_DISCO  -D USE_HAL_DRIVER  -ffreestanding   -D ARM_MATH_CM4 -std=gnu99  --specs=rdimon.specs


LIB_INC_DIR1 := $(PWD)/stm32f429/Inc
LIB_INC_DIR2 := $(PWD)/Drivers/CMSIS/Include
LIB_INC_DIR3 := $(PWD)/Drivers/CMSIS/Device/ST/STM32F4xx/Include
LIB_INC_DIR4 := $(PWD)/Drivers/STM32F4xx_HAL_Driver/Inc
LIB_INC_DIR5 := $(PWD)/Drivers/BSP/STM32F429I-Discovery
LIB_INC_DIR6 := $(PWD)/../platform/STM32F429I_DISCO


LIB_SRC := ${wildcard $(PWD)/stm32f429/Src/*.c}
LIB_SRC += ${wildcard $(PWD)/Drivers/STM32F4xx_HAL_Driver/Src/*.c}
LIB_SRC += ${wildcard $(PWD)/Drivers/BSP/STM32F429I-Discovery/*.c}

LIB_OBJ := $(LIB_SRC:%.c=%.o)


CFLAGS += -I $(LIB_INC_DIR1)
CFLAGS += -I $(LIB_INC_DIR2)
CFLAGS += -I $(LIB_INC_DIR3)
CFLAGS += -I $(LIB_INC_DIR4)
CFLAGS += -I $(LIB_INC_DIR5)
CFLAGS += -I $(LIB_INC_DIR6)






.PHONY: libstm32f4.a


all: libstm32f4.a

libstm32f4.a: $(LIB_OBJ)
	$(AR) -r $@ $(LIB_OBJ)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^


clean:
	rm -f $(LIB_OBJ) libstm32f4.a
