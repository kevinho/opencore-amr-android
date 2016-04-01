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

//               O S C L _ U T F 8 C O N V

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/** \file oscl_utf8conv.h
    \brief Utilities to convert unicode to utf8 and vice versa
*/


/********************************************************************************
                            UTF-8 Bit Distribution

UTF-16                                  1st Byte 2nd Byte 3rd Byte 4th Byte
-------- -------- -------- --------     -------- -------- -------- --------
00000000 0xxxxxxx                       0xxxxxxx
00000yyy yyxxxxxx                       110yyyyy 10xxxxxx
zzzzyyyy yyxxxxxx                       1110zzzz 10yyyyyy 10xxxxxx
110110ww wwzzzzyy 110111yy yyxxxxxx     11110uuu 10uuzzzz 10yyyyyy 10xxxxxx

NOTE:
 uuuuu = wwww+1 (to account for addition of 0x10000 as in Section 3.7, Surrogates)

**********************************************************************************/


#ifndef OSCL_UTF8CONV_H
#define OSCL_UTF8CONV_H

#ifndef OSCL_BASE_INCLUDED_H
#include "oscl_base.h"
#endif

/*!
** Define the maximum UTF8 representation in bytes.
** @todo Handle 4-byte surrogate pair representation
*/
#define MAX_NUMBER_OF_BYTE_PER_UTF8 3

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Function prototypes
/*!
    \brief Convert UTF8 byte sequence to Unicode string

           The function converts UTF8 byte sequence (or ASCII sequence) to Unicode string.
           The length of input UTF8 byte sequence is specified. It stops at two conditions:
           (A) Whole input UTF8 byte sequence is successfully converted.
           (B) Output buferr is not enough for output, or parse error.
           In case of (A), it adds a terminated '\0' at the end of the output Unicode string,
           and returns length of the output Unicode string(without counting terminated '\0').
           In case of (B), it converts as much as possible to the output buffer and adds a terminated '\0'
           at the end of the output Unicode string"(no '\0' added if outLength is less than or
           equal to 0, return 0)", and returns 0.

    \param input            Ptr to an input UTF8 byte sequence. '\0' termanation is not neccesary.
    \param inLength         The length of the input UTF8 byte sequence, without counting terminated '\0'(if any).
    \param output           Ptr to an output buffer which output Unicode string is written in.
    \param outLength        The size of output buffer, also the maximum number of oscl_wchar could be written in.
    \return                 Length of output (excludes '\0') : completely converts all input string and appends '\0' to output;
                            0 : insufficient buffer or error in conversion
*/

OSCL_IMPORT_REF int32 oscl_UTF8ToUnicode(const char *input, int32 inLength, oscl_wchar *output, int32 outLength);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Function prototypes
/*!
    \brief Convert Unicode string to UTF8 byte sequence

           The function converts Unicode string to UTF8 byte sequence.
           The length of input Unicode string is specified. It stops at two conditions:
           (A) Whole input Unicode string is successfully converted.
           (B) Destination buferr is not enough for output.
           In case of (A), it adds a terminated '\0' at the end of the output UTF8 byte sequence.
           and returns length of the output UTF8 byte sequence(without counting terminated '\0').
           In case of (B), it converts as much as possible to the output buffer and adds a terminated '\0'
           at the end of the output UTF8 byte sequence"(no '\0' added if outLength is less than or
           equal to 0, return 0)", and returns 0.

    \param input            Ptr to an input Unicode string. '\0' termanation is not neccesary.
    \param inLength         The length of the input Unicode string, without counting terminated '\0'(if any).
    \param output           Ptr to an output buffer which output UTF8 byte sequence is written in.
    \param outLength        The size of output buffer, also the maximum number of char could be written in.
    \return                 length of output (excludes '\0') : completely converts all input string and appends '\0' to output;
                            0 : insufficient buffer or error in conversion
*/

OSCL_IMPORT_REF int32 oscl_UnicodeToUTF8(const oscl_wchar *input, int32 inLength, char *output, int32 outLength);

#endif /* OSCL_UTF8CONV_H */

/*! @} */
