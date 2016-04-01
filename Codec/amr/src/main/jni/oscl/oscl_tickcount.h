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

//               O S C L _ T I C K C O U N T

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/** \file oscl_tickcount.h
    \brief Defines a data structure for string containment/manipulations where the storage for the string is maintained externally.
*/

#ifndef OSCL_TICKCOUNT_H_INCLUDED
#define OSCL_TICKCOUNT_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#define OSCLTICKCOUNT_MAX_TICKS 0xffffffff

/**
 * OsclTickCount class is used to retrieve the system tick
 * count and the tick counter's frequency.
 *
 * The maximum tick count value is equivalent to the maximum
 * uint32 value.
 */
class OsclTickCount
{
    public:
        /**
         * This function returns the current system tick count
         *
         * @return returns the tick count
         */
        static uint32 TickCount();

        /**
         * This function returns the tick frequency in ticks
         * per second
         *
         * @return ticks per second
         */
        static uint32 TickCountFrequency();

        /**
         * This function returns the tick period in
         * microseconds per tick
         *
         * @return microseconds per tick
         */
        static uint32 TickCountPeriod();

        /**
         * This function converts ticks to milliseconds
         *
         * @return milliseconds
         */
        static uint32 TicksToMsec(uint32 ticks);

        /**
         * This function converts milliseconds to ticks
         *
         * @return ticks
         */
        static uint32 MsecToTicks(uint32 msec);
};

#if !OSCL_DISABLE_INLINES
#include "oscl_tickcount.inl"
#endif

#endif // OSCL_TICK_UTILS_H_INCLUDED

/*! @} */
