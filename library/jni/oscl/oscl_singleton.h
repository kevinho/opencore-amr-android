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

//                     O S C L _ S I N G L E T O N

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/**
 *  @file oscl_singleton.h
 *  @brief This file defines the OsclSingleton class. This class
 *         provides a container which used to give access to a set of
 *         process-level singleton objects.  Each object is indexed
 *         by an integer ID, listed below.  There can only be one instance of
 *         each object per process at a given time.
 *
 *         OsclSingleton is initialized in OsclBase::Init.
 *
 */

#ifndef OSCL_SINGLETON_H_INCLUDED
#define OSCL_SINGLETON_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif


#if (OSCL_HAS_SINGLETON_SUPPORT)

//verify config-- singleton support requires global var support

// list of singleton objects
const uint32 OSCL_SINGLETON_ID_TEST           =  0;
const uint32 OSCL_SINGLETON_ID_OSCLMEM        =  1;
const uint32 OSCL_SINGLETON_ID_PVLOGGER       =  2;
const uint32 OSCL_SINGLETON_ID_PVSCHEDULER    =  3;
const uint32 OSCL_SINGLETON_ID_PVERRORTRAP    =  4;
const uint32 OSCL_SINGLETON_ID_SDPMEDIAPARSER =  5;
const uint32 OSCL_SINGLETON_ID_PAYLOADPARSER  =  6;
const uint32 OSCL_SINGLETON_ID_CPM_PLUGIN     =  7;
const uint32 OSCL_SINGLETON_ID_PVMFRECOGNIZER =  8;
const uint32 OSCL_SINGLETON_ID_OSCLREGISTRY   =  9;
const uint32 OSCL_SINGLETON_ID_OMX            = 10;
const uint32 OSCL_SINGLETON_ID_OMXMASTERCORE  = 11;
const uint32 OSCL_SINGLETON_ID_TICKCOUNT      = 12;
const uint32 OSCL_SINGLETON_ID_WMDRMLOCK      = 13;
const uint32 OSCL_SINGLETON_ID_LAST           = 14;


class OsclSingletonRegistry
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

        /*
        //These two APIs can be used to do "test and set" operations on a singleton.
        //Be sure to always call both APIs to avoid deadlock.
        */

        /*
        * Return the current value of the singleton and leave the singleton table locked
        * on return.
        * @param ID the singleton ID
        ** @param error (output) 0 for success or an error from TPVBaseErrorEnum
        * @returns the singleton value.
        */
        OSCL_IMPORT_REF static OsclAny* lockAndGetInstance(uint32 ID, int32& error);
        /*
        * Set the value of the singleton.  Assume the singleton table is locked on entry.
        * @param ptr the singleton value
        * @param ID the singleton ID
        ** @param error (output) 0 for success or an error from TPVBaseErrorEnum
        */
        OSCL_IMPORT_REF static void registerInstanceAndUnlock(OsclAny* ptr, uint32 ID, int32& error);

    private:
        OsclSingletonRegistry()
        {}
        typedef OsclAny* registry_type;
        typedef registry_type* registry_pointer_type;

    private:
        OSCL_IMPORT_REF static void initialize(Oscl_DefAlloc &alloc, int32 &error);
        OSCL_IMPORT_REF static void cleanup(Oscl_DefAlloc &alloc, int32 &error);
        friend class OsclBase;

    private:
        class SingletonTable
        {
            public:
                SingletonTable(): iRefCount(0)
                {
                    for (uint32 i = 0; i < OSCL_SINGLETON_ID_LAST; i++)
                        iSingletons[i] = NULL;
                }
                _OsclBasicLock iTableLock;
                uint32 iRefCount;
                OsclAny* iSingletons[OSCL_SINGLETON_ID_LAST];
                _OsclBasicLock iSingletonLocks[OSCL_SINGLETON_ID_LAST];
        };
        //The singleton table is a global variable.
        static SingletonTable* iSingletonTable;
};

template < class T, uint32 ID, class Registry = OsclSingletonRegistry > class OsclSingleton
{
    private:
        // make the copy constructor and assignment operator private
        OsclSingleton& operator=(OsclSingleton& _Y)
        {
            return(*this);
        }

    protected:
        T* _Ptr;

    public:
        OsclSingleton()
        {
            int32 err;
            _Ptr = OSCL_STATIC_CAST(T*, Registry::getInstance(ID, err));
        }

        ~OsclSingleton() {};

        /**
        * @brief The indirection operator (*) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OsclSingleton can be used like the
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
        * This operator ensures that the OsclSingleton can be used like the
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


#endif //OSCL_HAS_SINGLETON_SUPPORT

#endif

