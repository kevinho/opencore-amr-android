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

//               O S C L _ S T R I N G _ X M L

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/** \file oscl_string_xml.h
    \brief Utilities to escape special characters in XML strings
*/


/*!
 * \par XML String Manipualation
 * These routines handle the special characters, which needs to be escaped, for xml document.
 *
 */
#ifndef OSCL_STRING_XML_H
#define OSCL_STRING_XML_H

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Function prototypes
/*!
    \brief Check if the input string contains any special ASCII character
           like &, <, >, ', ".
           The function scans the string and check if each character is a special character.
           It stops at the first NULL character (if max_bytes = 0), or the max_byte value.

    \param str_buf          Ptr to an input string, which may not terminate with null, to be checked
    \param num_escape_bytes This is an output parameter which is the number of bytes needed to hold
                            the result string.
                            Value 0 indicates that there is no special character found.
                            If max_bytes = 0, the return value does not include the null character.
    \param max_bytes        The maximum number of bytes to read (a zero value means read to the first NULL character).
    \return                 True if the function succeeds, and
                            num_escape_bytes = 0 means that no special character is found,
                            num_escape_bytes >0 means the number of bytes of the result string.
                            False if there is any error occurred.
*/
OSCL_IMPORT_REF bool  oscl_str_need_escape_xml(const char *str_buf, uint32& num_escape_bytes, uint32 max_bytes = 0);
/*!
    \brief Escape any of the following special characters in the string
           Special ASCII characters: &, <, >, ', ".

           The function scans the string and replaces each special character with its corresponding
           escape sequence. It stops at the first NULL character, the max_byte value.

    \param str_buf_in       Ptr to an input string
    \param str_buf_out      Ptr to an output buffer which stores the modified string
    \param max_out_buf_bytes The size of str_buf_out.
    \param max_bytes        The maximum number of bytes to read (a zero value means read to the first NULL character).
                            It is the length of str_buf_in.
    \param num_bytes_written Number of bytes written in the output buffer, str_buf_out
    \return                 It returns the number of bytes in the str_buf_outring if succeeded.
                            It returns negative number if failed, and its absolute value indicates
                            the total number bytes written to the output buffer, str_buf_out,
                            if str_buf_out != null.
*/
OSCL_IMPORT_REF int32  oscl_str_escape_xml(const char *str_buf_in, char *str_buf_out, uint32 max_out_buf_bytes, uint32 max_bytes = 0,
        uint32 * num_bytes_written = NULL);
#endif


/*! @} */
