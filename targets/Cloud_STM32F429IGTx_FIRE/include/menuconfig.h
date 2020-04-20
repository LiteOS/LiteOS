/*
 * Automatically generated C config: don't edit
 */
/*
 * Version Number
 */
#ifndef LOSCFG_MODULE_SWITCH
#define LOSCFG_MODULE_SWITCH
/*
 * Compiler
 */
#define LOSCFG_COMPILER_EABI_32 1

/*
 * Platform
 */
#define __LOSCFG_PLATFORM__ "Cloud_STM32F429IGTx_FIRE"
#define LOSCFG_PLATFORM_STM32F429IGTX 1
#define LOSCFG_PLATFORM_BSP_GIC_V2 1
#undef LOSCFG_PLATFORM_BSP_GIC_V3
#define LOSCFG_ARCH_ARM 1
#define LOSCFG_ARCH_ARM_AARCH32 1
#define LOSCFG_ARCH_ARM_V7M 1
#define __LOSCFG_ARCH_ARM_VER__ "armv7-m"
#define LOSCFG_ARCH_FPU_VFP_V4 1
#define LOSCFG_ARCH_FPU_VFP_D16 1
#define __LOSCFG_ARCH_FPU__ "fpv4-sp-d16"
#define LOSCFG_ARCH_CORTEX_M4 1
#define __LOSCFG_ARCH_CPU__ "cortex-m4"

/*
 * Extra Configurations
 */
#define LOSCFG_ARCH_FPU_DISABLE 1

/*
 * Kernel
 */
#define LOSCFG_KERNEL_LITEKERNEL 1
#undef LOSCFG_KERNEL_MEM_BESTFIT
#define LOSCFG_KERNEL_MEM_BESTFIT_LITTLE 1
#undef LOSCFG_KERNEL_EXTKERNEL
#define LOSCFG_SCHED_SQ 1

/*
 * Lib
 */
#define LOSCFG_LIB_LIBC 1
#undef LOSCFG_LIB_LIBM

/*
 * Compat
 */
#define LOSCFG_COMPAT_CMSIS 1
#define LOSCFG_CMSIS_VERSION 1
#undef LOSCFG_COMPAT_POSIX
#undef LOSCFG_COMPAT_LINUX

/*
 * FileSystem
 */
#undef LOSCFG_FS_VFS

/*
 * Net
 */
#undef LOSCFG_NET_LWIP_SACK

/*
 * Debug
 */
#define LOSCFG_COMPILE_DEBUG 1
#undef LOSCFG_PLATFORM_ADAPT
#undef LOSCFG_COREDUMP
#undef LOSCFG_GDB
#undef LOSCFG_ENABLE_MAGICKEY
#define LOSCFG_THUMB 1
#undef LOSCFG_DEBUG_VERSION
#undef LOSCFG_DRIVERS_UART
#undef LOSCFG_PLATFORM_UART_WITHOUT_VFS
#define LOSCFG_PLATFORM_NO_UART 1

/*
 * Driver
 */
#undef LOSCFG_DRIVERS

/*
 * Stack Smashing Protector (SSP) Compiler Feature
 */
#define LOSCFG_CC_NO_STACKPROTECTOR 1
#undef LOSCFG_CC_STACKPROTECTOR
#undef LOSCFG_CC_STACKPROTECTOR_STRONG
#undef LOSCFG_CC_STACKPROTECTOR_ALL

#endif
