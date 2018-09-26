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
 /** ============================================================================
 *  @file       clock_sync.h
 *
 *  @brief      clock synchronization
 *
 *  ============================================================================
 */

/*! @cond*/

#ifndef __CLOCK_SYNC_H__
#define __CLOCK_SYNC_H__

//*****************************************************************************
// includes
//*****************************************************************************
#include <stdint.h>
#include <time.h>

//*****************************************************************************
// defines
//*****************************************************************************

/* Time Zones List */
#define CLOCKSYNC_TIMEZONE_DATELINE             (-720)  //    (GMT-12:00) International Date Line West
#define CLOCKSYNC_TIMEZONE_SAMOA                (-660)  //    (GMT-11:00) Midway Island, Samoa
#define CLOCKSYNC_TIMEZONE_HAWAIIAN             (-600)  //    (GMT-10:00) Hawaii
#define CLOCKSYNC_TIMEZONE_ALASKAN              (-540)  //    (GMT-09:00) Alaska
#define CLOCKSYNC_TIMEZONE_PACIFIC              (-480)  //    (GMT-08:00) Pacific Time (US and Canada); Tijuana
#define CLOCKSYNC_TIMEZONE_MOUNTAIN             (-420)  //    (GMT-07:00) Mountain Time (US and Canada)
#define CLOCKSYNC_TIMEZONE_US_MOUNTAIN          (-420)  //    (GMT-07:00) Arizona
#define CLOCKSYNC_TIMEZONE_MOUNTAIN_MEXICO      (-420)  //    (GMT-07:00) Chihuahua, La Paz, Mazatlan
#define CLOCKSYNC_TIMEZONE_CANADA_CENTRAL       (-420)  //    (GMT-06:00) Saskatchewan
#define CLOCKSYNC_TIMEZONE_CENTRAL_AMERICA      (-360)  //    (GMT-06:00) Central America
#define CLOCKSYNC_TIMEZONE_CENTRAL              (-360)  //    (GMT-06:00) Central Time (US and Canada)
#define CLOCKSYNC_TIMEZONE_CENTRAL_MEXICO       (-360)  //    (GMT-06:00) Guadalajara, Mexico City, Monterrey
#define CLOCKSYNC_TIMEZONE_SA_PACIFIC           (-300)  //    (GMT-05:00) Bogota, Lima, Quito
#define CLOCKSYNC_TIMEZONE_US_EASTERN           (-300)  //    (GMT-05:00) Indiana (East)
#define CLOCKSYNC_TIMEZONE_EASTERN              (-300)  //    (GMT-05:00) Eastern Time (US and Canada)
#define CLOCKSYNC_TIMEZONE_CENTRAL_BRAZILIAN    (-240)  //    (GMT-04:00) Manaus
#define CLOCKSYNC_TIMEZONE_SA_WESTERN           (-240)  //    (GMT-04:00) Caracas, La Paz
#define CLOCKSYNC_TIMEZONE_PACIFIC_SA           (-240)  //    (GMT-04:00) Santiago
#define CLOCKSYNC_TIMEZONE_ATLANTIC             (-240)  //    (GMT-04:00) Atlantic Time (Canada)
#define CLOCKSYNC_TIMEZONE_GREENLAND            (-180)  //    (GMT-03:00) Greenland
#define CLOCKSYNC_TIMEZONE_SA_EASTERN           -180)  //    (GMT-03:00) Buenos Aires, Georgetown
#define CLOCKSYNC_TIMEZONE_E_SOUTH_AMERICA      (-180)  //    (GMT-03:00) Brasilia
#define CLOCKSYNC_TIMEZONE_MID_ATLANTIC         (-120)  //    (GMT-02:00) Mid-Atlantic
#define CLOCKSYNC_TIMEZONE_CAPE_VERDE           (-60)   //    (GMT-01:00) Cape Verde Islands
#define CLOCKSYNC_TIMEZONE_AZORES               (-60)   //    (GMT-01:00) Azores
#define CLOCKSYNC_TIMEZONE_GMT                  (0)     //    (GMT) Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London
#define CLOCKSYNC_TIMEZONE_GREENWICH            (0)     //    (GMT) Casablanca, Monrovia
#define CLOCKSYNC_TIMEZONE_ROMANCE              (60)    //    (GMT+01:00) Brussels, Copenhagen, Madrid, Paris
#define CLOCKSYNC_TIMEZONE_CENTRAL_EUROPE       (60)    //    (GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague
#define CLOCKSYNC_TIMEZONE_CENTRAL_EUROPEAN     (60)    //    (GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb
#define CLOCKSYNC_TIMEZONE_W_CENTRAL_AFRICA     (60)    //    (GMT+01:00) West Central Africa
#define CLOCKSYNC_TIMEZONE_W_EUROPE             (60)    //    (GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna
#define CLOCKSYNC_TIMEZONE_E_EUROPE             (120)   //    (GMT+02:00) Minsk
#define CLOCKSYNC_TIMEZONE_SOUTH_AFRICA         (120)   //    (GMT+02:00) Harare, Pretoria
#define CLOCKSYNC_TIMEZONE_GTB                  (120)   //    (GMT+02:00) Athens, Bucharest, Istanbul
#define CLOCKSYNC_TIMEZONE_EGYPT                (120)   //    (GMT+02:00) Cairo
#define CLOCKSYNC_TIMEZONE_FLE                  (120)   //    (GMT+02:00) Helsinki, Kiev, Riga, Sofia, Tallinn, Vilnius
#define CLOCKSYNC_TIMEZONE_NAMIBIA              (120)   //    (GMT+02:00) Windhoek
#define CLOCKSYNC_TIMEZONE_ISRAEL               (120)   //    (GMT+02:00) Jerusalem
#define CLOCKSYNC_TIMEZONE_ARABIC               (180)   //    (GMT+03:00) Baghdad
#define CLOCKSYNC_TIMEZONE_RUSSIAN              (180)   //    (GMT+03:00) Moscow, St. Petersburg, Volgograd
#define CLOCKSYNC_TIMEZONE_ARAB                 (180)   //    (GMT+03:00) Kuwait, Riyadh
#define CLOCKSYNC_TIMEZONE_E_AFRICA             (180)   //    (GMT+03:00) Nairobi
#define CLOCKSYNC_TIMEZONE_IRAN                 (210)   //    (GMT+03:30) Tehran
#define CLOCKSYNC_TIMEZONE_NEWFOUNDLAND         (210)   //    (GMT-03:30) Newfoundland and Labrador
#define CLOCKSYNC_TIMEZONE_CAUCASUS             (240)   //    (GMT+04:00) Yerevan
#define CLOCKSYNC_TIMEZONE_AZERBAIJAN           (240)   //    (GMT+04:00) Baku
#define CLOCKSYNC_TIMEZONE_GEORGIAN             (240)   //    (GMT+04:00) Tblisi
#define CLOCKSYNC_TIMEZONE_ARABIAN              (240)   //    (GMT+04:00) Abu Dhabi, Muscat
#define CLOCKSYNC_TIMEZONE_AFGHANISTAN          (270)   //    (GMT+04:30) Kabul
#define CLOCKSYNC_TIMEZONE_EKATERINBURG         (300)   //    (GMT+05:00) Ekaterinburg
#define CLOCKSYNC_TIMEZONE_WEST_ASIA            (300)   //    (GMT+05:00) Islamabad, Karachi, Tashkent
#define CLOCKSYNC_TIMEZONE_INDIA                (330)   //    (GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi
#define CLOCKSYNC_TIMEZONE_NEPAL                (345)   //    (GMT+05:45) Kathmandu
#define CLOCKSYNC_TIMEZONE_CENTRAL_ASIA         (360)   //    (GMT+06:00) Astana, Dhaka
#define CLOCKSYNC_TIMEZONE_N_CENTRAL_ASIA       (360)   //    (GMT+06:00) Almaty, Novosibirsk
#define CLOCKSYNC_TIMEZONE_SRI_LANKA            (360)   //    (GMT+06:00) Sri Jayawardenepura
#define CLOCKSYNC_TIMEZONE_MYANMAR              (390)   //    (GMT+06:30) Yangon (Rangoon)
#define CLOCKSYNC_TIMEZONE_NORTH_ASIA           (420)   //    (GMT+07:00) Krasnoyarsk
#define CLOCKSYNC_TIMEZONE_SE_ASIA              (420)   //    (GMT+07:00) Bangkok, Hanoi, Jakarta
#define CLOCKSYNC_TIMEZONE_SINGAPORE            (480)   //    (GMT+08:00) Kuala Lumpur, Singapore
#define CLOCKSYNC_TIMEZONE_CHINA                (480)   //    (GMT+08:00) Beijing, Chongqing, Hong Kong SAR, Urumqi
#define CLOCKSYNC_TIMEZONE_TAIPEI               (480)   //    (GMT+08:00) Taipei
#define CLOCKSYNC_TIMEZONE_NORTH_ASIA_EAST      (480)   //    (GMT+08:00) Irkutsk, Ulaanbaatar
#define CLOCKSYNC_TIMEZONE_W_AUSTRALIA          (480)   //    (GMT+08:00) Perth
#define CLOCKSYNC_TIMEZONE_KOREA                (540)   //    (GMT+09:00) Seoul
#define CLOCKSYNC_TIMEZONE_TOKYO                (540)   //    (GMT+09:00) Osaka, Sapporo, Tokyo
#define CLOCKSYNC_TIMEZONE_YAKUTSK              (540)   //    (GMT+09:00) Yakutsk
#define CLOCKSYNC_TIMEZONE_CEN_AUSTRALIA        (570)   //    (GMT+09:30) Adelaide
#define CLOCKSYNC_TIMEZONE_AUS_CENTRAL          (570)   //    (GMT+09:30) Darwin
#define CLOCKSYNC_TIMEZONE_WEST_PACIFIC         (600)   //    (GMT+10:00) Guam, Port Moresby
#define CLOCKSYNC_TIMEZONE_AUS_EASTERN          (600)   //    (GMT+10:00) Canberra, Melbourne, Sydney
#define CLOCKSYNC_TIMEZONE_E_AUSTRALIA          (600)   //    (GMT+10:00) Brisbane
#define CLOCKSYNC_TIMEZONE_TASMANIA             (600)   //    (GMT+10:00) Hobart
#define CLOCKSYNC_TIMEZONE_VLADIVOSTOK          (600)   //    (GMT+10:00) Vladivostok
#define CLOCKSYNC_TIMEZONE_CENTRAL_PACIFIC      (660)   //    (GMT+11:00) Magadan, Solomon Islands, New Caledonia
#define CLOCKSYNC_TIMEZONE_FIJI                 (720)   //    (GMT+12:00) Fiji Islands, Kamchatka, Marshall Islands
#define CLOCKSYNC_TIMEZONE_NEW_ZEALAND          (720)   //    (GMT+12:00) Auckland, Wellington
#define CLOCKSYNC_TIMEZONE_TONGA                (780)   //    (GMT+13:00) Nuku'alofa



/* error codes */
#define CLOCKSYNC_ERROR_MUTEX                   (-201)  // error in create, lock or unlock the mutex
#define CLOCKSYNC_ERROR_SNTP                    (-202)  // error in SNTP module
#define CLOCKSYNC_ERROR_INTERVAL                (-203)  // interval time did not pass from last update 
#define CLOCKSYNC_ERROR_TIMEZONE                (-204)  // timezone out of range
#define CLOCKSYNC_ERROR_VALUE                   (-205)  // invalid api value

/* config params */
#define CLOCKSYNC_PARAM_INTERVAL                (1)     // interval time between clock updates
#define CLOCKSYNC_PARAM_UPDATE_NWP              (2)     // update NWP clock or not

//*****************************************************************************
// typedefs
//*****************************************************************************


/*! @endcond*/

//*****************************************************************************
// function prototypes
//*****************************************************************************

/*!
    \brief          ClockSync_getTimeZone

    This routine get the offset time zone

    \param          none
    
    \return         Upon successful, the function shall return time offset 
                    in minutes from UTC, else return 0xFFFF
*/
int16_t ClockSync_getTimeZone(void);


/*!
    \brief          ClockSync_setTimeZone

    This routine set the offset time zone

    \param          timezone         -   offset from UTC in minutes
    
    \return         Upon successful, the function shall return 0,
                    else return error
*/
int32_t ClockSync_setTimeZone(int16_t timezone);


/*!
    \brief          ClockSync_convert

    This routine convert GMT time (gmTime) to local time in tm struct (localTime)
    The routine make use of time zone.

    \param          gmTime           -   number of seconds since 1.1.1900
    \param          localTime        -   point to tm struct which hold the result local time
    
    \return         Upon successful, the function shall return 0,
                    else return error
*/
int32_t ClockSync_convert(uint32_t gmTime, struct tm *localTime);


/*!
    \brief          ClockSync_get

    This routine update the local time if needed
    and return the updated time in localTime

    \param          localTime     -  point to tm struct which hold the result local time 
    
    \return         Upon successful, the function shall return 0,
                    else return error
*/
int32_t ClockSync_get(struct tm *localTime);


/*!
    \brief          ClockSync_update

    This routine update the time by SNTP request
    in case the elapsed time between updates was passed.
    The routine also update the NWP clock conditionally.

    \param          none
    
    \return         Upon successful, the function shall return 0,
                    else return error
*/
int32_t ClockSync_update(void);

/*!
    \brief          ClockSync_getRTC

    This routine get the rtc time

    \param          none
    
    \return         the function shall return the slow clock time in seconds
                    which shall be used as offset criteria to the NTP time
*/
uint64_t ClockSync_getRTC(void);

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
int32_t ClockSync_config(uint8_t param, uint32_t value);


/** @} */



#endif /* __CLOCK_SYNC_H__ */

