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
#include "delay.h"
#include "stm32l4xx.h"
static unsigned int cpuclkfeq; 

void DelayInit(void)
{
    cpuclkfeq = SystemCoreClock;
    DEM_CR         |=  DEM_CR_TRCENA; 
		//DWT_CYCCNT      = 0u; 
    DWT_CR         |= DWT_CR_CYCCNTENA;
}


void Delayus(unsigned int usec)
{
     unsigned int startts,endts,ts;
     startts = DWT_CYCCNT;
     ts =  usec * (cpuclkfeq /(1000*1000) ); 
     endts = startts + ts;          
      if(endts > startts)        
      {
          while(DWT_CYCCNT < endts);      
       }
       else
      {
           while(DWT_CYCCNT > endts);    
           while(DWT_CYCCNT < endts); 
      }

}

void delay_us(unsigned int us)
{
        unsigned int temp;
        temp= DWT_CYCCNT;
        us *=  SystemCoreClock/1000000;
        while((unsigned int)( DWT_CYCCNT - temp)< us);
}

void delay_ms(unsigned int ms)
{
        unsigned int temp;
        temp= DWT_CYCCNT;
        ms *=  SystemCoreClock/1000; 
        while((unsigned int)( DWT_CYCCNT - temp)< ms);
}
