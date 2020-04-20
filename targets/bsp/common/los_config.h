/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: System Config HeadFile
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

/**
 * @defgroup los_config System configuration items
 */

#ifndef _LOS_CONFIG_H
#define _LOS_CONFIG_H

#include "platform_config.h"
#include "system_config.h"
#include "los_tick.h"
#include "board.h"
#include "sys_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_config
 * int stack start addr
 */
extern CHAR __int_stack_start;
extern CHAR __rodata_start;
extern CHAR __rodata_end;
extern CHAR __bss_start;
extern CHAR __bss_end;
extern UINT32 __heap_start;
extern UINT32 __heap_end;

/**
 * @ingroup los_config
 * Number of sort link
 */
#define OS_TSK_SORTLINK_LEN_CONFIG                  8U

/**
 * @ingroup los_config
 * Number of priority queue
 */
#define OS_PRIORITY_QUEUE_NUM_CONFIG                32

/**
 * @ingroup los_config
 * Configuration lib configurable feature to open
 */
#ifndef LOSCFG_LIB_CONFIGURABLE
#define LOSCFG_LIB_CONFIGURABLE                             NO
#endif

/**
 * @ingroup los_config
 * Eexception handle
 */
#ifndef LOSCFG_PLATFORM_EXC
#define LOSCFG_PLATFORM_EXC                                 NO
#endif

/****************************** System clock module configuration ****************************/
/**
 * @ingroup los_config
 * System clock (unit: HZ)
 */
#ifndef OS_SYS_CLOCK
#define OS_SYS_CLOCK (get_bus_clk())
#endif
/**
 * @ingroup los_config
 * time timer clock (unit: HZ)
 */
#ifndef OS_TIME_TIMER_CLOCK
#define OS_TIME_TIMER_CLOCK OS_SYS_CLOCK
#endif

/**
 * @ingroup los_config
 * limit addr range when search for  'func local(frame pointer)' or 'func name'
 */
#ifndef OS_SYS_FUNC_ADDR_START
#define OS_SYS_FUNC_ADDR_START ((UINTPTR)&__int_stack_start)
#endif
#ifndef OS_SYS_FUNC_ADDR_END
#define OS_SYS_FUNC_ADDR_END g_sys_mem_addr_end
#endif

/**
 * @ingroup los_config
 * Number of Ticks in one second
 */
#ifndef LOSCFG_BASE_CORE_TICK_PER_SECOND
#define LOSCFG_BASE_CORE_TICK_PER_SECOND 100
#endif

/**
 * @ingroup los_config
 * Microseconds of adjtime in one second
 */
#ifndef LOSCFG_BASE_CORE_ADJ_PER_SECOND
#define LOSCFG_BASE_CORE_ADJ_PER_SECOND 500
#endif

/**
 * @ingroup los_config
 * Sched clck interval
 */
#define SCHED_CLOCK_INTETRVAL_TICKS 100

/**
 * @ingroup los_config
 * External configuration item for timer tailoring
 */
#ifndef LOSCFG_BASE_CORE_TICK_HW_TIME
#define LOSCFG_BASE_CORE_TICK_HW_TIME NO
#endif

/****************************** Hardware interrupt module configuration ******************************/
/**
 * @ingroup los_config
 * Configuration item for hardware interrupt tailoring
 */
#ifndef LOSCFG_PLATFORM_HWI
#define LOSCFG_PLATFORM_HWI YES
#endif

/**
 * @ingroup los_config
 * Maximum number of used hardware interrupts, including Tick timer interrupts.
 */
#ifndef LOSCFG_PLATFORM_HWI_LIMIT
#define LOSCFG_PLATFORM_HWI_LIMIT 96
#endif

/**
 * @ingroup los_config
 * The binary point value decide the maximum preemption level.
 * If preemption supported, the config value is [0, 1, 2, 3, 4, 5, 6],
 * to the corresponding preemption level value is [128, 64, 32, 16, 8, 4, 2].
 */
#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
#ifndef MAX_BINARY_POINT_VALUE
#define MAX_BINARY_POINT_VALUE  4
#endif
#endif

/****************************** Task module configuration ********************************/
/**
 * @ingroup los_config
 * Minimum stack size.
 *
 * 0x600 bytes, aligned on a boundary of 8.
 * 0x600 bytes, aligned on a boundary of 4.
 */
#ifndef LOS_TASK_MIN_STACK_SIZE
#ifdef __LP64__
#define LOS_TASK_MIN_STACK_SIZE (ALIGN(0x800, 8))
#else
#define LOS_TASK_MIN_STACK_SIZE (ALIGN(0x800, 4))
#endif
#endif

/**
 * @ingroup los_config
 * Default task priority
 */
#ifndef LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO
#define LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO 10
#endif

/**
 * @ingroup los_config
 * Maximum supported number of tasks except the idle task rather than the number of usable tasks
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                               g_taskLimits;
#define LOSCFG_BASE_CORE_TSK_LIMIT                          g_taskLimits
#else
#define LOSCFG_BASE_CORE_TSK_LIMIT                          LOSCFG_BASE_CORE_TSK_CONFIG
#endif


/**
 * @ingroup los_config
 * Size of the idle task stack
 */
#ifndef LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE SIZE(0x800)
#endif

/**
 * @ingroup los_config
 * Default task stack size
 */
#ifndef LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE SIZE(0x6000)
#endif

/**
 * @ingroup los_config
 * Configuration item for task Robin tailoring
 */
#ifndef LOSCFG_BASE_CORE_TIMESLICE
#define LOSCFG_BASE_CORE_TIMESLICE YES
#endif

/**
 * @ingroup los_config
 * Longest execution time of tasks with the same priorities
 */
#ifndef LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT
#define LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT 2
#endif

/**
 * @ingroup los_config
 * Configuration item for task (stack) monitoring module tailoring
 */
#ifndef LOSCFG_BASE_CORE_TSK_MONITOR
#define LOSCFG_BASE_CORE_TSK_MONITOR YES
#endif

/**
 * @ingroup los_config
 * Configuration item for task perf task filter hook
 */
#ifndef OS_PERF_TSK_FILTER
#define OS_PERF_TSK_FILTER NO
#endif

/**
 * @ingroup los_config
 * Check configuration specifications valid
 */
#if (LOSCFG_BASE_CORE_TSK_CONFIG <= 0)
#error "task maxnum cannot be zero"
#endif

/****************************** Semaphore module configuration ******************************/
/**
 * @ingroup los_config
 * Configuration item for semaphore module tailoring
 */
#ifndef LOSCFG_BASE_IPC_SEM
#define LOSCFG_BASE_IPC_SEM YES
#endif

#if (LOSCFG_BASE_IPC_SEM == YES)
#if (LOSCFG_BASE_IPC_SEM_CONFIG <= 0)
#error "sem maxnum cannot be zero"
#endif
#endif

/**
 * @ingroup los_config
 * Maximum supported number of semaphores
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                              g_semLimits;
#define LOSCFG_BASE_IPC_SEM_LIMIT                          g_semLimits
#else
#define LOSCFG_BASE_IPC_SEM_LIMIT                          LOSCFG_BASE_IPC_SEM_CONFIG
#endif

/**
 * @ingroup los_config
 * Maximum supported number of sortlink
 */
#define OS_TSK_SORTLINK_LEN                          OS_TSK_SORTLINK_LEN_CONFIG

/**
 * @ingroup los_config
 * Maximum supported number of priority queue
 */
#define OS_PRIORITY_QUEUE_NUM                         OS_PRIORITY_QUEUE_NUM_CONFIG

/****************************** mutex module configuration ******************************/
/**
 * @ingroup los_config
 * Configuration item for mutex module tailoring
 */
#ifndef LOSCFG_BASE_IPC_MUX
#define LOSCFG_BASE_IPC_MUX YES
#endif

#if (LOSCFG_BASE_IPC_MUX == YES)
#if (LOSCFG_BASE_IPC_MUX_CONFIG <= 0)
#error "mux maxnum cannot be zero"
#endif
#endif

/**
 * @ingroup los_config
 * Maximum supported number of mutexes
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                              g_muxLimits;
#define LOSCFG_BASE_IPC_MUX_LIMIT                          g_muxLimits
#else
#define LOSCFG_BASE_IPC_MUX_LIMIT                          LOSCFG_BASE_IPC_MUX_CONFIG
#endif


/****************************** Queue module configuration ********************************/
/**
 * @ingroup los_config
 * Configuration item for queue module tailoring
 */
#ifndef LOSCFG_BASE_IPC_QUEUE
#define LOSCFG_BASE_IPC_QUEUE YES
#endif

/**
 * @ingroup los_config
 * Maximum supported number of queues rather than the number of usable queues
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                                g_queLimit;
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                          g_queLimit
#else
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                          LOSCFG_BASE_IPC_QUEUE_CONFIG
#endif
/****************************** Software timer module configuration **************************/
#if (LOSCFG_BASE_IPC_QUEUE == YES)

#if (LOSCFG_BASE_IPC_QUEUE_CONFIG <= 0)
#error "queue maxnum cannot be zero"
#endif
/**
 * @ingroup los_config
 * Configuration item for software timer module tailoring
 */
#ifndef LOSCFG_BASE_CORE_SWTMR
#define LOSCFG_BASE_CORE_SWTMR YES
#endif

#if (LOSCFG_BASE_CORE_SWTMR == YES)
#if (LOSCFG_BASE_CORE_SWTMR_CONFIG <= 0)
#error "software timer maxnum cannot be zero"
#endif
#endif

/**
 * @ingroup los_config
 * Maximum supported number of software timers rather than the number of usable software timers
 */
#if (LOSCFG_LIB_CONFIGURABLE == YES)
extern UINT32                                                 g_swtmrLimits;
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                          g_swtmrLimits
#else
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                          LOSCFG_BASE_CORE_SWTMR_CONFIG
#endif

/**
 * @ingroup los_config
 * Max number of software timers ID
 *
 * 0xFFFF: max number of all software timers
 */
#ifndef OS_SWTMR_MAX_TIMERID
#define OS_SWTMR_MAX_TIMERID ((0xFFFF / LOSCFG_BASE_CORE_SWTMR_LIMIT) * LOSCFG_BASE_CORE_SWTMR_LIMIT)
#endif
/**
 * @ingroup los_config
 * Maximum size of a software timer queue
 */
#ifndef OS_SWTMR_HANDLE_QUEUE_SIZE
#define OS_SWTMR_HANDLE_QUEUE_SIZE LOSCFG_BASE_CORE_SWTMR_LIMIT
#endif

#endif

/****************************** Memory module configuration **************************/
#ifndef LOSCFG_KERNEL_MEM_STATISTICS
#define LOSCFG_KERNEL_MEM_STATISTICS          YES
#endif

#ifndef OS_EXC_INTERACTMEM_SIZE
#define OS_EXC_INTERACTMEM_SIZE (g_excInteractMemSize)
#endif

/**
 * @ingroup los_config
 * Starting address of the system memory
 */
#ifndef OS_SYS_MEM_START
#define OS_SYS_MEM_START                        __bss_end
#endif

/**
 * @ingroup los_config
 * Starting address of the system memory
 */
#ifndef OS_SYS_MEM_ADDR
#define OS_SYS_MEM_ADDR                        &m_aucSysMem1[0]
#endif

/**
 * @ingroup los_config
 * Memory size
 */
#ifndef OS_SYS_MEM_SIZE
#define OS_SYS_MEM_SIZE ((g_sys_mem_addr_end) - \
                         ((OS_EXC_INTERACTMEM_SIZE + ((UINTPTR)&__bss_end) + (64 - 1)) & ~(64 - 1)))
#endif

#ifndef OS_SYS_MEM_NUM
#define OS_SYS_MEM_NUM                            20
#endif

/****************************** fw Interface configuration **************************/
/**
 * @ingroup los_config
 * Configuration item for the monitoring of task communication
 */
#ifndef LOSCFG_COMPAT_CMSIS_FW
#define LOSCFG_COMPAT_CMSIS_FW                          NO
#endif

#ifndef LOSCFG_COMPAT_CMSIS
#define LOSCFG_COMPAT_CMSIS                             NO
#endif

/****************************** CPU module configuration **************************/
#define LOSCFG_KERNEL_CORE_NUM                          1

#define LOSCFG_KERNEL_CPU_MASK                          ((1 << LOSCFG_KERNEL_CORE_NUM) - 1)

/****************************** trace module configuration **************************/
#ifndef LOSCFG_KERNEL_TRACE
#define LOSCFG_KERNEL_TRACE                             NO
#endif

/**
 * @ingroup los_trace
 * It's the total size of trace buffer. It's in the unit of char
 */
#if (LOSCFG_KERNEL_TRACE == YES)
#define LOS_TRACE_BUFFER_SIZE                           2048
#endif

/****************************** Dynamic loading module configuration **************************/
#ifndef OS_AUTOINIT_DYNLOADER
#define OS_AUTOINIT_DYNLOADER YES
#endif

/****************************** exception information  configuration ******************************/
#ifdef LOSCFG_SHELL_EXCINFO
/**
 * @ingroup los_config
 * the size of space for recording exception information
 */
#define EXCINFO_RECORD_BUF_SIZE (16 * 1024)

/**
 * @ingroup los_config
 * the address of space for recording exception information
 * @attention
 * <ul>
 * <li> if uses, the address must be valid in flash, and it should not overlap with other addresses
 * used to store valid information.  </li>
 * </ul>
 *
 */
#define EXCINFO_RECORD_ADDR (0xffffffff)

/**
 * @ingroup los_config
 * @brief  define the type of functions for reading or writing exception information .
 *
 * @par Description:
 * <ul>
 * <li>This defination is used to declare the type of functions for reading or writing exception information</li>
 * </ul>
 * @attention
 * <ul>
 * <li> "startAddr" must be left to save the exception address space, the size of "buf" is "space"  </li>
 * </ul>
 *
 * @param startAddr    [IN] Address of storage ,its must be left to save the exception address space
 * @param space        [IN] size of storage.its is also the size of "buf"
 * @param rwFlag       [IN] writer-read flag, 0 for writing,1 for reading, other number is to do nothing.
 * @param buf          [IN] the buffer of storing data.
 *
 * @retval none.
 * @par Dependency:
 * <ul><li>los_config.h: the header file that contains the type defination.</li></ul>
 * @see
 * @since Huawei LiteOS V200R002C00
 */
typedef VOID (*log_read_write_fn)(UINT32 startAddr, UINT32 space, UINT32 rwFlag, CHAR *buf);

/**
 * @ingroup los_config
 * @brief Register recording exception information function.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to Register recording exception information function,
 * and specify location and space and size</li>
 * </ul>
 * @attention
 * <ul>
 * <li> "startAddr" must be left to save the exception address space, the size of "buf" is "space",
 * the space of "buf" is malloc or free in user's code  </li>
 * </ul>
 *
 * @param startAddr    [IN] Address of storage, it must be left to save the exception address space
 * @param space        [IN] size of storage space, it is also the size of "buf"
 * @param buf          [IN] the buffer of storing exception information, the space of "buf" is malloc or free
                            in user's code
 * @param hook         [IN] the function for reading or writing exception information.
 *
 * @retval none.
 * @par Dependency:
 * <ul><li>los_config.h: the header file that contains the API declaration.</li></ul>
 * @see
 * @since Huawei LiteOS V200R002C00
 */
VOID LOS_ExcInfoRegHook(UINT32 startAddr, UINT32 space, CHAR *buf, log_read_write_fn hook);
#endif

UINT32 OsMain(VOID);
VOID OsStart(VOID);
extern UINT32 LOS_KernelInit(VOID);
extern LITE_OS_SEC_TEXT_INIT VOID OsRegister(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_CONFIG_H */
