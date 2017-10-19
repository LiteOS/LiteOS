#/*----------------------------------------------------------------------------
#* Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
#* All rights reserved.
#* Redistribution and use in source and binary forms, with or without modification,
#* are permitted provided that the following conditions are met:
#* 1. Redistributions of source code must retain the above copyright notice, this list of
#* conditions and the following disclaimer.
#* 2. Redistributions in binary form must reproduce the above copyright notice, this list
#* of conditions and the following disclaimer in the documentation and/or other materials
#* provided with the distribution.
#* 3. Neither the name of the copyright holder nor the names of its contributors may be used
#* to endorse or promote products derived from this software without specific prior written
#* permission.
#* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
#* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#*---------------------------------------------------------------------------*/
#*----------------------------------------------------------------------------
#* Notice of Export Control Law
#* ===============================================
#* Huawei LiteOS may be subject to applicable export control laws and regulations, which might
#* include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
#* Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
#* applicable export control laws and regulations.
#*---------------------------------------------------------------------------*/

PROJECT           := LiteOS_${PLAT}

TARGET            := $(PROJECT)
TARGET_ELF        := $(TARGET).elf
TARGET_BIN        := $(TARGET).bin
TARGET_HEX        := $(TARGET).hex

OBJCPFLAGS_ELF_TO_BIN   = -Obinary
OBJCPFLAGS_ELF_TO_HEX   = -Oihex
OBJCPFLAGS_BIN_TO_HEX   = -Ibinary -Oihex
OBJCPFLAGS_ELF_TO_SREC  = -Osrec
OBJCPFLAGS_ELF_TO_LIST  = -S

ifeq (${ARCH}, arm)
CROSS_COMPILE := arm-none-eabi-
endif

ifeq ($(USE_CCACHE),1)
CCACHE := ccache
endif
 
# Make variables (CC, etc...)
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
ifeq ($(USE_CCACHE),1)
CC		= $(CCACHE) $(CROSS_COMPILE)gcc
CXX		= $(CCACHE) $(CROSS_COMPILE)g++
else
CC		:= $(CROSS_COMPILE)gcc
CXX		:= $(CROSS_COMPILE)g++
endif
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
GDB		= $(CROSS_COMPILE)gdb
READELF		= $(CROSS_COMPILE)readelf

ifndef SOURCE_ROOT
    # Assume we're in the source directory if not specified.
    SOURCE_ROOT=.
    export SOURCE_ROOT
endif

APPDIR ?= ${SOURCE_ROOT}/user
 
############################################################
### Build parameters
############################################################

ARCH_LIST:=arm
CPU_LIST:=cortex-m3 cortex-m4 cortex-m7
PLAT_LIST:=STM32F429I_DISCO STM32F746ZG_NUCLEO STM32F103ZE-ISO
ARMV_LIST:=armv7-m armv7e-m armv7ve

#ARCH:=arm
#PLAT:=STM32F429I_DISCO


# we do need them if we want to build anything else
$(if $(filter ${ARCH},${ARCH_LIST}),, \
	$(error ARCH ${ARCH} invalid or undefined, should be one of [ ${ARCH_LIST} ]))

$(if $(filter ${PLAT},${PLAT_LIST}),, \
	$(error PLAT ${PLAT} invalid or undefined, should be one of [ ${PLAT_LIST} ]))

ifeq (${ARCH}, arm)
$(if $(filter ${CPU},${CPU_LIST}),, \
	$(error CPU ${CPU} invalid or undefined, should be one of [ ${CPU_LIST} ]))

$(if $(filter ${ARMV},${ARMV_LIST}),, \
	$(error ARMV ${ARMV} invalid or undefined, should be one of [ ${ARMV_LIST} ]))
endif


########################################
###verbose building
########################################

# Set V=1 for verbose building, this can be passed in on the command line
# Set V=2 to have make echo out commands before executing them

ifeq ($V, 1)
	MAKEFLAGS = -s
	Q =
else
ifeq ($V, 2)
	MAKEFLAGS = 
	Q =
else
	MAKEFLAGS = -s
	Q = @
endif
endif


ifeq (${ARCH}, arm)
CFLAGS += -Wall -mlittle-endian  -mcpu=${CPU}  -march=${ARMV}  -ffreestanding -mthumb -mthumb-interwork -std=gnu99 --specs=rdimon.specs
ASFLAGS += -Wall -mlittle-endian  -mcpu=${CPU}  -march=${ARMV}  -ffreestanding -mthumb -mthumb-interwork -std=gnu99 --specs=rdimon.specs
LDFLAGS += -Wall -mlittle-endian  -mcpu=${CPU}  -march=${ARMV}  -ffreestanding -mthumb -mthumb-interwork -std=gnu99 --specs=rdimon.specs
DEFINES += ARCH_ARM
ifeq (${CPU}, cortex-m4)
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 
ASFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 
else ifeq (${CPU}, cortex-m7)
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 
ASFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 
endif
endif


# If a parent Makefile has passed us DEFINES, assume they will be missing -D.
DEFINES := ${DEFINES:%=-D%}
ifeq (${ARCH}, arm)
DEFINES += -DLOS_GCC_COMPILE
DEFINES += -D_GCC_WRAP_STDINT_H
ifeq (${CPU}, cortex-m4)
DEFINES += -DARM_MATH_CM4
endif
endif

ifeq (${PLAT}, M2S150_RV32)
DEFINES += -DLOS_M2S150_RV32
endif

INCLUDES += ${INCLUDE_DIRS:%=-I%}

ifndef NDEBUG
DEFINES += -DDEBUG
CFLAGS  += -ggdb -g3 
endif

vpath %.c    ${SOURCE_ROOT}
vpath %.h    ${SOURCE_ROOT}
vpath %.S    ${SOURCE_ROOT}
vpath %.s    ${SOURCE_ROOT}


INCLUDE_DIRS = ${SOURCE_ROOT}/example/include                 \
	       ${SOURCE_ROOT}/kernel/include                  \
               ${SOURCE_ROOT}/kernel/base/include 

INCLUDES += ${INCLUDE_DIRS:%=-I%}

ifeq (${ARCH}, arm)
INCLUDES += "-I${SOURCE_ROOT}/kernel/cmsis"
endif
INCLUDES += "-I${SOURCE_ROOT}/kernel/config"
INCLUDES += "-I${SOURCE_ROOT}/kernel/cpu/${ARCH}/${CPU}"
INCLUDES += "-I${SOURCE_ROOT}/kernel/link/gcc"
INCLUDES += "-I${SOURCE_ROOT}/platform/${PLAT}"


CFLAGS += ${DEFINES}
CFLAGS += ${INCLUDES}

LINKER_SCRIPT = ${SOURCE_ROOT}/platform/${PLAT}/LiteOS.ld
LDFLAGS    +=  -T $(LINKER_SCRIPT) 

############################################################
### Sub-makefiles
############################################################

include ${SOURCE_ROOT}/example/api/Makefile
include ${SOURCE_ROOT}/kernel/Makefile
include ${APPDIR}/Makefile
include ${SOURCE_ROOT}/platform/${PLAT}/Makefile

OBJECTS += ${patsubst %.c,build/%.o,$(C_SOURCES)}
OBJECTS += ${patsubst %.s,build/%.o,$(ASM_SOURCES)}


.PHONY: all default clean 

default: all



all:build build/$(TARGET_BIN)

build:
	mkdir -p $@

build/$(TARGET_BIN):build/$(TARGET_ELF)
	$(OBJCOPY) $(OBJCPFLAGS_ELF_TO_BIN) -S $^ $@
	@#$(OBJDUMP) -sStD $(TARGET_ELF) > map

build/$(TARGET_ELF):$(OBJECTS) 
	$(CC) -o $@ $^ $(LDFLAGS)

build/%.o:%.c 
	#$(CC) $(CFLAGS) -c  -o  $(addprefix $(dir $^), $(notdir $@))    $^
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

build/%.o:%.s 
	$(CC) $(ASFLAGS) -c -o $@ $^

clean:
	rm build -rf
	find $(SOURCE_ROOT) -iname '*.o' -delete
	find $(SOURCE_ROOT) -iname '*.bin' -delete
	find $(SOURCE_ROOT) -iname '*.elf' -delete
	find $(SOURCE_ROOT) -iname '*.map' -delete
