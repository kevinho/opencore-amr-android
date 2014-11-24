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

//               O S C L _ S T R I N G _ U T F 8

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/** \file oscl_string_utf8.h
    \brief Utilities to validate and truncate UTF-8 encoded strings.
*/

/*!
 * \par UTF-8 String Manipualation
 * These routines operate on UTF-8 character string.
 *
 */
#ifndef OSCL_STRING_UTF8_H
#define OSCL_STRING_UTF8_H

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Function prototypes
/*!
    \brief Check if the input string contains any illegal UTF-8 character.
           The function scans the string and validate that each character is a valid utf-8.
           It stops at the first NULL character, invalid character or the max_byte value.
           The string is valid if and only if every character is a valid utf-8 character and
           the scanning stopped on a character boundary.

    \param str_buf          Ptr to an input string, which may not terminate with null, to be checked
    \param num_valid_chars  This is an output parameter which is the number of valid utf-8 characters actually read.
    \param max_bytes        The maximum number of bytes to read (a zero value means read to the first NULL character).
    \param max_char_2_valid This is an input parameter.
                            Specify the number of utf-8 characters the caller wants to validate.
    \param num_byte_4_char  This is an output parameter.
                            The number of bytes used by the max_char characters
    \return                 True if the string is valid and false otherwise.
*/
OSCL_IMPORT_REF  bool  oscl_str_is_valid_utf8(const uint8 *str_buf, uint32& num_valid_characters, uint32 max_bytes = 0,
        uint32 max_char_2_valid = 0, uint32 * num_byte_4_char = NULL);
/*!
    \brief Truncates the UTF-8 string upto the required size.

           The function will modify the str_buf so that it contains AT MOST len valid
           utf-8 characters.  If a NULL character is found before reading len utf-8
           characters, then the function does not modify the string and simply returns
           the number of characters.  If an invalid character is found, then it will insert
           a NULL character after the last valid character and return the length.  Otherwise,
           it will insert a NULL character after len valid utf-8 characters and return the length.
    \param str_buf          Ptr to an input string which may not terminate with null
    \param max_char         The max number of the UTF-8 CHARACTERS
    \param max_bytes        The maximum number of bytes to read (a zero value means read to the first NULL character).
    \return                 It returns the length of the truncated string in utf-8 characters.
*/
OSCL_IMPORT_REF int32  oscl_str_truncate_utf8(uint8 *str_buf, uint32 max_char, uint32 max_bytes = 0);

#endif

/*! @} */
