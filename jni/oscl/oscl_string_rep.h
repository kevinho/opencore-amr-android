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

//               O S C L_ S T R I N G _ R E P
//
//    This file contains a standardized set of string containers that
//    can be used in place of character arrays.

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/*!
 * \file oscl_string_rep.h
 * \brief Contains some internal implementation for string containers.
 *
 */


#ifndef OSCL_STRING_REP_H_INCLUDED
#define OSCL_STRING_REP_H_INCLUDED


#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif



/** For internal use only-- heap string representation
*/
class CHeapRep
{
    public:
        CHeapRep()
                : refcount(0)
                , buffer(NULL)
                , maxsize(0)
                , size(0)
        {}

        OSCL_IMPORT_REF static void set_rep(CHeapRep*&, Oscl_DefAlloc&, const char*, uint32);
        OSCL_IMPORT_REF static void set_rep(CHeapRep*&, Oscl_DefAlloc&, const oscl_wchar*, uint32);
        OSCL_IMPORT_REF static void append_rep(CHeapRep*&, Oscl_DefAlloc&, const char*, uint32);
        OSCL_IMPORT_REF static void append_rep(CHeapRep*&, Oscl_DefAlloc&, const oscl_wchar*, uint32);

        uint32 refcount;
        OsclAny* buffer;
        uint32 maxsize;
        uint32 size;
        OSCL_IMPORT_REF bool set(uint32, const char*, Oscl_DefAlloc &);
        OSCL_IMPORT_REF bool set(uint32, const oscl_wchar*, Oscl_DefAlloc &);
        OSCL_IMPORT_REF bool append(uint32, const char*, uint32, const char*, Oscl_DefAlloc &);
        OSCL_IMPORT_REF bool append(uint32, const oscl_wchar*, uint32, const oscl_wchar*, Oscl_DefAlloc &);
        OSCL_IMPORT_REF void add_ref();
        OSCL_IMPORT_REF void remove_ref(Oscl_DefAlloc &);

        OSCL_IMPORT_REF static void assign(CHeapRep*&, CHeapRep*, Oscl_DefAlloc &);
    private:
        static CHeapRep *New(Oscl_DefAlloc &);
};

/** For internal use only-- stack string representation
*/
class CStackRep
{
    public:
        CStackRep()
                : maxsize(0)
                , size(0)
                , buffer(NULL)
        {}
        uint32 maxsize;
        uint32 size;
        OsclAny* buffer;
        OSCL_IMPORT_REF void set(const char* cp, uint32 len);
        OSCL_IMPORT_REF void set(const oscl_wchar* cp, uint32 len);
        OSCL_IMPORT_REF void append(const char* cp, uint32 len);
        OSCL_IMPORT_REF void append(const oscl_wchar* cp, uint32 len);
};


/** For internal use only-- fast string representation
*/
class CFastRep
{
    public:
        CFastRep()
                : maxsize(0)
                , size(0)
                , buffer(NULL)
                , writable(false)
                , overwrite(false)
        {}
        uint32 maxsize;
        uint32 size;
        OsclAny* buffer;
        bool writable;
        bool overwrite;
        OSCL_IMPORT_REF void set_w(char* cp, uint32 len, uint32 maxlen);
        OSCL_IMPORT_REF void set_w(oscl_wchar* cp, uint32 len, uint32 maxlen);
        OSCL_IMPORT_REF void set_r(const char* cp, uint32 len);
        OSCL_IMPORT_REF void set_r(const oscl_wchar* cp, uint32 len);
        OSCL_IMPORT_REF void append(const char* cp, uint32 len);
        OSCL_IMPORT_REF void append(const oscl_wchar* cp, uint32 len);
};


#endif   // OSCL_STRING_REP_H_INCLUDED

/*! @} */
