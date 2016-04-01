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

//               O S C L _ S T R I N G _ U T I L S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/** \file oscl_string_utils.h
    \brief Utilities to parse and convert strings
*/

#ifndef OSCL_STRING_UTILS_H_INCLUDED
#define OSCL_STRING_UTILS_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#define oscl_isdigit(c) ((c) >= '0' && (c) <= '9')

OSCL_IMPORT_REF const char* skip_whitespace(const char *ptr);

OSCL_IMPORT_REF char * skip_whitespace(char * ptr);

OSCL_IMPORT_REF const char* skip_whitespace(const char *start, const char *end);

OSCL_IMPORT_REF const char* skip_to_whitespace(const char *start, const char *end);

OSCL_IMPORT_REF const char * skip_to_line_term(const char *start_ptr, const char *end_ptr);

OSCL_IMPORT_REF const char* skip_whitespace_and_line_term(const char *start, const char *end);

OSCL_IMPORT_REF int extract_string(const char * in_ptr, char *outstring, int maxsize);

OSCL_IMPORT_REF int extract_string(const char * start, const char *end, char *outstring, int maxsize);

OSCL_IMPORT_REF bool PV_atoi(const char *buf, const char new_format, uint32& value);

OSCL_IMPORT_REF bool PV_atoi(const char *buf, const char new_format, int length, uint32& value);
OSCL_IMPORT_REF bool PV_atoi(const char *buf, const char new_format, int length, uint64& value);
OSCL_IMPORT_REF bool PV_atof(const char *buf, OsclFloat& value);
OSCL_IMPORT_REF bool PV_atof(const char *buf, int length, OsclFloat& value);
OSCL_IMPORT_REF int oscl_abs(int aVal);
#endif // OSCL_STRING_UTILS_H_INCLUDED

/*! @} */
