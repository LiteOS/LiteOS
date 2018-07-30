	
CC=gcc
CPP=g++
LD=ld
AR=ar

BUILD_DIR=build

# file match pattern
# eg: "*wifi*.c" -- match the file that contains *wifi*.c in the path
# if not use this, please define as "*"
FILE_PATTERN = "*lwm2m_client*.c"

FOR32 := true
BIN:= bin
LIB:= $(BUILD_DIR)/libliteos.a
	
HW_LITEOSDIR:=../../../../..
ifeq ($(FOR32), true)
CFLAGS := -m32  -march=i386 -mtune=i386
else
CFLAGS := 
endif


CFLAGS += -DSTM32F429xx -DUSE_HAL_DRIVER -DUSE_MBED_TLS -DMBEDTLS_CONFIG_FILE="<los_mbedtls_config.h>" -DLWM2M_LITTLE_ENDIAN -DWITH_LWIP -DWITH_LINUX -DLWM2M_CLIENT_MODE -DNDEBUG
CFLAGS += -DCONFIG_FEATURE_FOTA -DATINY_DEBUG
CFLAGS += -DWITH_LOGS
CFLAGS += -DLWIP_TIMEVAL_PRIVATE=0 -DBYTE_ORDER -DLWIP_COMPAT_SOCKETS=0
CFLAGS += -fprofile-arcs -ftest-coverage
GCOV_FLAGS := --rc lcov_branch_coverage=1 --rc  genhtml_branch_coverage=1

#compile dynamic library of liteos module	
SRC:= $(wildcard $(HW_LITEOSDIR)/components/connectivity/agent_tiny/lwm2m_client/*.c)
SRC+= $(wildcard $(HW_LITEOSDIR)/components/connectivity/agent_tiny/lwm2m_client/fota/*.c)
SRC+= $(wildcard $(HW_LITEOSDIR)/components/connectivity/agent_tiny/examples/*.c)
SRC+= $(wildcard $(HW_LITEOSDIR)/components/connectivity/lwm2m/core/*.c)
SRC+= $(wildcard $(HW_LITEOSDIR)/components/connectivity/lwm2m/core/er-coap-13/*.c)
SRC+= $(wildcard $(HW_LITEOSDIR)/components/connectivity/agent_tiny/osadapter/*.c) 
SRC+= $(wildcard $(HW_LITEOSDIR)/components/connectivity/agent_tiny/firmware_update/*.c) 
SRC+= $(wildcard $(HW_LITEOSDIR)/components/fota/fota_package_storage_device/*.c) 
SRC+= $(wildcard $(HW_LITEOSDIR)/components/fota/ota/*.c) 
SRC+= $(wildcard $(HW_LITEOSDIR)/components/fota/utility/*.c) 
SRC+= $(wildcard $(HW_LITEOSDIR)/tests/common/*.c)
SRC+= $(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/Src/fota_port.c

LIB_OBJ:=$(addprefix $(BUILD_DIR)/, $(notdir $(patsubst %.c, %.o, $(SRC))))
vpath %.c $(sort $(dir $(SRC)))
	
INCLUDES:= \
	-I$(HW_LITEOSDIR)/arch/arm/common/cmsis/ \
	-I$(HW_LITEOSDIR)/arch/arm/arm-m/include/ \
	-I$(HW_LITEOSDIR)/arch/arm/arm-m/cortex-m4/ \
	-I$(HW_LITEOSDIR)/components/cmsis/ \
	-I$(HW_LITEOSDIR)/components/connectivity/agent_tiny/comm/include/ \
    -I$(HW_LITEOSDIR)/components/connectivity/agent_tiny/examples/ \
    -I$(HW_LITEOSDIR)/components/connectivity/agent_tiny/firmware_update/ \
	-I$(HW_LITEOSDIR)/components/connectivity/agent_tiny/lwm2m_client/ \
	-I$(HW_LITEOSDIR)/components/connectivity/agent_tiny/lwm2m_client/fota/ \
    -I$(HW_LITEOSDIR)/components/connectivity/agent_tiny/mqtt_client/ \
	-I$(HW_LITEOSDIR)/components/connectivity/agent_tiny/osadapter/ \
    -I$(HW_LITEOSDIR)/components/connectivity/at_frame/ \
	-I$(HW_LITEOSDIR)/components/connectivity/lwm2m/core/ \
    -I$(HW_LITEOSDIR)/components/connectivity/lwm2m/core/er-coap-13 \
    -I$(HW_LITEOSDIR)/components/connectivity/lwm2m/examples/shared \
    -I$(HW_LITEOSDIR)/components/connectivity/lwm2m/examples/client \
    -I$(HW_LITEOSDIR)/components/fota/fota_package_storage_device/ \
    -I$(HW_LITEOSDIR)/components/fota/ota/ \
    -I$(HW_LITEOSDIR)/components/fota/utility/ \
	-I$(HW_LITEOSDIR)/components/net/lwip-2.0.3/src/include/ \
	-I$(HW_LITEOSDIR)/components/net/lwip_port/ \
	-I$(HW_LITEOSDIR)/components/net/lwip_port/OS \
	-I$(HW_LITEOSDIR)/components/security/mbedtls/mbedtls-2.6.0/include/ \
	-I$(HW_LITEOSDIR)/components/security/mbedtls/mbedtls-2.6.0/include/mbedtls/ \
	-I$(HW_LITEOSDIR)/components/security/mbedtls/mbedtls_port/ \
	-I$(HW_LITEOSDIR)/kernel/base/include/ \
	-I$(HW_LITEOSDIR)/kernel/include/ \
	-I$(HW_LITEOSDIR)/kernel/extended/include/ \
	-I$(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/OS_CONFIG/ \
	-I$(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/Inc/ \
	-I$(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/Hardware/Inc \
	-I$(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Inc/ \
	-I$(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy/ \
	-I$(HW_LITEOSDIR)/tests/common/ 
	

CPP_SRC := $(wildcard *.cpp)
CPP_OBJ = $(addprefix $(BUILD_DIR)/, $(notdir $(patsubst %.cpp, %.o, $(CPP_SRC))))

C_SRC   := $(wildcard *.c)
C_OBJ:=$(addprefix $(BUILD_DIR)/, $(notdir $(patsubst %.c, %.o, $(C_SRC))))

all:$(BUILD_DIR) lib $(BIN) 
lib:$(LIB)
TEST_GCDA:=$(patsubst %.o, %.gcda, $(CPP_OBJ))
TEST_GCDA+=$(patsubst %.o, %.gcda, $(C_OBJ))
TEST_GCDA+=$(patsubst %.o, %.gcda, $(LIB_OBJ))
	
TEST_GCNO:=$(patsubst %.o, %.gcno, $(CPP_OBJ))
TEST_GCNO+=$(patsubst %.o, %.gcno, $(C_OBJ))
TEST_GCNO+=$(patsubst %.o, %.gcno, $(LIB_OBJ))
	
$(BIN):$(C_OBJ) $(CPP_OBJ)
	$(CPP) $(CFLAGS) $(INCLUDES) -o $@ $^ -lliteos -L$(BUILD_DIR) -lcpptest -lpthread

$(CPP_OBJ):$(BUILD_DIR)/%.o:%.cpp
	$(CPP) -c $< $(INCLUDES) $(CFLAGS) -o $@

$(C_OBJ):$(BUILD_DIR)/%.o:%.c
	$(CC) -c $< $(INCLUDES) $(CFLAGS) -o $@

$(LIB):$(LIB_OBJ)
#	$(CPP) -shared -o $@ $^
	$(AR) -r $@ $^

$(LIB_OBJ):$(BUILD_DIR)/%.o:%.c
	$(CC) $(CFLAGS)	$(INCLUDES) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
    
clean:
	rm -rf $(BUILD_DIR) bin gcov *.htm *.log

run:
	./$(BIN)

cov:
	mkdir -p gcov
	./$(BIN)
	lcov -c -o gcov/gcov.info -d . -b . --rc lcov_branch_coverage=1
	lcov -e gcov/gcov.info $(FILE_PATTERN) -o gcov/gcov.info --rc lcov_branch_coverage=1
	genhtml -o gcov/html gcov/gcov.info --rc genhtml_branch_coverage=1

memcheck:
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --trace-children=yes  --smc-check=all ./$(BIN) 2>&1 | tee memcheck.log