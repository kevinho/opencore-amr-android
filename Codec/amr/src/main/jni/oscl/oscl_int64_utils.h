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

#ifndef OSCL_INT64_UTILS_H_INCLUDED
#define OSCL_INT64_UTILS_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

//! The Oscl_Int64_Utils class provides a wrapper for commonly used int64/uint64 operations
/*!
 * The Oscl_Int64_Utils class:
 *
 * Provides a wrapper for commonly used operations to mask the differences between OSes
 * that have an int64/uint64 class instead of a 64-bit integer.
 */
class Oscl_Int64_Utils
{
    public:
        OSCL_IMPORT_REF static void set_int64(int64& input_value, const int32 upper, const uint32 lower);

        OSCL_IMPORT_REF static int32 get_int64_upper32(const int64& input_value);

        OSCL_IMPORT_REF static uint32 get_int64_lower32(const int64& input_value);

        OSCL_IMPORT_REF static uint32 get_int64_middle32(const int64& input_value);

        OSCL_IMPORT_REF static void set_uint64(uint64& input_value, const uint32 upper, const uint32 lower);

        OSCL_IMPORT_REF static uint32 get_uint64_upper32(const uint64& input_value);

        OSCL_IMPORT_REF static uint32 get_uint64_lower32(const uint64& input_value);

        OSCL_IMPORT_REF static uint32 get_uint64_middle32(const uint64& input_value);
};

/**
 * OsclInteger64Transport Structure
 *
 * Structure to only transport 64-bit integer values
 * uint64 and int64 could be classes so needed for cases
 * where having a class will not work.
 */
typedef struct OsclInteger64Transport
{
    uint32 iHigh;
    uint32 iLow;
} _OsclInteger64Transport;

#endif

