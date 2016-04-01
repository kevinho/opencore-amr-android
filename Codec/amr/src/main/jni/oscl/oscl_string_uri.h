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

//               O S C L _ S T R I N G _ U R I

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/** \file oscl_string_uri.h
    \brief Utilities to unescape URIs.
*/

/*!
 * \par URI String Manipualation
 * These routines handle all of the special escape sequences in the URI.
 *
 */
#ifndef OSCL_STRING_URI_H
#define OSCL_STRING_URI_H

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Function prototypes
/*!
    \brief unescape any of the special escape sequence in the uri string

           The function scans the string and replaces each escape sequence with its corresponding
           character. It stops at the first null character, or the max_byte value.
           It returns false if the string contains any illegal escape sequence or the output
           buffer is not big enough. The out_buf_len value indicates the needed buffer length
           or the index of the byte that causes the error respectively.

    \param str_buf_in       Ptr to an input string
    \param str_buf_out      Ptr to an output buffer which stores the modified string
    \param max_out_buf_bytes The size of str_buf_out.
    \param max_bytes        The maximum number of bytes to read.
                            It is the length of str_buf_in.
    \param out_buf_len      The length of the result string (not including the null character)
    \return                 It returns true if succeeds, otherwise false.

*/
OSCL_IMPORT_REF bool  oscl_str_unescape_uri(const char *str_buf_in, char *str_buf_out, uint32 max_out_buf_bytes, uint32 max_bytes, uint32& out_buf_len);
/*!
    \brief unescape any of the special escape sequence in the uri string

           The function scans the string and replaces each escape sequence with its corresponding
           character. It stops at the first null character, or the max_byte value.
           It returns false if the string contains any illegal escape sequence or the output
           buffer is not big enough. The out_buf_len value indicates the needed buffer length
           or the index of the byte that causes the error respectively.

    \param oscl_str_in      Ptr to an input OSCL_String
    \param oscl_str_out     Ptr to an output OSCL_String which stores the modified string
    \param out_buf_len      The length of the result string (not including the null character)
    \return                 It returns true if succeeds, otherwise false.
*/
//Old Definition
//bool  oscl_str_unescape_uri(const OSCL_String<char>& oscl_str_in, OSCL_String<char>& oscl_str_out, uint32& out_buf_len);
//New definition
OSCL_IMPORT_REF bool  oscl_str_unescape_uri(const OSCL_String& oscl_str_in, OSCL_String& oscl_str_out, uint32& out_buf_len);
#endif

/*! @} */
