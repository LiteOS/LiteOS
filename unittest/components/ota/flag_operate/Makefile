#	 = 是最基本的赋值
#	:= 是覆盖之前的值
#	?= 是如果没有被赋值过就赋予等号后面的值
#	+= 是添加等号后面的值	

#c编译器
CC=gcc
#C++编译器
CPP=g++
#连接器（ld命令）
LD=ld
#生成静态库的编译器（ar命令）
AR=ar
#目标输出地址
BUILD_DIR=build
#文件模式
FILE_PATTERN = "*flag_operate*.c"
FOR32 := true
#生成的可执行文件
BIN:= bin
#需要生成的静态链接库文件（依赖列表）
LIB:= $(BUILD_DIR)/libliteos.a
#工程的上层目录
HW_LITEOSDIR:=../../../../


#美元符号$，主要扩展打开makefile中定义的变量
#CFLAGS 表示用于 C 编译器的选项,涵盖了编译和汇编两个步骤。
ifeq ($(FOR32), true)	
CFLAGS := -m32  -march=i386 -mtune=i386
else
CFLAGS := 
endif
#为C编译器 添加其他信息
CFLAGS += -w -DSTM32F429xx -DUSE_HAL_DRIVER -DUSE_MBED_TLS -DMBEDTLS_CONFIG_FILE="<los_mbedtls_config.h>" -DLWM2M_LITTLE_ENDIAN -DWITH_LWIP -DWITH_LINUX -DLWM2M_CLIENT_MODE -DNDEBUG
CFLAGS += -DCONFIG_FEATURE_FOTA -DATINY_DEBUG
CFLAGS += -DWITH_LOGS
CFLAGS += -DLWIP_TIMEVAL_PRIVATE=0 -DBYTE_ORDER -DLWIP_COMPAT_SOCKETS=0
CFLAGS += -fprofile-arcs -ftest-coverage
CFLAGS += -DUSE_BOOTLOADER

#GCOV+LCOV 代码调试和覆盖率统计工具
GCOV_FLAGS := --rc lcov_branch_coverage=1 --rc  genhtml_branch_coverage=1

#compile dynamic library of liteos module	
# 取出 $(HW_LITEOSDIR)/components/ota/utility/*.c ，$(HW_LITEOSDIR)/components/ota/flag_operate/*.c ，$(HW_LITEOSDIR)/unittest/stub/*.c
# 注意这里只有第一个等式 用“：=” ，其他的用 “+=”
SRC:= $(wildcard $(HW_LITEOSDIR)/components/ota/utility/*.c)
SRC+= $(wildcard $(HW_LITEOSDIR)/components/ota/flag_operate/*.c)
SRC+= $(wildcard $(HW_LITEOSDIR)/unittest/stub/*.c)

#从上述模块SRC中取出c文件，并指定目标输出地址。这些信息存放在LIB_OBJ中
LIB_OBJ:=$(addprefix $(BUILD_DIR)/, $(notdir $(patsubst %.c, %.o, $(SRC))))

#在其它目录下搜索目标和依赖呢？在Makefile中可以使用VPATH或者vpath
vpath %.c $(sort $(dir $(SRC)))

#头文件路径
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
    -I$(HW_LITEOSDIR)/components/ota/utility/ \
	-I$(HW_LITEOSDIR)/components/ota/flag_operate/ \
    -I$(HW_LITEOSDIR)/components/fota/utility/ \
	-I$(HW_LITEOSDIR)/components/net/lwip-2.0.3/src/include/ \
	-I$(HW_LITEOSDIR)/components/net/lwip_port/ \
	-I$(HW_LITEOSDIR)/components/net/lwip_port/OS \
	-I$(HW_LITEOSDIR)/components/security/mbedtls/mbedtls-2.6.0/include/ \
	-I$(HW_LITEOSDIR)/components/security/mbedtls/mbedtls-2.6.0/include/mbedtls/ \
	-I$(HW_LITEOSDIR)/components/security/mbedtls/mbedtls_port/ \
	-I$(HW_LITEOSDIR)/components/fota/utility/ \
	-I$(HW_LITEOSDIR)/kernel/base/include/ \
	-I$(HW_LITEOSDIR)/kernel/include/ \
	-I$(HW_LITEOSDIR)/kernel/extended/include/ \
	-I$(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/OS_CONFIG/ \
	-I$(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/Inc/ \
	-I$(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/Hardware/Inc \
	-I$(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Inc/ \
	-I$(HW_LITEOSDIR)/targets/Cloud_STM32F429IGTx_FIRE/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy/ \
	-I$(HW_LITEOSDIR)/unittest/stub/
	
#“$(wildcard *.cpp)”来获取工作目录下的所有的.cpp文件列表(即 test_XXX.cpp 和 main.cpp)
CPP_SRC := $(wildcard *.cpp)
#从上述模块CPP_SRC中取出c文件，并指定目标输出地址。这些信息存放在CPP_OBJ中
CPP_OBJ = $(addprefix $(BUILD_DIR)/, $(notdir $(patsubst %.cpp, %.o, $(CPP_SRC))))

# 使用wildcard通配符 获取当前路径下 所有的C文件 （包括子目录下的C文件），保存到C_SRC中
C_SRC   := $(wildcard *.c)
#将上述的所有的C文件生成O文件，保存到指定路径下。
C_OBJ:=$(addprefix $(BUILD_DIR)/, $(notdir $(patsubst %.c, %.o, $(C_SRC))))

all:$(BUILD_DIR) lib $(BIN) 
	@echo "ota OK"
lib:$(LIB)
TEST_GCDA:=$(patsubst %.o, %.gcda, $(CPP_OBJ))
TEST_GCDA+=$(patsubst %.o, %.gcda, $(C_OBJ))
TEST_GCDA+=$(patsubst %.o, %.gcda, $(LIB_OBJ))
	
TEST_GCNO:=$(patsubst %.o, %.gcno, $(CPP_OBJ))
TEST_GCNO+=$(patsubst %.o, %.gcno, $(C_OBJ))
TEST_GCNO+=$(patsubst %.o, %.gcno, $(LIB_OBJ))
	
#		$(BIN)					是目标bin文件 
#		$(C_OBJ)和$(CPP_OBJ)	是需要生成的O文件的文件名
#		@$(CPP) 				表示G++编译器
#		$(CFLAGS)				表示C编译器的配置参数
#		$(INCLUDES)				表示头文件路径
#		-o						后面接编译输出的文件名
#		$@						是Makefile的通配符，代指你前面指定的文件名
#		$<						表示搜索到的第一个匹配的文件
#		$^						表示依赖列表
#https://www.cnblogs.com/alan666/p/8311984.html 
#-I(大写的i)参数是用来指定头文件目录（前面有使用）
#-L参数跟着的是库文件所在的目录名
#-l参数就是用来指定程序要链接的库，-l参数紧接着就是库名。
#pthread是动态库，需要用-lpthread，所有的动态库都需要用-lxxx来引用，包括-lliteos -lcpptest


#下面这句话可以看成：g++ (配置信息) -I(头文件路径) -o bin 依赖列表 动态链接
$(BIN):$(C_OBJ) $(CPP_OBJ)
	@$(CPP) $(CFLAGS) $(INCLUDES) -o $@ $^ -lliteos -L$(BUILD_DIR) -lcpptest -lpthread
#下面这句话可以看成：g++ -c  目标CPP文件 -I(头文件路径) (配置信息) -o O文件
$(CPP_OBJ):$(BUILD_DIR)/%.o:%.cpp
	@$(CPP) -c $< $(INCLUDES) $(CFLAGS) -o $@
#下面这句话可以看成：gcc -c  目标C文件 -I(头文件路径) (配置信息) -o O文件
$(C_OBJ):$(BUILD_DIR)/%.o:%.c
	@$(CC) -c $< $(INCLUDES) $(CFLAGS) -o $@
	
	
#生成静态链接库（LIB:= build/libliteos.a）
#下面这句话可以看成：ar -r build/libliteos.a 依赖列表
$(LIB):$(LIB_OBJ)
	@$(AR) -r $@ $^
#$(LIB_OBJ) 是生成库的中间文件，O文件
#下面这句话可以看成：gcc   (配置信息) -I(头文件路径) -c  目标C文件 -o O文件
$(LIB_OBJ):$(BUILD_DIR)/%.o:%.c
	@$(CC) $(CFLAGS)	$(INCLUDES) -c $< -o $@

#创建build文件夹
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# make clean	
clean:
	rm -rf $(BUILD_DIR) bin gcov *.htm *.log

# make run 执行bin文件
run:
	./$(BIN)

# make cov 生成gcov文件夹，得到覆盖率相关信息
cov:
	mkdir -p gcov
	./$(BIN)
	lcov -c -o gcov/gcov.info -d . -b . #--rc lcov_branch_coverage=1
	lcov -e gcov/gcov.info $(FILE_PATTERN) -o gcov/gcov.info #--rc lcov_branch_coverage=1
	genhtml -o gcov/html gcov/gcov.info #--rc genhtml_branch_coverage=1

# make memcheck 内存检查
memcheck:
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --trace-children=yes  --smc-check=all ./$(BIN) 2>&1 | tee memcheck.log
