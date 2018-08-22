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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <ti/net/sntp/sntp.h>
#include <ti/net/utils/clock_sync.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/dpl/MutexP.h>

//*****************************************************************************
//defines
//*****************************************************************************


#define CLOCKSYNC_INTERVAL              (60) /* interval time in seconds between clock updates */
#define CLOCKSYNC_UPDATE_NWP            (0)  /* 1 - Update NWP clock; 0 - do not update */

#define CLOCKSYNC_BASE_YAER             (1900)
#define CLOCKSYNC_SECONDS_PER_DAY       (24 * 3600)
#define CLOCKSYNC_LEAP_YEAR(year)       (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define CLOCKSYNC_DAYS_PER_YEAR(year)   (CLOCKSYNC_LEAP_YEAR(year) ? 366 : 365)
#define CLOCKSYNC_FIRST_DAY             (1) /* monday was the day in 1.1.1900 */
#define CLOCKSYNC_TIMEZONE_MIN          (-720) /* -12 hours */
#define CLOCKSYNC_TIMEZONE_MAX	        (840)  /* +14 hours */
#define CLOCKSYNC_TIMEOUT               (1500) /* Time to wait for reply from server in milliseconds */

//*****************************************************************************
//Typedefs
//*****************************************************************************
/* Control block definition */
typedef struct _ClockSync_ControlBlock_t_
{
    int64_t diff;
    uint64_t lastUpdate;
    int32_t timezone;
    MutexP_Handle mutex;
    uint32_t interval;
    uint32_t updateNwp;
}ClockSync_ControlBlock;


//*****************************************************************************
// Globals
//*****************************************************************************
ClockSync_ControlBlock ClockSync_CB = 
{
    0,0,0,NULL,CLOCKSYNC_INTERVAL,CLOCKSYNC_UPDATE_NWP
};

const uint8_t ClockSync_daysNumber[2][12] = 
{
  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

//*****************************************************************************
// Internal Functions
//*****************************************************************************

/*!
    \brief          ClockSync_mutexLock

    This routine obtains the module mutex

    \param          none
    
    \return         Upon successful, the function shall return key,
                    which will be used while releasing the mutex,
                    else return 0xFFFF
*/
uintptr_t ClockSync_mutexLock(void)
{
    uintptr_t key;

    if (ClockSync_CB.mutex == NULL)
    {
        ClockSync_CB.mutex = MutexP_create(NULL);
        if (ClockSync_CB.mutex == NULL)
        {
            return 0xFFFF;
        }
    }
    key = MutexP_lock(ClockSync_CB.mutex);
    return key;
}

/*!
    \brief          ClockSync_mutexUnlock

    This routine release the module mutex

    \param          key            -   mutex key to release 
    
    \return         none
*/
void ClockSync_mutexUnlock(uintptr_t key)
{
    MutexP_unlock(ClockSync_CB.mutex,key);
}


//*****************************************************************************
// API Functions
//*****************************************************************************

/*!
    \brief          ClockSync_config

    This routine configure specific parameter

    \param          param  - parameter to config
                    CLOCKSYNC_PARAM_INTERVAL - interval time between clock updates
                    CLOCKSYNC_PARAM_UPDATE_NWP - update NWP clock or not
    \param          value  - parameter value
                    for CLOCKSYNC_PARAM_INTERVAL - value in seconds
                    for CLOCKSYNC_PARAM_UPDATE_NWP - 0 or 1
    \return         Upon successful, the function shall return 0 
                    else return error
*/
int32_t ClockSync_config(uint8_t param, uint32_t value)
{
    uintptr_t key;
    int32_t status = 0;

    key = ClockSync_mutexLock();
    if (key != 0xFFFF)
    {
        switch (param)
        {
            case CLOCKSYNC_PARAM_INTERVAL:
                ClockSync_CB.interval = value;
            break;
            case CLOCKSYNC_PARAM_UPDATE_NWP:
                ClockSync_CB.updateNwp = value;
            break;
            default:
                status = CLOCKSYNC_ERROR_VALUE;
            break;
        }
        ClockSync_mutexUnlock(key);
    }
    else 
    {
        status = CLOCKSYNC_ERROR_MUTEX;
    }

    return (status);
}

/*!
    \brief          ClockSync_getTimeZone

    This routine get the offset time zone

    \param          none
    
    \return         Upon successful, the function shall return time offset 
                    in minutes from UTC, else return 0xFFFF
*/
int16_t ClockSync_getTimeZone(void)
{
    uintptr_t key;
    int16_t timezone = CLOCKSYNC_TIMEZONE_GMT;

    key = ClockSync_mutexLock();
    if (key != 0xFFFF)
    {
        /* convert time zone to minutes */
        timezone = ClockSync_CB.timezone / 60;

        ClockSync_mutexUnlock(key);
    }
    return (timezone);
}


/*!
    \brief          ClockSync_setTimeZone

    This routine set the offset time zone

    \param          timezone         -   offset from UTC in minutes
    
    \return         Upon successful, the function shall return 0,
                    else return error
*/
int32_t ClockSync_setTimeZone(int16_t timezone)
{
    uintptr_t key;

    key = ClockSync_mutexLock();
    if (key == 0xFFFF)
    {
        return CLOCKSYNC_ERROR_MUTEX;
    }
    if ((timezone < CLOCKSYNC_TIMEZONE_MIN) || (timezone > CLOCKSYNC_TIMEZONE_MAX))
    {
        return CLOCKSYNC_ERROR_TIMEZONE;
    }
    /* convert time zone from minutes to seconds */
    ClockSync_CB.timezone = timezone * 60;

    ClockSync_mutexUnlock(key);

    return 0;
}

/*!
    \brief          ClockSync_convert

    This routine convert GMT time (gmTime) to local time in tm struct (localTime)
    The routine make use of time zone.

    \param          gmTime           -   number of seconds since 1.1.1900
    \param          localTime        -   point to tm struct
    
    \return         Upon successful, the function shall return 0,
                    else return error
*/
int32_t ClockSync_convert(uint32_t gmTime, struct tm *localTime)
{
    uint32_t currTime;
    uint32_t yearDay;
    uint32_t year;

    if (localTime == NULL)
    {
        return CLOCKSYNC_ERROR_VALUE;
    }
    /* convert timer from GMT to local */
    gmTime += (ClockSync_getTimeZone() * 60);
    
    /* break down gmTime to time and date */
    currTime = gmTime % CLOCKSYNC_SECONDS_PER_DAY;
    yearDay = gmTime / CLOCKSYNC_SECONDS_PER_DAY;

    localTime->tm_hour = currTime / 3600;
    localTime->tm_min = (currTime % 3600) / 60;
    localTime->tm_sec = currTime % 60;
    localTime->tm_wday = (yearDay + CLOCKSYNC_FIRST_DAY) % 7;
  
    for (year = CLOCKSYNC_BASE_YAER; yearDay >= CLOCKSYNC_DAYS_PER_YEAR(year); year++)
    {
        yearDay -= CLOCKSYNC_DAYS_PER_YEAR(year);
    }
    localTime->tm_year = year - CLOCKSYNC_BASE_YAER;
    localTime->tm_yday = yearDay;

    for (localTime->tm_mon = 0; (yearDay >= ClockSync_daysNumber[CLOCKSYNC_LEAP_YEAR(year)][localTime->tm_mon]); localTime->tm_mon++)
    {
        yearDay -= ClockSync_daysNumber[CLOCKSYNC_LEAP_YEAR(year)][localTime->tm_mon];
    }
    localTime->tm_mday = yearDay + 1;
    localTime->tm_isdst = 0;

    return 0;
}

/*!
    \brief          ClockSync_get

    This routine update the local time if needed
    and return the updated time in localTime

    \param          localTime     -  point to tm struct which hold the result local time 
    
    \return         Upon successful, the function shall return 0,
                    else return error
*/
int32_t ClockSync_get(struct tm *localTime)
{
    int32_t status = 0;
    uint64_t rtcTime; 
    uint32_t syncTime;
    uintptr_t key;

    key = ClockSync_mutexLock();
    if (key == 0xFFFF)
    {
        return CLOCKSYNC_ERROR_MUTEX;
    }

    if (localTime == NULL)
    {
        return CLOCKSYNC_ERROR_VALUE;
    }

    /* check for sync update */
    status = ClockSync_update();

    /* get the RTC time */
    rtcTime = ClockSync_getRTC();
    /* calculate the local time as offset from NTP time */
    syncTime = rtcTime - ClockSync_CB.diff;
    /* convert the local time to tm struct */
    ClockSync_convert(syncTime,localTime);

    ClockSync_mutexUnlock(key);
    
    return status;
}

/*!
    \brief          ClockSync_update

    This routine update the time by SNTP request
    in case the elapsed time between updates was passed.
    The routine also update the NWP clock conditionally.

    \param          none
    
    \return         Upon successful, the function shall return 0,
                    else return error
*/
int32_t ClockSync_update(void)
{
    int32_t status = -1;
    struct tm netTime;
    uint64_t rtcTime; 
    SlDateTime_t nwpTime;
    uintptr_t key;
    uint64_t netSecs;
    SlNetSock_Timeval_t timeout;

    key = ClockSync_mutexLock();    
    if (key == 0xFFFF)
    {
        return CLOCKSYNC_ERROR_MUTEX;
    }

    /* get the RTC time */
    rtcTime = ClockSync_getRTC();
    if (((rtcTime - ClockSync_CB.lastUpdate) > ClockSync_CB.interval) || (ClockSync_CB.lastUpdate == 0))
    {
        /* SNTP request */
        timeout.tv_sec = CLOCKSYNC_TIMEOUT /1000;
        timeout.tv_usec = (CLOCKSYNC_TIMEOUT %1000) * 1000;
        status = SNTP_getTime(NULL,0,&timeout,&netSecs);
        if (status == 0)
        {
            /* remove the Seconds Fraction */
            //netSecs = (uint32_t)(netSecs);
            netSecs = (0xFFFFFFFF00000000 & netSecs) >> 32;

            /* update offset between RTC and NTP */
            ClockSync_CB.diff = rtcTime - netSecs;

            /* update last sync time */
            ClockSync_CB.lastUpdate = rtcTime;

            /* check for NWP update */
            if (ClockSync_CB.updateNwp == 1)
            {
                /* convert the local time to tm struct */
                ClockSync_convert(netSecs,&netTime);

                nwpTime.tm_day  = netTime.tm_mday;
                nwpTime.tm_mon  = netTime.tm_mon + 1;
                nwpTime.tm_year = netTime.tm_year + CLOCKSYNC_BASE_YAER;
                nwpTime.tm_hour = netTime.tm_hour;
                nwpTime.tm_min  = netTime.tm_min;
                nwpTime.tm_sec  = netTime.tm_sec;
                sl_DeviceSet(SL_DEVICE_GENERAL, SL_DEVICE_GENERAL_DATE_TIME, sizeof(SlDateTime_t), (uint8_t *)(&nwpTime));
            }
        }
        else
        {
            status = CLOCKSYNC_ERROR_SNTP;
        }
    }
    else
    {
        status = CLOCKSYNC_ERROR_INTERVAL;
    }
    
    ClockSync_mutexUnlock(key);
    
    return (status);
}
