/*
 Copyright (c) 2018 Chinamobile

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

*/


#include <cis_if_sys.h>
#include <cis_def.h>
#include <cis_config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cis_api.h>
#include <assert.h>
#include "cis_internals.h"
#include <stdlib.h> 

#include "os-api.h"

#include <time.h>
#include "win_timers.h"
#include "los_sys.h"
#include "los_memory.h"
#include "cmsis_os.h"


static uint32_t startTime = 0;


cis_ret_t cissys_init(const cis_cfg_sys_t* cfg)
{
  osMemInit();
  startTime = osKernelSysTick();
  return 1;
}

uint32_t cissys_gettime()
{
  return (osKernelSysTick()-startTime);
}

void cissys_sleepms(uint32_t ms)
{
  osDelayMS(ms);
}

void cissys_logwrite(uint8_t* buffer,uint32_t length)
{
  PRINT_INFO("%s",(char*)buffer);
}
void assert_failed(uint8_t* file, uint32_t line)
{
  PRINT_ERR("pannic: file %s on line %d\r\n", file, line);
  while(1);
}

void* cissys_malloc(size_t length)
{
  return osMalloc(length);
}

void cissys_free(void* buffer)
{
  osFree(buffer);
}


void* cissys_memset( void* s, int c, size_t n)
{
  return memset( s, c, n);
}

void* cissys_memcpy(void* dst, const void* src, size_t n)
{
  return memcpy( dst, src, n);
}

void* cissys_memmove(void* dst, const void* src, size_t n)
{
  return memmove( dst, src, n);
}

int cissys_memcmp( const void* s1, const void* s2, size_t n)
{
  return memcmp( s1, s2, n);
}

void cissys_fault(uint16_t id)
{
  PRINT_ERR("fall in cissys_fault.id=%d\r\n", id);
}

uint32_t cissys_rand()
{
  srand(cissys_gettime());
  union{
    uint8_t varray[4];
    uint32_t vint;
  }val;

  for(int i=0;i<4;i++)
  {
    val.varray[i]=rand()%0xFF;
  }
  return val.vint;
}

void cissys_lockcreate(void** mutex)
{
  *mutex = os_mutex_create();
  cissys_assert(*mutex);
}

void cissys_lockdestory(void* mutex)
{
  cissys_assert(mutex != NULL);
  os_mutex_destroy(mutex);
}

cis_ret_t cissys_lock(void* mutex, uint32_t ms)
{
  cissys_assert(mutex != NULL);
  os_mutex_lock (mutex, ms);
  return CIS_RET_OK;
}

void cissys_unlock(void* mutex)
{
  cissys_assert(mutex != NULL);
  os_mutex_unlock (mutex);
}

bool cissys_save(uint8_t* buffer,uint32_t length)
{

  return true;
}

bool cissys_load(uint8_t* buffer,uint32_t length)
{
  return true;
}

uint8_t	cissys_getIMEI(uint8_t* buffer,uint32_t maxlen)
{
  uint8_t len=utils_strlen(IMEI);
  memcpy(buffer,IMEI,len);
  return len;
}
uint8_t cissys_getIMSI(uint8_t* buffer,uint32_t maxlen)
{
  uint8_t len=utils_strlen(IMSI);
  memcpy(buffer,IMSI,len);
  return len;
}
