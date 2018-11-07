#ifndef _TARGET_CONFIG_H
#define _TARGET_CONFIG_H

#include "los_typedef.h"

/*=============================================================================
                                        System clock module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * System clock (unit: HZ)
 */
#define OS_SYS_CLOCK                                        (1000UL)

/**
 * @ingroup los_config
 * Number of Ticks in one second
 */
#define LOSCFG_BASE_CORE_TICK_PER_SECOND                    (5)

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

/**
 * @ingroup los_config
 * Configuration item for hardware interrupt tailoring
 */
#define LOSCFG_PLATFORM_HWI                                YES 

/**
 * @ingroup los_config
 * Maximum number of used hardware interrupts, including Tick timer interrupts.
 */
#define LOSCFG_PLATFORM_HWI_LIMIT                           64


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
#define LOSCFG_BASE_CORE_TSK_LIMIT                          6              // max num task

/**
 * @ingroup los_config
 * Size of the idle task stack
 */
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE                (LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE)        // IDLE task stack

/**
 * @ingroup los_config
 * Default task stack size
 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE             (8192)        // default stack; use glibc's SIGSTKSZ 

/**
 * @ingroup los_config
 * Minimum stack size.
 */
#define LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE                 (2048)              //use glibc's MINSIGSTKSZ

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
#define LOSCFG_BASE_CORE_TSK_MONITOR                        YES

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
#define LOSCFG_BASE_IPC_SEM_LIMIT                           10              // the max sem-numb


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
#define LOSCFG_BASE_IPC_MUX_LIMIT                           10              // the max mutex-num


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
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                         10              //the max queue-numb


/*=============================================================================
                                       Software timer module configuration
=============================================================================*/

#if (LOSCFG_BASE_IPC_QUEUE == YES)
/**
 * @ingroup los_config
 * Configuration item for software timer module tailoring
 */
#define LOSCFG_BASE_CORE_SWTMR                              NO

#define LOSCFG_BASE_CORE_TSK_SWTMR_STACK_SIZE               LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE

#define LOSCFG_BASE_CORE_SWTMR_TASK                         NO

#define LOSCFG_BASE_CORE_SWTMR_ALIGN                        NO
#if(LOSCFG_BASE_CORE_SWTMR == NO && LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
#error "swtmr align first need support swmtr, should make LOSCFG_BASE_CORE_SWTMR = YES"
#endif

/**
 * @ingroup los_config
 * Maximum supported number of software timers rather than the number of usable software timers
 */
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                        0             // the max SWTMR numb

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
#define LOSCFG_COMPAT_CMSIS_FW                              NO


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
#define LOSCFG_BASE_MISC_TRACK_MAX_COUNT                    256

#define CN_OS_SHELL   1  //enable the shell module
#define CN_OS_AT      1  //enable the at module

#endif /* _TARGET_CONFIG_H */

