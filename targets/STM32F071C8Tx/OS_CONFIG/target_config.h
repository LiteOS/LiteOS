/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/**@defgroup los_config System configuration items
 * @ingroup kernel
 */

#ifndef _TARGET_CONFIG_H
#define _TARGET_CONFIG_H

#include "los_typedef.h"
#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*=============================================================================
                                        System clock module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * System clock (unit: HZ)
 */
#define OS_SYS_CLOCK                                        (SystemCoreClock)

/**
 * @ingroup los_config
 * Number of Ticks in one second
 */
#define LOSCFG_BASE_CORE_TICK_PER_SECOND                    (1000UL)

/**
 * @ingroup los_config
 * External configuration item for timer tailoring
 */
#define LOSCFG_BASE_CORE_TICK_HW_TIME                       NO

/**
 * @ingroup los_config
 * Configuration liteos kernel tickless
 */
#define LOSCFG_KERNEL_TICKLESS                              NO

/*=============================================================================
                                        Hardware interrupt module configuration
=============================================================================*/
/* default LiteOS ram size level
    RAM_SIZE_LEVEL_0 means kernel ram < 8k  ,
    RAM_SIZE_LEVEL_1 means kernel ram < 16k,
    RAM_SIZE_LEVEL_2 means means kernel ram>=32k
*/
/**
 * @ingroup los_config
 * Configuration item for hardware interrupt tailoring
 */
#if defined (RAM_SIZE_LEVEL_0)
#define LOSCFG_PLATFORM_HWI                             YES
#elif defined(RAM_SIZE_LEVEL_1)
#define LOSCFG_PLATFORM_HWI                             YES
#elif defined(RAM_SIZE_LEVEL_2)
#define LOSCFG_PLATFORM_HWI                             YES
#else
#define LOSCFG_PLATFORM_HWI                             YES
#endif

/**
 * @ingroup los_config
 * Maximum number of used hardware interrupts, including Tick timer interrupts.
 */
#define LOSCFG_PLATFORM_HWI_LIMIT                           96


/*=============================================================================
                                       Task module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Default task priority
 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO                   10

/**
 * @ingroup los_config
 * Maximum supported number of tasks except the idle task rather than the number of usable tasks
 */
#if defined (RAM_SIZE_LEVEL_0)
#define LOSCFG_BASE_CORE_TSK_LIMIT                      4              // max num task
#elif defined (RAM_SIZE_LEVEL_1)
#define LOSCFG_BASE_CORE_TSK_LIMIT                      7              // max num task
#elif defined (RAM_SIZE_LEVEL_2)
#define LOSCFG_BASE_CORE_TSK_LIMIT                      15              // max num task
#else
#define LOSCFG_BASE_CORE_TSK_LIMIT                      15              // max num task
#endif

/**
 * @ingroup los_config
 * Size of the idle task stack
 */
#if defined (RAM_SIZE_LEVEL_0)
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE            SIZE(0x2D0)     // IDLE task stack
#elif defined (RAM_SIZE_LEVEL_1)
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE            SIZE(0x300)     // IDLE task stack
#elif defined (RAM_SIZE_LEVEL_2)
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE            SIZE(0x500)     // IDLE task stack
#else
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE            SIZE(0x500)     // IDLE task stack
#endif

/**
 * @ingroup los_config
 * Default task stack size
 */
#if defined (RAM_SIZE_LEVEL_0)
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE         SIZE(0x200)     // default stack
#elif defined (RAM_SIZE_LEVEL_1)
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE         SIZE(0x2D0)     // default stack
#elif defined (RAM_SIZE_LEVEL_2)
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE         SIZE(0x2D0)     // default stack
#else
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE         SIZE(0x2D0)     // default stack
#endif

/**
 * @ingroup los_config
 * Minimum stack size.
 */
#define LOS_TASK_MIN_STACK_SIZE                         (ALIGN(0x130, 16))

/**
 * @ingroup los_config
 * Configuration item for task Robin tailoring
 */
#define LOSCFG_BASE_CORE_TIMESLICE                          YES

/**
 * @ingroup los_config
 * Longest execution time of tasks with the same priorities
 */
#define LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT                  10

/**
 * @ingroup los_config
 * Configuration item for task (stack) monitoring module tailoring
 */
#define LOSCFG_BASE_CORE_TSK_MONITOR                        NO

/**
 * @ingroup los_config
 * Configuration item for task perf task filter hook
 */
#define LOSCFG_BASE_CORE_EXC_TSK_SWITCH                     YES

/**
 * @ingroup los_config
 * Configuration item for performance moniter unit
 */
#define OS_INCLUDE_PERF                                     YES

/**
 * @ingroup los_config
 * Define a usable task priority.Highest task priority.
 */
#define LOS_TASK_PRIORITY_HIGHEST                           0

/**
 * @ingroup los_config
 * Define a usable task priority.Lowest task priority.
 */
#define LOS_TASK_PRIORITY_LOWEST                            31


/*=============================================================================
                                       Semaphore module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for semaphore module tailoring
 */
#define LOSCFG_BASE_IPC_SEM                                 YES

/**
 * @ingroup los_config
 * Maximum supported number of semaphores
 */
#if defined (RAM_SIZE_LEVEL_0)
#define LOSCFG_BASE_IPC_SEM_LIMIT                       5              // the max sem-numb
#else
#define LOSCFG_BASE_IPC_SEM_LIMIT                       10              // the max sem-numb
#endif


/*=============================================================================
                                       Mutex module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for mutex module tailoring
 */
#define LOSCFG_BASE_IPC_MUX                                 YES

/**
 * @ingroup los_config
 * Maximum supported number of mutexes
 */
#if defined (RAM_SIZE_LEVEL_0)
#define LOSCFG_BASE_IPC_MUX_LIMIT                       5              // the max mutex-num
#else
#define LOSCFG_BASE_IPC_MUX_LIMIT                       15              // the max mutex-num
#endif


/*=============================================================================
                                       Queue module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for queue module tailoring
 */
#define LOSCFG_BASE_IPC_QUEUE                               YES

/**
 * @ingroup los_config
 * Maximum supported number of queues rather than the number of usable queues
 */
#if defined (RAM_SIZE_LEVEL_0)
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                     5              //the max queue-numb
#elif defined (RAM_SIZE_LEVEL_1)
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                     7              //the max queue-numb
#elif defined (RAM_SIZE_LEVEL_2)
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                     10              //the max queue-numb
#else
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                     10              //the max queue-numb
#endif


/*=============================================================================
                                       Software timer module configuration
=============================================================================*/

#if (LOSCFG_BASE_IPC_QUEUE == YES)
/**
 * @ingroup los_config
 * Configuration item for software timer module tailoring
 */
#define LOSCFG_BASE_CORE_SWTMR                              YES

#define LOSCFG_BASE_CORE_TSK_SWTMR_STACK_SIZE               LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE

#define LOSCFG_BASE_CORE_SWTMR_TASK                         YES

#define LOSCFG_BASE_CORE_SWTMR_ALIGN                        YES
#if(LOSCFG_BASE_CORE_SWTMR == NO && LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    #error "swtmr align first need support swmtr, should make LOSCFG_BASE_CORE_SWTMR = YES"
#endif

/**
 * @ingroup los_config
 * Maximum supported number of software timers rather than the number of usable software timers
 */
#if defined (RAM_SIZE_LEVEL_0)
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                    4                   // the max SWTMR numb
#elif defined (RAM_SIZE_LEVEL_1)
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                    7                   // the max SWTMR numb
#elif defined (RAM_SIZE_LEVEL_2)
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                    16                  // the max SWTMR numb
#else
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                    16                  // the max SWTMR numb
#endif

/**
 * @ingroup los_config
 * Max number of software timers ID
 */
#define OS_SWTMR_MAX_TIMERID                                ((65535/LOSCFG_BASE_CORE_SWTMR_LIMIT) * LOSCFG_BASE_CORE_SWTMR_LIMIT)

/**
 * @ingroup los_config
 * Maximum size of a software timer queue
 */
#define OS_SWTMR_HANDLE_QUEUE_SIZE                          (LOSCFG_BASE_CORE_SWTMR_LIMIT + 0)

/**
 * @ingroup los_config
 * Minimum divisor of software timer multiple alignment
 */
#define LOS_COMMON_DIVISOR                                  10
#endif


/*=============================================================================
                                       Memory module configuration
=============================================================================*/

extern UINT8 *m_aucSysMem0;
extern UINT32 __LOS_HEAP_ADDR_START__;
extern UINT32 __LOS_HEAP_ADDR_END__;

/**
 * @ingroup los_config
 * Starting address of the memory
 */
#define OS_SYS_MEM_ADDR                                     (VOID *)__LOS_HEAP_ADDR_START__

/**
 * @ingroup los_config
 * Ending address of the memory
 */
extern UINT32 g_sys_mem_addr_end;

/**
 * @ingroup los_config
 * Memory size
 */
#define OS_SYS_MEM_SIZE                                     ((UINT32)(__LOS_HEAP_ADDR_END__ - __LOS_HEAP_ADDR_START__ + 1))
//#if defined (RAM_SIZE_LEVEL_0)
//#define OS_SYS_MEM_SIZE                                     0x00002650          // 1A00
//#elif defined (RAM_SIZE_LEVEL_1)
//#define OS_SYS_MEM_SIZE                                     0x00002800          // size
//#elif defined (RAM_SIZE_LEVEL_2)
//#define OS_SYS_MEM_SIZE                                     0x00007400          // size
//#else
//#define OS_SYS_MEM_SIZE                                     0x00008000          // size
//#endif

/**
 * @ingroup los_config
 * Configuration module tailoring of mem node integrity checking
 */
#define LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK                YES

/**
 * @ingroup los_config
 * Configuration module tailoring of mem node size checking
 */
#define LOSCFG_BASE_MEM_NODE_SIZE_CHECK                     YES

#define LOSCFG_MEMORY_BESTFIT                               YES

/**
 * @ingroup los_config
 * Configuration module tailoring of more mempry pool checking
 */
#define LOSCFG_MEM_MUL_POOL                                 YES

/**
 * @ingroup los_config
 * Number of memory checking blocks
 */
#define OS_SYS_MEM_NUM                                      20

/**
 * @ingroup los_config
 * Configuration module tailoring of slab memory
 */
#define LOSCFG_KERNEL_MEM_SLAB                              YES


/*=============================================================================
                                       fw Interface configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for the monitoring of task communication
 */
#define LOSCFG_COMPAT_CMSIS_FW                              YES


/*=============================================================================
                                       others
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration system wake-up info to open
 */
#define OS_SR_WAKEUP_INFO                                   YES

/**
 * @ingroup los_config
 * Configuration CMSIS_OS_VER
 */
#define CMSIS_OS_VER                                        2


/*=============================================================================
                                        Exception module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for exception tailoring
 */
#define LOSCFG_PLATFORM_EXC                                 NO


/*=============================================================================
                                       Runstop module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for runstop module tailoring
 */
#define LOSCFG_KERNEL_RUNSTOP                               NO


/*=============================================================================
                                        track configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for track
 */
#define LOSCFG_BASE_MISC_TRACK                              NO

/**
 * @ingroup los_config
 * Max count of track items
 */
#define LOSCFG_BASE_MISC_TRACK_MAX_COUNT                    1024


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _TARGET_CONFIG_H */
