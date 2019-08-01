/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
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

#include "los_tick.ph"

#include "los_base.h"
#include "los_task.ph"
#include "los_swtmr.h"
#include "los_hwi.h"

#include "gd32vf103.h"
#include "n22_tmr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

LITE_OS_SEC_BSS UINT32  g_uwCyclesPerTick;

void eclic_mtip_handler(void)
{
    *(UINT64 *)(TMR_CTRL_ADDR + TMR_MTIME) = 0;
    /* vPortSetupTimerInterrupt() uses LPIT0 to generate the tick interrupt. */
    osTickHandler();
}


/*lint -save -e40 -e10 -e26 -e1013*/
/*****************************************************************************
Function   : LOS_TickHandler
Description: los system tick handler 
Input   : none
output  : none
return  : none
*****************************************************************************/
void LOS_TickHandler(void)
{
    UINT32 uwIntSave;

    uwIntSave = LOS_IntLock();
    g_vuwIntCount++;
    LOS_IntRestore(uwIntSave);

    osTickHandler();
    
    uwIntSave = LOS_IntLock();
    g_vuwIntCount--;
    LOS_IntRestore(uwIntSave);
    
    return ;
}

/*****************************************************************************
Function   : osTickStart
Description: Configure Tick Interrupt Start
Input   : none
output  : none
return  : LOS_OK - Success , or LOS_ERRNO_TICK_CFG_INVALID - failed
*****************************************************************************/
LITE_OS_SEC_TEXT_INIT UINT32 osTickStart(VOID)
{
    UINT32 uwRet;

//    if ((0 == OS_SYS_CLOCK)
//        || (0 == LOSCFG_BASE_CORE_TICK_PER_SECOND)
//        || (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK))
//    {
//        return LOS_ERRNO_TICK_CFG_INVALID;
//    }
//
//#if (LOSCFG_PLATFORM_HWI == YES)
//#if (OS_HWI_WITH_ARG == YES)
//    osSetVector(SysTick_IRQn, (HWI_PROC_FUNC)osTickHandler, NULL);
//#else
//    osSetVector(SysTick_IRQn, osTickHandler);
//#endif
//#endif

    g_uwCyclesPerTick = OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    g_ullTickCount = 0;

//    uwRet = SysTick_Config(OS_SYS_CLOCK/LOSCFG_BASE_CORE_TICK_PER_SECOND);
//    if (uwRet == 1)
//    {
//        return LOS_ERRNO_TICK_PER_SEC_TOO_SMALL;
//    }
    *(UINT64 *)(TMR_CTRL_ADDR + TMR_MTIMECMP) = OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND / 4;

    *(UINT64 *)(TMR_CTRL_ADDR + TMR_MTIME) = 0;
    eclic_irq_enable(CLIC_INT_TMR, 0, 0);

    g_bSysTickStart = TRUE;

    return LOS_OK;
}

/*****************************************************************************
Function   : LOS_SetTickSycle
Description: set g_uwCyclesPerTick value
Input   : ticks, the cpu Sycles per tick
output  : none
return  : none
*****************************************************************************/
void LOS_SetTickSycle(UINT32 ticks)
{
    g_uwCyclesPerTick = ticks;
    return ;
}

/*****************************************************************************
Function   : LOS_GetCpuCycle
Description: Get System cycle count
Input   : none
output  : puwCntHi  --- CpuTick High 4 byte
          puwCntLo  --- CpuTick Low 4 byte
return  : none
*****************************************************************************/
LITE_OS_SEC_TEXT_MINOR VOID LOS_GetCpuCycle(UINT32 *puwCntHi, UINT32 *puwCntLo)
{
    UINT64 ullSwTick;
    UINT64 ullCycle;
    UINT32 uwIntSta;
    UINT32 uwHwCycle;
    UINTPTR uwIntSave;

    uwIntSave = LOS_IntLock();

    ullSwTick = g_ullTickCount;

//<nvd>:TODO
//    uwHwCycle = *(volatile UINT32*)OS_SYSTICK_CURRENT_REG;
//    uwIntSta  = *(volatile UINT32*)OS_NVIC_INT_CTRL;
//
//    /*tick has come, but may interrupt environment, not counting the Tick interrupt response, to do +1 */
//    if (((uwIntSta & 0x4000000) != 0))
//    {
//        uwHwCycle = *(volatile UINT32*)OS_SYSTICK_CURRENT_REG;
//        ullSwTick++;
//    }

    ullCycle = (((ullSwTick) * g_uwCyclesPerTick) + (g_uwCyclesPerTick - uwHwCycle));
    *puwCntHi = ullCycle >> 32;
    *puwCntLo = ullCycle & 0xFFFFFFFFU;

    LOS_IntRestore(uwIntSave);

    return;
}

UINT64 osKernelGetTickCount (void)
{
    UINT64 ticks;
    UINTPTR uvIntSave;

    if(OS_INT_ACTIVE)
    {
        ticks = 0U;
    }
    else
    {
        uvIntSave = LOS_IntLock();
        ticks = g_ullTickCount;
        LOS_IntRestore(uvIntSave);
    }

    return ticks;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
