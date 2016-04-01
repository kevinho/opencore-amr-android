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

#ifndef OSCLCONFIG_H_INCLUDED
#include "osclconfig.h"
#endif

#ifndef OSCL_SINGLETON_H_INCLUDED
#include "oscl_singleton.h"
#endif


//See if the gettimeofday implementation is available.
//It relies on timeval and global variables...

#ifndef OSCLCONFIG_TIME_H_INCLUDED
#include "osclconfig_time.h"
#endif

#define PV_USE_GETTIMEOFDAY 1


/************************
* in each the following routines, there must be an valid clause
* in the #if - #elif chain for each platform.  If not, a #error
* will be encountered, at compile time.
*
* If, in doing a new port, you do not yet know the appropriate value
* for one of these routines, an "OSCL_ASSERT(false)"  could provide
* a temporary expedient.  Better, though, not to depend on the runtime
* check .. which may occur at an inopportune time.
*/

OSCL_COND_EXPORT_REF OSCL_INLINE uint32 OsclTickCount::TickCount()
{
#if   PV_USE_GETTIMEOFDAY
#define ROLLBACK_THRESHOLD 0x80000000
    // lock this function against other threads changing the static variables
    // ignore return value and error code
    int32 errorCode = 0;
    OsclSingletonRegistry::lockAndGetInstance(OSCL_SINGLETON_ID_TICKCOUNT, errorCode);

    struct timeval tv;

    static struct timeval stv = {0, 0};
    static  uint32 prev_val = 0;

    if ((0 == stv.tv_sec) && (0 == stv.tv_usec))
        gettimeofday(&stv, NULL);

    gettimeofday(&tv, NULL);
    uint32 clk_val = (tv.tv_sec - stv.tv_sec) * 1000 + (tv.tv_usec - stv.tv_usec) / 1000;

    if ((clk_val - prev_val) > ROLLBACK_THRESHOLD)
    {
        // assume clock rolled backwards so this update will be
        // ignored since we require that the tick counter be monotonic
        // non-decreasing.
        clk_val = prev_val;
    }
    prev_val = clk_val;
    OsclSingletonRegistry::registerInstanceAndUnlock(0, OSCL_SINGLETON_ID_TICKCOUNT, errorCode);

    return clk_val;

#else
#error No definition for OsclTickCount::TickCount
#endif
}

// how many ticks per second
OSCL_COND_EXPORT_REF OSCL_INLINE uint32 OsclTickCount::TickCountFrequency()
{
#if   PV_USE_GETTIMEOFDAY
    return 1000;
#else
#error No definition for OsclTickCount::TickCountFrequency
#endif
}

// how many microseconds per tick
OSCL_COND_EXPORT_REF OSCL_INLINE uint32 OsclTickCount::TickCountPeriod()
{
#if   PV_USE_GETTIMEOFDAY
    return 1000;
#else
#error No definition for OsclTickCount::TickCountPeriod
#endif
}

OSCL_COND_EXPORT_REF OSCL_INLINE uint32 OsclTickCount::TicksToMsec(uint32 ticks)
{
#if   PV_USE_GETTIMEOFDAY
    return ticks;
#else
#error No definition for OsclTickCount::TicksToMsec
#endif
}

OSCL_COND_EXPORT_REF OSCL_INLINE uint32 OsclTickCount::MsecToTicks(uint32 msec)
{
#if   PV_USE_GETTIMEOFDAY
    return msec;
#else
#error No definition for OsclTickCount::msectoticks
#endif
}


