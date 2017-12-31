ifeq (${CMBACKTRACE}, true)

DIRECTORIES += thirdparty/CmBacktrace

C_SOURCES += thirdparty/CmBacktrace/cm_backtrace.c \
             thirdparty/CmBacktrace/los_cmbacktrace.c
             
INCLUDES += "-Ithirdparty/CmBacktrace"


DEFINES += -DLOS_CMBACKTRACE_COMPILE
ifeq (${CPU}, cortex-m0)
DEFINES += -DCMB_CPU_PLATFORM_TYPE=CMB_CPU_ARM_CORTEX_M0
else ifeq (${CPU}, cortex-m3)
DEFINES += -DCMB_CPU_PLATFORM_TYPE=CMB_CPU_ARM_CORTEX_M3
else ifeq (${CPU}, cortex-m4)
DEFINES += -DCMB_CPU_PLATFORM_TYPE=CMB_CPU_ARM_CORTEX_M4
else ifeq (${CPU}, cortex-m7)
DEFINES += -DCMB_CPU_PLATFORM_TYPE=CMB_CPU_ARM_CORTEX_M7
endif

endif