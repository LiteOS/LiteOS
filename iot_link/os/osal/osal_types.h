/*
 * osal_types.h
 *
 *  Created on: 2019��4��26��
 *      Author: zhangqf
 */

#ifndef __OSAL_TYPES_H
#define __OSAL_TYPES_H

#ifndef bool_t
#define bool_t int
#endif
#ifndef true
#define true  1
#endif
#ifndef false
#define false 0
#endif
typedef void*  osal_mutex_t;
#define cn_mutex_invalid  ((osal_mutex_t)0xFFFFFFFF)

typedef void*  osal_semp_t;
#define cn_semp_invalid   ((osal_semp_t)0xFFFFFFFF)

#define cn_osal_timeout_forever  0xFFFFFFFF

typedef struct
{
    unsigned long long dead_time;
}osal_loop_timer_t;

typedef void (*fn_interrupt_handle)(void* arg);

enum swtmr_type
{
    SWTMR_MODE_ONCE,                 /**< One-off software timer */
    SWTMR_MODE_PERIOD,               /**< Periodic software timer */
    SWTMR_MODE_NO_SELFDELETE,        /**< One-off software timer, but not self-delete */
};

#endif /* __OSAL_TYPES_H */
