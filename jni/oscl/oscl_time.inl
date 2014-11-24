/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */

// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ T I M E   ( T I M E   F U N C T I O N S )

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

#include "oscl_int64_utils.h"
#include "oscl_assert.h"
#include "oscl_stdstring.h"
#include "oscl_string_utils.h"

//------ NTPTime ------------
OSCL_COND_EXPORT_REF OSCL_INLINE NTPTime::NTPTime()
{
    set_to_current_time();
}

OSCL_COND_EXPORT_REF OSCL_INLINE NTPTime::NTPTime(const NTPTime& src)
{
    timevalue = src.timevalue;
}

OSCL_COND_EXPORT_REF OSCL_INLINE NTPTime::NTPTime(const uint32 seconds)
{
    Oscl_Int64_Utils::set_uint64(timevalue, seconds, 0);
}

OSCL_COND_EXPORT_REF OSCL_INLINE NTPTime::NTPTime(const int32 seconds)
{
    if (seconds < 0) return;
    timevalue = uint64(seconds) << 32;
}

OSCL_COND_EXPORT_REF OSCL_INLINE NTPTime::NTPTime(const uint64 value)
{
    timevalue = value;
}

OSCL_COND_EXPORT_REF OSCL_INLINE NTPTime& NTPTime::operator=(uint32 newval)
{
    Oscl_Int64_Utils::set_uint64(timevalue, newval, 0);
    return *this;
}

OSCL_COND_EXPORT_REF OSCL_INLINE NTPTime& NTPTime::operator=(uint64 newval)
{
    timevalue = newval;
    return *this;
}

OSCL_COND_EXPORT_REF OSCL_INLINE NTPTime& NTPTime::operator+=(uint64 val)
{
    timevalue += val;
    return *this;
}

OSCL_COND_EXPORT_REF OSCL_INLINE NTPTime NTPTime::operator-(const NTPTime &ntpt) const
{
    NTPTime temp;
    temp.timevalue = timevalue - ntpt.timevalue;
    return temp;
}

OSCL_COND_EXPORT_REF OSCL_INLINE void NTPTime::set_from_system_time(const uint32 systemtime)
{
    timevalue = int64(systemtime + 2208988800U) << 32;
}

OSCL_COND_EXPORT_REF OSCL_INLINE uint32 NTPTime::get_middle32() const
{
    return Oscl_Int64_Utils::get_uint64_middle32(timevalue);
}

OSCL_COND_EXPORT_REF OSCL_INLINE uint32 NTPTime::get_upper32() const
{
    return Oscl_Int64_Utils::get_uint64_upper32(timevalue);
}

OSCL_COND_EXPORT_REF OSCL_INLINE uint32 NTPTime::get_lower32() const
{
    return Oscl_Int64_Utils::get_uint64_lower32(timevalue);
}

OSCL_COND_EXPORT_REF OSCL_INLINE int32 NTPTime::to_system_time() const
{
    return (uint32)((timevalue >> 32) - 2208988800U);
}

OSCL_COND_EXPORT_REF OSCL_INLINE uint64 NTPTime::get_value() const
{
    return timevalue;
}

//------ TimeValue ----------
OSCL_COND_EXPORT_REF OSCL_INLINE bool TimeValue::is_zero()
{
    return ((ts.tv_usec == 0) && (ts.tv_sec == 0));
}

OSCL_COND_EXPORT_REF OSCL_INLINE bool TimeValue::is_zulu() const
{
    return zulu;
}

OSCL_COND_EXPORT_REF OSCL_INLINE void TimeValue::set_zulu(bool is_zulu)
{
    zulu = is_zulu;
}
OSCL_COND_EXPORT_REF OSCL_INLINE uint32 TimeValue::get_sec() const
{
    return ts.tv_sec;
};


OSCL_COND_EXPORT_REF OSCL_INLINE int32 TimeValue::to_msec() const
{
    return ((ts.tv_usec / 1000) + (ts.tv_sec * 1000));
}

OSCL_COND_EXPORT_REF OSCL_INLINE uint32 TimeValue::get_usec() const
{
    return ts.tv_usec;
}

OSCL_COND_EXPORT_REF OSCL_INLINE uint64 TimeValue::get_timevalue_in_usec() const
{

    uint64 aTempSec = 0;
    uint64 aTempUsec = 0;

    Oscl_Int64_Utils::set_uint64(aTempSec, 0, ts.tv_sec);
    Oscl_Int64_Utils::set_uint64(aTempUsec, 0, ts.tv_usec);

    return ((aTempSec * 1000000) + (aTempUsec));

}

OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue operator -(const TimeValue& a, const TimeValue& b)
{
    TimeValue c;
    c = a;
    c -= b;
    return c;
}


OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue operator+(const TimeValue& a, const int32 bSeconds)
{
    TimeValue c = a;
    return  c += bSeconds;
}

OSCL_COND_IMPORT_REF OSCL_INLINE TimeValue operator+(const int32 aSeconds, const TimeValue& b)
{
    TimeValue c = b;
    return  c += aSeconds;
}

OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue operator-(const TimeValue& a, const int32 bSeconds)
{
    TimeValue c = a;
    return c -= bSeconds;
}

OSCL_COND_IMPORT_REF OSCL_INLINE TimeValue operator-(const int32 aSeconds, const TimeValue& b)
{
    TimeValue c = b;
    return c -= aSeconds;
}

OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue& TimeValue::operator+=(const int32 aSeconds)
{
    // Adding a negative number?
    if (aSeconds < 0)
    {
        // Negative number.  Convert to a positive.
        int32 sec_value = OSCL_ABS(aSeconds);

        //  Subtract instead.
        return *this -= sec_value;
    }

    TimeValue aSec(aSeconds, SECONDS);
    return *this += aSec;
}

OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue& TimeValue::operator-=(const int32 aSeconds)
{
    // Subtracting a negative number?
    if (aSeconds < 0)
    {
        // Negative number.  Convert to a positive.
        int32 sec_value = OSCL_ABS(aSeconds);

        //  Add instead.
        return *this += sec_value;
    }

    TimeValue aSec(aSeconds, SECONDS);
    return *this -= aSec;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue& TimeValue::operator =(const TimeValue & a)
{
    this->zulu = a.zulu;
    this->ts.tv_usec = a.ts.tv_usec;
    this->ts.tv_sec = a.ts.tv_sec;
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue& TimeValue::operator +=(const TimeValue & a)
{
    this->ts.tv_usec += a.ts.tv_usec;
    this->ts.tv_sec += a.ts.tv_sec;

    if (this->ts.tv_usec >= USEC_PER_SEC)
    {
        this->ts.tv_usec -= USEC_PER_SEC;
        ++this->ts.tv_sec;
    }
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue& TimeValue::operator -=(const TimeValue & a)
{
    this->ts.tv_usec -= a.ts.tv_usec;
    this->ts.tv_sec -= a.ts.tv_sec;

    if ((this->ts.tv_sec > 0) && (this->ts.tv_usec < 0))
    {
        this->ts.tv_usec += USEC_PER_SEC;
        --this->ts.tv_sec;
    }
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue& TimeValue::operator *=(const int scale)
{
    this->ts.tv_usec *= scale;
    this->ts.tv_sec *= scale;
    unsigned long secs = this->ts.tv_usec / USEC_PER_SEC;
    this->ts.tv_sec += secs;
    this->ts.tv_usec -= secs * USEC_PER_SEC;
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


OSCL_COND_EXPORT_REF OSCL_INLINE bool operator ==(const TimeValue& a, const TimeValue& b)
{
    return timercmp(&a.ts, &b.ts, ==);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OSCL_COND_EXPORT_REF OSCL_INLINE bool operator !=(const TimeValue& a, const TimeValue& b)
{
    return timercmp(&a.ts, &b.ts, !=);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OSCL_COND_EXPORT_REF OSCL_INLINE bool operator <=(const TimeValue& a, const TimeValue& b)
{

    return !timercmp(&a.ts, &b.ts, >);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OSCL_COND_EXPORT_REF OSCL_INLINE bool operator >=(const TimeValue& a, const TimeValue& b)
{
    return !timercmp(&a.ts, &b.ts, <);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


OSCL_COND_EXPORT_REF OSCL_INLINE bool operator >(const TimeValue& a, const TimeValue& b)
{
    return timercmp(&a.ts, &b.ts, >);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OSCL_COND_EXPORT_REF OSCL_INLINE bool operator <(const TimeValue& a, const TimeValue& b)
{

    return timercmp(&a.ts, &b.ts, <);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
OSCL_COND_EXPORT_REF OSCL_INLINE void TimeValue::set_to_current_time()
{
    gettimeofday(&ts, NULL);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "oscl_mem_basic_functions.h"

OSCL_COND_EXPORT_REF OSCL_INLINE void TimeValue::set_to_zero()
{
    oscl_memset(&ts, 0, sizeof(ts));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue::TimeValue()
{
    zulu = false;
    gettimeofday(&ts, NULL);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue::TimeValue(const TimeValue& Tv)
{
    zulu = Tv.zulu;
    ts.tv_usec = Tv.ts.tv_usec;
    ts.tv_sec  = Tv.ts.tv_sec;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue::TimeValue(const OsclBasicTimeStruct& in_tv)
{
    zulu = false;
    ts.tv_usec = in_tv.tv_usec;
    ts.tv_sec = in_tv.tv_sec;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue::TimeValue(uint16 aYear, uint16 aMonth, uint16 aDay, uint16 aHour,
        uint16 aMinute, uint16 aSecond,  uint16 aMilliseconds)
{

    /*struct tm aTmBuffer;*/

    OsclBasicDateTimeStruct aTmBuffer;
    memset(&aTmBuffer, 0, sizeof(aTmBuffer));

    aTmBuffer.tm_sec   = aSecond;       /* seconds afterthe minute - [0, 61] */
    /* for sleap seconds */
    aTmBuffer.tm_min   = aMinute;       /* minutes after the hour - [0, 59] */
    aTmBuffer.tm_hour  = aHour;         /* hour since midnight - [0, 23] */
    aTmBuffer.tm_mday  = aDay;          /* day of the month- [1, 31] */
    aTmBuffer.tm_mon   = aMonth - 1 ;     /* months since January - [0, 11] */
    aTmBuffer.tm_year  = (aYear - 1900);/* years since 1900*/

    time_t AbsoluteTime = mktime(&aTmBuffer);

    if (AbsoluteTime == (time_t) - 1)
    {
        OSCL_ASSERT(false); // wrong input to this api
    }
    else
    {
        /*Filling the TimeVal ts Structure for further Use */
        memset(&ts, 0 , sizeof(ts));
        ts.tv_sec = AbsoluteTime ;
        ts.tv_usec = ((aMilliseconds) * (1000));
    }


}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue::TimeValue(long num_units, TimeUnits units)
{
    zulu = false;
    ts.tv_sec = num_units / MapToSeconds[units];
    long diff = num_units - MapToSeconds[units] * ts.tv_sec;
    ts.tv_usec = diff * MapToUSeconds[units];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
OSCL_COND_EXPORT_REF OSCL_INLINE void TimeValue::set_from_ntp_time(const uint32 ntp_offset_seconds)
{
    ts.tv_sec = ntp_offset_seconds - unix_ntp_offset;
    ts.tv_usec = 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue::TimeValue(const ISO8601timeStrBuf time_strbuf)
{
    char buf[5];

    int year, mon, day, hour, min, sec;

    strncpy(buf, time_strbuf, 4);
    buf[4] = 0;
    year = atoi(buf);
    buf[2] = 0;
    strncpy(buf, time_strbuf + 5, 2);
    mon = atoi(buf);
    strncpy(buf, time_strbuf + 8, 2);
    day = atoi(buf);
    strncpy(buf, time_strbuf + 11, 2);
    hour = atoi(buf);
    strncpy(buf, time_strbuf + 14, 2);
    min = atoi(buf);
    strncpy(buf, time_strbuf + 17, 2);
    sec = atoi(buf);

    zulu = (time_strbuf[19] == 'Z') ? true : false;

    // Fill in the time structure
    OsclBasicDateTimeStruct timeStruct;

    timeStruct.tm_sec = sec;
    timeStruct.tm_min = min;
    timeStruct.tm_hour = hour;
    timeStruct.tm_mday = day;
    timeStruct.tm_mon = mon - 1;
    timeStruct.tm_year = year - 1900;
    timeStruct.tm_wday = -1;
    timeStruct.tm_yday = -1;
    timeStruct.tm_isdst = -1;
    timeStruct.tm_gmtoff = -1;

    // Convert to a timeval
    ts.tv_sec = mktime(&timeStruct);
    ts.tv_usec = 0;

    if (zulu)
    {
        // mktime sets the tv_sec to local time.
        // Adjust the ts.tv_sec according to the GMT
        ts.tv_sec += timeStruct.tm_gmtoff;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
OSCL_COND_EXPORT_REF OSCL_INLINE TimeValue::TimeValue(OsclBasicDateTimeStruct ints)
{
    ts.tv_sec = mktime(&ints);
    ts.tv_usec = 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OSCL_COND_EXPORT_REF OSCL_INLINE int32 TimeValue::get_local_time()
{
    // Daylight saving time offset of 1 hour in the summer
    uint dst_offset = 60 * 60;
    int32 GMTime = ts.tv_sec;

    struct tm *timeptr;
    struct tm buffer;
    int dstFlag;
    timeptr = localtime_r(&ts.tv_sec, &buffer);

    GMTime -= timeptr->tm_gmtoff;

    dstFlag = timeptr->tm_isdst;

    if (dstFlag == 1)
    {
        // Daylight saving time. Add an hour's offset to get GMT
        GMTime += dst_offset;
    }

    return GMTime;
}

OSCL_COND_EXPORT_REF OSCL_INLINE OsclBasicTimeStruct * TimeValue::get_timeval_ptr()
{
    return &ts;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

