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

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/*! \file oscl_time.h
    \brief The file oscl_time.h defines to classes NTPTime and TimeValue
for getting, manipulating,  and formatting time values.
The TimeValue class is based on the native system time format while
NTPTime is used for the standard Network Time Protocol format.
*/


#ifndef OSCL_TIME_H_INCLUDED
#define OSCL_TIME_H_INCLUDED

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCLCONFIG_TIME_H_INCLUDED
#include "osclconfig_time.h"
#endif

#ifndef OSCL_INT64_UTILS_INCLUDED
#include "oscl_int64_utils.h"
#endif

const int CTIME_BUFFER_SIZE = 26;

const int PV8601TIME_BUFFER_SIZE = 21;
const int ISO8601TIME_BUFFER_SIZE = 21;

typedef char CtimeStrBuf[CTIME_BUFFER_SIZE];
typedef char PV8601timeStrBuf[PV8601TIME_BUFFER_SIZE];
typedef char ISO8601timeStrBuf[ISO8601TIME_BUFFER_SIZE];

class TimeValue;  // Forward definition

OSCL_IMPORT_REF void PV8601ToRFC822(PV8601timeStrBuf pv8601_buffer, CtimeStrBuf ctime_buffer);
OSCL_IMPORT_REF void ISO8601ToRFC822(ISO8601timeStrBuf iso8601_buffer, CtimeStrBuf ctime_buffer);
OSCL_IMPORT_REF void RFC822ToPV8601(CtimeStrBuf ctime_buffer, PV8601timeStrBuf);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//! The TimeUnits enum can be used when constructing a TimeValue class.
typedef enum { SECONDS = 0, MILLISECONDS = 1, MICROSECONDS = 2 } TimeUnits;

const long USEC_PER_SEC = 1000000;
const long MSEC_PER_SEC = 1000;

//! The following are used internally so not documented in the API
static const long MapToSeconds[] = {1, 1000, 1000000};
static const long MapToUSeconds[] = {1000000, 1000, 1};
static const long MapTo100NanoSeconds[] = {10000000, 10000, 10};
const uint32 unix_ntp_offset = 2208988800U;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//! The NTPTime class represents a time value as the number of seconds since 0h (UTC) Jan. 1, 1900
/*!
   The NTPTime class:
   Conversion to/from Unix (epoch at 0h Jan. 1, 1970) amount to addition/subtraction of 2208988800.
   A single 64 bit value is used to represent the time. This value represents the number of seconds
   since 0h (UTC) Jan. 1, 1900. There is an implied binary point betweeen the upper 32 bits and lower
   32 bits (this is referred to as a 32.32 fractional representation).
   For example a binary value of 00000000 00000000 00000000 00000011 10000000 00000000 00000000 00000000
   represents 3.5 seconds since Jan 1, 1900.
*/
class NTPTime
{

    private:
        uint64 timevalue;

    public:
        //! The default constructor creates an NTPTime instance representing the current system time.
        OSCL_COND_IMPORT_REF NTPTime();

        //! Copy constructor to create a new NTPTime from an existing one.
        OSCL_COND_IMPORT_REF NTPTime(const NTPTime& src);

        //! Construct an NTPTime from a uint32.
        /*! \param seconds The uint32 input represents the number of seconds since Jan. 1, 1900. */
        OSCL_COND_IMPORT_REF NTPTime(const uint32 seconds);

        //! Construct an NTPTime from a int.
        /*! \param seconds The int input represents the number of seconds since Jan. 1, 1900. */
        OSCL_COND_IMPORT_REF NTPTime(const int32 seconds);

        //! Construct a NTPTime instance from a TimeValue instance.
        /*!
           This constructor creates an NTPTime value representing the same absolute time as the TimeValue parameter.
           \param t A reference to a TimeValue object.
        */
        OSCL_COND_IMPORT_REF NTPTime(const TimeValue& t);

        //! Construct a NTPTime instance from a uint64 value.
        /*! \param value A 64 bit integer argument which is used as the ntp 32.32 fractional representation.*/
        OSCL_COND_IMPORT_REF NTPTime(const uint64 value);


        //! The assignment operator for a 32 bit integer.
        /*! \param newval A 32 bit integer representing the upper 32 bits of the 32.32 NTP time (e.g. the number of whole seconds since Jan 1, 1900 UTC).*/
        OSCL_COND_IMPORT_REF NTPTime& operator=(uint32 newval);

        //! The assignment operator for a 64 bit integer.
        /*! \param newval A 64 bit value which represents the 32.32 fractional representation of the ntp time. */
        OSCL_COND_IMPORT_REF NTPTime& operator=(uint64 newval);

        //! The += operator is used to add a 64 bit 32.32 value to an existing NTPTime value.
        /*! \param val The 64 bit 32.32 value to add to this object's value. */
        OSCL_COND_IMPORT_REF NTPTime& operator+=(uint64 val);

        //! The - operator allows subtraction of one NTPTime value from another.  This is useful to measure an interval.
        /*! \param ntpt A reference to the NTPTime object to be subracted from this one. */
        OSCL_COND_IMPORT_REF NTPTime operator-(const NTPTime &ntpt) const;

        //! This method converts a 32-bit system time to NTP time.
        /*!
            This method sets the value of the NTPTime instance to the absolute time
            represented by the 32 bit input argument.
          \param systemtime This 32-bit value is interpreted as the number of seconds
            since the unix epoch Jan. 1 1970.
        */
        void set_from_system_time(const uint32 systemtime);

        //! Grab the middle 32 bits of the 64 bit 32.32 representation.
        /* \return This method returns the middle 32 bits of the 32.32 representation. */
        OSCL_COND_IMPORT_REF uint32 get_middle32() const;

        //! This method returns the upper 32 bits of the 32.32 representation.
        OSCL_COND_IMPORT_REF uint32 get_upper32() const;

        //! This method returns the lower 32 bits of the 32.32 representation.
        OSCL_COND_IMPORT_REF uint32 get_lower32() const;

        //! This method converts the ntp time value to system time.
        /*!
            This method returns a 32 bit value representing the same absolute time as the NTP time value,
            but expressed as whole seconds since the unix epoch. The fractional part of the ntp value is discarded.
        */
        int32 to_system_time() const;

        //! This method returns the 32.32 ntp representation.
        OSCL_COND_IMPORT_REF uint64 get_value() const;

        //! This method sets the 32.32 representation to the current system time value.
        OSCL_IMPORT_REF int set_to_current_time();

};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//! The TimeValue class represents a time value in a format native to the system.
/*!
    This class provides common time functions independent of any OS. The actual representation used
    is native to the system that the class is compiled on to increase efficiency.
   Macros used in this class:
   - OSCL_HAS_ANSI_STRING_SUPPORT:

   Definitions to determine the type of basic time structure used to store the time
   - OSCL_HAS_UNIX_TIME_FUNCS
   - OSCL_HAS_SYMBIAN_SUPPORT
   - OSCL_HAS_MSWIN_SUPPORT


*/
class TimeValue
{

    public:
        //! Create a TimeValue representing the current time.
        OSCL_COND_IMPORT_REF TimeValue();

        //! Copy constructor
        OSCL_COND_IMPORT_REF TimeValue(const TimeValue& Tv);

        //! Create a TimeValue representing an interval of tv units.
        /*!
          \param tv The number of units in the interval to be represented by this TimeValue.
          \param units The units of the tv argument. Must be in the enumeration TimeUnits.
        */
        OSCL_COND_IMPORT_REF TimeValue(long tv, TimeUnits units);

        //! Create a TimeValue representing the absolute time specified by the BasicTimeStruct.
        /*! \param in_tv OsclBasicTimeStruct as defined for each platform.
        */
        OSCL_COND_IMPORT_REF TimeValue(const OsclBasicTimeStruct& in_tv);

        OSCL_COND_IMPORT_REF TimeValue(const ISO8601timeStrBuf time_strbuf);
        //! Create a TimeValue representing the absolute time specified by the
        //! year/month/day/hours/minutes/seconds/microseconds values passed as argument.
        /*!
         * TimeValue constructor that sets time according to following input parameter for a specific date time.
         * Please note that no argument is check for its validity (range etc)
         * It might assert incase wrong argument are passed by user of this api.
         * @param [in]    uint16 wYear;
         * @param [in]    uint16 wMonth; Jan = 1 to Dec = 12
         * @param [in]    uint16 wDay;  1-28/29/30/31
         * @param [in]    uint16 wHour;  0 to 23
         * @param [in]    uint16 wMinute;  0 to 59
         * @param [in]    uint16 wSecond;  0 to 59
         * @param [in]    uint16 wMilliseconds; 0 to 999
         */
        OSCL_COND_IMPORT_REF TimeValue(uint16 aYear, uint16 aMonth, uint16 aDay, uint16 aHour,
                                       uint16 aMinute, uint16 aSecond,  uint16 aMilliseconds);

        //! Create a TimeValue representing the absolute time specified by the BasicDateTimeStruct.
        /*! \param in_ts OsclBasicDateTimeStruct as defined for each platform provides the date in a
            readable format (i.e. day, date , week etc.)
            Notes: Implementation incomplete (= not done) on Win32, Wince, Symbian
        */
        OSCL_COND_IMPORT_REF TimeValue(OsclBasicDateTimeStruct in_ts);

        //! Get the local time after having adjusted for daylight saving
        /*!
            Notes: Implementation incomplete (= not done) on Win32, Wince, Symbian
        */
        OSCL_COND_IMPORT_REF int32 get_local_time();


        friend class NTPTime;

        //! Set the time value to zero (represents a zero interval).
        OSCL_COND_IMPORT_REF void set_to_zero();

        //! Set the time value to the current system time.
        OSCL_COND_IMPORT_REF void set_to_current_time();

        //! This method coverts a 32-bit NTP offset to system time
        /*!
            This method takes a 32-bit ntp offset which is the number of seconds from
            0 h Jan 1, 1900 and converts it to the system time
         */
        OSCL_COND_IMPORT_REF void set_from_ntp_time(const uint32 ntp_offset);

        //! Get a 32 bit value representing the seconds since the (system dependent) epoch.
        /*!
            \return This call returns a 32 bit value representing the nubmer of seconds since the epoch.
            On unix systems this represents the number of seconds since the unix epoch Jan 1 1970.
            On Win32 this represents the number of seconds since Jan 1, 1601. This is intended to be used
            for intervals rather than for absolute time. (On Win32 for example, a 32 bit value would be too
            small to represent the number of seconds from the epoch until the current time.)
         */
        OSCL_COND_IMPORT_REF uint32 get_sec() const ;

        OSCL_COND_IMPORT_REF int32 to_msec() const;

        //! Get a 32 bit value representing the number of microseconds in the time value.
        /*!
            \return Returns a uint32 value representing the number of microseconds in the time value after subtracting off the whole seconds.
        */
        OSCL_COND_IMPORT_REF uint32 get_usec() const ;
        //! Get a 64 bit value representing the time value converted to microseconds
        /*!
            \return Returns a uint64 value representing the time value in terms of microseconds.The
          The time origin is dependent on platform for which OSCL is compiled. For example
          for symbian it is midnight, January 1st, 0 AD
          for windows it is January 1, 1601 (UTC)
        */
        OSCL_COND_IMPORT_REF uint64 get_timevalue_in_usec() const ;

        //! Get a string containing a text representation of this TimeValue object.
        /*!
           \param ctime_strbuf A CtimeStrBuf object to which the text representation will be written,
           \return A pointer to the input CtimeStrBuf is returned. This string is null terminated of the form
            "Wed Jun 30 21:49:08 1993".
        */
        OSCL_IMPORT_REF char *get_str_ctime(CtimeStrBuf ctime_strbuf);

        //! Get a PV extended text representation of the Time based on the PV 8601 format.
        /*!
           \param time_strbuf A PV8601timeStrBuf object to which the text representation will be written,
           \return The number of characters copied to the buffer, not including the terminating null.  The returned string is of the form "19850412T101530.047Z".
         */
        OSCL_IMPORT_REF int get_pv8601_str_time(PV8601timeStrBuf time_strbuf);

        //! Get a PV extended text representation of the Time based on the ISO 8601 format.
        /*!
           \param time_strbuf A ISO8601timeStrBuf object to which the text representation will be written,
           \return The number of characters copied to the buffer, not including the terminating null.  The returned string is of the form "1985-04-12 10:15:30Z".
         */
        OSCL_IMPORT_REF int get_ISO8601_str_time(ISO8601timeStrBuf time_strbuf);

        //! Get a text representation of the time in the GMT timezone based on the RFC 822 / RFC 1123 (also described in the HTTP spec RFC 2068 and RFC 2616.
        /*!
            \param max_time_strlen The maximum number of characters that can be written to the buffer.
            \param time_str A pointer to the buffer to which the characters will be written.
            \return Returns a pointer to the buffer (same as time_str) containing a null terminated (c-style) string
            of the form "Wed, 30 Jun 1993 21:49:08 GMT".
         */
        OSCL_IMPORT_REF char *get_rfc822_gmtime_str(int max_time_strlen,
                char *time_str);

        //! Determine if the time value is zero.
        OSCL_COND_IMPORT_REF bool is_zero();

        //! Manipulate internal flags to mark the time value as being in "zulu" (GMT) time.

        OSCL_COND_IMPORT_REF bool is_zulu() const;
        OSCL_COND_IMPORT_REF void set_zulu(bool is_zulu);

        // comparison operators
        OSCL_COND_IMPORT_REF friend bool operator ==(const TimeValue& a, const TimeValue& b);
        OSCL_COND_IMPORT_REF friend bool operator !=(const TimeValue& a, const TimeValue& b);
        OSCL_COND_IMPORT_REF friend bool operator <=(const TimeValue& a, const TimeValue& b);
        OSCL_COND_IMPORT_REF friend bool operator >=(const TimeValue& a, const TimeValue& b);
        OSCL_COND_IMPORT_REF friend bool operator <(const TimeValue& a, const TimeValue& b);
        OSCL_COND_IMPORT_REF friend bool operator >(const TimeValue& a, const TimeValue& b);

        //! Assignment operator
        OSCL_COND_IMPORT_REF TimeValue& operator =(const TimeValue& a);

        // arithmetic operators
        //! += operator
        OSCL_COND_IMPORT_REF TimeValue& operator +=(const TimeValue& a);
        //! -= operator
        OSCL_COND_IMPORT_REF TimeValue& operator -=(const TimeValue& a);
        //! This operator scales the time value by a constant.
        OSCL_COND_IMPORT_REF TimeValue& operator *=(const int scale);

        OSCL_COND_IMPORT_REF OsclBasicTimeStruct * get_timeval_ptr();

        OSCL_COND_IMPORT_REF TimeValue& operator+=(const int32 aSeconds);
        OSCL_COND_IMPORT_REF TimeValue& operator-=(const int32 aSeconds);
    private:

        bool zulu;
        OsclBasicTimeStruct ts;
        const OsclBasicTimeStruct *getBasicTimeStruct() const
        {
            return &ts;
        };

};

OSCL_COND_IMPORT_REF TimeValue operator -(const TimeValue& a, const TimeValue& b);
OSCL_COND_IMPORT_REF TimeValue operator+(const TimeValue& a, const int32 bSeconds);
OSCL_COND_IMPORT_REF TimeValue operator+(const int32 aSeconds, const TimeValue& b);
OSCL_COND_IMPORT_REF TimeValue operator-(const TimeValue& a, const int32 bSeconds);
OSCL_COND_IMPORT_REF TimeValue operator-(const int32 aSeconds, const TimeValue& b);

#if (!OSCL_DISABLE_INLINES)
#include "oscl_time.inl"
#endif


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*! @} */


#endif // OSCL_TIME_H_INCLUDED
