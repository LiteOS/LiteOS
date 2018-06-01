/*******************************************************************************
 *
 * Copyright (c) 2017 China Mobile and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Bai Jie & Long Rong, China Mobile - initial API and implementation
 *    Baijie & Longrong, China Mobile - Please refer to git log
 *
 *******************************************************************************/

/************************************************************************/
/* nb-iot middle software of china mobile api                           */
/************************************************************************/

#include "cis_def.h"
#include "cis_config.h"

#ifndef _CIS_INTERFACE_SYS_H_
#define _CIS_INTERFACE_SYS_H_

#define NBSYS_IMEI_MAXLENGTH    (16)
#define NBSYS_IMSI_MAXLENGTH    (16)

#ifdef __cplusplus
extern "C" {
#endif
void assert_failed(uint8_t* file, uint32_t line);	
#define cissys_assert(flag)  if(!flag) assert_failed((uint8_t *)__FILE__, __LINE__)
cis_ret_t   cissys_init(const cis_cfg_sys_t* cfg);
//negative return value for error;return millisecond;
uint32_t    cissys_gettime(void);
void        cissys_sleepms(uint32_t ms);
void        cissys_logwrite(uint8_t* buffer,uint32_t length);
void*       cissys_malloc(size_t length);
void        cissys_free(void* ptr);

void*       cissys_memset(void* s, int c, size_t n);
void*       cissys_memcpy(void* dst, const void* src, size_t n);
void*       cissys_memmove(void* dst, const void* src, size_t n);
int         cissys_memcmp(const void* s1, const void* s2, size_t n);
void        cissys_fault(uint16_t id);

uint32_t    cissys_rand(void);
uint8_t     cissys_getIMEI(uint8_t* buffer,uint32_t maxlen);
uint8_t     cissys_getIMSI(uint8_t* buffer,uint32_t maxlen);

void        cissys_lockcreate(void** mutex);
void        cissys_lockdestory(void* mutex);
cis_ret_t   cissys_lock(void* mutex,uint32_t ms);
void        cissys_unlock(void* mutex);

bool        cissys_save(uint8_t* buffer,uint32_t length);
bool        cissys_load(uint8_t* buffer,uint32_t length);

#ifdef __cplusplus
};
#endif

#endif//_CIS_INTERFACE_SYS_H_
