/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//*****************************************************************************
// includes
//*****************************************************************************
#include <stdint.h>
#include <ti/devices/cc32xx/inc/hw_types.h>
#include <ti/devices/cc32xx/driverlib/prcm.h>
#include <ti/devices/cc32xx/driverlib/rom_map.h>

//*****************************************************************************
//defines
//*****************************************************************************

/*
 * macro to resolve RTC count when reading from 40MHz domain (need to read 3
 * times and compare values to ensure proper sync with RTC running at 32786Hz)
 */
#define COUNT_WITHIN_TRESHOLD(a, b, c, th) ((((b) - (a)) <= (th)) ? (b) : (c))

//*****************************************************************************
//Typedefs
//*****************************************************************************

//*****************************************************************************
// Globals
//*****************************************************************************

//*****************************************************************************
// Internal Functions
//*****************************************************************************
/*!
    \brief          ClockSync_getRTC

    This routine get the rtc time

    \param          none
    
    \return         the function shall return the slow clock time in seconds
                    which shall be used as offset criteria to the NTP time
*/
uint64_t ClockSync_getRTC(void)
{
    uint64_t count[3];
    uint64_t curr;
    uint32_t i;

    /*
     *  get the current RTC count, using the fast interface; to use the
     *  fast interface the count must be read three times, and then
     *  the value that matches on at least two of the reads is chosen
     */
    for (i = 0; i < 3; i++)
    {
        count[i] = MAP_PRCMSlowClkCtrFastGet();
    }
    curr = COUNT_WITHIN_TRESHOLD(count[0], count[1], count[2], 1);

    /* convert to seconds by devide to 32k */
    return (curr >> 15);
}

