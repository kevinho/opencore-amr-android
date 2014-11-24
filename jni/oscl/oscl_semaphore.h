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

//                     O S C L _ S E M A P H O R E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/**
 *  @file oscl_semaphore.h
 *  @brief This file provides implementation of mutex
 *
 */

// Definition file for OSCL Semaphore
#ifndef OSCL_SEMAPHORE_H_INCLUDED
#define OSCL_SEMAPHORE_H_INCLUDED

#ifndef OSCLCONFIG_PROC_H_INCLUDED
#include "osclconfig_proc.h"
#endif

#ifndef OSCL_THREAD_H_INCLUDED
#include "oscl_thread.h"
#endif


/**
 * Class Semaphore
 */
class OsclSemaphore
{
    public:

        /**
         * Class constructor
         */
        OSCL_IMPORT_REF OsclSemaphore();

        /**
         * Class destructor
         */
        OSCL_IMPORT_REF ~OsclSemaphore();

        /**
         * Creates the Semaphore
         *
         * @param Intialcount
         *
         * @return Returns the Error whether it is success or failure
         *incase of failure it will return what is the specific error
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError Create(uint32 initVal = 0);

        /**
         * Closes the Semaphore
         *
         * @param It wont take any parameters
         *
         * @return Returns the Error whether it is success or failure
         *incase of failure it will return what is the specific error
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError Close();

        /**
         * Makes the thread to wait on the Semaphore
         *
         * @param It wont take any parameters
         *
         * @return Returns the Error whether it is success or failure
         *incase of failure it will return what is the specific error
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError Wait();

        /**
         * Makes the thread to wait on the Semaphore, with a timeout.
         *
         * @param timeout in milliseconds.
         *
         * @return Returns SUCCESS_ERROR if the semaphore was aquired,
         * WAIT_TIMEOUT_ERROR if the timeout expired without acquiring the
         * semaphore, or an error code if the operation failed.
         * Note: this function may not be supported on all platforms, and
         * may return NOT_IMPLEMENTED.
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError Wait(uint32 timeout_msec);

        /**
         * Try to acquire semaphore ,if the semaphore is already acquired by another thread,
         *  calling thread immediately returns with out blocking
         *
         * @param It wont take any parameters
         *
         * @return Returns SUCCESS_ERROR if the semaphore was acquired,
         * SEM_LOCKED_ERROR if the semaphore cannot be acquired without waiting,
         * or an error code if the operation failed.
         * Note: this function may not be supported on all platforms, and
         * may return NOT_IMPLEMENTED.
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError TryWait();

        /**
         * Signals that the thread is finished with the Semaphore
         *
         * @param It wont take any parameters
         *
         * @return Returns the Error whether it is success or failure
         *incase of failure it will return what is the specific error
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError Signal();

    private:



        bool bCreated;
        //for pthreads implementations without sem timedwait support.
        TOsclMutexObject ObjMutex;
        TOsclConditionObject ObjCondition;
        uint32 iCount;


};





#endif  //  END OF File

