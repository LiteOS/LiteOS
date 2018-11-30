/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     ck_rtc.c
 * @brief    CSI Source File for RTC Driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <csi_config.h>
#include <stdbool.h>
#include <string.h>
#include "ck_rtc.h"
#include "csi_core.h"
#include "drv_rtc.h"
#include "soc.h"

#define ERR_RTC(errno) (CSI_DRV_ERRNO_RTC_BASE | errno)

#define RTC_NULL_PARAM_CHK(para)                    \
    do {                                        \
        if (para == NULL) {                     \
            return ERR_RTC(DRV_ERROR_PARAMETER);   \
        }                                       \
    } while (0)

typedef struct {
    uint32_t base;
    uint32_t irq;
    rtc_event_cb_t cb_event;
    struct tm rtc_base;
} ck_rtc_priv_t;

extern void mdelay(uint32_t ms);
extern int32_t target_get_rtc_count(void);
extern int32_t target_get_rtc(int32_t idx, uint32_t *base, uint32_t *irq);

static ck_rtc_priv_t rtc_instance[CONFIG_RTC_NUM];
static uint8_t leap_year[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static uint8_t noleap_year[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const uint16_t g_noleap_daysbeforemonth[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
//static const uint16_t g_leap_daysbeforemonth[13] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};

static const rtc_capabilities_t rtc_capabilities = {
    .interrupt_mode = 1,    /* supports Interrupt mode */
    .wrap_mode = 0          /* supports wrap mode */
};

static inline int clock_isleapyear(int year)
{
    return (year % 400) ? ((year % 100) ? ((year % 4) ? 0 : 1) : 0) : 1;
}

static inline int32_t ck_rtc_enable(ck_rtc_reg_t *addr)
{
    uint32_t value;

    value = addr->RTC_CCR;
    value |= 1 << 2;
    addr->RTC_CCR = value;
    return 0;
}

static inline int32_t ck_rtc_disable(ck_rtc_reg_t *addr)
{
    uint32_t value;

    value = addr->RTC_CCR;
    value &= ~(1 << 2);
    addr->RTC_CCR = value;
    return 0;
}

static int ck_rtc_settime(ck_rtc_reg_t *addr, uint32_t settime)
{
    addr->RTC_CLR = settime;
    return 0;
}

static uint64_t ck_rtc_readtime(ck_rtc_reg_t *addr)
{
    uint64_t time;
    uint64_t load;

    time = addr->RTC_CCVR;
    load = addr->RTC_CLR;
#ifdef CONFIG_CHIP_CH2201
    time = (time - load) * 32768 * 1000 / 7394 ;
#elif CONFIG_CHIP_PHOBOS
    time = (time - load) * 16384 / (SYSTEM_CLOCK / 1000);
#else
    uint32_t clockdiv = addr->RTC_DIV;
    clockdiv = (clockdiv + 1) * 2;
    time = (time - load) / ((SYSTEM_CLOCK / clockdiv) / 1000);
#endif
    return time;
}

static int ck_check_tm_ok(struct tm *rtctime)
{

    if (rtctime->tm_year < 70 || rtctime->tm_year >= 200) {
        goto error_time;
    }

    int32_t leap = 1;

    leap = clock_isleapyear(rtctime->tm_year + 1900);

    if (rtctime->tm_sec < 0 || rtctime->tm_sec >= 60) {
        goto error_time;
    }

    if (rtctime->tm_min < 0 || rtctime->tm_min >= 60) {
        goto error_time;
    }

    if (rtctime->tm_hour < 0 || rtctime->tm_hour >= 24) {
        goto error_time;
    }

    if (rtctime->tm_mon < 0 || rtctime->tm_mon > 12) {	// rtctime->tm_mon >= 12 20181128 去除等于号
        goto error_time;
    }

    if (leap) {
        if (rtctime->tm_mday < 1 || rtctime->tm_mday > leap_year[rtctime->tm_mon]) {
            goto error_time;
        }
    } else {
        if (rtctime->tm_mday < 1 || rtctime->tm_mday > noleap_year[rtctime->tm_mon]) {
            goto error_time;
        }
    }

    return 0;
error_time:
    return ERR_RTC(RTC_ERROR_TIME);

}

static int clock_daysbeforemonth(int month, bool leapyear)
{
    int retval = g_noleap_daysbeforemonth[month];

    if (month >= 2 && leapyear) {
        retval++;
    }

    return retval;
}

static time_t clock_calendar2utc(int year, int month, int day)
{
    time_t days;

    /* Years since epoch in units of days (ignoring leap years). */

    days = (year - 1970) * 365;

    /* Add in the extra days for the leap years prior to the current year. */

    days += (year - 1969) >> 2;

    /* Add in the days up to the beginning of this month. */

    days += (time_t)clock_daysbeforemonth(month, clock_isleapyear(year));

    /* Add in the days since the beginning of this month (days are 1-based). */

    days += day - 1;

    /* Then convert the seconds and add in hours, minutes, and seconds */

    return days;
}


time_t mktime(struct tm *tp)
{
    time_t ret;
    time_t jdn;

    /* Get the EPOCH-relative julian date from the calendar year,
     * month, and date
     */

    ret = ck_check_tm_ok(tp);

    if (ret < 0) {
        return -1;
    }

    jdn = clock_calendar2utc(tp->tm_year + 1900, tp->tm_mon, tp->tm_mday);

    /* Return the seconds into the julian day. */

    ret = ((jdn * 24 + tp->tm_hour) * 60 + tp->tm_min) * 60 + tp->tm_sec;

    return ret;
}
static void clock_utc2calendar(time_t days, int *year, int *month,
                               int *day)
{

    /* There is one leap year every four years, so we can get close with the
     * following:
     */

    int value   = days  / (4 * 365 + 1); /* Number of 4-years periods since the epoch */
    days   -= value * (4 * 365 + 1); /* Remaining days */
    value <<= 2;                     /* Years since the epoch */

    /* Then we will brute force the next 0-3 years */
    bool leapyear;
    int  tmp;

    for (; ;) {
        /* Is this year a leap year (we'll need this later too) */

        leapyear = clock_isleapyear(value + 1970);

        /* Get the number of days in the year */

        tmp = (leapyear ? 366 : 365);

        /* Do we have that many days? */

        if (days >= tmp) {
            /* Yes.. bump up the year */

            value++;
            days -= tmp;
        } else {
            /* Nope... then go handle months */

            break;
        }
    }

    /* At this point, value has the year and days has number days into this year */

    *year = 1970 + value;

    /* Handle the month (zero based) */
    int  min = 0;
    int  max = 11;

    do {
        /* Get the midpoint */

        value = (min + max) >> 1;

        /* Get the number of days that occurred before the beginning of the month
         * following the midpoint.
         */

        tmp = clock_daysbeforemonth(value + 1, leapyear);

        /* Does the number of days before this month that equal or exceed the
         * number of days we have remaining?
         */

        if (tmp > days) {
            /* Yes.. then the month we want is somewhere from 'min' and to the
             * midpoint, 'value'.  Could it be the midpoint?
             */

            tmp = clock_daysbeforemonth(value, leapyear);

            if (tmp > days) {
                /* No... The one we want is somewhere between min and value-1 */

                max = value - 1;
            } else {
                /* Yes.. 'value' contains the month that we want */

                break;
            }
        } else {
            /* No... The one we want is somwhere between value+1 and max */

            min = value + 1;
        }

        /* If we break out of the loop because min == max, then we want value
         * to be equal to min == max.
         */

        value = min;
    } while (min < max);

    /* The selected month number is in value. Subtract the number of days in the
     * selected month
     */

    days -= clock_daysbeforemonth(value, leapyear);

    /* At this point, value has the month into this year (zero based) and days has
     * number of days into this month (zero based)
     */

    *month = value + 1; /* 1-based */
    *day   = days + 1;  /* 1-based */
}


struct tm *gmtime_r(const time_t *timer, struct tm *result)
{
    time_t epoch;
    time_t jdn;
    int    year;
    int    month;
    int    day;
    int    hour;
    int    min;
    int    sec;

    /* Get the seconds since the EPOCH */

    epoch = *timer;

    /* Convert to days, hours, minutes, and seconds since the EPOCH */

    jdn    = epoch / SEC_PER_DAY;
    epoch -= SEC_PER_DAY * jdn;

    hour   = epoch / SEC_PER_HOUR;
    epoch -= SEC_PER_HOUR * hour;

    min    = epoch / SEC_PER_MIN;
    epoch -= SEC_PER_MIN * min;

    sec    = epoch;

    /* Convert the days since the EPOCH to calendar day */

    clock_utc2calendar(jdn, &year, &month, &day);


    /* Then return the struct tm contents */

    result->tm_year  = (int)year - 1900; /* Relative to 1900 */
    result->tm_mon   = (int)month - 1;   /* zero-based */
    result->tm_mday  = (int)day;         /* one-based */
    result->tm_hour  = (int)hour;
    result->tm_min   = (int)min;
    result->tm_sec   = (int)sec;
    return result;
}
static int ck_rtc_setmarchtime(ck_rtc_reg_t *addr, int64_t settime)
{
    int64_t time = settime;

    if (settime < 0 || time >= 0x8000000000000000) {
        return ERR_RTC(DRV_ERROR_PARAMETER);
    }

#ifdef CONFIG_CHIP_CH2201
    time  = time * 7394 / 32768;
    time  = (time < 5) ? 5:time;
#elif CONFIG_CHIP_PHOBOS
    time  = time * 20000 / 16384;
#else
    uint32_t clockdiv = addr->RTC_DIV;
    clockdiv = (clockdiv + 1) * 2;
    time  = time * ((SYSTEM_CLOCK / clockdiv) / 1000);
#endif

	//
    time += addr->RTC_CCVR;

    addr->RTC_CMR = (uint32_t)time;
    return 0;
}

static int32_t ck_rtc_int_enable(ck_rtc_reg_t *addr)
{
#ifdef CONFIG_CHIP_CH2201
    while (addr->RTC_STAT || addr->RTC_RSTAT) {
        addr->RTC_EOI;
    }
#endif

    int value = addr->RTC_CCR;
    uint32_t last_value, new_value;
    value |= 1 << 0;
    addr->RTC_CCR = value;

    last_value = addr->RTC_CCVR;
    do {
        new_value = addr->RTC_CCVR;
    } while (new_value <= last_value);
    return 0;
}

static int32_t ck_rtc_int_disable(ck_rtc_reg_t *addr)
{
    uint32_t value = addr->RTC_CCR;
    value &= ~(1 << 0);
    addr->RTC_CCR = value;
    ck_rtc_setmarchtime(addr, ck_rtc_readtime(addr));
    return 0;
}

void ck_rtc_irqhandler(int32_t idx)
{
	printf("\n[info]进入RTC底层中断函数\n");
    ck_rtc_priv_t *rtc_priv = &rtc_instance[idx];
    ck_rtc_reg_t *addr = (ck_rtc_reg_t *)(rtc_priv->base);

    addr->RTC_EOI;

    if (rtc_priv->cb_event) {
        rtc_priv->cb_event(idx, RTC_EVENT_TIMER_INTRERRUPT);
    }

}

/**
  \brief       Initialize RTC Interface. 1. Initializes the resources needed for the RTC interface 2.registers event callback function
  \param[in]   idx  rtc index
  \param[in]   cb_event  Pointer to \ref rtc_event_cb_t
  \return      pointer to rtc instance
*/
rtc_handle_t csi_rtc_initialize(int32_t idx, rtc_event_cb_t cb_event)
{
    if (idx < 0 || idx >= CONFIG_RTC_NUM) {
        return NULL;
    }

    int32_t real_idx;
    uint32_t base = 0u;
    uint32_t irq;

    real_idx = target_get_rtc(idx, &base, &irq);

    if (real_idx != idx) {
        return NULL;
    }

    ck_rtc_priv_t *rtc_priv;

    rtc_priv = &rtc_instance[idx];
    rtc_priv->base = base;
    rtc_priv->irq  = irq;

    ck_rtc_reg_t *addr = (ck_rtc_reg_t *)(rtc_priv->base);

    rtc_priv->cb_event = cb_event;
    addr->RTC_CCR = 0;
    addr->RTC_EOI;
    csi_vic_clear_pending_irq(rtc_priv->irq);
    csi_vic_enable_irq(rtc_priv->irq);

    return (rtc_handle_t)rtc_priv;
}

/**
  \brief       De-initialize RTC Interface. stops operation and releases the software resources used by the interface
  \param[in]   handle rtc handle to operate.
  \return      \ref execution_status
*/
int32_t csi_rtc_uninitialize(rtc_handle_t handle)
{
    RTC_NULL_PARAM_CHK(handle);

    ck_rtc_priv_t *rtc_priv = handle;

    rtc_priv->cb_event = NULL;
    csi_vic_disable_irq(rtc_priv->irq);

    return 0;
}

/**
  \brief       Get driver capabilities.
  \param[in]   idx  rtc index
  \return      \ref rtc_capabilities_t
*/
rtc_capabilities_t csi_rtc_get_capabilities(int32_t idx)
{
    if (idx < 0 || idx >= CONFIG_RTC_NUM) {
        rtc_capabilities_t ret;
        memset(&ret, 0, sizeof(rtc_capabilities_t));
        return ret;
    }

    return rtc_capabilities;
}

/**
  \brief       Set RTC timer.
  \param[in]   handle rtc handle to operate.
  \param[in]   rtctime \ref struct tm
  \return      \ref execution_status
*/

int32_t csi_rtc_set_time(rtc_handle_t handle, const struct tm *rtctime)
{
    RTC_NULL_PARAM_CHK(handle);
    RTC_NULL_PARAM_CHK(rtctime);

    ck_rtc_priv_t *rtc_priv = handle;
    ck_rtc_reg_t *addr = (ck_rtc_reg_t *)(rtc_priv->base);

    int32_t ret = ck_check_tm_ok((struct tm *)rtctime);

    if (ret < 0) {
        return ret;
    }

    rtc_priv->rtc_base.tm_sec  = rtctime->tm_sec;
    rtc_priv->rtc_base.tm_min  = rtctime->tm_min;
    rtc_priv->rtc_base.tm_hour = rtctime->tm_hour;
    rtc_priv->rtc_base.tm_mday = rtctime->tm_mday;
    rtc_priv->rtc_base.tm_mon  = rtctime->tm_mon;
    rtc_priv->rtc_base.tm_year = rtctime->tm_year;

    /* clr register should not be zero */
    ck_rtc_settime(addr, 10);
    return 0;
}

/**
  \brief       Get RTC timer.
  \param[in]   handle rtc handle to operate.
  \param[in]   rtctime \ref struct tm
  \return      \ref execution_status
*/
int32_t csi_rtc_get_time(rtc_handle_t handle, struct tm *rtctime)
{
    RTC_NULL_PARAM_CHK(handle);
    RTC_NULL_PARAM_CHK(rtctime);

    ck_rtc_priv_t *rtc_priv = handle;
    ck_rtc_reg_t *addr = (ck_rtc_reg_t *)(rtc_priv->base);

    
#if 1		//不正常的部分
	uint32_t timeout_count = 0;
	//RTC_CCVR CurrentCounterValueRegister	当前计数值
	//RTC_CLR  CounterLoadRegister			计数载入值
//	printf("addr->RTC_CCVR:%d\n",addr->RTC_CCVR);
//	printf("addr->RTC_CLR:%d\n",addr->RTC_CLR);
    while (addr->RTC_CCVR < addr->RTC_CLR) 
	{
        if (timeout_count++ > 0xfffffff) {
            break;
        }
    }
#endif

    time_t time = ck_rtc_readtime(addr);
    time = time / 1000;
    time += mktime(&(rtc_priv->rtc_base));
    gmtime_r(&time, rtctime);

    return 0;

}

/**
  \brief       Start RTC timer.
  \param[in]   handle rtc handle to operate.
  \return      \ref execution_status
*/
int32_t csi_rtc_start(rtc_handle_t handle)
{
    RTC_NULL_PARAM_CHK(handle);

    ck_rtc_priv_t *rtc_priv = handle;
    ck_rtc_reg_t *addr = (ck_rtc_reg_t *)(rtc_priv->base);

    ck_rtc_enable(addr);
    return 0;
}

/**
  \brief       Stop RTC timer.
  \param[in]   handle rtc handle to operate.
  \return      \ref execution_status
*/
int32_t csi_rtc_stop(rtc_handle_t handle)
{
    RTC_NULL_PARAM_CHK(handle);

    ck_rtc_priv_t *rtc_priv = handle;
    ck_rtc_reg_t *addr = (ck_rtc_reg_t *)(rtc_priv->base);

    ck_rtc_disable(addr);
    return 0;
}

/**
  \brief       Get RTC status.
  \param[in]   handle rtc handle to operate.
  \return      RTC status \ref rtc_status_t
*/
rtc_status_t csi_rtc_get_status(rtc_handle_t handle)
{
    rtc_status_t rtc_status = {0};

    if (handle == NULL) {
        return rtc_status;
    }

    ck_rtc_priv_t *rtc_priv = handle;
    ck_rtc_reg_t *addr = (ck_rtc_reg_t *)(rtc_priv->base);

    if (addr->RTC_RSTAT & 0x1) //interrupt raw status register 中断原始状态寄存器(只有1位)
	{
        rtc_status.active = 1;
    }

    return rtc_status;
}

/**
  \brief       config RTC timer.
  \param[in]   handle rtc handle to operate.
  \param[in]   rtctime time to wake up
  \return      error code
*/
int32_t csi_rtc_set_alarm(rtc_handle_t handle, const struct tm *rtctime)
{
    int32_t ret;
    struct tm current_time;
    int64_t settime = 0;

    RTC_NULL_PARAM_CHK(handle);
    RTC_NULL_PARAM_CHK(rtctime);

    ck_rtc_priv_t *rtc_priv = (ck_rtc_priv_t *)handle;

    ck_rtc_reg_t *addr = (ck_rtc_reg_t *)(rtc_priv->base);
    ret = ck_check_tm_ok((struct tm *)rtctime);

    if (ret < 0) {
        goto error_time;
    }

    ret = csi_rtc_get_time(handle, &current_time);

#ifdef CONFIG_CHIP_CH2201	//已定义
    settime = (mktime((struct tm *)rtctime) - mktime(&current_time));
#else
    settime = (mktime((struct tm *)rtctime) - mktime(&current_time)) * 1000;
#endif

    if (settime < 0) {
        goto error_time;
    }

    ck_rtc_setmarchtime(addr, settime);

    return 0;
error_time:
    return ERR_RTC(RTC_ERROR_TIME);

}

/**
  \brief       disable or enable RTC timer.
  \param[in]   handle rtc handle to operate.
  \param[in]   flag  1 - enable rtc alarm 0 - disable rtc alarm
  \return      error code
*/
int32_t csi_rtc_enable_alarm(rtc_handle_t handle, uint8_t en)
{
    RTC_NULL_PARAM_CHK(handle);

    ck_rtc_priv_t *rtc_priv = handle;
    ck_rtc_reg_t *addr = (ck_rtc_reg_t *)(rtc_priv->base);

    if (en == 1) {
        ck_rtc_int_enable(addr);
    } else if (en == 0) {
        ck_rtc_int_disable(addr);
    } else {
        return ERR_RTC(DRV_ERROR_PARAMETER);
    }

    return 0;
}

