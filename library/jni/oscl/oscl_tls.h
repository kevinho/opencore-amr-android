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

//                     O S C L _ T L S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/**
 *  @file oscl_TLS.h
 *  @brief This file defines the OsclTLS template class. This class
 *         provides a container which used to give access to a single instance
 *         of a class within the scope of the TLS.
 *
 */

#ifndef OSCL_TLS_H_INCLUDED
#define OSCL_TLS_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif


#if (OSCL_TLS_IS_KEYED)

//Keyed TLS requires global variable support
#else

//unused value.
typedef OsclAny TOsclTlsKey;

#endif //OSCL_TLS_IS_KEYED


// list of TLS objects
const uint32 OSCL_TLS_ID_MAGICNUM       = 0;
const uint32 OSCL_TLS_ID_ERRORHOOK      = 1;
const uint32 OSCL_TLS_ID_PVLOGGER       = 2;
const uint32 OSCL_TLS_ID_TEST           = 3;
const uint32 OSCL_TLS_ID_PVSCHEDULER    = 4;
const uint32 OSCL_TLS_ID_PVERRORTRAP    = 5;
const uint32 OSCL_TLS_ID_SDPMEDIAPARSER = 6;
const uint32 OSCL_TLS_ID_PAYLOADPARSER  = 7;
const uint32 OSCL_TLS_ID_PVMFRECOGNIZER = 8;
const uint32 OSCL_TLS_ID_WMDRM          = 9;
const uint32 OSCL_TLS_ID_OSCLREGISTRY   = 10;
const uint32 OSCL_TLS_ID_SQLITE3        = 11;
const uint32 OSCL_TLS_ID_BASE_LAST      = 11; // should always equal the largest ID defined here

#define OSCL_TLS_BASE_SLOTS OSCL_TLS_ID_BASE_LAST +1

//There may be additional slots defined in the osclconfig.h for the build.
#ifndef OSCL_TLS_EXTERNAL_SLOTS
#define OSCL_TLS_EXTERNAL_SLOTS 0
#endif

#define OSCL_TLS_MAX_SLOTS ( OSCL_TLS_BASE_SLOTS + OSCL_TLS_EXTERNAL_SLOTS)

class TLSStorageOps
{
    public:
        OSCL_IMPORT_REF static void save_registry(TOsclTlsKey* key, OsclAny* ptr, int32&);
        OSCL_IMPORT_REF static OsclAny* get_registry(TOsclTlsKey* key);
};

class OsclTLSRegistry
{
    public:
        /*
        ** Get an entry
        ** @param ID: identifier
        ** @param error (output) 0 for success or an error from TPVBaseErrorEnum
        ** @returns: the entry value
        */
        OSCL_IMPORT_REF static OsclAny* getInstance(uint32 ID, int32 &error);
        /*
        ** Set an entry
        ** @param ID: identifier
        ** @param error (output) 0 for success or an error from TPVBaseErrorEnum
        ** @returns: the entry value
        */
        OSCL_IMPORT_REF static void registerInstance(OsclAny* ptr, uint32 ID, int32 &error);

    private:
        OsclTLSRegistry()
        {}
        typedef OsclAny* registry_type;
        typedef registry_type* registry_pointer_type;

#if ( OSCL_TLS_IS_KEYED)
        class TlsKey
        {
            public:
                TlsKey(): iRefCnt(0), iOsclTlsKey(NULL)
                {}
                _OsclBasicLock iLock;
                uint32 iRefCnt;
                TOsclTlsKey *iOsclTlsKey;
        };

        //The key is a global variable.
        static TlsKey* iTlsKey;
#endif

    private:
        OSCL_IMPORT_REF static void initialize(Oscl_DefAlloc &alloc, int32 &error);
        OSCL_IMPORT_REF static void cleanup(Oscl_DefAlloc &alloc, int32 &error);
        friend class OsclBase;

};

template < class T, uint32 ID, class Registry = OsclTLSRegistry > class OsclTLS
{
    private:
        // make the copy constructor and assignment operator private
        OsclTLS& operator=(OsclTLS& _Y)
        {
            return(*this);
        }

    protected:
        T* _Ptr;

    public:
        OsclTLS()
        {
            int32 err;
            _Ptr = OSCL_STATIC_CAST(T*, Registry::getInstance(ID, err));
        }

        ~OsclTLS() {};

        /**
        * @brief The indirection operator (*) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OsclTLS can be used like the
        * regular pointer that it was initialized with.
        */
        T& operator*() const
        {
            return(*_Ptr);
        }

        /**
        * @brief The indirection operator (->) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OsclTLS can be used like the
        * regular pointer that it was initialized with.
        */
        T *operator->() const
        {
            return(_Ptr);
        }


        /**
        * @brief set() method sets ownership to the pointer, passed.
        * This method is needed when the class is created with a default
        * constructor. Returns false in case the class is non-empty.
        *
        */
        bool set()
        {
            int32 err;
            _Ptr = OSCL_STATIC_CAST(T*, Registry::getInstance(ID, err));
            return (_Ptr ? true : false);
        }

};

/*! @} */



#endif

