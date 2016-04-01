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

//                     O S C L _ N A M E S T R I N G

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclerror OSCL Error
 *
 * @{
 */


/** \file oscl_namestring.h
    \brief Name string class include file.
*/

#ifndef OSCL_NAMESTRING_H_INCLUDED
#define OSCL_NAMESTRING_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

/**
// Name string class appropriate for passing
// short constant ASCII strings around.
// All strings are automatically truncated
// and null-terminated.
*/
template<int __len>
class OsclNameString
{
    public:
        OsclNameString()
        {
            Set("");
        }

        OsclNameString(const char a[])
        {
            Set((uint8*)a);
        }

        OsclNameString(uint8* a)
        {
            Set(a);
        }

        /**
        * Set the string to the input value.  The string
        * will be truncated to fit the storage class and
        * automatically null-terminated.
        *
        * @param a (input param): null-terminated character
        *    string.
        */
        void Set(uint8* a)
        {
            for (int i = 0; i < __len; i++)
                iStr[i] = '\0';
            if (a)
            {
                for (int i = 0; i < __len - 1; i++)
                {
                    iStr[i] = a[i];
                    if (a[i] == '\0')
                        return;
                }
            }
        }

        void Set(const char a[])
        {
            Set((uint8*)a);
        }

        uint8* Str()const
        {
            return (uint8*)&iStr[0];
        }

        int32 MaxLen()const
        {
            return __len;
        }

    private:
        uint8 iStr[__len];
};
#endif

/*! @} */
