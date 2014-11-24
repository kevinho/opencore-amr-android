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

//               O S C L _ M E D I A _ S T A T U S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/** \file oscl_media_status.h
    \brief Defines a status values for the MediaData containers.
*/

#ifndef OSCL_MEDIA_STATUS_H
#define OSCL_MEDIA_STATUS_H


const int32 APPEND_MEDIA_AT_END = -1;

class BufFragStatusClass
{
    public:
        typedef enum
        {
            BFG_SUCCESS = 0,
            TOO_MANY_FRAGS = 1,
            NOT_ENOUGH_SPACE = 2,
            EMPTY_FRAGMENT = 3,
            NULL_INPUT = 4,
            FIXED_FRAG_LOC_FULL = 5,
            INTERNAL_ERROR,
            INVALID_ID
        } status_t;
};

class MediaStatusClass : public BufFragStatusClass {};

#endif

/*! @} */
