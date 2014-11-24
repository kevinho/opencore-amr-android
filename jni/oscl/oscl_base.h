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

//                           O S C L _ B A S E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/*! \file oscl_base.h
    \brief The file oscl_base.h is the public header that should be included to pick up the platform configuration, basic type definitions, and common macros.*/


#ifndef OSCL_BASE_H_INCLUDED
#define OSCL_BASE_H_INCLUDED

#include "osclconfig.h"
#include "oscl_base_macros.h"
#include "oscl_types.h"
#include "osclconfig_check.h"

//singleton support derives from global var support.
#define OSCL_HAS_SINGLETON_SUPPORT 1

#ifdef __cplusplus

class OsclLockBase;

class OsclBase
{
    public:
        /**
         * Initializes OsclBase functionality.
         * OsclBase must be initialized before any OsclBase
         * functionality can be used.
         *
         * Note: The first call to OsclBase::Init will initialize
         *  the thread lock that is used to avoid thread contention
         *  for process scope singleton access.  The last call to
         *  OsclBase::Cleanup will cleanup the thread lock.
         *  Case should be taken to avoid possible thread contention
         *  on the first Init and the last Cleanup call.
         *
         * @return 0 on success
         */
        OSCL_IMPORT_REF static int32 Init();

        /**
         * Cleanup OsclBase functionality
         * OsclBase should be cleaned once OsclBase
         * functions are no longer needed
         * @return 0 on success
         */
        OSCL_IMPORT_REF static int32 Cleanup();
};

/**
//OsclBase error codes.  These values are used as return codes for OsclBase, OsclTLSRegistry,
//and OsclSingletonRegistry.
//Maintenance note: _OsclBaseToErrorMap in oscl_error must match this list
*/
enum TPVBaseErrorEnum
{
    EPVErrorBaseNotInstalled = 1
    , EPVErrorBaseAlreadyInstalled = 2
    , EPVErrorBaseOutOfMemory = 3
    , EPVErrorBaseSystemCallFailed = 4
    , EPVErrorBaseTooManyThreads = 5
};

#include "oscl_lock_base.h"

/**
 * _OsclBasicLock is a simple thread lock class for internal use by
 * OsclTLSRegistry and OsclSingleton.
 * Higher-level code should use OsclMutex instead.
 */
#if (OSCL_HAS_BASIC_LOCK)
class _OsclBasicLock : public OsclLockBase
{
    public:

        /**
         * Class constructor.
         */
        OSCL_IMPORT_REF _OsclBasicLock();

        /**
         * Class destructor
         */
        OSCL_IMPORT_REF ~_OsclBasicLock();

        /**
         * Takes the lock
         *
         */
        OSCL_IMPORT_REF void Lock();

        /**
         * Releases the lock
         *
         */
        OSCL_IMPORT_REF void Unlock();


        /**
        * Set to non-zero on error
        */
        int32 iError;

    private:
        TOsclBasicLockObject    ObjLock;

};
#else
typedef OsclNullLock _OsclBasicLock;
#endif

#else

/**
 * Initializes OsclBase functionality.
 * OsclBase must be initialized before any OsclBase
 * functionality can be used.
 *
 * @exception leaves if out-of-memory
 */
void PVOsclBase_Init();

/**
 * Cleanup OsclBase functionality
 * OsclBase should be cleaned once OsclBase
 * functions are no longer needed
 */
void PVOsclBase_Cleanup();

#endif

/*! @} */

#endif  // OSCL_BASE_H_INCLUDED
