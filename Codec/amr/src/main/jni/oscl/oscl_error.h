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

//               O S C L _ E R R O R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclerror OSCL Error
 *
 * @{
 */


/** \file oscl_error.h
    \brief OSCL Error trap and cleanup include file
*/

#ifndef OSCL_ERROR_H_INCLUDED
#define OSCL_ERROR_H_INCLUDED

#ifndef OSCL_HEAPBASE_H_INCLUDED
#include "oscl_heapbase.h"
#endif

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

#ifndef OSCL_ERROR_CODES_H_INCLUDED
#include "oscl_error_codes.h"
#endif

/**
//Per-thread Error trap init.
*/
class OsclErrorTrapImp;
class OsclErrorTrap
{
    public:
        /**
         * Allocate and initialize error trap for
         * the calling thread.
         * @param aAlloc: optional, allocator to use for
         *   the internal implementation.
         * @return 0 for success, or an error
         */
        OSCL_IMPORT_REF static int32 Init(Oscl_DefAlloc *aAlloc = NULL);
        /**
         * Cleanup and destroy error trap for
         * the calling thread.
         * @return 0 for success, or an error
         */
        OSCL_IMPORT_REF static int32 Cleanup();
        /**
         * Get the ErrorTrapImp for the current thread.
         * Leaves on error.
         */
        OSCL_IMPORT_REF static OsclErrorTrapImp* GetErrorTrapImp();
};


/**
//User Error class
*/
class OsclError
{
    public:
        /**
        //Cleanup stack operations.
        */

        /** Push an _OsclHeapBase item onto the cleanup stack.
        */
        OSCL_IMPORT_REF static void PushL(_OsclHeapBase * aPtr);

        /** Push an OsclAny item onto the cleanup stack.
        */
        OSCL_IMPORT_REF static void PushL(OsclAny* aPtr);

        /** Push an OsclTrapItem onto the cleanup stack
        */
        OSCL_IMPORT_REF static void PushL(OsclTrapItem anItem);

        /** Pop the cleanup stack
        */
        OSCL_IMPORT_REF static void Pop();

        /** Pop the cleanup stack N times
        */
        OSCL_IMPORT_REF static void Pop(int32 aCount);

        /** Destroy the item on the top of the cleanup
        * stack and pop it
        */
        OSCL_IMPORT_REF static void PopDealloc();

        /** PopDealloc N times
        */
        OSCL_IMPORT_REF static void PopDealloc(int32 aCount);

        /** Do a Leave error, with the given reason code.
        ** When a leave occurs, all items on the cleanup stack
        ** for the current trap level will be destroyed, and
        ** execution will jump to the trap handler.
        */
        OSCL_IMPORT_REF static void Leave(int32 aReason);

        /** Evaluate the input parameter, and if it is null,
        ** do a Leave with OsclErrNoMemory reason code.
        */
        OSCL_IMPORT_REF static void LeaveIfNull(OsclAny *a);

        /** Evaluate the input parameter, and if it is an
        ** error code (non-zero), then do a Leave with the
        ** provided reason code.
        */
        OSCL_IMPORT_REF static void LeaveIfError(int32 aReason);

};

/** Cleanup Stack user macros
*/
#define OSCL_TRAPSTACK_PUSH(a) OsclError::PushL(a)
#define OSCL_TRAPSTACK_POP() OsclError::Pop()
#define OSCL_TRAPSTACK_POPDEALLOC() OsclError::PopDealloc()

/**
* TLS & Singleton registry calls that throw exceptions on errors.
*/

//Map TPVBaseErrorEnum return codes to Oscl Error leave codes
//Some of these codes indicate failure to init Oscl layer, in
//that case they map to zero and assert.
static const int32 _OsclBaseToErrorMap[] =
{
    /*0*/OsclErrGeneral
    ,/*EPVErrorBaseNotInstalled=1*/0
    ,/*EPVErrorBaseAlreadyInstalled=2*/OsclErrAlreadyInstalled
    ,/*EPVErrorBaseOutOfMemory=3*/OsclErrNoMemory
    ,/*EPVErrorBaseSystemCallFailed=4*/OsclErrSystemCallFailed
    ,/*EPVErrorBaseTooManyThreads=5*/0
    ,/*EPVErrorBaseNotSupported=6*/OsclErrNotSupported
    ,/*EPVErrorBaseNotReady=7*/OsclErrNotReady
};

#include "oscl_singleton.h"
#include "oscl_assert.h"
#if(OSCL_HAS_SINGLETON_SUPPORT)
class OsclSingletonRegistryEx
{
    public:
        /*
        ** Get an entry
        ** @param ID: identifier
        ** @returns: the entry value
        ** @exception: leaves on error.
        */
        static OsclAny* getInstance(uint32 ID)
        {
            int32 error;
            OsclAny* val = OsclSingletonRegistry::getInstance(ID, error);
            if (error)
            {
                OSCL_ASSERT(_OsclBaseToErrorMap[error]);
                OsclError::Leave(_OsclBaseToErrorMap[error]);
            }
            return val;
        }

        /*
        ** Set an entry
        ** @param ID: identifier
        ** @returns: the entry value
        ** @exception: leaves on error.
        */
        static void registerInstance(OsclAny* ptr, uint32 ID)
        {
            int32 error;
            OsclSingletonRegistry::registerInstance(ptr, ID, error);
            if (error)
            {
                OSCL_ASSERT(_OsclBaseToErrorMap[error]);
                OsclError::Leave(_OsclBaseToErrorMap[error]);
            }
        }

        /*
        //These two APIs can be used to do "test and set" operations on a singleton.
        //Be sure to always call both APIs to avoid deadlock.
        */

        /*
        * Return the current value of the singleton and leave the singleton table locked
        * on return.
        * @param ID the singleton ID
        * @returns the singleton value.
        ** @exception: leaves on error.
        */
        static OsclAny* lockAndGetInstance(uint32 ID)
        {
            int32 error;
            OsclAny* val = OsclSingletonRegistry::lockAndGetInstance(ID, error);
            if (error)
            {
                OSCL_ASSERT(_OsclBaseToErrorMap[error]);
                OsclError::Leave(_OsclBaseToErrorMap[error]);
            }
            return val;
        }

        /*
        * Set the value of the singleton.  Assume the singleton table is locked on entry.
        * @param ptr the singleton value
        * @param ID the singleton ID
        ** @exception: leaves on error.
        */
        static void registerInstanceAndUnlock(OsclAny* ptr, uint32 ID)
        {
            int32 error;
            OsclSingletonRegistry::registerInstanceAndUnlock(ptr, ID, error);
            if (error)
            {
                OSCL_ASSERT(_OsclBaseToErrorMap[error]);
                OsclError::Leave(_OsclBaseToErrorMap[error]);
            }
        }
};

template < class T, uint32 ID, class Registry = OsclSingletonRegistryEx > class OsclSingletonEx
{
    private:
        // make the copy constructor and assignment operator private
        OsclSingletonEx& operator=(OsclSingletonEx& _Y)
        {
            return(*this);
        }

    protected:
        T* _Ptr;

    public:
        OsclSingletonEx(): _Ptr(OSCL_STATIC_CAST(T*, Registry::getInstance(ID))) {};

        ~OsclSingletonEx() {};

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
            _Ptr = OSCL_STATIC_CAST(T*, Registry::getInstance(ID));
            return (_Ptr ? true : false);
        }

};
#endif //OSCL_HAS_SINGLETON_SUPPORT

#include "oscl_tls.h"
#include "oscl_assert.h"
class OsclTLSRegistryEx
{
    public:
        /*
        ** Get an entry
        ** @param ID: identifier
        ** @returns: the entry value
        ** @exception: leaves on error.
        */
        static OsclAny* getInstance(uint32 ID)
        {
            int32 error;
            OsclAny* val = OsclTLSRegistry::getInstance(ID, error);
            if (error)
            {
                OSCL_ASSERT(_OsclBaseToErrorMap[error]);
                OsclError::Leave(_OsclBaseToErrorMap[error]);
            }
            return val;
        }
        /*
        ** Set an entry
        ** @param ID: identifier
        ** @returns: the entry value
        ** @exception: leaves on error.
        */
        static void registerInstance(OsclAny* ptr, uint32 ID)
        {
            int32 error;
            OsclTLSRegistry::registerInstance(ptr, ID, error);
            if (error)
            {
                OSCL_ASSERT(_OsclBaseToErrorMap[error]);
                OsclError::Leave(_OsclBaseToErrorMap[error]);
            }
        }
};

template < class T, uint32 ID, class Registry = OsclTLSRegistryEx > class OsclTLSEx
{
    private:
        // make the copy constructor and assignment operator private
        OsclTLSEx& operator=(OsclTLSEx& _Y)
        {
            return(*this);
        }

    protected:
        T* _Ptr;

    public:
        OsclTLSEx(): _Ptr(OSCL_STATIC_CAST(T*, Registry::getInstance(ID))) {};

        ~OsclTLSEx() {};

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
            _Ptr = OSCL_STATIC_CAST(T*, Registry::getInstance(ID));
            return (_Ptr ? true : false);
        }

};

#endif

/*! @} */
