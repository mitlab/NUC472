#include "system.h"

volatile unsigned long sysTickCount=0;
volatile unsigned long sysTickCountOld=0;
volatile systype_t SysType = BASIC_IO;

#ifdef RTC_ENABLE

#define SECONDS_BY_MINUTES 60
#define MINUTES_BY_HOUR 60
#define SECONDS_BY_HOUR (SECONDS_BY_MINUTES * MINUTES_BY_HOUR)
#define HOURS_BY_DAY 24 
#define SECONDS_BY_DAY (SECONDS_BY_HOUR * HOURS_BY_DAY)

static const uint32_t seconds_before_month[2][12] = {
    {
        0,
        31 * SECONDS_BY_DAY,
        (31 + 28) * SECONDS_BY_DAY,
        (31 + 28 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
    },
    {
        0,
        31 * SECONDS_BY_DAY,
        (31 + 29) * SECONDS_BY_DAY,
        (31 + 29 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) * SECONDS_BY_DAY,
        (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) * SECONDS_BY_DAY,
    }
};

bool _rtc_is_leap_year(int year) {
    /* 
     * since in practice, the value manipulated by this algorithm lie in the 
     * range [70 : 138], the algorith can be reduced to: year % 4.
     * The algorithm valid over the full range of value is: 

        year = 1900 + year;
        if (year % 4) {
            return false;
        } else if (year % 100) {
            return true;
        } else if (year % 400) {
            return false;
        }
        return true;

     */ 
    return (year) % 4 ? false : true;
}


void wait_us(int us) 
{
    //uint32_t start = us_ticker_read();
    //while ((us_ticker_read() - start) < (uint32_t)us);
}

void wait(float s) 
{
    wait_us(s * 1000000.0f);
}

void wait_ms(int ms) 
{
    wait_us(ms * 1000);
}



time_t _rtc_mktime(const struct tm* time) 
{
    // partial check for the upper bound of the range
    // normalization might happen at the end of the function 
    // this solution is faster than checking if the input is after the 19th of 
    // january 2038 at 03:14:07.  
    if ((time->tm_year < 70) || (time->tm_year > 138)) { 
        return ((time_t) -1);
    }

    uint32_t result = time->tm_sec;
    result += time->tm_min * SECONDS_BY_MINUTES;
    result += time->tm_hour * SECONDS_BY_HOUR;
    result += (time->tm_mday - 1) * SECONDS_BY_DAY;
    result += seconds_before_month[_rtc_is_leap_year(time->tm_year)][time->tm_mon];

    if (time->tm_year > 70) { 
        // valid in the range [70:138] 
        uint32_t count_of_leap_days = ((time->tm_year - 1) / 4) - (70 / 4);
        result += (((time->tm_year - 70) * 365) + count_of_leap_days) * SECONDS_BY_DAY;
    }

    if (result > INT32_MAX) { 
        return (time_t) -1;
    }

    return result;
}

bool _rtc_localtime(time_t timestamp, struct tm* time_info) 
{
    if (((int32_t) timestamp) < 0) { 
        return false;
    } 

    time_info->tm_sec = timestamp % 60;
    timestamp = timestamp / 60;   // timestamp in minutes
    time_info->tm_min = timestamp % 60;
    timestamp = timestamp / 60;  // timestamp in hours
    time_info->tm_hour = timestamp % 24;
    timestamp = timestamp / 24;  // timestamp in days;

    // compute the weekday
    // The 1st of January 1970 was a Thursday which is equal to 4 in the weekday
    // representation ranging from [0:6]
    time_info->tm_wday = (timestamp + 4) % 7;

    // years start at 70
    time_info->tm_year = 70;
    while (true) { 
        if (_rtc_is_leap_year(time_info->tm_year) && timestamp >= 366) {
            ++time_info->tm_year;
            timestamp -= 366;
        } else if (!_rtc_is_leap_year(time_info->tm_year) && timestamp >= 365) {
            ++time_info->tm_year;
            timestamp -= 365;
        } else {
            // the remaining days are less than a years
            break;
        }
    }

    time_info->tm_yday = timestamp;

    // convert days into seconds and find the current month
    timestamp *= SECONDS_BY_DAY;
    time_info->tm_mon = 11;
    bool leap = _rtc_is_leap_year(time_info->tm_year);
    for (uint32_t i = 0; i < 12; ++i) {
        if ((uint32_t) timestamp < seconds_before_month[leap][i]) {
            time_info->tm_mon = i - 1;
            break;
        }
    }

    // remove month from timestamp and compute the number of days.
    // note: unlike other fields, days are not 0 indexed.
    timestamp -= seconds_before_month[leap][time_info->tm_mon];
    time_info->tm_mday = (timestamp / SECONDS_BY_DAY) + 1;

    return true;
}

void rtc_init(void)
{
    if (rtc_inited) {
        return;
    }
    rtc_inited = 1;
    
    // Enable IP clock
    CLK_EnableModuleClock(rtc_modinit.clkidx);
    
    RTC_Open(NULL);
}


time_t rtc_read(void)
{
    if (! rtc_inited) {
        rtc_init();
    }
    
    S_RTC_TIME_DATA_T rtc_datetime;
    RTC_GetDateAndTime(&rtc_datetime);
    
    struct tm timeinfo;

    // Convert struct tm to S_RTC_TIME_DATA_T
    timeinfo.tm_year = rtc_datetime.u32Year - YEAR0;
    timeinfo.tm_mon  = rtc_datetime.u32Month - 1;
    timeinfo.tm_mday = rtc_datetime.u32Day;
    timeinfo.tm_wday = rtc_datetime.u32DayOfWeek;
    timeinfo.tm_hour = rtc_datetime.u32Hour;
    timeinfo.tm_min  = rtc_datetime.u32Minute;
    timeinfo.tm_sec  = rtc_datetime.u32Second;

    // Convert to timestamp
    time_t t = _rtc_mktime(&timeinfo);

    return t;
}

void rtc_write(time_t t)
{
    if (! rtc_inited) {
        rtc_init();
    }
    
    // Convert timestamp to struct tm
    struct tm timeinfo;
    if (_rtc_localtime(t, &timeinfo) == false) {
        return;
    }

    S_RTC_TIME_DATA_T rtc_datetime;
    
    // Convert S_RTC_TIME_DATA_T to struct tm
    rtc_datetime.u32Year        = timeinfo.tm_year + YEAR0;
    rtc_datetime.u32Month       = timeinfo.tm_mon + 1;
    rtc_datetime.u32Day         = timeinfo.tm_mday;
    rtc_datetime.u32DayOfWeek   = timeinfo.tm_wday;
    rtc_datetime.u32Hour        = timeinfo.tm_hour;
    rtc_datetime.u32Minute      = timeinfo.tm_min;
    rtc_datetime.u32Second      = timeinfo.tm_sec;
    rtc_datetime.u32TimeScale   = RTC_CLOCK_24;
    
    // NOTE: Timing issue with write to RTC registers. This delay is empirical, not rational.
    RTC_SetDateAndTime(&rtc_datetime);
    //nu_nop(6000);
    wait_us(100);
}

#endif




/* EOF */
