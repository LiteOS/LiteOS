/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2014, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

#ifndef UTILITY_H
#define UTILITY_H

#include "chip.h"
 


#define RESET_CYCLE_COUNTER()  do { \
        CoreDebug->DEMCR = CoreDebug_DEMCR_TRCENA_Msk; \
        __DSB(); DWT->LAR = 0xC5ACCE55; __DSB(); \
        DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; \
        DWT->CYCCNT = 0; \
        DWT->CTRL = DWT_CTRL_CYCCNTENA_Msk; \
    }while(0)

#define GET_CYCLE_COUNTER(x)                x=DWT->CYCCNT;   

#define LockMutex(mut, timeout)             get_lock(&mut, 1, &timeout)
    
#define ReleaseMutex(mut)                   free_lock(&mut)

#define GetResource(mut, max, timeout)      get_lock(&mut, max, &timeout)
    
#define FreeResource(mut)                   free_lock(&mut)


__STATIC_INLINE uint8_t Is_LockFree(volatile uint8_t *Lock_Variable)
{
    /* return Variable value*/
    return __LDREXB(Lock_Variable); 
    
}

__STATIC_INLINE uint8_t get_lock(volatile uint8_t *Lock_Variable, const uint8_t maxValue, volatile uint32_t *pTimeout)
{
    while (*pTimeout)
	{
        if(__LDREXB(Lock_Variable) < maxValue)
        {
          /* Set the Variable */
          while( __STREXB(((*Lock_Variable) + 1), Lock_Variable) )
          {            
            if(!(*pTimeout)--)
            {
              return 1;       // quit if timeout
            }
          }
          /* Memory access barrier */
          __DMB();
          TRACE_DEBUG("Mutex locked ");
          return 0;
        }
        
        ((*pTimeout)--);
	}
    return 1;
}



__STATIC_INLINE uint8_t free_lock(volatile uint8_t *Lock_Variable)
{
    /* Memory access barrier Ensure memory operations completed before releasing lock  */
    __DSB();
    if(__LDREXB(Lock_Variable))
    {
      __STREXB( ((*Lock_Variable) - 1), Lock_Variable);
      TRACE_DEBUG("Mutex freed ");
      __DSB();
      __DMB(); // Ensure memory operations completed before
      return 0;
    }
    else
    {
      return 1;
    }
    
    
}


#endif /* UTILITY_H */
