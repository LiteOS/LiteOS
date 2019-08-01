/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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
/**
 *  DATE                AUTHOR      INSTRUCTION
 *  2019-07-26 18:05  zhangqianfu  The first version
 *
 */
#ifndef LITEOS_LAB_IOT_LINK_STIMER_STIMER_H_
#define LITEOS_LAB_IOT_LINK_STIMER_STIMER_H_

#include <stddef.h>
#include <stdint.h>




////////////////DEFINES FOR THE STIMER HEADER///////////////////////////////////
///< defines for the soft timer operation method,you could use it with the stimer_ioctl function
typedef enum
{
    en_stimer_opt_stop = 0,   ///< stop the soft timer
    en_stimer_opt_start,      ///< start the soft timer
    en_stimer_opt_gettime,    ///< get the left time
    en_stimer_opt_recycle,    ///< reset the cycle
    en_stimer_opt_reflag,     ///< reset the flag
}en_stimer_opt_t;

///< defines for the soft timer handle returned by the create function and used for ioctl and delete function
///< the user should never supposed any data structure for the handle, you could only use the api supplied
typedef void* stimer_t;

///< defines for the flag when create the soft timer
#define cn_stimer_flag_once  (1<<0)
#define cn_stimer_flag_start (1<<1)
///< define the soft timer interrupt service function
typedef void  (*fn_stimer_handler)(void *arg);

#if CONFIG_STIMER_ENABLE

/**
 * @brief: this function used for initialize the timer component, should be called after the os
 *
 * @return: 0 success while -1 failed
 * */
int32_t stimer_init();

/**
 * @brief: you could use this function for create a soft timer as you wished
 *
 * @param[in]:name, the soft timer name
 * @param[in]:handler, the soft timer handler when the timer is activated
 * @param[in]:arg, the argument will supplied for the handler
 * @param[in]:cycle, the soft timer cycle
 * @param[in]:flag, you could use cn_stimer_flag_once or cn_stimer_flag_start or both
 *
 * @return:soft timer handler, you could use it for ioctl or delete;if failed NULL
 *         will be returned
 *
 * */
stimer_t stimer_create(const char *name,fn_stimer_handler handler, \
                                 void *arg,uint32_t cycle,uint32_t flag);

/**
 * @brief: you could use this function to delete the soft timer you created
 *
 * @param[in]:timer handler,returned by the create function
 *
 * @return:0 success while -1 or others failed
 *
 * */
int32_t       stimer_delete(stimer_t timer);

/**
 * @brief: you could use this function to control the function
 *
 * @param[in]:timer,returned by the create function
 * @param[in]:opt,defined by en_stimer_opt_t, and the arg is corresponding parameters
 * @param[in]:arg, corresponding to the opt
 *
 * @return:0 success while -1 or others failed
 *
 * */
int32_t       stimer_ioctl(stimer_t timer,en_stimer_opt_t opt, void *arg);


#else

#define  stimer_init()                                  -1

#define  stimer_create(name, handler, arg, cycle, flag)  NULL
#define  stimer_delete(timer)                           -1
#define  stimer_ioctl(timer,opt,arg)                    -1

#endif
#endif /* LITEOS_LAB_IOT_LINK_STIMER_STIMER_H_ */
